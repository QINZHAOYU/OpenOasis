/** ***********************************************************************************
 *    @File      :  SpaceAreaAdaptor.cpp
 *    @Brief     :  AdaptedOutput multiplies the values of the valueset with the area.
 *
 ** ***********************************************************************************/
#include "SpaceAreaAdaptor.h"
#include "ElementMapper.h"
#include "ExtensionMethods.h"
#include "Models/CommImp/Dimension.h"
#include "Models/CommImp/Unit.h"
#include "Models/CommImp/Quantity.h"
#include "Models/Utils/Exception.h"
#include "Models/CommImp/Spatial/GeomCalculator.h"
#include "ThirdPart/MagicEnum/WrapMagicEnum.hpp"


namespace OpenOasis::CommImp::DevSupports
{
using namespace Spatial;
using namespace Utils;
using namespace std;

SpaceAreaAdaptor::SpaceAreaAdaptor(
    const string &id, const shared_ptr<IOutput> &adaptee) :
    AbstractAdaptedOutput(id, adaptee)
{
    mAreaExponent = 1.0;
    mAreaArgument = make_shared<ArgumentDouble>("AreaExponent", 1.0);

    mArguments.emplace(make_pair("area", mAreaArgument));
    Initialize();

    // Check the adaptee data.
    auto elementSet = adaptee->GetElementSet();
    if (elementSet == nullptr)
    {
        throw invalid_argument("Adaptee must have an IElementSet as SpatialDefinition");
    }
    if (elementSet->GetElementType() != ElementType::Polygon)
    {
        throw invalid_argument(
            "Adaptee must have a SpatialDefinition having polygons as elements");
    }

    if (adaptee->GetValueDefinition()->GetValueType() != typeid(real))
    {
        throw invalid_argument("Adaptee valuetype must be typeof(float point)");
    }

    if (!dynamic_pointer_cast<IQuantity>(adaptee->GetValueDefinition()))
    {
        throw invalid_argument("Adaptee valueDefinition must be an IQuantity");
    }
}

void SpaceAreaAdaptor::Initialize()
{
    CalculateFactors(GetElementSet());
    UpdateQuantity();
}

void SpaceAreaAdaptor::SetValues(shared_ptr<IValueSet> value)
{}

shared_ptr<IValueDefinition> SpaceAreaAdaptor::GetValueDefinition() const
{
    return mOutput.lock()->GetValueDefinition();
}

shared_ptr<IElementSet> SpaceAreaAdaptor::GetElementSet() const
{
    return mOutput.lock()->GetElementSet();
}

void SpaceAreaAdaptor::SetTimeSet(shared_ptr<ITimeSet> times)
{}

shared_ptr<ITimeSet> SpaceAreaAdaptor::GetTimeSet() const
{
    return mOutput.lock()->GetTimeSet();
}

void SpaceAreaAdaptor::Refresh()
{
    for (auto adaptedOutput : mAdaptedOutputs)
    {
        adaptedOutput->Refresh();
    }
}

void SpaceAreaAdaptor::SetElementSet(shared_ptr<IElementSet> elements)
{}

void SpaceAreaAdaptor::Reset()
{
    mAreaArgument.reset();
    mQuantity.reset();
    mOutput.reset();
    mConsumers.clear();
    mAdaptedOutputs.clear();
    mFactors.clear();

    mAreaExponent = 1.0;

    BroadcastEventWithMsg("Output item reseted.");
    mItemChanged.Clear();

    Initialize();
}

shared_ptr<ITimeSet> SpaceAreaAdaptor::GetTimeExtent() const
{
    return mOutput.lock()->GetTimeSet();
}

shared_ptr<ITime> SpaceAreaAdaptor::GetCurrTime() const
{
    return ExtensionMethods::End(GetTimeExtent()->GetTimeHorizon());
}

shared_ptr<ISpatialDefinition> SpaceAreaAdaptor::GetSpatialDefinition() const
{
    return GetElementSet();
}

void SpaceAreaAdaptor::CalculateFactors(const shared_ptr<IElementSet> &elementSet)
{
    mFactors      = vector<real>(elementSet->GetElementCount());
    mAreaExponent = any_cast<real>(mAreaArgument->GetValue());

    for (std::size_t i = 0; i < mFactors.size(); i++)
    {
        Polygon element = ElementMapper::CreateXYPolygon(elementSet, i);
        real    area    = GeomCalculator::CalculateAreaOfPolygon(element);
        if (mAreaExponent == 1)
        {
            mFactors[i] = area;
        }
        else if (mAreaExponent == -1)
        {
            mFactors[i] = 1.0 / area;
        }
        else
        {
            mFactors[i] = pow(area, mAreaExponent);
        }
    }
}

void SpaceAreaAdaptor::UpdateQuantity()
{
    auto sourceQuantity =
        dynamic_pointer_cast<IQuantity>(mOutput.lock()->GetValueDefinition());
    auto sourceUnit = sourceQuantity->GetUnit();

    auto dimension = make_shared<Dimension>();
    for (auto dimBase : magic_enum::enum_values<DimensionBase>())
    {
        dimension->SetPower(dimBase, sourceUnit->GetDimension()->GetPower(dimBase));
    }

    dimension->SetPower(
        DimensionBase::Length,
        dimension->GetPower(DimensionBase::Length) + mAreaExponent);

    string pu = StringHelper::FormatSimple(" * m^{}", 2 * mAreaExponent);
    string pq = StringHelper::FormatSimple(" * area^{}", mAreaExponent);

    auto unit = make_shared<Unit>(
        dimension,
        sourceUnit->GetCaption() + pu,
        sourceUnit->GetDescription() + pu,
        sourceUnit->GetConversionFactorToSI(),
        sourceUnit->GetOffSetToSI());

    mQuantity = make_shared<Quantity>(
        unit, sourceQuantity->GetCaption() + pq, sourceQuantity->GetDescription() + pq);
}

shared_ptr<IValueSet> SpaceAreaAdaptor::GetValues()
{
    return ExtensionMethods::MultiplyElementValues(
        mOutput.lock()->GetValues(), mFactors);
}

shared_ptr<SpaceAreaAdaptor> SpaceAreaAdaptor::GetInstance()
{
    return dynamic_pointer_cast<SpaceAreaAdaptor>(shared_from_this());
}

}  // namespace OpenOasis::CommImp::DevSupports