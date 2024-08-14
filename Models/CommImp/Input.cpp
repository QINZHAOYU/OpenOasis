/** ***********************************************************************************
 *    @File      :  Input.cpp
 *    @Brief     :  To provide an input exchange item that can accept values.
 *
 ** ***********************************************************************************/
#include "Input.h"
#include "ExchangeItemEventArgs.h"
#include "ElementSet.h"
#include "TimeSet.h"
#include "ValueSet2D.h"
#include "Quantity.h"
#include "DevSupports/ExchangeItemHelper.h"
#include "DevSupports/ExtensionMethods.h"
#include "Models/Utils/Exception.h"


namespace OpenOasis::CommImp
{
using namespace DevSupports;
using namespace Utils;
using namespace std;


Input::Input(const string &id, const shared_ptr<ILinkableComponent> &comp)
{
    mId        = id;
    mComponent = comp;
    mEventArg  = make_shared<ExchangeItemEventArgs>();
}

Input::Input(Input &&obj)
{
    mId            = obj.mId;
    mCaption       = obj.mCaption;
    mDescription   = obj.mDescription;
    mValues        = obj.mValues;
    mTimeSet       = obj.mTimeSet;
    mElementSet    = obj.mElementSet;
    mEventArg      = obj.mEventArg;
    mItemListeners = obj.mItemListeners;
    mProviders     = obj.mProviders;

    mComponent = obj.mComponent;
}

string Input::GetId() const
{
    return mId;
}

string Input::GetCaption() const
{
    return mCaption;
}

void Input::SetCaption(const string &caption)
{
    mCaption = caption;
}

string Input::GetDescription() const
{
    return mDescription;
}

void Input::SetDescription(const string &discription)
{
    mDescription = discription;
}

shared_ptr<IElementSet> Input::GetElementSet() const
{
    return mElementSet;
}

void Input::SetElementSet(shared_ptr<IElementSet> value)
{
    mElementSet = value;

    BroadcastEventWithMsg("ElementSet reseted");
}

shared_ptr<ITimeSet> Input::GetTimeSet() const
{
    return mTimeSet;
}

void Input::SetTimeSet(shared_ptr<ITimeSet> value)
{
    mTimeSet = value;

    BroadcastEventWithMsg("TimeSet reseted");
}

void Input::SetValues(shared_ptr<IValueSet> values)
{
    mValues = values;

    BroadcastEventWithMsg("ValueSet reseted");
}

weak_ptr<ILinkableComponent> Input::GetComponent() const
{
    return mComponent;
}

shared_ptr<ISpatialDefinition> Input::GetSpatialDefinition() const
{
    return mElementSet;
}

shared_ptr<IValueDefinition> Input::GetValueDefinition() const
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

vector<weak_ptr<IOutput>> Input::GetProviders() const
{
    return mProviders;
}

void Input::AddProvider(shared_ptr<IOutput> provider)
{
    // Check duplication.
    auto iter =
        find_if(begin(mProviders), end(mProviders), [&provider](const auto &elem) {
            return provider->GetId() == elem.lock()->GetId()
                   && provider->GetComponent().lock()->GetId()
                          == elem.lock()->GetComponent().lock()->GetId();
        });
    if (iter != mProviders.end())
    {
        return;
    }

    // Check value definition compatibility.
    if (!ExchangeItemHelper::OutputAndInputValueDefinitionFit(provider, GetInstance()))
    {
        throw IllegalArgumentException(
            "Value definition incompatible provider(" + provider->GetId()
            + ") added to Output(" + mId + ")");
    }

    // Attach provider and add this as a consumer of the provider automatically.
    mProviders.push_back(provider);
    provider->AddConsumer(GetInstance());
}

void Input::RemoveProvider(const shared_ptr<IOutput> &provider)
{
    // Search comsumer.
    auto iter =
        remove_if(begin(mProviders), end(mProviders), [&provider](const auto &elem) {
            return provider->GetId() == elem.lock()->GetId()
                   && provider->GetComponent().lock()->GetId()
                          == elem.lock()->GetComponent().lock()->GetId();
        });
    if (iter == mProviders.end())
    {
        return;
    }

    // Detach provider.
    provider->RemoveConsumer(GetInstance());
    mProviders.erase(iter, mProviders.end());
}

shared_ptr<IValueSet> Input::GetValues()
{
    Update();

    return mValues;
}

void Input::Update()
{
    // Asuming all values accepted is fitted with elmentset and timeset. Then,
    // just add the values directly.
    vector<shared_ptr<IValueSet>> acceptedValues;
    for (const auto &provider : mProviders)
    {
        // Here we hasn't specified the querier, because the AdaptedOutputs
        // are used to adapt this input item.
        const auto &valueset = provider.lock()->GetValues();
        if (valueset->GetIndexCount({0}) == 0)
            continue;
        acceptedValues.emplace_back(valueset);
    }
    AcceptValues(acceptedValues);
}

bool Input::IsValidQuerySpecifier(const shared_ptr<IBaseExchangeItem> &querier) const
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

void Input::AcceptValues(const vector<shared_ptr<IValueSet>> &values)
{
    for (int t = 0; t < (int)mTimeSet->GetTimes().size(); ++t)
    {
        for (int e = 0; e < mElementSet->GetElementCount(); ++e)
        {
            real value = 0.0;
            for (const auto &valueset : values)
            {
                if (t >= valueset->GetIndexCount({0})
                    || e >= valueset->GetIndexCount({0, 0}))
                    continue;

                real data = any_cast<real>(valueset->GetValue({t, e}));
                any  miss = valueset->GetValueDefinition()->GetMissingDataValue();

                if (data == any_cast<real>(miss))
                    continue;

                value += data;
            }

            mValues->SetOrAddValue({t, e}, value);
        }
    }
}

void Input::Reset()
{
    mValues.reset();
    mElementSet.reset();
    mTimeSet.reset();
    mComponent.reset();
    mProviders.clear();

    BroadcastEventWithMsg("Input item reseted.");
    mItemListeners.Clear();
}

void Input::AddListener(
    const function<void(shared_ptr<ExchangeItemChangeEventArgs>)> &func)
{
    mItemListeners += func;
}

void Input::RemoveListener(
    const function<void(shared_ptr<ExchangeItemChangeEventArgs>)> &func)
{
    mItemListeners -= func;
}

shared_ptr<ITimeSet> Input::GetTimeExtent() const
{
    return mTimeSet;
}

shared_ptr<ITime> Input::GetCurrTime() const
{
    return ExtensionMethods::End(mTimeSet->GetTimeHorizon());
}

shared_ptr<Input> Input::GetInstance()
{
    return shared_from_this();
}

void Input::BroadcastEventWithMsg(const string &msg)
{
    if (!mItemListeners.IsEmpty())
    {
        mEventArg->SetExchangeItem(GetInstance());
        mEventArg->SetMessages("ExchangeItem(" + mId + "): " + msg);
        mItemListeners.Invoke(mEventArg);
    }
}

}  // namespace OpenOasis::CommImp