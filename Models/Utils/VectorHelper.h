/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  VectorHelper.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide vector elements searching and positioning methods.
 *
 ** ***********************************************************************************/
#pragma once
#include <vector>
#include <algorithm>
#include <stdexcept>


namespace OpenOasis
{
namespace Utils
{
/// @brief Helper class for vector operations.
/// @attention For the custom types, operator `==` should be overrided.
class VectorHelper
{
public:
    /// @brief Searches the index of the first `item` appears in the specified vector.
    /// If specified `item` not in the vector, -1 returned.
    template <typename T>
    static int IndexOf(const std::vector<T> &list, T item)
    {
        auto it = std::find(list.begin(), list.end(), item);
        if (it != list.end())
            return std::distance(list.begin(), it);
        else
            return -1;
    }

    /// @brief Searches the index of first `item` appears in the vector after `offset`.
    /// If specified `item` not in the vector, -1 returned.
    /// If specified `offset` greater than the vector size, an exception thrown.
    template <typename T>
    static int IndexOf(const std::vector<T> &list, T item, int offset)
    {
        if (offset <= (int)list.size())
        {
            auto it = std::find(list.begin() + offset, list.end(), item);
            if (it != list.end())
                return std::distance(list.begin(), it);
            else
                return -1;
        }
        else
        {
            throw std::runtime_error(
                "The 'offset' parameter is greater than the vector size.");
        }
    }

    /// @brief Searches the index of the last `item` appears in the vector.
    /// If specified `item` not in the vector, -1 returned.
    template <typename T>
    static int LastIndexOf(const std::vector<T> &list, T item)
    {
        auto it = std::find(list.rbegin(), list.rend(), item);
        if (it != list.rend())
            return std::distance(it, list.rend()) - 1;
        else
            return -1;
    }

    /// @brief Searches the index of the last `item` appears in vector before `offset`.
    /// If specified `item` not in the vector, -1 returned.
    /// If specified `offset` greater than the vector size, an exception thrown.
    template <typename T>
    static int LastIndexOf(const std::vector<T> &list, T item, int offset)
    {
        if (offset <= (int)list.size())
        {
            auto it = std::find(list.rbegin() + offset, list.rend(), item);
            if (it != list.rend())
                return std::distance(it, list.rend()) - 1;
            else
                return -1;
        }
        else
        {
            throw std::runtime_error(
                "The 'offset' parameter should be less than the vector size.");
        }
    }

    /// @brief Checks whether one vector is a subset of another.
    template <typename T>
    static bool IsSubset(const std::vector<T> &vec, const std::vector<T> &subVec)
    {
        for (auto &elem : subVec)
        {
            if (std::find(vec.begin(), vec.end(), elem) == vec.end())
                return false;
        }
        return true;
    }

    /// @brief Checks if specified item contained in the vector.
    template <typename T>
    static bool IsContained(const std::vector<T> &vec, const T &item)
    {
        if (std::find(vec.begin(), vec.end(), item) != vec.end())
            return true;
        else
            return false;
    }
};

}  // namespace Utils
}  // namespace OpenOasis