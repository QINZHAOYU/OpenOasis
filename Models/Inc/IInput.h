/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IInput.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide an input item that can accept values for an
 *                 `ILinkableComponent` object.
 *
 *    This item is a combination of an `IValueDefinition`, an `IElementSet`, and an
 *    `ITimeSet`. This combination specifies which type of data is required, where
 *    and when, as input for an `ILinkableComponent`.
 *
 ** ***********************************************************************************/
#pragma once
#include "IBaseExchangeItem.h"
#include "IValueSet.h"
#include "ITimeSet.h"


namespace OpenOasis
{
class IOutput;

/// @brief An input item that can accept values for an `ILinkableComponent`.
/// The input item may have multiple providers.
class IInput : public IBaseExchangeItem
{
public:
    /// @brief Gets the providers that this input should get its values from.
    ///
    /// An `IOutput` item provides data while calling its `GetValues()`.
    virtual std::vector<std::weak_ptr<IOutput>> GetProviders() const = 0;

    /// @brief Adds a provider for this input item.
    virtual void AddProvider(std::shared_ptr<IOutput> provider) = 0;

    /// @brief Removes specified provider from this input item.
    virtual void RemoveProvider(const std::shared_ptr<IOutput> &provider) = 0;
};

}  // namespace OpenOasis
