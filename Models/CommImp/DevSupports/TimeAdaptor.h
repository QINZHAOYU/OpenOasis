/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  TimeAdaptor.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Class contains temporal adaptive data operations.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IAdaptedOutput.h"
#include "Models/Inc/ILinkableComponent.h"
#include "Models/CommImp/AbstractAdaptedOutput.h"
#include "Models/CommImp/Temporal/TimeBuffer.h"
#include "Models/Utils/EventHandler.h"


namespace OpenOasis::CommImp::DevSupports
{
using namespace Temporal;
using namespace Utils;

/// @brief Class contains temporal operations the providing component should carry out
/// on the data.
class TimeAdaptor : public AbstractAdaptedOutput
{
protected:
    /// The TimeSet and ValueSet2D contained by the adaptor item.
    TimeBuffer mBuffers;

    /// The mTimeSet contains times saved.
    std::shared_ptr<ITimeSet> mTimeSet;

public:
    virtual ~TimeAdaptor()
    {}

    TimeAdaptor(const std::string &id);
    TimeAdaptor(const std::string &id, const std::shared_ptr<IOutput> &adaptee);

    ///////////////////////////////////////////////////////////////////////////////////
    // Override methods.
    //

    virtual std::shared_ptr<IValueSet> GetValues() override;

    virtual void Reset() override;

    virtual void SetValues(std::shared_ptr<IValueSet> value) override;

    virtual std::shared_ptr<ITimeSet> GetTimeSet() const override;

    virtual std::shared_ptr<IElementSet> GetElementSet() const override;

    virtual void SetTimeSet(std::shared_ptr<ITimeSet> times) override;

    virtual void SetElementSet(std::shared_ptr<IElementSet> elements) override;

    virtual void Refresh() override;

    virtual std::shared_ptr<ITimeSet> GetTimeExtent() const override;

    virtual std::shared_ptr<ITime> GetCurrTime() const override;

    virtual std::shared_ptr<ISpatialDefinition> GetSpatialDefinition() const override;

protected:
    std::shared_ptr<TimeAdaptor> GetInstance();

    virtual bool Update(const std::shared_ptr<IBaseExchangeItem> &specifier);

    void RefreshAdaptedOutputs();
};

}  // namespace OpenOasis::CommImp::DevSupports