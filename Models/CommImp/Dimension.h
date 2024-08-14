/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Dimension.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide the dimension for a unit.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IDimension.h"
#include <unordered_map>
#include <memory>


namespace OpenOasis
{
namespace CommImp
{
/// @brief Predefined dimension types for units in common scenarios.
///
/// Such as, `PredefinedDimensions::length` represents the dimension for a length unit,
/// which DimensionBase::Length=1, 0 for all others.
enum class PredefinedDimensions
{
    Length,                  // m^1
    Area,                    // m^2
    Volume,                  // m^3
    LengthPerTime,           // m^1 * s^-1
    VolumePerTime,           // m^3 * s^-1
    VolumePerTimePerLength,  // m^2 * s^-1
    VolumePerTimePerArea,    // m^1 * s^-1
    Mass,                    // kg^1
    MassPerTime              // kg^1 * s^-1
};


/// @brief The Dimension class contains the dimension for a unit.
class Dimension : public IDimension
{
protected:
    std::unordered_map<DimensionBase, double> mBaseDimensionPower;

public:
    virtual ~Dimension()
    {}

    Dimension();
    Dimension(PredefinedDimensions predefDimension);
    Dimension(const std::shared_ptr<IDimension> &source);

    bool EqualTo(const std::shared_ptr<IDimension> &obj);

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IDimension`.
    //

    double GetPower(DimensionBase baseQuantity) const override;

    void SetPower(DimensionBase baseQuantity, double power) override;
};


}  // namespace CommImp
}  // namespace OpenOasis