/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IUnit.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To describe the physical unit of `IQuantity`.
 *
 ** ***********************************************************************************/
#pragma once
#include "IDescribable.h"
#include "IDimension.h"
#include <memory>


namespace OpenOasis
{
/// @brief Unit interface describes the physical unit.
class IUnit : public IDescribable
{
public:
    /// @brief The unit's dimension.
    virtual std::shared_ptr<IDimension> GetDimension() const = 0;

    /// @brief Conversion factor to SI.
    /// ('A' in: SI-value = A * quantValue + B).
    virtual double GetConversionFactorToSI() const = 0;

    /// @brief OffSet to SI.
    /// ('B' in: SI-value = A * quantValue + B).
    virtual double GetOffSetToSI() const = 0;
};

}  // namespace OpenOasis
