/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Quantity.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Generic definition for a variable, inherited from `IQuantity`.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IQuantity.h"
#include <memory>
#include <any>


namespace OpenOasis
{
namespace CommImp
{
/// @brief Generic definition for a quantity variable.
class Quantity : public IQuantity
{
protected:
    std::shared_ptr<IUnit> mUnit;
    std::string            mDescription;
    std::string            mCaption;
    std::any               mMissingDataValue;

public:
    virtual ~Quantity()
    {}

    Quantity(const std::shared_ptr<IQuantity> &source);
    Quantity(Quantity &&obj);
    Quantity(
        std::shared_ptr<IUnit> unit, const std::string &caption,
        const std::string &description, const std::any &miss = -9999);

    bool EqualTo(const std::shared_ptr<IQuantity> &obj) const;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IDescribable`.
    //

    std::string GetDescription() const override;

    void SetDescription(const std::string &value) override;

    std::string GetCaption() const override;

    void SetCaption(const std::string &value) override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IValueDefinition`.
    //

    std::any GetMissingDataValue() const override;

    const std::type_info &GetValueType() const override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IQuantity`.
    //

    std::shared_ptr<IUnit> GetUnit() const override;
};

}  // namespace CommImp
}  // namespace OpenOasis