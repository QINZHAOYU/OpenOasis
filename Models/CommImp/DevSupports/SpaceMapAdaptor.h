/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  SpaceAdaptor.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  AdaptedOutput that does element mapping.
 *
 ** ***********************************************************************************/
#pragma once
#include "ElementMapper.h"
#include "Models/Inc/IInput.h"
#include "Models/CommImp/AbstractAdaptedOutput.h"


namespace OpenOasis::CommImp::DevSupports
{
using namespace Spatial;

/// @brief An adapted output item that does element mapping from one spatial definition
/// (elementset) to another.
/// The adapted output retrieves values from the adaptee, and converts them
/// to the target element set that was provided in the constructor.
class SpaceMapAdaptor : public AbstractAdaptedOutput
{
protected:
    std::shared_ptr<ElementMapper> mElementMapper;
    std::shared_ptr<IIdentifiable> mMethodId;
    std::shared_ptr<IElementSet>   mTarget;

    /// Query input, used when calling GetValues on adaptee, i.e.
    std::shared_ptr<IInput> mQuery;

public:
    virtual ~SpaceMapAdaptor()
    {}

    SpaceMapAdaptor(
        std::shared_ptr<IIdentifiable>  methodId,
        const std::shared_ptr<IOutput> &adaptee, std::shared_ptr<IElementSet> target);

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

    /// The SpatialDefinition returned by this adapted output
    /// is the target element set.
    virtual std::shared_ptr<ISpatialDefinition> GetSpatialDefinition() const override;

protected:
    /// @brief Compared to "GetValues(IBaseExchangeItem)",
    /// this version adds the values to the targetSet (for reuse or adding to a
    /// targetSet).
    void GetValues(
        const std::shared_ptr<IValueSet>         &targetSet,
        const std::shared_ptr<IBaseExchangeItem> &querySpecifier);

    std::shared_ptr<SpaceMapAdaptor> GetInstance();
};

}  // namespace OpenOasis::CommImp::DevSupports