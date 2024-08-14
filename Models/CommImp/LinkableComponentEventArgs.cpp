/** ***********************************************************************************
 *    @File      :  LinkableComponentEventArgs.cpp
 *    @Brief     :  To contain the information firing the StatusChanged event.
 *
 ** ***********************************************************************************/
#include "LinkableComponentEventArgs.h"


namespace OpenOasis::CommImp
{
using namespace std;

LinkableComponentEventArgs::LinkableComponentEventArgs()
{}

LinkableComponentEventArgs::LinkableComponentEventArgs(
    const shared_ptr<LinkableComponentStatusChangeEventArgs> &obj)
{
    mLinkableComponent = obj->GetLinkableComponent();
    mOldStatus         = obj->GetOldStatus();
    mNewStatus         = obj->GetNewStatus();
    mMessage           = obj->GetMessages();
}

shared_ptr<ILinkableComponent> LinkableComponentEventArgs::GetLinkableComponent() const
{
    return mLinkableComponent;
}

void LinkableComponentEventArgs::SetLinkableComponent(
    shared_ptr<ILinkableComponent> value)
{
    mLinkableComponent = value;
}

LinkableComponentStatus LinkableComponentEventArgs::GetOldStatus() const
{
    return mOldStatus;
}

void LinkableComponentEventArgs::SetOldStatus(LinkableComponentStatus value)
{
    mOldStatus = value;
}

LinkableComponentStatus LinkableComponentEventArgs::GetNewStatus() const
{
    return mNewStatus;
}

void LinkableComponentEventArgs::SetNewStatus(LinkableComponentStatus value)
{
    mNewStatus = value;
}

std::string LinkableComponentEventArgs::GetMessages() const
{
    return mMessage;
}

void LinkableComponentEventArgs::SetMessages(const std::string &value)
{
    mMessage = value;
}

}  // namespace OpenOasis::CommImp