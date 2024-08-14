/** ***********************************************************************************
 *    @File      :  Output.cpp
 *    @Brief     :  To provide an output exchange item that can deliver values.
 *
 ** ***********************************************************************************/
#include "Output.h"
#include "ExchangeItemEventArgs.h"
#include "ElementSet.h"
#include "TimeSet.h"
#include "Time.h"
#include "ValueSet2D.h"
#include "Quantity.h"
#include "DevSupports/ExchangeItemHelper.h"
#include "DevSupports/ExtensionMethods.h"
#include "Models/Utils/Exception.h"


namespace OpenOasis::CommImp
{
using namespace DevSupports;
using namespace std;

Output::Output(const string &id, const shared_ptr<ILinkableComponent> &comp)
{
    mId        = id;
    mComponent = comp;
    mEventArg  = make_shared<ExchangeItemEventArgs>();
}

Output::Output(Output &&obj)
{
    mId             = obj.mId;
    mCaption        = obj.mCaption;
    mDescription    = obj.mDescription;
    mValues         = obj.mValues;
    mTimeSet        = obj.mTimeSet;
    mElementSet     = obj.mElementSet;
    mEventArg       = obj.mEventArg;
    mItemListeners  = obj.mItemListeners;
    mConsumers      = obj.mConsumers;
    mAdaptedOutputs = obj.mAdaptedOutputs;

    mComponent = obj.mComponent;
}

string Output::GetId() const
{
    return mId;
}

string Output::GetCaption() const
{
    return mCaption;
}

void Output::SetCaption(const string &caption)
{
    mCaption = caption;
}

string Output::GetDescription() const
{
    return mDescription;
}

void Output::SetDescription(const string &discription)
{
    mDescription = discription;
}

shared_ptr<IElementSet> Output::GetElementSet() const
{
    return mElementSet;
}

void Output::SetElementSet(shared_ptr<IElementSet> value)
{
    mElementSet = value;

    BroadcastEventWithMsg("ElementSet reseted");
}

shared_ptr<ITimeSet> Output::GetTimeSet() const
{
    return mTimeSet;
}

void Output::SetTimeSet(shared_ptr<ITimeSet> value)
{
    mTimeSet = value;

    BroadcastEventWithMsg("TimeSet reseted");
}

vector<weak_ptr<IInput>> Output::GetConsumers() const
{
    return mConsumers;
}

void Output::AddConsumer(shared_ptr<IInput> consumer)
{
    // Check duplication.
    auto iter =
        find_if(begin(mConsumers), end(mConsumers), [&consumer](const auto &elem) {
            return consumer->GetId() == elem.lock()->GetId()
                   && consumer->GetComponent().lock()->GetId()
                          == elem.lock()->GetComponent().lock()->GetId();
        });
    if (iter != mConsumers.end())
    {
        return;
    }

    // Check value definition compatibility.
    if (!ExchangeItemHelper::OutputAndInputValueDefinitionFit(GetInstance(), consumer))
    {
        throw IllegalArgumentException(
            "Value definition incompatible consumer(" + consumer->GetId()
            + ") added to Output(" + mId + ")");
    }

    // Check time and elment set compatibility.
    if (!ExchangeItemHelper::ConsumersCompatible(GetInstance(), consumer))
    {
        throw IllegalArgumentException(
            "Time/element set incompatible consumer(" + consumer->GetId()
            + ") added to Output(" + mId + ")");
    }

    // Attach consumer and add this as a provider of the consumer automatically.
    mConsumers.push_back(consumer);
    consumer->AddProvider(GetInstance());
}

void Output::RemoveConsumer(const shared_ptr<IInput> &consumer)
{
    // Search comsumer.
    auto iter =
        remove_if(begin(mConsumers), end(mConsumers), [&consumer](const auto &elem) {
            return consumer->GetId() == elem.lock()->GetId()
                   && consumer->GetComponent().lock()->GetId()
                          == elem.lock()->GetComponent().lock()->GetId();
        });
    if (iter == mConsumers.end())
    {
        return;
    }

    // Detach consumer.
    consumer->RemoveProvider(GetInstance());
    mConsumers.erase(iter, mConsumers.end());
}

vector<shared_ptr<IAdaptedOutput>> Output::GetAdaptedOutputs() const
{
    return mAdaptedOutputs;
}

void Output::AddAdaptedOutput(shared_ptr<IAdaptedOutput> adaptedOutput)
{
    // Check duplication.
    const auto &adaptedOutputId   = adaptedOutput->GetId();
    const auto &adaptedOutputComp = adaptedOutput->GetComponent().lock();
    const auto &compId = (adaptedOutputComp) ? adaptedOutputComp->GetId() : "";
    for (const auto &elem : mAdaptedOutputs)
    {
        const auto &elemComp = elem->GetComponent().lock();
        if (!elemComp)
        {
            continue;
        }

        if (adaptedOutputId == elem->GetId() && compId == elemComp->GetId())
        {
            return;
        }
    }

    // Check compatibility.
    // if (!ExchangeItemHelper::OutputAndInputValueDefinitionFit(
    //         GetInstance(), adaptedOutput))
    // {
    //     throw IllegalArgumentException(
    //         "Value definition incompatible AdaptedOutput(" + adaptedOutputId
    //         + ") added to Output(" + mId + ")");
    // }

    // Check time and elment set compatibility.
    if (!ExchangeItemHelper::OutputAndInputFit(GetInstance(), adaptedOutput))
    {
        throw IllegalArgumentException(
            "Time/element set incompatible AdaptedOutput(" + adaptedOutputId
            + ") added to Output(" + mId + ")");
    }

    // Attach adapted output.
    mAdaptedOutputs.push_back(adaptedOutput);
    adaptedOutput->SetAdaptee(GetInstance());
}

void Output::RemoveAdaptedOutput(const shared_ptr<IAdaptedOutput> &adaptedOutput)
{
    const auto &adaptedOutputId   = adaptedOutput->GetId();
    const auto &adaptedOutputComp = adaptedOutput->GetComponent().lock();
    const auto &compId = (adaptedOutputComp) ? adaptedOutputComp->GetId() : "";

    // Remove adapted output and others invalid.
    for (auto it = mAdaptedOutputs.begin(); it != mAdaptedOutputs.end();)
    {
        const auto &comp = (*it)->GetComponent().lock();
        if (!comp)
        {
            it = mAdaptedOutputs.erase(it);
            continue;
        }

        if (adaptedOutputId == (*it)->GetId() && compId == comp->GetId())
        {
            it = mAdaptedOutputs.erase(it);
        }
        else
        {
            it++;
        }
    }

    // Dettach adapted output.
    adaptedOutput->SetAdaptee(nullptr);
}

void Output::Reset()
{
    mValues.reset();
    mElementSet.reset();
    mTimeSet.reset();
    mComponent.reset();
    mConsumers.clear();
    mAdaptedOutputs.clear();

    BroadcastEventWithMsg("Output item reseted.");
    mItemListeners.Clear();
}

shared_ptr<ITimeSet> Output::GetTimeExtent() const
{
    return mTimeSet;
}

shared_ptr<ITime> Output::GetCurrTime() const
{
    return ExtensionMethods::End(mTimeSet->GetTimeHorizon());
}

shared_ptr<ISpatialDefinition> Output::GetSpatialDefinition() const
{
    return mElementSet;
}

void Output::AddListener(const ListenFunc &func)
{
    mItemListeners += func;
}

void Output::RemoveListener(const ListenFunc &func)
{
    mItemListeners -= func;
}

shared_ptr<IValueDefinition> Output::GetValueDefinition() const
{
    if (mValues)
    {
        return mValues->GetValueDefinition();
    }
    else
    {
        return nullptr;
    }
}

shared_ptr<IValueSet> Output::GetValues()
{
    // Get the earlist time which no value request will be made earlier than.
    const auto earliestTime =
        ExchangeItemHelper::GetEarliestConsumerTime(GetInstance());

    Update();

    ReduceValuesAndTimes(earliestTime);

    return mValues;
}

bool Output::IsValidQuerySpecifier(const shared_ptr<IBaseExchangeItem> &querier) const
{
    if (!querier)
        return false;

    try
    {
        auto queryValueDef = ExtensionMethods::Quantity(querier);
        auto valueDef = dynamic_pointer_cast<Quantity>(mValues->GetValueDefinition());
        if (!valueDef->EqualTo(queryValueDef))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    catch (...)
    {
        return false;
    }
}

void Output::Update()
{
    const auto &latestTime = ExchangeItemHelper::GetLatestConsumerTime(GetInstance());
    if (!latestTime)
        return;

    double availableTimestamp =
        ExtensionMethods::End(mTimeSet->GetTimeHorizon())->GetTimeStamp();
    double queryTimestamp = latestTime->GetTimeStamp();

    const auto &comp   = mComponent.lock();
    auto        status = comp->GetStatus();

    // Update the component to the latest time from all consumers.
    while ((status == LinkableComponentStatus::Updated)
           && availableTimestamp < queryTimestamp)
    {
        // The mComponent is responsible for updating output's timeset and valueset.
        comp->Update();
        BroadcastEventWithMsg("component updated");

        status = comp->GetStatus();
        availableTimestamp =
            ExtensionMethods::End(mTimeSet->GetTimeHorizon())->GetTimeStamp();
    }

    // Refresh adapted outputs.
    RefreshAdaptedOutputs();
}

void Output::ReduceValuesAndTimes(const shared_ptr<ITime> &lastTime)
{
    if (mTimeSet->GetTimes().empty())
        return;

    double lastTimestamp = mTimeSet->GetTimes().back()->GetTimeStamp();
    if (lastTime)
    {
        lastTimestamp = lastTime->GetTimeStamp();
    }

    while (!mTimeSet->GetTimes().empty())
    {
        if (mTimeSet->GetTimes().front()->GetTimeStamp() < lastTimestamp)
        {
            // Remove the expired time and values synchronously.
            mTimeSet->RemoveTime(0);
            mValues->RemoveValue({0});
        }
        else
        {
            break;
        }
    }
}

void Output::RefreshAdaptedOutputs()
{
    for (auto &adaptor : mAdaptedOutputs)
    {
        if (!adaptor->GetConsumers().empty() || !adaptor->GetAdaptedOutputs().empty())
        {
            adaptor->Refresh();
        }
    }
}

void Output::SetValues(shared_ptr<IValueSet> values)
{
    mValues = values;

    BroadcastEventWithMsg("ValueSet reseted");
}

weak_ptr<ILinkableComponent> Output::GetComponent() const
{
    return mComponent;
}

void Output::SetComponent(shared_ptr<ILinkableComponent> component)
{
    mComponent = component;
}

shared_ptr<Output> Output::GetInstance()
{
    return shared_from_this();
}

void Output::BroadcastEventWithMsg(const string &msg)
{
    if (!mItemListeners.IsEmpty())
    {
        mEventArg->SetExchangeItem(GetInstance());
        mEventArg->SetMessages("ExchangeItem(" + mId + "): " + msg);
        mItemListeners.Invoke(mEventArg);
    }
}

}  // namespace OpenOasis::CommImp