/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IDescribable.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide descriptive information on an OpenOasis entity.
 *
 ** ***********************************************************************************/
#pragma once
#include <string>


namespace OpenOasis
{
/// @brief An entity that is describable has a caption (title or heading) and a
/// description. These are not to be used for identification.
class IDescribable
{
public:
    ///////////////////////////////////////////////////////////////////////////////////
    // Caption string.
    //

    virtual std::string GetCaption() const = 0;

    virtual void SetCaption(const std::string &value) = 0;

    ///////////////////////////////////////////////////////////////////////////////////
    // Additional descriptive information about the entity.
    //

    virtual std::string GetDescription() const = 0;

    virtual void SetDescription(const std::string &value) = 0;
};

}  // namespace OpenOasis
