/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IAdaptedOutput.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To adds one or more data operations on top of an output item.
 *
 *    `IAdaptedOutput` objects are created by means of an `IAdaptedOutputFactory`.
 *    It is based on the adaptor design pattern.It adapts an `IOutput` or another
 *    `IAdaptedOutput` to make it suitable for new use or purpose.
 *
 ** ***********************************************************************************/
#pragma once
#include "IArgument.h"
#include "IOutput.h"


namespace OpenOasis
{
/// @brief Extension class for adding data operations on top of an output item.
///
/// The `IAdaptedOutput` extends an `IOutput` item with functionality, such as
/// spatial interpolation, temporal interpolation, unit conversion etc.
class IAdaptedOutput : public IOutput
{
public:
    /// @brief Arguments needed to let the adapted output do its work.
    ///
    /// An unmodifiable list of the (modifiable) arguments should be returned
    /// that can be used to get info on the arguments and to modify argument values.
    ///
    /// @return Unmodifiable list of `IArgument`.
    virtual std::vector<std::shared_ptr<IArgument>> GetArguments() const = 0;

    /// @brief Let the adapted output initialize itself based on the current values
    /// specified by the arguments.
    ///
    /// Only after `Initialize()` is called, the `Refresh()` method might be called.
    /// A component must invoke the `Initialize()` method of all its
    /// adapted outputs at the end of the component's prepare phase.
    /// In case of stacked adapted outputs, the adaptee must be initialized firstly.
    virtual void Initialize() = 0;

    /// @brief `IOutput` item that this `IAdaptedOutput` extracts content from.
    ///
    /// In the adapter design pattern, it is the item being adapted.
    virtual std::weak_ptr<IOutput> GetAdaptee() const = 0;

    /// @brief Sets an output which requiring the provided adaptive data operations.
    virtual void SetAdaptee(std::shared_ptr<IOutput> adaptee) = 0;

    /// @brief Requests the adapted output to refresh itself.
    ///
    /// After updating itself, the adapted output must call `Refresh()` method on
    /// all its adapted outputs, so the chain of outputs refreshes itself.
    virtual void Refresh() = 0;
};

}  // namespace OpenOasis
