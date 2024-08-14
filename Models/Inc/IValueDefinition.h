/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IValueDefinition.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide value definition description of an OpenOasis entity.
 *
 ** ***********************************************************************************/
#pragma once
#include "IDescribable.h"
#include <any>
#include <typeinfo>


namespace OpenOasis
{
/// @brief Class describes value definition.
class IValueDefinition : public IDescribable
{
public:
    /// @brief Gets object value type.
    virtual const std::type_info &GetValueType() const = 0;

    /// @brief Gets flag representing missing data.
    virtual std::any GetMissingDataValue() const = 0;
};

}  // namespace OpenOasis
