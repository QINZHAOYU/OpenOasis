/** ***********************************************************************************
 *    @File      :  LinkableComponent.cpp
 *    @Brief     :  Generic implementation of `ITimeSpaceComponent` with more details.
 *
 ** ***********************************************************************************/
#include "LinkableComponent.h"
#include "LinkableComponentEventArgs.h"
#include "TimeSet.h"
#include "Time.h"
#include "Arguments.h"
#include "TimeAdaptedOutputFactory.h"
#include "SpaceAdaptedOutputFactory.h"
#include "DevSupports/ExtensionMethods.h"
#include "DevSupports/ExchangeItemHelper.h"
#include "Models/Utils/Exception.h"
#include "Models/Utils/StringHelper.h"
#include "Models/Utils/MapHelper.h"
#include <array>


namespace OpenOasis::CommImp
{
using namespace DevSupports;
using namespace Utils;
using namespace std;

LinkableComponent::LinkableComponent(const string &id)
{
    mId        = id;
    mEventArgs = make_shared<LinkableComponentEventArgs>();

    mRequiredArguments = {"ID", "OUTPUTTERS", "INPUTTERS"};

    mArguments["ID"] = make_shared<ArgumentString>("ID", mId);

    // Set the output/input needed,
    // formated as "object_type, object_id, state_id".
    mArguments["OUTPUTTERS"] =
        make_shared<TArgument<vector<array<string, 3>>>>("OUTPUTTERS");
    mArguments["INPUTTERS"] =
        make_shared<TArgument<vector<array<string, 3>>>>("INPUTTERS");
}

LinkableComponent::LinkableComponent(LinkableComponent &&obj)
{
    mId                     = obj.mId;
    mCaption                = obj.mCaption;
    mDescription            = obj.mDescription;
    mStatusListeners        = obj.mStatusListeners;
    mEventArgs              = obj.mEventArgs;
    mRequiredArguments      = obj.mRequiredArguments;
    mAdaptedOutputFactories = obj.mAdaptedOutputFactories;
    mTimeExtent             = obj.mTimeExtent;
    mCurrentTime            = obj.mCurrentTime;
    mArguments              = obj.mArguments;
    mInputs                 = obj.mInputs;
    mOutputs                = obj.mOutputs;

    mStatus                       = obj.mStatus;
    mCascadingUpdateCallsDisabled = obj.mCascadingUpdateCallsDisabled;
}

string LinkableComponent::GetCaption() const
{
    return mCaption;
}

void LinkableComponent::SetCaption(const string &value)
{
    mCaption = value;
}

string LinkableComponent::GetDescription() const
{
    return mDescription;
}

void LinkableComponent::SetDescription(const string &value)
{
    mDescription = value;
}

string LinkableComponent::GetId() const
{
    if (mId == "")
    {
        throw runtime_error("Id not Set");
    }
    return mId;
}

vector<shared_ptr<IArgument>> LinkableComponent::GetArguments() const
{
    return MapHelper::GetValues(mArguments);
}

void LinkableComponent::SetStatus(LinkableComponentStatus value, const string &msg)
{
    if (value == mStatus)
        return;

    LinkableComponentStatus oldStatus = mStatus;

    mStatus = value;

    BroadcastEvent(msg, oldStatus, value);
}

LinkableComponentStatus LinkableComponent::GetStatus() const
{
    return mStatus;
}

vector<shared_ptr<IInput>> LinkableComponent::GetInputs() const
{
    return mInputs;
}

vector<shared_ptr<IOutput>> LinkableComponent::GetOutputs() const
{
    return mOutputs;
}

vector<shared_ptr<IAdaptedOutputFactory>>
LinkableComponent::GetAdaptedOutputFactories() const
{
    return mAdaptedOutputFactories;
}

void LinkableComponent::SetAdaptedOutputFactories()
{
    if (mAdaptedOutputFactories.empty())
    {
        auto timeFactory = make_shared<TimeAdaptedOutputFactory>(GetId() + "-Time");
        mAdaptedOutputFactories.push_back(timeFactory);

        auto spaceFactory = make_shared<SpaceAdaptedOutputFactory>(GetId() + "-Space");
        mAdaptedOutputFactories.push_back(spaceFactory);
    }
}

void LinkableComponent::AddListener(const ListenFunc &func)
{
    mStatusListeners += func;
}

void LinkableComponent::RemoveListener(const ListenFunc &func)
{
    mStatusListeners -= func;
}

void LinkableComponent::SetCascadingUpdateCallsDisabled(bool status)
{
    mCascadingUpdateCallsDisabled = status;
}

bool LinkableComponent::GetCascadingUpdateCallsDisabled() const
{
    return mCascadingUpdateCallsDisabled;
}

bool LinkableComponent::IsIterationConverged() const
{
    return true;
}

bool LinkableComponent::IsOptimizationTerminated() const
{
    return true;
}

shared_ptr<ITime> LinkableComponent::GetStartTime() const
{
    return ExtensionMethods::Start(mTimeExtent->GetTimeHorizon());
}

shared_ptr<ITime> LinkableComponent::GetEndTime() const
{
    return ExtensionMethods::End(mTimeExtent->GetTimeHorizon());
}

shared_ptr<ITime> LinkableComponent::GetNowTime() const
{
    return mCurrentTime;
}

void LinkableComponent::Initialize()
{
    SetStatus(LinkableComponentStatus::Initializing);

    InitializeArguments();
    InitializeSpace();
    InitializeTime();
    InitializeInputs();
    InitializeOutputs();

    SetAdaptedOutputFactories();

    SetStatus(LinkableComponentStatus::Initialized);
}

vector<string> LinkableComponent::Validate()
{
    SetStatus(LinkableComponentStatus::Validating);

    const auto &validationResults = OnValidate();
    if (validationResults.empty())
    {
        SetStatus(LinkableComponentStatus::Valid);
    }
    else
    {
        SetStatus(LinkableComponentStatus::Invalid);
    }

    return validationResults;
}

void LinkableComponent::Prepare()
{
    SetStatus(LinkableComponentStatus::Preparing);

    OnPrepare();

    SetStatus(LinkableComponentStatus::Updated);
}

void LinkableComponent::OnPrepare()
{
    PrepareInputs();

    PrepareOutputs();
}

void LinkableComponent::Update()
{
    // No more update.
    if (mStatus == LinkableComponentStatus::Done
        || mStatus == LinkableComponentStatus::Finished
        || mStatus == LinkableComponentStatus::Failed)
    {
        return;
    }

    // Update with estimates if component is blocked.
    if (mStatus == LinkableComponentStatus::Updating
        || mStatus == LinkableComponentStatus::WaitingForData)
    {
        UpdateOutputs(mOutputs);
        return;
    }

    // Indicate that we are waiting data.
    SetStatus(LinkableComponentStatus::WaitingForData);

    // Prepare all required input data.
    PullInputs();

    // Indicate that we are starting to compute.
    SetStatus(LinkableComponentStatus::Updating);

    // Compute.
    PerformTimestep(mOutputs);

    if (mStatus == LinkableComponentStatus::Failed)
    {
        Finish();
        return;
    }

    // Update active output items.
    UpdateOutputs(mOutputs);

    // Assuming the time step was successfull, then we can update the time horizon
    // start time of all input items, to indicate that we are never going to ask
    // data before this time. Done after the `PerformTimestep` in order to support
    // redoing of time steps.
    if (!mCascadingUpdateCallsDisabled)
        UpdateInputs();

    // Indicate that Update is done.
    SetStatus(
        mCurrentTime->GetTimeStamp() >= GetEndTime()->GetTimeStamp() ?
            LinkableComponentStatus::Done :
            LinkableComponentStatus::Updated);
}

void LinkableComponent::PullInputs()
{
    for (const auto &input : mInputs)
    {
        if (input->GetProviders().empty())
            continue;

        const auto &values = input->GetValues();
        ApplyInputData(values);
    }
}

void LinkableComponent::UpdateInputs()
{
    double lastTimestamp = mCurrentTime->GetTimeStamp();

    for (const auto &input : mInputs)
    {
        if (input->GetProviders().empty())
            continue;

        const auto &timeset  = input->GetTimeSet();
        const auto &valueset = input->GetValues();

        int elments = valueset->GetIndexCount({0});
        while (!timeset->GetTimes().empty())
        {
            if (timeset->GetTimes().front()->GetTimeStamp() < lastTimestamp)
            {
                // Remove the expired time and values synchronously.
                timeset->RemoveTime(0);
                valueset->RemoveValue({0});
            }
            else
            {
                break;
            }
        }

        if (timeset->GetTimes().empty())
        {
            timeset->AddTime(mCurrentTime);

            auto missValue = valueset->GetValueDefinition()->GetMissingDataValue();
            for (int i = 0; i < elments; i++)
            {
                valueset->SetOrAddValue({0, i}, missValue);
            }
        }
    }
}

void LinkableComponent::Finish()
{
    SetStatus(LinkableComponentStatus::Finishing);

    Reset();

    SetStatus(LinkableComponentStatus::Finished);
}

void LinkableComponent::Reset()
{
    // Reset all object states only.
    mCascadingUpdateCallsDisabled = false;

    mTimeExtent.reset();
    mCurrentTime.reset();

    for (const auto &input : mInputs)
        input->Reset();

    for (const auto &output : mOutputs)
        output->Reset();

    mArguments.clear();
}

shared_ptr<IIdentifiable> LinkableComponent::KeepCurrentState()
{
    throw NotImplementedException();
}

void LinkableComponent::RestoreState(const shared_ptr<IIdentifiable> &stateId)
{
    throw NotImplementedException();
}

void LinkableComponent::ClearState(const shared_ptr<IIdentifiable> &stateId)
{
    throw NotImplementedException();
}

void LinkableComponent::BroadcastEvent(
    const string &msg, LinkableComponentStatus oldState,
    LinkableComponentStatus newState)
{
    if (!mStatusListeners.IsEmpty())
    {
        mEventArgs->SetLinkableComponent(shared_from_this());
        mEventArgs->SetOldStatus(oldState);
        mEventArgs->SetNewStatus(newState);
        mEventArgs->SetMessages(msg);

        mStatusListeners.Invoke(mEventArgs);
    }
}

}  // namespace OpenOasis::CommImp