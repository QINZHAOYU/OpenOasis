/** ***********************************************************************************
 *    @File      :  AbstractAdaptedOutput.cpp
 *    @Brief     :  A class contains operations the providing component should carry
 *                  out on the data.
 *
 ** ***********************************************************************************/
#include "AbstractAdaptedOutput.h"
#include "ExchangeItemEventArgs.h"
#include "Models/CommImp/DevSupports/ExchangeItemHelper.h"
#include "Models/Utils/Exception.h"
#include "Models/Utils/MapHelper.h"


namespace OpenOasis::CommImp
{
using namespace DevSupports;
using namespace Utils;
using namespace std;


AbstractAdaptedOutput::AbstractAdaptedOutput(const string &id)
{
    mId       = id;
    mEventArg = make_shared<ExchangeItemEventArgs>();
}

AbstractAdaptedOutput::AbstractAdaptedOutput(
    const string &id, const shared_ptr<IOutput> &adaptee) :
    AbstractAdaptedOutput(id)
{
    mOutput   = adaptee;
    mQunatity = adaptee->GetValueDefinition();
}

string AbstractAdaptedOutput::GetId() const
{
    return mId;
}

string AbstractAdaptedOutput::GetCaption() const
{
    return mCaption;
}

void AbstractAdaptedOutput::SetCaption(const string &caption)
{
    mCaption = caption;
}

string AbstractAdaptedOutput::GetDescription() const
{
    return mDescription;
}

void AbstractAdaptedOutput::SetDescription(const string &discription)
{
    mDescription = discription;
}

shared_ptr<IValueDefinition> AbstractAdaptedOutput::GetValueDefinition() const
{
    return mQunatity;
}

void AbstractAdaptedOutput::AddListener(
    const function<void(shared_ptr<ExchangeItemChangeEventArgs>)> &func)
{
    mItemChanged += func;
}

void AbstractAdaptedOutput::RemoveListener(
    const function<void(shared_ptr<ExchangeItemChangeEventArgs>)> &func)
{
    mItemChanged -= func;
}

vector<weak_ptr<IInput>> AbstractAdaptedOutput::GetConsumers() const
{
    return mConsumers;
}

void AbstractAdaptedOutput::AddConsumer(shared_ptr<IInput> consumer)
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
            + ") added to IAdaptedOutput(" + mId + ")");
    }

    // Check time and elment set compatibility.
    if (!ExchangeItemHelper::ConsumersCompatible(GetInstance(), consumer))
    {
        throw IllegalArgumentException(
            "Time/element set incompatible consumer(" + consumer->GetId()
            + ") added to IAdaptedOutput(" + mId + ")");
    }

    // Attach consumer and add this as a provider of the consumer automatically.
    mConsumers.push_back(consumer);
    consumer->AddProvider(GetInstance());
}

void AbstractAdaptedOutput::RemoveConsumer(const shared_ptr<IInput> &consumer)
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

vector<shared_ptr<IAdaptedOutput>> AbstractAdaptedOutput::GetAdaptedOutputs() const
{
    return mAdaptedOutputs;
}

void AbstractAdaptedOutput::AddAdaptedOutput(shared_ptr<IAdaptedOutput> adaptedOutput)
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
    //         + ") added to IAdaptedOutput(" + mId + ")");
    // }

    // Check time and elment set compatibility.
    // if (!ExchangeItemHelper::OutputAndInputFit(GetInstance(), adaptedOutput))
    if (!ExchangeItemHelper::OutputAndInputElementSetsFit(GetInstance(), adaptedOutput))
    {
        throw IllegalArgumentException(
            "Element set incompatible AdaptedOutput(" + adaptedOutputId
            + ") added to IAdaptedOutput(" + mId + ")");
    }

    // Attach adapted output.
    mAdaptedOutputs.push_back(adaptedOutput);
    adaptedOutput->SetAdaptee(GetInstance());
}

void AbstractAdaptedOutput::RemoveAdaptedOutput(
    const shared_ptr<IAdaptedOutput> &adaptedOutput)
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

weak_ptr<ILinkableComponent> AbstractAdaptedOutput::GetComponent() const
{
    if (mOutput.lock())
    {
        return mOutput.lock()->GetComponent();
    }
    else
    {
        return {};
    }
}

void AbstractAdaptedOutput::Initialize()
{}

weak_ptr<IOutput> AbstractAdaptedOutput::GetAdaptee() const
{
    return mOutput;
}

void AbstractAdaptedOutput::SetAdaptee(shared_ptr<IOutput> adaptee)
{
    mOutput = adaptee;
}

vector<shared_ptr<IArgument>> AbstractAdaptedOutput::GetArguments() const
{
    return MapHelper::GetValues(mArguments);
}

shared_ptr<IAdaptedOutput> AbstractAdaptedOutput::GetInstance()
{
    return shared_from_this();
}

void AbstractAdaptedOutput::BroadcastEventWithMsg(const string &msg)
{
    if (!mItemChanged.IsEmpty())
    {
        mEventArg->SetExchangeItem(GetInstance());
        mEventArg->SetMessages("ExchangeItem(" + mId + "): " + msg);
        mItemChanged.Invoke(mEventArg);
    }
}

}  // namespace OpenOasis::CommImp