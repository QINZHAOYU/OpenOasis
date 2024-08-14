/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IQuantity.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Specify values as an amount of some unit.
 *
 ** ***********************************************************************************/
#pragma once
#include "IUnit.h"
#include "IValueDefinition.h"


namespace OpenOasis
{
/// @brief Class specifies values as an amount of some unit.
class IQuantity : public IValueDefinition
{
public:
    /// @brief Unit of quantity.
    virtual std::shared_ptr<IUnit> GetUnit() const = 0;
};

}  // namespace OpenOasis
