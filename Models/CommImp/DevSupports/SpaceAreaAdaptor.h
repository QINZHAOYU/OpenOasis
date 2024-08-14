/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  SpaceAreaAdaptor.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  AdaptedOutput multiplies the values of the valueset with the area.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/CommImp/AbstractAdaptedOutput.h"
#include "Models/CommImp/Arguments.h"
#include "Models/Utils/CommConstants.h"


namespace OpenOasis::CommImp::DevSupports
{
/// @brief A IAdaptedOutput that multiplies the values of the valueset with the area
/// of the element to some power. By default the power is one.
///
/// Assumes that the "IElementSet" has "ElementType::Polygon" as elements in the
/// "SpatialDefinition" and "IValueDefinition.ValueType" as typeof(double).
class SpaceAreaAdaptor : public AbstractAdaptedOutput
{
protected:
    Utils::real                     mAreaExponent;
    std::shared_ptr<ArgumentDouble> mAreaArgument;

    std::vector<Utils::real>   mFactors;
    std::shared_ptr<IQuantity> mQuantity;

public:
    virtual ~SpaceAreaAdaptor()
    {}

    SpaceAreaAdaptor(const std::string &id, const std::shared_ptr<IOutput> &adaptee);

    void Initialize() override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Override methods.
    //

    virtual void Reset() override;

    virtual void SetValues(std::shared_ptr<IValueSet> value) override;

    virtual std::shared_ptr<IValueDefinition> GetValueDefinition() const override;

    virtual std::shared_ptr<IElementSet> GetElementSet() const override;

    virtual void SetTimeSet(std::shared_ptr<ITimeSet> times) override;

    virtual std::shared_ptr<ITimeSet> GetTimeSet() const override;

    virtual void Refresh() override;

    virtual void SetElementSet(std::shared_ptr<IElementSet> elements) override;

    virtual std::shared_ptr<IValueSet> GetValues() override;

    virtual std::shared_ptr<ITimeSet> GetTimeExtent() const override;

    virtual std::shared_ptr<ITime> GetCurrTime() const override;

    virtual std::shared_ptr<ISpatialDefinition> GetSpatialDefinition() const override;

protected:
    void CalculateFactors(const std::shared_ptr<IElementSet> &elementSet);
    void UpdateQuantity();

    std::shared_ptr<SpaceAreaAdaptor> GetInstance();
};

}  // namespace OpenOasis::CommImp::DevSupports