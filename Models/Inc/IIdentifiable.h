/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IIdentifiable.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide the id of an OpenOasis entity.
 *
 ** ***********************************************************************************/
#pragma once
#include "IDescribable.h"
#include <string>


namespace OpenOasis
{
/// @brief To define a method to get the Id of an OpenOasis entity.
class IIdentifiable : public IDescribable
{
public:
    /// @brief Gets id string.
    ///
    /// The Id must be unique within its context but does not need to be globally
    /// unique. e.g. the id of an input exchange item must be unique in the list
    /// of inputs of `ILinkableComponent`, but a similar Id might be used by an exchange
    /// item of another `ILinkableComponent`.
    virtual std::string GetId() const = 0;
};

}  // namespace OpenOasis
