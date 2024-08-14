/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ITimeExtension.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Methods that are specific for an time-space component.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/ITimeSet.h"
#include <memory>


namespace OpenOasis
{
namespace AdditionalControl
{
/// @brief Methods that are specific for a time-space component.
class ITimeExtension
{
public:
    /// @brief The property describes in what time span the component can operate.
    /// This can be used to support the user when creating a composition.
    virtual std::shared_ptr<ITimeSet> GetTimeExtent() const = 0;

    /// @brief The property describes what time stamp the component is at.
    virtual std::shared_ptr<ITime> GetCurrTime() const = 0;
};

}  // namespace AdditionalControl
}  // namespace OpenOasis
