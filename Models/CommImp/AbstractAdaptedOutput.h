/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  AbstractAdaptedOutput.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  A class contains operations the providing component
 *                  should carry out on the data.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IAdaptedOutput.h"
#include "Models/Inc/ISpatialDefinition.h"
#include "Models/CommImp/Output.h"
#include <unordered_map>


namespace OpenOasis
{
namespace CommImp
{
/// @brief The `AbstractAdaptedOutput` class contains operations the providing
/// component should carry out on the data.
class AbstractAdaptedOutput : public IAdaptedOutput,
                              public ITimeExtension,
                              public ISpaceExtension,
                              public std::enable_shared_from_this<AbstractAdaptedOutput>
{
protected:
    /// The `mItemChanged` event is fired when the content of current item has changed.
    /// This might be because its ValueDefinition has changed, its TimeSet has changed,
    /// its ElementSet has changed, its Values have changed, etc.
    EventHandler<void, std::shared_ptr<ExchangeItemChangeEventArgs>> mItemChanged;

    std::shared_ptr<ExchangeItemChangeEventArgs> mEventArg;

    std::unordered_map<std::string, std::shared_ptr<IArgument>> mArguments;

    std::string mId          = "";
    std::string mCaption     = "";
    std::string mDescription = "";

    std::shared_ptr<IValueDefinition> mQunatity;

    /// 'mOutput', which the adaptor item belongs to; 'mConsumers', which this adaptor
    /// item has registered.
    std::weak_ptr<IOutput>             mOutput;
    std::vector<std::weak_ptr<IInput>> mConsumers;

    std::vector<std::shared_ptr<IAdaptedOutput>> mAdaptedOutputs;

public:
    virtual ~AbstractAdaptedOutput()
    {}

protected:
    AbstractAdaptedOutput(const std::string &id);
    AbstractAdaptedOutput(
        const std::string &id, const std::shared_ptr<IOutput> &adaptee);

public:
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

    virtual std::shared_ptr<IValueSet> GetValues() override = 0;

    virtual void Reset() override = 0;

    virtual void SetValues(std::shared_ptr<IValueSet> value) override = 0;

    virtual std::shared_ptr<IValueDefinition> GetValueDefinition() const override;

    virtual std::weak_ptr<ILinkableComponent> GetComponent() const override;

    virtual std::shared_ptr<IElementSet> GetElementSet() const override = 0;

    virtual std::shared_ptr<ITimeSet> GetTimeSet() const override = 0;

    virtual void SetTimeSet(std::shared_ptr<ITimeSet> times) override = 0;

    virtual void SetElementSet(std::shared_ptr<IElementSet> elements) override = 0;

    virtual void AddListener(
        const std::function<void(std::shared_ptr<ExchangeItemChangeEventArgs>)> &func)
        override;

    virtual void RemoveListener(
        const std::function<void(std::shared_ptr<ExchangeItemChangeEventArgs>)> &func)
        override;

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
    // Implement methods inherited from "IAdaptedOutput"
    //

    virtual void Initialize() override;

    virtual std::weak_ptr<IOutput> GetAdaptee() const override;

    virtual void SetAdaptee(std::shared_ptr<IOutput> adaptee) override;

    virtual std::vector<std::shared_ptr<IArgument>> GetArguments() const override;

    virtual void Refresh() override = 0;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods extended from `ITimeExtension` and `ISpatialExtension`.
    //

    virtual std::shared_ptr<ITimeSet> GetTimeExtent() const override = 0;

    virtual std::shared_ptr<ITime> GetCurrTime() const override = 0;

    virtual std::shared_ptr<ISpatialDefinition>
    GetSpatialDefinition() const override = 0;

protected:
    std::shared_ptr<IAdaptedOutput> GetInstance();

    void BroadcastEventWithMsg(const std::string &msg);
};


}  // namespace CommImp
}  // namespace OpenOasis