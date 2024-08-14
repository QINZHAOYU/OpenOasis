/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IDimension.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To define the order of dimension in each `DimensionBase` for a unit.
 *
 ** ***********************************************************************************/
#pragma once


namespace OpenOasis
{
/// @brief Enumeration for base dimensions.
enum class DimensionBase
{
    Length,             // Base dimension length.
    Mass,               // Base dimension mass.
    Time,               // Base dimension time.
    ElectricCurrent,    // Base dimension electric current.
    Temperature,        // Base dimension temperature.
    AmountOfSubstance,  // Base dimension amount of substance.
    LuminousIntensity,  // Base dimension luminous intensity.
    Currency            // Base dimension currency.
};


/// @brief Define the order of each dimension in `DimensionBase` for a unit.
class IDimension
{
public:
    /// @brief Gets the power for the requested dimension.
    ///
    /// Example, For a quantity such as flow, which may have the unit m3/s,
    /// the `GetPower` method must work as follows:
    ///     `Flow.GetPower(DimensionBase::AmountOfSubstance)` --> returns  0
    ///     `Flow.GetPower(DimensionBase::Currency)`          --> returns  0
    ///     `Flow.GetPower(DimensionBase::ElectricCurrent)`   --> returns  0
    ///     `Flow.GetPower(DimensionBase::Length)`            --> returns  3
    ///     `Flow.GetPower(DimensionBase::LuminousIntensity)` --> returns  0
    ///     `Flow.GetPower(DimensionBase::Mass)`              --> returns  0
    ///     `Flow.GetPower(DimensionBase::Temperature)`       --> returns  0
    ///     `Flow.GetPower(DimensionBase::Time)`              --> returns -1
    virtual double GetPower(DimensionBase baseQuantity) const = 0;

    /// @brief Sets a power for a base dimension.
    ///
    /// @param baseQuantity The base dimension.
    /// @param power The power.
    virtual void SetPower(DimensionBase baseQuantity, double power) = 0;
};

}  // namespace OpenOasis
