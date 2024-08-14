/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  LinkableComponentEventArgs.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To contain the information firing the StatusChanged event.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/LinkableComponentStatusChangeEventArgs.h"


namespace OpenOasis
{
namespace CommImp
{
/// @brief Generic implementation of interface `LinkableComponentStatusChangeEventArgs`.
class LinkableComponentEventArgs : public LinkableComponentStatusChangeEventArgs
{
private:
    std::shared_ptr<ILinkableComponent> mLinkableComponent = nullptr;

    LinkableComponentStatus mOldStatus;
    LinkableComponentStatus mNewStatus;
    std::string             mMessage;

public:
    virtual ~LinkableComponentEventArgs()
    {}

    LinkableComponentEventArgs();
    LinkableComponentEventArgs(
        const std::shared_ptr<LinkableComponentStatusChangeEventArgs> &obj);

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `LinkableComponentStatusChangeEventArgs`.
    //

    virtual LinkableComponentStatus GetOldStatus() const override;

    virtual void SetOldStatus(LinkableComponentStatus value) override;

    virtual LinkableComponentStatus GetNewStatus() const override;

    virtual void SetNewStatus(LinkableComponentStatus value) override;

    virtual std::string GetMessages() const override;

    virtual void SetMessages(const std::string &value) override;

    virtual std::shared_ptr<ILinkableComponent> GetLinkableComponent() const override;

    virtual void SetLinkableComponent(std::shared_ptr<ILinkableComponent> obj) override;
};

}  // namespace CommImp
}  // namespace OpenOasis