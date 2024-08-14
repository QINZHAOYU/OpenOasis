/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ExchangeItemChangeEventArgs.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To contain the information that will be passed when the
 *                 `IBaseExchangeItem` fires the `ExchangeItemValueChanged` event.
 *
 ** ***********************************************************************************/
#pragma once
#include <memory>
#include <string>


namespace OpenOasis
{
class IBaseExchangeItem;

/// @brief Class containing the information that will be passed when firing an
/// `ExchangeItemValueChanged` event.
///
/// Sending exchange item events is optional, so it should not be used as a mechanism
/// to build critical functionality upon.
class ExchangeItemChangeEventArgs
{
public:
    ///////////////////////////////////////////////////////////////////////////////////
    // To deal with the exchange item of which the status has been changed.
    //

    virtual std::shared_ptr<IBaseExchangeItem> GetExchangeItem() const = 0;

    virtual void SetExchangeItem(std::shared_ptr<IBaseExchangeItem> obj) = 0;

    ///////////////////////////////////////////////////////////////////////////////////
    // To deal with the message that describes the way in which the status
    // of the exchange item has been changed.
    //

    virtual std::string GetMessages() const = 0;

    virtual void SetMessages(const std::string &value) = 0;
};

}  // namespace OpenOasis
