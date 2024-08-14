/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IOutput.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide an output exchange item that can deliver values from an
 *                 `ILinkableComponent`.
 *
 ** ***********************************************************************************/
#pragma once
#include "IBaseExchangeItem.h"
#include "IInput.h"
#include "IValueSet.h"


namespace OpenOasis
{
class IAdaptedOutput;

/// @brief Output exchange item that can deliver values from an `ILinkableComponent`.
///
/// If an output does not provide the data in the way a consumer would like to have,
/// the output can be adapted by an `IAdaptedOutput`, which can transform
/// the data according to the consumer's wishes.
class IOutput : public IBaseExchangeItem
{
public:
    /// @brief Gets inputs that will consume the value, by calling the `GetValues()`
    /// method. Every input item needs to call the `AddConsumer()` method first.
    ///
    /// Please be aware that the unadulterated value in the output item provided by
    /// the read only 'GetValues()' method, may be called anyway, even if there are
    /// no values available.
    virtual std::vector<std::weak_ptr<IInput>> GetConsumers() const = 0;

    /// @brief Adds a consumer to the output item. Every input item needs
    /// to add itself as a consumer first.
    ///
    /// The `AddConsumer()` method must and will automatically add the consumer
    /// provider by calling the `IInput::AddProvider()` method.
    ///
    /// @throw If a consumer is added that can't be handled, or it's
    /// incompatible with the already added consumers, throw an exception.
    virtual void AddConsumer(std::shared_ptr<IInput> consumer) = 0;

    /// @brief Removes a consumer.
    ///
    /// If an input item isn't interested any longer in calling `GetValues()`
    /// method, it should remove the input item.
    ///
    /// The `RemoveConsumer()` method must and will automatically remove the consumer's
    /// provider by calling the `IInput::RemoveProvider()` method.
    virtual void RemoveConsumer(const std::shared_ptr<IInput> &consumer) = 0;

    /// @brief Gets the adapted outputs.
    ///
    /// As the output item's values have been updated, each `IAdaptedOutput`
    /// its `Refresh()` method must be called after.
    virtual std::vector<std::shared_ptr<IAdaptedOutput>> GetAdaptedOutputs() const = 0;

    /// @brief Removes a `IAdaptedOutput`.
    ///
    /// If an `IAdaptedOutput` is not interested any longer in this output item's
    /// data, it should remove itself by calling this method.
    ///
    /// The `RemoveAdaptedOutput()` method must and will automatically remove
    /// the adapted output's adaptee by setting it to null.
    virtual void
    RemoveAdaptedOutput(const std::shared_ptr<IAdaptedOutput> &adaptedOutput) = 0;

    /// @brief Adds an adapted output to this output item as a transformer.
    ///
    /// The `AddAdaptedOutput()` method must and will automatically add the adapted
    /// output's adaptee by calling its `SetAdaptee()` method.
    ///
    /// @throw If an `IAdaptedOutput` item is added that can not be handled or
    /// is incompatible with the already added `IAdaptedOutput`s,
    /// an exception will be thrown.
    virtual void AddAdaptedOutput(std::shared_ptr<IAdaptedOutput> adaptedOutput) = 0;
};

}  // namespace OpenOasis
