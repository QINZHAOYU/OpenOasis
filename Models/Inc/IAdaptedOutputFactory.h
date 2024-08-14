/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IAdaptedOutputFactory.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Factory to create instance of `IAdaptedOutput`.
 *
 ** ***********************************************************************************/
#pragma once
#include "IAdaptedOutput.h"
#include "IOutput.h"


namespace OpenOasis
{
/// @brief Factory class for creating instances of `IAdaptedOutput` items.
class IAdaptedOutputFactory : public IIdentifiable
{
public:
    /// @brief Gets a list of identifier of the available `IAdaptedOutput` that can
    /// make the adaptee match the target.
    ///
    /// If the target is null, the identifiers of all `IAdaptedOutput`s that
    /// can adapt the adaptee are returned.
    ///
    /// @param adaptee `IOutput` to adapt.
    /// @param target `IInput` to adapt the adaptee to, can be `null`.
    ///
    /// @return List of identifier of the available `IAdaptedOutput`.
    virtual std::vector<std::shared_ptr<IIdentifiable>> GetAvailableAdaptedOutputIds(
        const std::shared_ptr<IOutput> &adaptee,
        const std::shared_ptr<IInput>  &target) = 0;

    /// @brief Creates `IAdaptedOutput` adapting the adaptee, so that it fits
    /// the target.
    ///
    /// The adaptedOutputId used must be one of the `IIdentifiable` instances
    /// returned by the `GetAvailableAdaptedOutputIds()` method. The returned
    /// `IAdaptedOutput` will already be registered with this adaptee.
    ///
    /// @param adaptedOutputId The identifier of the `IAdaptedOutput` to create.
    /// @param adaptee `IOutput` to adapt.
    /// @param target `IInput` to adapt the adaptee to, can be `null`.
    ///
    /// @return The `IAdaptedOutput` item adaptes the adaptee to fit the target item.
    virtual std::shared_ptr<IAdaptedOutput> CreateAdaptedOutput(
        const std::shared_ptr<IIdentifiable> &adaptedOutputId,
        const std::shared_ptr<IOutput>       &adaptee,
        const std::shared_ptr<IInput>        &target) = 0;
};

}  // namespace OpenOasis
