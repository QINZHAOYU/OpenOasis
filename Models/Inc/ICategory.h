/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ICategory.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To describe one item of a possible categorization.
 *
 ** ***********************************************************************************/
#pragma once
#include "IDescribable.h"
#include <any>


namespace OpenOasis
{
/// @brief Class describes one item of a possible categorization.
///
/// It is used by the `IQuality` interface for describing qualitative data. A category
/// defines one "class" within a "set of classes".
///
/// For qualitative data the `IValueSet` exchanged between `ILinkableComponent`
/// contains one of the possible `ICategory` instances per data element.
class ICategory : public IDescribable
{
public:
    /// @brief Value for this category.
    ///
    /// Example, "blue" in a {"red", "green", "blue"} set.
    virtual std::any GetValue() const = 0;
};

}  // namespace OpenOasis
