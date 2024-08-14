/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IBaseExchangeItem.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide an item that can be exchanged, either as input or output.
 *
 ** ***********************************************************************************/
#pragma once
#include "IValueDefinition.h"
#include "IValueSet.h"
#include "IIdentifiable.h"
#include "ITimeSet.h"
#include "IElementSet.h"
#include "ExchangeItemChangeEventArgs.h"
#include <functional>


namespace OpenOasis
{
class ILinkableComponent;

/// @brief Class presenting a item that can be exchanged, either as an input or output.
class IBaseExchangeItem : public IIdentifiable
{
public:
    using ListenFunc =
        std::function<void(std::shared_ptr<ExchangeItemChangeEventArgs>)>;

public:
    /// @brief Definition of the values in the exchange item.
    ///
    /// This `IValueDefinition` should never be returned directly; all implementing
    /// classes should return either `IQuality`, `IQuantity`, or a custom
    /// derived value definition interface.
    virtual std::shared_ptr<IValueDefinition> GetValueDefinition() const = 0;

    /// @brief Resets the exchange item.
    virtual void Reset() = 0;

    /// @brief Provides the values matching the value definition specified.
    /// Extensions can overwrite this base version to
    /// include more details in the query, e.g. time and space.
    ///
    /// For an input item, this means to get the input item's value pulled from its
    /// provider(s). For an output item, this means to provide the values
    /// required by consumers.
    virtual std::shared_ptr<IValueSet> GetValues() = 0;

    /// @brief Gets the exchange item's elements.
    /// If it's id-based, return null.
    virtual std::shared_ptr<IElementSet> GetElementSet() const = 0;

    /// @brief Sets the exchange item's values.
    virtual void SetValues(std::shared_ptr<IValueSet> value) = 0;

    /// @brief Gets the exchange item's available time set.
    /// If it's time independent, return null.
    virtual std::shared_ptr<ITimeSet> GetTimeSet() const = 0;

    /// @brief Sets the exchange item's time set.
    virtual void SetTimeSet(std::shared_ptr<ITimeSet> times) = 0;

    /// @brief Sets the exchange item's elements.
    virtual void SetElementSet(std::shared_ptr<IElementSet> elements) = 0;

    /// @brief Gets the owner of the exchange item.
    ///
    /// For an output exchange item this is the component responsible for providing
    /// the content of the output item. It's possible for an exchange item to
    /// have no owner, in this case the method will return null.
    virtual std::weak_ptr<ILinkableComponent> GetComponent() const = 0;

    /// @brief Adds a listener to the exchange item.
    virtual void AddListener(const ListenFunc &func) = 0;

    /// @brief Removes the listener if it is no more interested at this item.
    virtual void RemoveListener(const ListenFunc &func) = 0;
};

}  // namespace OpenOasis
