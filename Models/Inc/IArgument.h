/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IArgument.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide an argument interface.
 *
 ** ***********************************************************************************/
#pragma once
#include "IIdentifiable.h"
#include <any>
#include <vector>
#include <typeinfo>


namespace OpenOasis
{
/// @brief Class for providing arguments for an `ILinkableComponent` and others.
class IArgument : public IIdentifiable
{
public:
    /// @brief Gets the type of the value of the argument.
    virtual const std::type_info &GetValueType() const = 0;

    /// @brief Specifies whether the argument is optional.
    ///
    /// If false, the argument's value must be set before it could be used.
    virtual bool IsOptional() const = 0;

    /// @brief Defines whether the value property can be edited.
    virtual bool IsReadOnly() const = 0;

    /// @brief Gets the current value of the argument.
    ///
    /// If no value has been set yet, default value is returned.
    virtual std::any GetValue() const = 0;

    /// @brief Sets the argument value, if settable.
    virtual void SetValue(std::any value) = 0;

    /// @brief The default value of the argument.
    virtual std::any GetDefaultValue() const = 0;

    /// @brief List of possible allowed values for the argument.
    virtual std::vector<std::any> GetPossibleValues() const = 0;
};

}  // namespace OpenOasis
