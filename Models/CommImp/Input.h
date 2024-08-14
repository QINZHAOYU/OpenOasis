/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Input.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide an input exchange item that can accept values.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IInput.h"
#include "Models/Inc/IQuantity.h"
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

/// @brief Generic implementation of `IInput` item which accepting values for component
/// @attention The Copy and Move constructor wouldn't consider registered listeners.
class Input : public IInput,
              public ITimeExtension,
              public ISpaceExtension,
              public std::enable_shared_from_this<Input>
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

    /// The 'mElementSet' contains the spatial elements where this input located.
    std::shared_ptr<IElementSet> mElementSet;

    /// The mTimeSet contains times required.
    std::shared_ptr<ITimeSet> mTimeSet;

    /// The 'mValues' contains
    /// the whole valuse needed for updating the component once.
    std::shared_ptr<IValueSet> mValues;

    /// 'mComponent', which this input item belongs to; `mProviders`, which this
    /// input item has registered.
    std::weak_ptr<ILinkableComponent>   mComponent;
    std::vector<std::weak_ptr<IOutput>> mProviders;

public:
    virtual ~Input() = default;

    Input(const std::string &id, const std::shared_ptr<ILinkableComponent> &comp);
    Input(Input &&obj);

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

    virtual void AddListener(
        const std::function<void(std::shared_ptr<ExchangeItemChangeEventArgs>)> &func)
        override;

    virtual void RemoveListener(
        const std::function<void(std::shared_ptr<ExchangeItemChangeEventArgs>)> &func)
        override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IInput`.
    //

    virtual std::vector<std::weak_ptr<IOutput>> GetProviders() const override;

    virtual void AddProvider(std::shared_ptr<IOutput> provider) override;

    virtual void RemoveProvider(const std::shared_ptr<IOutput> &provider) override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods extended from `ITimeExtension` and `ISpatialExtension`.
    //

    virtual std::shared_ptr<ITimeSet> GetTimeExtent() const override;

    /// @brief Gets the latest time.
    virtual std::shared_ptr<ITime> GetCurrTime() const override;

    virtual std::shared_ptr<ISpatialDefinition> GetSpatialDefinition() const override;

protected:
    std::shared_ptr<Input> GetInstance();

    virtual void Update();
    virtual bool
    IsValidQuerySpecifier(const std::shared_ptr<IBaseExchangeItem> &specifier) const;

    // TODO: refactor AcceptValues() to be more generic for multi-providers case.
    void AcceptValues(const std::vector<std::shared_ptr<IValueSet>> &values);
    void BroadcastEventWithMsg(const std::string &msg);
};

}  // namespace CommImp
}  // namespace OpenOasis