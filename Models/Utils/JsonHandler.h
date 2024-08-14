/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  JsonHandler.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide unified json file read and write function.
 *
 ** ***********************************************************************************/
#pragma once
#include "ThirdPart/Json/json.hpp"
#include <optional>
#include <vector>
#include <type_traits>
#include <stdexcept>


namespace OpenOasis
{
namespace Utils
{
/// @brief JsonLoader loads and parses the configurations from specified json file.
/// @note  JsonLoader reads the configurations with type.
class JsonLoader
{
private:
    std::string    mFile;
    nlohmann::json mJson;

public:
    JsonLoader(const std::string &file);

    nlohmann::json GetJson() const;

    std::string GetFile() const;

    std::vector<std::string> GetKeys(const nlohmann::json &json) const;

    int GetArraySize(const nlohmann::json &json) const;

    template <typename Arg>
    bool IsNull(const nlohmann::json &json, const Arg &arg) const
    {
        auto jsonCopy = GetJson(json, arg);
        if (jsonCopy.has_value() && jsonCopy.value().is_null())
        {
            return true;
        }

        return false;
    }

    std::optional<nlohmann::json> GetJson(const nlohmann::json &json, int index) const
    {
        nlohmann::json jsonCopy = json;
        if (jsonCopy.empty() || jsonCopy.is_null())
        {
            return std::nullopt;
        }

        if (jsonCopy.is_array() && jsonCopy.size() > index)
        {
            return jsonCopy[index];
        }
        else
        {
            return std::nullopt;
        }
    }

    std::optional<nlohmann::json>
    GetJson(const nlohmann::json &json, std::string key) const
    {
        nlohmann::json jsonCopy = json;
        if (jsonCopy.empty() || jsonCopy.is_null())
        {
            return std::nullopt;
        }

        if (jsonCopy.contains(key))
        {
            return jsonCopy.at(key);
        }
        else
        {
            return std::nullopt;
        }
    }

    std::optional<nlohmann::json>
    GetJson(const nlohmann::json &json, const char *key) const
    {
        return GetJson(json, std::string(key));
    }

    template <typename T, class... Args>
    std::optional<nlohmann::json>
    GetJson(const nlohmann::json &json, T arg, Args... rest) const
    {
        auto jsonCopy = GetJson(json, arg);
        if (!jsonCopy.has_value() || jsonCopy.value().is_null())
        {
            return std::nullopt;
        }

        return GetJson(jsonCopy, rest...);
    }

    template <typename T, typename Arg>
    std::optional<T> GetValue(const nlohmann::json &json, const Arg &arg) const
    {
        auto jsonCopy = GetJson(json, arg);

        if (!jsonCopy.has_value() || jsonCopy.value().is_null())
        {
            return std::nullopt;
        }
        return jsonCopy.value();
    }

    template <typename Arg>
    std::unordered_map<std::string, std::string>
    GetMap(const nlohmann::json &json, const Arg &arg) const
    {
        std::unordered_map<std::string, std::string> map;

        auto jsonCopy = GetJson(json, arg);
        if (!jsonCopy.has_value() || jsonCopy.value().is_null())
        {
            return map;
        }

        for (const auto &it : jsonCopy.value().items())
        {
            map.insert(make_pair(it.key(), it.value()));
        }

        return map;
    }

    template <typename T, typename Arg>
    std::vector<T> GetList(const nlohmann::json &json, const Arg &arg) const
    {
        std::vector<T> list;

        auto jsonCopy = GetJson(json, arg);
        if (!jsonCopy.has_value() || jsonCopy.value().is_null())
        {
            return list;
        }

        if (jsonCopy.value().is_null() || !jsonCopy.value().is_array())
        {
            return list;
        }

        for (const auto &val : jsonCopy.value().at(arg))
        {
            list.push_back(val);
        }

        return list;
    }
};


/// @brief JsonWriter class writes the configurations to a json file.
class JsonWriter
{
private:
    std::string    mFilePath;
    nlohmann::json mJson;

    static unsigned int sTmpFileCount;

public:
    JsonWriter(const std::string &filePath);

    std::string GetOutputFilePath() const;

    void Save();

    template <typename T>
    void SetValue(
        const std::vector<std::string> &levels, const std::string &key, const T &value)
    {
        nlohmann::json parentJson = {{key, value}};
        for (auto it = levels.rbegin(); it != levels.rend(); ++it)
        {
            parentJson = nlohmann::json{{*it, parentJson}};
        }

        for (const auto &[key, value] : parentJson.items())
        {
            mJson[key] = value;
        }
    }

    template <typename T>
    void SetArray(
        const std::vector<std::string> &levels, const std::string &key,
        const std::vector<T> &values)
    {
        nlohmann::json json;
        if (values.empty())
            json[key] = nlohmann::json::array();
        else
            json[key] = values;

        nlohmann::json parentJson = json;
        for (auto it = levels.rbegin(); it != levels.rend(); ++it)
        {
            parentJson = nlohmann::json{{*it, parentJson}};
        }

        for (const auto &[key, value] : parentJson.items())
        {
            mJson[key] = value;
        }
    }
};


}  // namespace Utils
}  // namespace OpenOasis