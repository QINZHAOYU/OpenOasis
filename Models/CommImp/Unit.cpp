/** ***********************************************************************************
 *    @File      :  Unit.cpp
 *    @Brief     :  Generic definition for a unit for a quantity.
 *
 ** ***********************************************************************************/
#include "Unit.h"
#include "Dimension.h"


namespace OpenOasis::CommImp
{
using namespace std;

Unit::Unit(const shared_ptr<IUnit> &source)
{
    mConversionFactor = source->GetConversionFactorToSI();
    mConversionOffset = source->GetOffSetToSI();
    mDescription      = source->GetDescription();
    mCaption          = source->GetCaption();

    mDimension = make_shared<Dimension>(source->GetDimension());
}

Unit::Unit(Unit &&obj)
{
    mDescription      = obj.mDescription;
    mCaption          = obj.mCaption;
    mDimension        = obj.mDimension;
    mConversionFactor = obj.mConversionFactor;
    mConversionOffset = obj.mConversionOffset;
}

Unit::Unit(PredefinedUnits predefinedUnit)
{
    switch (predefinedUnit)
    {
    case PredefinedUnits::Meter:
        mCaption     = "m";
        mDescription = "meter";
        mDimension   = make_shared<Dimension>(PredefinedDimensions::Length);
        break;
    case PredefinedUnits::LiterPerSecond:
        mCaption          = "L/s";
        mDescription      = "liter per second";
        mConversionFactor = 0.001;
        mDimension        = make_shared<Dimension>(PredefinedDimensions::VolumePerTime);
        break;
    case PredefinedUnits::Liter:
        mCaption          = "L";
        mDescription      = "Liter";
        mConversionFactor = 0.001;  // turn (liter) to (m3).
        mDimension        = make_shared<Dimension>(PredefinedDimensions::Volume);
        break;
    case PredefinedUnits::CubicMeterPerSecond:
        mCaption     = "m3/s";
        mDescription = "cubic meter per second";
        mDimension   = make_shared<Dimension>(PredefinedDimensions::VolumePerTime);
        break;
    case PredefinedUnits::MillimeterPerDay:
        mCaption          = "mm/day";
        mDescription      = "millimeters per day";
        mConversionFactor = 1.15741E-08;  // turn (mm/d) to (m/s).
        mDimension = make_shared<Dimension>(PredefinedDimensions::VolumePerTimePerArea);
        break;
    }
}

Unit::Unit(
    shared_ptr<IDimension> dim, const string &caption, const string &description,
    double conversionFactor, double conversionOffset)
{
    mDimension        = dim;
    mCaption          = caption;
    mDescription      = description;
    mConversionFactor = conversionFactor;
    mConversionOffset = conversionOffset;
}

string Unit::GetDescription() const
{
    return mDescription;
}

void Unit::SetDescription(const string &value)
{
    mDescription = value;
}

string Unit::GetCaption() const
{
    return mCaption;
}

void Unit::SetCaption(const string &value)
{
    mCaption = value;
}

shared_ptr<IDimension> Unit::GetDimension() const
{
    return mDimension;
}

double Unit::GetConversionFactorToSI() const
{
    return mConversionFactor;
}

double Unit::GetOffSetToSI() const
{
    return mConversionOffset;
}

bool Unit::EqualTo(const std::shared_ptr<IUnit> &obj) const
{
    if (!obj)
    {
        return false;
    }

    if (mCaption != obj->GetCaption())
    {
        return false;
    }
    if (mDescription != obj->GetDescription())
    {
        return false;
    }
    if (mDimension != obj->GetDimension())
    {
        return false;
    }
    if (mConversionFactor != obj->GetConversionFactorToSI())
    {
        return false;
    }
    if (mConversionOffset != obj->GetOffSetToSI())
    {
        return false;
    }

    return true;
}

}  // namespace OpenOasis::CommImp