/** ***********************************************************************************
 *    @File      :  ExchangeItemEventArgs.cpp
 *    @Brief     :  To contain the information for firing exchange item change event.
 *
 ** ***********************************************************************************/
#include "ExchangeItemEventArgs.h"


namespace OpenOasis::CommImp
{
using namespace std;

ExchangeItemEventArgs::ExchangeItemEventArgs()
{}

ExchangeItemEventArgs::ExchangeItemEventArgs(
    shared_ptr<IBaseExchangeItem> exchangeItem, const string &message)
{
    mExchangeItem = exchangeItem;
    mMessage      = message;
}

shared_ptr<IBaseExchangeItem> ExchangeItemEventArgs::GetExchangeItem() const
{
    return mExchangeItem;
}

void ExchangeItemEventArgs::SetExchangeItem(shared_ptr<IBaseExchangeItem> value)
{
    mExchangeItem = value;
}

string ExchangeItemEventArgs::GetMessages() const
{
    return mMessage;
}

void ExchangeItemEventArgs::SetMessages(const string &value)
{
    mMessage = value;
}

}  // namespace OpenOasis::CommImp