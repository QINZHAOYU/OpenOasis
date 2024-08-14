/** ***********************************************************************************
 *    @File      :  SpaceAdaptor.cpp
 *    @Brief     :  An adapted output item that does element mapping.
 *
 ** ***********************************************************************************/
#include "SpaceMapAdaptor.h"
#include "ExtensionMethods.h"
#include "Models/CommImp/Input.h"
#include "Models/CommImp/ValueSet2D.h"
#include "Models/Utils/Exception.h"


namespace OpenOasis::CommImp::DevSupports
{
using namespace Spatial;
using namespace Utils;
using namespace std;


SpaceMapAdaptor::SpaceMapAdaptor(
    shared_ptr<IIdentifiable> methodId, const shared_ptr<IOutput> &adaptee,
    shared_ptr<IElementSet> target) :
    AbstractAdaptedOutput(adaptee->GetId() + "->" + methodId->GetId(), adaptee)
{
    mMethodId = methodId;
    mTarget   = target;

    // The query item must match the adaptee on every point but the timeset.
    mQuery = make_shared<Input>(mId, adaptee->GetComponent().lock());
    mQuery->SetCaption(mCaption);
    mQuery->SetDescription(mDescription);
    mQuery->SetElementSet(adaptee->GetElementSet());

    // Initialize the ElementMapper.
    mElementMapper = make_shared<ElementMapper>();
    mElementMapper->Initialise(mMethodId, mOutput.lock()->GetElementSet(), mTarget);
}

void SpaceMapAdaptor::Reset()
{
    mElementMapper.reset();
    mMethodId.reset();
    mOutput.reset();
    mQuery.reset();
    mTarget.reset();
    mConsumers.clear();
    mAdaptedOutputs.clear();

    BroadcastEventWithMsg("Output item reseted.");
    mItemChanged.Clear();
}

void SpaceMapAdaptor::SetValues(shared_ptr<IValueSet> value)
{}

shared_ptr<IValueDefinition> SpaceMapAdaptor::GetValueDefinition() const
{
    return mOutput.lock()->GetValueDefinition();
}

shared_ptr<IElementSet> SpaceMapAdaptor::GetElementSet() const
{
    return mOutput.lock()->GetElementSet();
}

void SpaceMapAdaptor::SetTimeSet(shared_ptr<ITimeSet> times)
{}

shared_ptr<ITimeSet> SpaceMapAdaptor::GetTimeSet() const
{
    return mOutput.lock()->GetTimeSet();
}

void SpaceMapAdaptor::Refresh()
{
    for (auto adaptedOutput : mAdaptedOutputs)
    {
        adaptedOutput->Refresh();
    }
}

void SpaceMapAdaptor::SetElementSet(shared_ptr<IElementSet> elements)
{}

shared_ptr<IValueSet> SpaceMapAdaptor::GetValues()
{
    shared_ptr<IBaseExchangeItem> querier = mConsumers.back().lock();

    // Time set of query must be defined and have at least 1 time.
    if (querier->GetTimeSet() == nullptr || querier->GetTimeSet()->GetTimes().empty())
    {
        throw runtime_error(StringHelper::FormatSimple(
            "Invalid query specifier [{}] for in GetValues() call to space adaptor [{}] .",
            querier->GetId(),
            mId));
    }

    // Set query time to internal query item.
    mQuery->SetTimeSet(querier->GetTimeSet());

    const auto &incomingValues = mOutput.lock()->GetValues();
    auto        resultValues   = ElementMapper::CreateResultValueSet(
        ExtensionMethods::TimesCount(incomingValues),
        GetSpatialDefinition()->GetElementCount());

    auto result = dynamic_pointer_cast<ValueSet2D>(resultValues);
    result->SetValueDefinition(mOutput.lock()->GetValueDefinition());

    // Transform the values from the adaptee.
    mElementMapper->MapValues(result, incomingValues);

    return result;
}

void SpaceMapAdaptor::GetValues(
    const shared_ptr<IValueSet>         &targetSet,
    const shared_ptr<IBaseExchangeItem> &querySpecifier)
{
    if (!dynamic_pointer_cast<IInput>(querySpecifier))
    {
        throw runtime_error("Get Values can only be called with an Input as argument.");
    }

    // Set query time to internal query item.
    mQuery->SetTimeSet(querySpecifier->GetTimeSet());

    const auto &incomingValues = mOutput.lock()->GetValues();

    // Transform the values from the adaptee
    mElementMapper->MapValues(targetSet, incomingValues);
}

shared_ptr<ITimeSet> SpaceMapAdaptor::GetTimeExtent() const
{
    return mOutput.lock()->GetTimeSet();
}

shared_ptr<ITime> SpaceMapAdaptor::GetCurrTime() const
{
    return ExtensionMethods::End(GetTimeExtent()->GetTimeHorizon());
}

shared_ptr<ISpatialDefinition> SpaceMapAdaptor::GetSpatialDefinition() const
{
    return dynamic_pointer_cast<ISpatialDefinition>(mTarget);
}

shared_ptr<SpaceMapAdaptor> SpaceMapAdaptor::GetInstance()
{
    return dynamic_pointer_cast<SpaceMapAdaptor>(shared_from_this());
}

}  // namespace OpenOasis::CommImp::DevSupports