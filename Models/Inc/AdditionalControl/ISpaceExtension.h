/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ISpaceExtension.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Spatial information (usually an element set) on the values.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/ISpatialDefinition.h"
#include <memory>


namespace OpenOasis
{
namespace AdditionalControl
{
class ISpaceExtension
{
public:
    /// @brief Spatial information (usually an element set) on the values that are
    /// available in an output exchange item or required by an input exchange item.
    virtual std::shared_ptr<ISpatialDefinition> GetSpatialDefinition() const = 0;
};

}  // namespace AdditionalControl
}  // namespace OpenOasis
