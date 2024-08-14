/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ExchangeItemEventArgs.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To contain the information for firing exchange item change event.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/ExchangeItemChangeEventArgs.h"


namespace OpenOasis
{
namespace CommImp
{
/// @brief Generic implementation of `ExchangeItemChangeEventArgs`.
class ExchangeItemEventArgs : public ExchangeItemChangeEventArgs
{
private:
    std::shared_ptr<IBaseExchangeItem> mExchangeItem;
    std::string                        mMessage;

public:
    virtual ~ExchangeItemEventArgs()
    {}

    ExchangeItemEventArgs();
    ExchangeItemEventArgs(
        std::shared_ptr<IBaseExchangeItem> exchangeItem, const std::string &message);

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `ExchangeItemChangeEventArgs`.
    //

    virtual std::shared_ptr<IBaseExchangeItem> GetExchangeItem() const override;

    virtual void SetExchangeItem(std::shared_ptr<IBaseExchangeItem> value) override;

    virtual std::string GetMessages() const override;

    virtual void SetMessages(const std::string &value) override;
};

}  // namespace CommImp
}  // namespace OpenOasis