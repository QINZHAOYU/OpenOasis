/** ***********************************************************************************
 *    @File      :  Dimension.cpp
 *    @Brief     :  To provide the dimension for a unit.
 *
 ** ***********************************************************************************/
#include "Dimension.h"
#include "ThirdPart/MagicEnum/WrapMagicEnum.hpp"


namespace OpenOasis::CommImp
{
using namespace std;

Dimension::Dimension()
{
    for (auto dimBase : magic_enum::enum_values<DimensionBase>())
    {
        mBaseDimensionPower[dimBase] = 0.;
    }
}

Dimension::Dimension(const shared_ptr<IDimension> &source)
{
    for (auto dimBase : magic_enum::enum_values<DimensionBase>())
    {
        mBaseDimensionPower[dimBase] = source->GetPower(dimBase);
    }
}

Dimension::Dimension(PredefinedDimensions predefinedDimension) : Dimension()
{
    switch (predefinedDimension)
    {
    case PredefinedDimensions::Length: SetPower(DimensionBase::Length, 1); break;
    case PredefinedDimensions::Area: SetPower(DimensionBase::Length, 2); break;
    case PredefinedDimensions::Volume: SetPower(DimensionBase::Length, 3); break;
    case PredefinedDimensions::LengthPerTime:
        SetPower(DimensionBase::Length, 1);
        SetPower(DimensionBase::Time, -1);
        break;
    case PredefinedDimensions::VolumePerTime:
        SetPower(DimensionBase::Length, 3);
        SetPower(DimensionBase::Time, -1);
        break;
    case PredefinedDimensions::VolumePerTimePerLength:
        SetPower(DimensionBase::Length, 2);
        SetPower(DimensionBase::Time, -1);
        break;
    case PredefinedDimensions::VolumePerTimePerArea:
        SetPower(DimensionBase::Length, 1);
        SetPower(DimensionBase::Time, -1);
        break;
    case PredefinedDimensions::Mass: SetPower(DimensionBase::Mass, 1); break;
    case PredefinedDimensions::MassPerTime:
        SetPower(DimensionBase::Mass, 1);
        SetPower(DimensionBase::Time, -1);
        break;
    }
}

double Dimension::GetPower(DimensionBase baseQuantity) const
{
    if (mBaseDimensionPower.count(baseQuantity) == 0)
        return 0.0;
    else
        return mBaseDimensionPower.at(baseQuantity);
}

void Dimension::SetPower(DimensionBase baseQuantity, double power)
{
    mBaseDimensionPower[baseQuantity] = power;
}

bool Dimension::EqualTo(const shared_ptr<IDimension> &obj)
{
    if (!obj)
    {
        return false;
    }

    // Traverse the enumeration.
    for (auto dimBase : magic_enum::enum_values<DimensionBase>())
    {
        if (GetPower(dimBase) != obj->GetPower(dimBase))
        {
            return false;
        }
    }

    return true;
}

}  // namespace OpenOasis::CommImp