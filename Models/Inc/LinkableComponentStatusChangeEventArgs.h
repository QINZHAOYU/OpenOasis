/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  LinkableComponentStatusChangeEventArgs.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To contain the information that will be passed when the
 *                 `ILinkableComponent` fires the `StatusChanged` event.
 *
 ** ***********************************************************************************/
#pragma once
#include <memory>
#include <string>
#include "LinkableComponentStatus.h"


namespace OpenOasis
{
class ILinkableComponent;

/// @brief Class contains the information that will be passed when the
/// `ILinkableComponent` fires the `StatusChanged` event.
class LinkableComponentStatusChangeEventArgs
{
public:
    ///////////////////////////////////////////////////////////////////////////////////
    // The linkable component that fired the status change event.
    //

    virtual std::shared_ptr<ILinkableComponent> GetLinkableComponent() const = 0;

    virtual void SetLinkableComponent(std::shared_ptr<ILinkableComponent> obj) = 0;

    ///////////////////////////////////////////////////////////////////////////////////
    // The linkable component's status before the status change.
    //

    virtual LinkableComponentStatus GetOldStatus() const = 0;

    virtual void SetOldStatus(LinkableComponentStatus value) = 0;

    ///////////////////////////////////////////////////////////////////////////////////
    // The linkable component's status after the status change.
    //

    virtual LinkableComponentStatus GetNewStatus() const = 0;

    virtual void SetNewStatus(LinkableComponentStatus value) = 0;

    ///////////////////////////////////////////////////////////////////////////////////
    // Message providing additional information on the status change.
    //

    virtual std::string GetMessages() const = 0;

    virtual void SetMessages(const std::string &msg) = 0;
};

}  // namespace OpenOasis
