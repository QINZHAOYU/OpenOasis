/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IComparable.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide a unified interface for object comparison.
 *
 ** ***********************************************************************************/
#pragma once
#include <memory>


namespace OpenOasis
{
namespace AdditionalControl
{
/// @brief Interface class provides a unified interface for object comparison.
class IComparable
{
public:
    /// @brief Compare the two comparable objections.
    ///
    /// if this > obj, return +1;
    /// if this = obj, return 0;
    /// if this < obj, return -1;
    ///
    /// @param obj A comparable object.
    ///
    /// @return The comparison result.
    ///
    /// @throw If the two objects were not comparable, an exception thrown.
    virtual int CompareTo(const std::shared_ptr<IComparable> &obj) const = 0;
};


}  // namespace AdditionalControl
}  // namespace OpenOasis