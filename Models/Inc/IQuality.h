/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IQuality.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To describe qualitative data, where a value is specified as one
 *                  category within a number of predefined (possible) categories.
 *
 ** ***********************************************************************************/
#pragma once
#include "IValueDefinition.h"
#include "ICategory.h"
#include <vector>
#include <memory>


namespace OpenOasis
{
/// @brief Class describes qualitative data, where a value is specified as one category
/// within a number of predefined (possible) categories.
///
/// Qualitative data described items in terms of some quality or categorization that
/// may be 'informal' or may use relatively ill-defined characteristics such as
/// warmth and flavour. However, qualitative data can include well-defined aspects
/// such as gender, nationality or commodity type.
///
/// For qualitative data the `IValueSet` exchanged between `ILinkableComponent`s
/// contains one of the possible `ICategory` instances per element in the element set
/// involved. For examples:
///     Colors  : red, green, blue;
///     Land use: nature, recreation, industry, infrastructure;
///     Rating  : worse, same, better.
class IQuality : public IValueDefinition
{
public:
    /// @brief Gets a list of the possible `ICategory` allowed.
    ///
    /// If the quality is not ordered, the list contains the `ICategory`s
    /// in an unspecified order. When it's ordered, the list contains the
    /// `ICategory`s in the same sequence.
    virtual std::vector<std::shared_ptr<ICategory>> GetCategories() const = 0;

    /// @brief Checks if this `IQuality` is defined by an ordered set or not.
    virtual bool IsOrdered() const = 0;
};

}  // namespace OpenOasis
