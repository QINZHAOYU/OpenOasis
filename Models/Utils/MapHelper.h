/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  MapHelper.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide extened methods for map operation.
 *
 ** ***********************************************************************************/
#pragma once
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>


namespace OpenOasis
{
namespace Utils
{
/// @brief Helper class used providing extended methods for map operations.
/// @attention Only std::map, std::unordered_map supported now.
class MapHelper
{
public:
    template <typename KeyType, typename ValueType>
    static std::vector<ValueType> GetValues(const std::map<KeyType, ValueType> &maps)
    {
        auto value_selector = [](auto pair) { return pair.second; };

        std::vector<ValueType> temp(maps.size());
        std::transform(maps.begin(), maps.end(), temp.begin(), value_selector);

        return temp;
    }

    template <typename KeyType, typename ValueType>
    static std::vector<KeyType> GetKeys(const std::map<KeyType, ValueType> &maps)
    {
        auto key_selector = [](auto pair) { return pair.first; };

        std::vector<KeyType> temp(maps.size());
        std::transform(maps.begin(), maps.end(), temp.begin(), key_selector);

        return temp;
    }

    template <typename KeyType, typename ValueType>
    static std::vector<KeyType>
    GetKeys(const std::unordered_map<KeyType, ValueType> &maps)
    {
        auto key_selector = [](auto pair) { return pair.first; };

        std::vector<KeyType> temp(maps.size());
        std::transform(maps.begin(), maps.end(), temp.begin(), key_selector);

        return temp;
    }

    template <typename KeyType, typename ValueType>
    static std::vector<ValueType>
    GetValues(const std::unordered_map<KeyType, ValueType> &maps)
    {
        auto value_selector = [](auto pair) { return pair.second; };

        std::vector<ValueType> temp(maps.size());
        std::transform(maps.begin(), maps.end(), temp.begin(), value_selector);

        return temp;
    }
};

}  // namespace Utils
}  // namespace OpenOasis