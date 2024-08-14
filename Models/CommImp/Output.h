/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Output.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide an output exchange item that can deliver values.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IOutput.h"
#include "Models/Inc/IAdaptedOutput.h"
#include "Models/Inc/IQuantity.h"
#include "Models/Inc/ILinkableComponent.h"
#include "Models/Inc/ExchangeItemChangeEventArgs.h"
#include "Models/Inc/AdditionalControl/ITimeExtension.h"
#include "Models/Inc/AdditionalControl/ISpaceExtension.h"
#include "Models/Inc/IElementSet.h"
#include "Models/Utils/EventHandler.h"


namespace OpenOasis
{
namespace CommImp
{
using namespace AdditionalControl;
using namespace Utils;

/// @brief  Generic implementation of `IOutput` which delivering values for components.
/// @attention The Copy and Move constructor wouldn't consider registered listeners.
class Output : public IOutput,
               public ITimeExtension,
               public ISpaceExtension,
               public std::enable_shared_from_this<Output>
{
protected:
    /// The item listeners event is fired when the content of current item has changed.
    /// This might be because its ValueDefinition has changed, its TimeSet has changed,
    /// its ElementSet has changed, its Values have changed, etc.
    EventHandler<void, std::shared_ptr<ExchangeItemChangeEventArgs>> mItemListeners;

    std::shared_ptr<ExchangeItemChangeEventArgs> mEventArg;

    std::string mId          = "";
    std::string mCaption     = "";
    std::string mDescription = "";

    /// The mElementSet contains the spatial elements where the output located.
    std::shared_ptr<IElementSet> mElementSet;

    /// The mTimeSet contains times saved.
    std::shared_ptr<ITimeSet> mTimeSet;

    /// The 'mValues' contains
    /// the whole valuse from the earliest to the latest time for all cosumers.
    std::shared_ptr<IValueSet> mValues;

    /// mComponent, which this output item belongs to;'mConsumers', which this
    /// output item has registered.
    std::weak_ptr<ILinkableComponent>  mComponent;
    std::vector<std::weak_ptr<IInput>> mConsumers;

    std::vector<std::shared_ptr<IAdaptedOutput>> mAdaptedOutputs;

public:
    virtual ~Output() = default;

    Output(const std::string &id, const std::shared_ptr<ILinkableComponent> &comp);
    Output(Output &&obj);

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IDescribable`.
    //

    virtual void SetCaption(const std::string &value) override;

    virtual std::string GetCaption() const override;

    virtual std::string GetDescription() const override;

    virtual void SetDescription(const std::string &value) override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IIdentifiable`.
    //

    virtual std::string GetId() const override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IBaseExchangeItem`.
    //

    /// @brief Provides the values matching the value definition specified.
    /// Time and space are not considered in tihs version.
    /// Thus, there's no updating while querier specified.
    virtual std::shared_ptr<IValueSet> GetValues() override;

    virtual void Reset() override;

    virtual void SetValues(std::shared_ptr<IValueSet> value) override;

    virtual std::shared_ptr<IValueDefinition> GetValueDefinition() const override;

    virtual std::weak_ptr<ILinkableComponent> GetComponent() const override;

    virtual std::shared_ptr<IElementSet> GetElementSet() const override;

    virtual std::shared_ptr<ITimeSet> GetTimeSet() const override;

    virtual void SetTimeSet(std::shared_ptr<ITimeSet> times) override;

    virtual void SetElementSet(std::shared_ptr<IElementSet> elements) override;

    virtual void AddListener(const ListenFunc &func) override;

    virtual void RemoveListener(const ListenFunc &func) override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IOutput`.
    //

    virtual void RemoveConsumer(const std::shared_ptr<IInput> &consumer) override;

    virtual std::vector<std::weak_ptr<IInput>> GetConsumers() const override;

    virtual void AddConsumer(std::shared_ptr<IInput> consumer) override;

    virtual std::vector<std::shared_ptr<IAdaptedOutput>>
    GetAdaptedOutputs() const override;

    virtual void
    AddAdaptedOutput(std::shared_ptr<IAdaptedOutput> adaptedOutput) override;

    virtual void
    RemoveAdaptedOutput(const std::shared_ptr<IAdaptedOutput> &adaptedOutput) override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods extended from `ITimeExtension` and `ISpatialExtension`.
    //

    virtual std::shared_ptr<ITimeSet> GetTimeExtent() const override;

    /// @brief Gets the latest time.
    virtual std::shared_ptr<ITime> GetCurrTime() const override;

    virtual std::shared_ptr<ISpatialDefinition> GetSpatialDefinition() const override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Additional methods for iteration and optimization.
    //

    virtual void SetComponent(std::shared_ptr<ILinkableComponent> component);

protected:
    std::shared_ptr<Output> GetInstance();

    virtual void Update();
    virtual bool
    IsValidQuerySpecifier(const std::shared_ptr<IBaseExchangeItem> &specifier) const;

    void RefreshAdaptedOutputs();
    void ReduceValuesAndTimes(const std::shared_ptr<ITime> &lastTime);
    void BroadcastEventWithMsg(const std::string &msg);
};

}  // namespace CommImp
}  // namespace OpenOasis