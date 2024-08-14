/** ***********************************************************************************
 *    @File      :  Quantity.cpp
 *    @Brief     :  Generic definition for a variable, inherited from `IQuantity`.
 *
 ** ***********************************************************************************/
#include "Quantity.h"
#include "Unit.h"


namespace OpenOasis::CommImp
{
using namespace std;

Quantity::Quantity(const shared_ptr<IQuantity> &source)
{
    mDescription      = source->GetDescription();
    mCaption          = source->GetCaption();
    mMissingDataValue = source->GetMissingDataValue();

    mUnit = make_shared<Unit>(source->GetUnit());
}

Quantity::Quantity(Quantity &&obj)
{
    mDescription      = obj.mDescription;
    mCaption          = obj.mCaption;
    mUnit             = obj.mUnit;
    mMissingDataValue = obj.mMissingDataValue;
}

Quantity::Quantity(
    shared_ptr<IUnit> unit, const string &caption, const string &description,
    const any &miss)
{
    mUnit             = unit;
    mDescription      = description;
    mCaption          = caption;
    mMissingDataValue = miss;
}

string Quantity::GetCaption() const
{
    return mCaption;
}

void Quantity::SetCaption(const string &value)
{
    mCaption = value;
}

string Quantity::GetDescription() const
{
    return mDescription;
}

void Quantity::SetDescription(const string &value)
{
    mDescription = value;
}

any Quantity::GetMissingDataValue() const
{
    return mMissingDataValue;
}

const type_info &Quantity::GetValueType() const
{
    return typeid(*this);
}

shared_ptr<IUnit> Quantity::GetUnit() const
{
    return mUnit;
}

bool Quantity::EqualTo(const shared_ptr<IQuantity> &obj) const
{
    if (!obj)
    {
        return false;
    }

    if (mCaption != obj->GetCaption())
    {
        return false;
    }
    if (mUnit != obj->GetUnit())
    {
        return false;
    }
    if (mDescription != obj->GetDescription())
    {
        return false;
    }

    return true;
}

}  // namespace OpenOasis::CommImp
