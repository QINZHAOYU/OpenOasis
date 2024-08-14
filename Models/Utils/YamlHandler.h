/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  YamlHandler.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide unified json file read and write function.
 *
 ** ***********************************************************************************/
#pragma once
#include "ThirdPart/RapidYaml/rapidyaml.hpp"
#include <optional>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <type_traits>


namespace OpenOasis
{
namespace Utils
{
/// @brief YamlLoader class loads and parses the configurations from a yaml file.
/// @note  YamlLoader reads the configurations without type, all is string_view.
class YamlLoader
{
private:
    ryml::Tree mYaml;

public:
    YamlLoader(){};
    YamlLoader(const std::string &file);

    void LoadByContent(const std::string &content);
    void LoadByFile(const std::string &file);

    std::set<std::string> MapKeys(const std::vector<std::string> &levels) const;

    int SeqSize(const std::vector<std::string> &levels) const;

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods used for parsing map-fmt yaml data.
    //
    // Such as R"(arr: [1, 2, 3])". Further nesting is not supported, for example:
    // R"(arr: [1, 2, 3, {a: 1}])".

    std::optional<std::string> GetMapValueInStr(
        const std::vector<std::string> &levels, const std::string &key,
        int index = -1) const;

    std::optional<bool> GetMapValueInBool(
        const std::vector<std::string> &levels, const std::string &key,
        int index = -1) const;

    std::optional<int> GetMapValueInInt(
        const std::vector<std::string> &levels, const std::string &key,
        int index = -1) const;

    std::optional<double> GetMapValueInDbl(
        const std::vector<std::string> &levels, const std::string &key,
        int index = -1) const;

    // Get a map object that doesn't contain embedded array or sub-map.
    std::unordered_map<std::string, std::string>
    GetMap(const std::vector<std::string> &levels) const;

    std::unordered_map<std::string, std::string>
    GetMapInSeq(const std::vector<std::string> &level, int idx) const;

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods used for parsing array-fmt yaml data.
    //
    // Such as R"(arr: \n  - {a: 1}\n  - {b: 2})". Further nesting is not supported,
    // for example: R"(arr: \n  - {a: 1}\n  - {b: [1, 2, 3]})".

    std::optional<std::string> GetSeqValueInStr(
        const std::vector<std::string> &levels, int index,
        const std::string &key = "") const;

    std::optional<bool> GetSeqValueInBool(
        const std::vector<std::string> &levels, int index,
        const std::string &key = "") const;

    std::optional<int> GetSeqValueInInt(
        const std::vector<std::string> &levels, int index,
        const std::string &key = "") const;

    std::optional<double> GetSeqValueInDbl(
        const std::vector<std::string> &levels, int index,
        const std::string &key = "") const;

    // Get a array object that doesn't contain embedded map or subarray.
    template <typename T>
    typename std::enable_if<std::is_floating_point<T>::value, std::vector<T>>::type
    GetSeq(const std::vector<std::string> &levels) const
    {
        ryml::ConstNodeRef node = mYaml.rootref();
        try
        {
            node = GetNode(node, levels);
        }
        catch (...)
        {
            return {};
        }

        if (!node.is_seq() || !node[0].is_val())
            return {};

        std::vector<T> results;
        for (int i = 0; i < node.num_children(); ++i)
        {
            T value = GetValueFromNode<T>(node, i, "");
            results.push_back(value);
        }

        return results;
    }

    template <typename T>
    typename std::enable_if<!std::is_floating_point<T>::value, std::vector<T>>::type
    GetSeq(const std::vector<std::string> &levels) const
    {
        ryml::ConstNodeRef node = mYaml.rootref();
        try
        {
            node = GetNode(node, levels);
        }
        catch (...)
        {
            return {};
        }

        if (!node.is_seq() || !node[0].is_val())
            return {};

        std::vector<T> results;
        for (int i = 0; i < (int)node.num_children(); ++i)
        {
            T value = GetValueByNode<T>(node, i, "");
            results.push_back(value);
        }

        return results;
    }

private:
    template <typename T>
    std::optional<T> GetNonDblValue(
        const std::vector<std::string> &levels, const std::string &key, int index) const
    {
        ryml::ConstNodeRef node = mYaml.rootref();
        try
        {
            node = GetNode(node, levels);
            return GetValueByNode<T>(node, index, key);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }

    std::optional<double> GetDblValue(
        const std::vector<std::string> &levels, const std::string &key, int index) const
    {
        ryml::ConstNodeRef node = mYaml.rootref();
        try
        {
            node = GetNode(node, levels);
            return GetValueFromNode<double>(node, index, key);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }

    inline ryml::ConstNodeRef
    GetNode(ryml::ConstNodeRef node, const std::vector<std::string> &levels) const
    {
        for (const auto &level : levels)
        {
            ryml::csubstr k{level.c_str(), level.size()};
            if (node.has_child(k))
                node = node[k];
            else
                throw std::runtime_error("Invalid key.");
        }

        return node;
    }

    inline bool HasKeyInMap(ryml::ConstNodeRef node, const std::string &key) const
    {
        ryml::csubstr k{key.c_str(), key.size()};

        if (node.is_map() && !key.empty() && node.has_child(k))
            return true;
        else
            return false;
    }

    inline bool HasIdxInSeq(ryml::ConstNodeRef node, int idx) const
    {
        std::size_t idx2 = idx;
        if (node.is_seq() && idx2 >= 0 && node.num_children() > idx2)
            return true;
        else
            return false;
    }

    template <typename T>
    inline T
    GetValueFromNode(ryml::ConstNodeRef node, int index, const std::string &key) const
    {
        ryml::csubstr k{key.c_str(), key.size()};

        std::string result;
        if (HasIdxInSeq(node, index))
        {
            if (HasKeyInMap(node[index], key))
                node[index][k] >> result;
            else
                node[index] >> result;
        }
        else if (HasKeyInMap(node, key))
        {
            if (HasIdxInSeq(node[k], index))
                node[k][index] >> result;
            else
                node[k] >> result;
        }
        else
        {
            throw std::runtime_error("Invalid yaml inquering.");
        }

        std::istringstream iss(result);

        T value;
        iss >> value;

        return value;
    }

    template <typename T>
    inline T
    GetValueByNode(ryml::ConstNodeRef node, int index, const std::string &key) const
    {
        ryml::csubstr k{key.c_str(), key.size()};

        T value;
        if (HasIdxInSeq(node, index))
        {
            if (HasKeyInMap(node[index], key))
                node[index][k] >> value;
            else
                node[index] >> value;
        }
        else if (HasKeyInMap(node, key))
        {
            if (HasIdxInSeq(node[k], index))
                node[k][index] >> value;
            else
                node[k] >> value;
        }
        else
        {
            throw std::runtime_error("Invalid yaml inquering.");
        }

        return value;
    }

    std::string GetFileContents(const std::string &filename);
};


/// @brief YamlWriter class writes the configurations to a yaml file.
class YamlWriter
{};

}  // namespace Utils
}  // namespace OpenOasis