/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Unit.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Generic definition for a unit for a quantity.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IUnit.h"


namespace OpenOasis
{
namespace CommImp
{
/// @brief Predefined unit types for quantities in common scenarios.
enum class PredefinedUnits
{
    Meter,                // (m)
    Liter,                // (L or 0.001 m^3)
    CubicMeterPerSecond,  // (m^3/s)
    MillimeterPerDay,     // (mm/s)
    LiterPerSecond        // (L/s)
};


/// @brief The Unit class defines a unit for a quantity.
class Unit : public IUnit
{
protected:
    std::string mDescription = "";
    std::string mCaption     = "";

    std::shared_ptr<IDimension> mDimension = nullptr;

    double mConversionFactor = 1;
    double mConversionOffset = 0;

public:
    virtual ~Unit()
    {}

    Unit(const std::shared_ptr<IUnit> &source);
    Unit(PredefinedUnits predefUnit);
    Unit(Unit &&obj);
    Unit(
        std::shared_ptr<IDimension> dim, const std::string &caption,
        const std::string &description, double conversionFactor,
        double conversionOffset);

    bool EqualTo(const std::shared_ptr<IUnit> &obj) const;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IDescribable`.
    //

    std::string GetDescription() const override;

    void SetDescription(const std::string &value) override;

    std::string GetCaption() const override;

    void SetCaption(const std::string &value) override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IUnit`.
    //

    std::shared_ptr<IDimension> GetDimension() const override;

    double GetConversionFactorToSI() const override;

    double GetOffSetToSI() const override;
};

}  // namespace CommImp
}  // namespace OpenOasis