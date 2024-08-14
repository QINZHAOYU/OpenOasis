/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  StringHelper.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide help for converting, formatting, splitting,
 *                  and merging wide and narrow strings.
 *
 ** ***********************************************************************************/
#pragma once
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>


namespace OpenOasis
{
namespace Utils
{
/// @brief Helper class for wide and narrow string operations.
/// @attention Be careful with non-ascii characters.
class StringHelper
{
public:
    static std::string ToLower(const std::string &source);

    static std::string ToUpper(const std::string &source);

    /// @brief Trims the trim-chars contained at the beginning of input string.
    /// Return a new string without `trimChars` contained at the beginning.
    static std::string
    TrimStart(const std::string &source, const std::string &trimChars = " \t\n\r\v\f");

    /// @brief Trims the trim-chars contained at the end of input string.
    /// Return a new string without `trimChars` contained at the end.
    static std::string
    TrimEnd(const std::string &source, const std::string &trimChars = " \t\n\r\v\f");

    /// @brief Trims the trim-chars contained at the beginning and end of input string.
    /// Return a new string without `trimChars` contained at the beginning and end.
    static std::string
    Trim(const std::string &source, const std::string &trimChars = " \t\n\r\v\f");

    static std::string Replace(
        const std::string &source, const std::string &find, const std::string &replace);

    static bool StartsWith(const std::string &source, const std::string &value);

    static bool EndsWith(const std::string &source, const std::string &value);

    /// @brief Splites input string by delimiter.
    /// Empty item would not be in the return.
    static std::vector<std::string> Split(const std::string &source, char delimiter);

    static std::string WStringToString(const std::wstring &wstr);

    static std::wstring StringToWString(const std::string &str);

    static bool IsEmptyOrWhiteSpace(const std::string &source);

    template <typename T>
    static std::string ToString(const T &subject)
    {
        std::ostringstream ss;
        ss << subject;
        return ss.str();
    }

    template <typename T>
    static T FromString(const std::string &subject)
    {
        std::istringstream ss(subject);
        T                  target;
        ss >> target;
        return target;
    }

    /// @brief Formats string through the placeholder "{}" with default positional args.
    /// If there are multiple "{} in the input string, replace them all with `arg`.
    template <typename T>
    static std::string FormatSimple(const std::string &input, T arg)
    {
        std::ostringstream ss;

        std::size_t lastCloseBrace = std::string::npos;
        std::size_t openBrace      = std::string::npos;
        while ((openBrace = input.find('{', openBrace + 1)) != std::string::npos)
        {
            if (openBrace + 1 < input.length())
            {
                if (input[openBrace + 1] == '{')
                {
                    openBrace++;
                    continue;
                }

                std::size_t closeBrace = input.find('}', openBrace + 1);
                if (closeBrace != std::string::npos)
                {
                    ss << input.substr(
                        lastCloseBrace + 1, openBrace - lastCloseBrace - 1);
                    ss << arg;
                    lastCloseBrace = closeBrace;
                }
            }
        }

        if (lastCloseBrace + 1 < input.length())
        {
            ss << input.substr(lastCloseBrace + 1);
        }
        return ss.str();
    }

    /// @brief Formats string through the placeholder "{}" with vector.
    template <typename T>
    static std::string
    FormatSimple(const std::string &input, const std::vector<T> &args)
    {
        if (args.empty())
        {
            return input;
        }

        std::ostringstream ss;

        std::size_t vecSize        = args.size();
        std::size_t vecIndex       = 0;
        std::size_t lastCloseBrace = std::string::npos;
        std::size_t openBrace      = std::string::npos;
        while ((openBrace = input.find('{', openBrace + 1)) != std::string::npos)
        {
            if (openBrace + 1 < input.length())
            {
                if (input[openBrace + 1] == '{')
                {
                    openBrace++;
                    continue;
                }

                std::size_t closeBrace = input.find('}', openBrace + 1);
                if (closeBrace != std::string::npos)
                {
                    ss << input.substr(
                        lastCloseBrace + 1, openBrace - lastCloseBrace - 1);
                    ss << args[vecIndex];

                    lastCloseBrace = closeBrace;
                    vecIndex       = std::min(vecIndex + 1, vecSize - 1);
                }
            }
        }

        if (lastCloseBrace + 1 < input.length())
        {
            ss << input.substr(lastCloseBrace + 1);
        }
        return ss.str();
    }

    /// @brief Formats string through the placeholder "{}" with default positional args.
    template <typename T, class... Args>
    static std::string FormatSimple(const std::string &input, T arg, Args... rest)
    {
        std::ostringstream ss;

        std::size_t lastCloseBrace = std::string::npos;
        std::size_t openBrace      = std::string::npos;
        while ((openBrace = input.find('{', openBrace + 1)) != std::string::npos)
        {
            if (openBrace + 1 < input.length())
            {
                if (input[openBrace + 1] == '{')
                {
                    openBrace++;
                    continue;
                }

                std::size_t closeBrace = input.find('}', openBrace + 1);
                if (closeBrace != std::string::npos)
                {
                    ss << input.substr(
                        lastCloseBrace + 1, openBrace - lastCloseBrace - 1);
                    ss << arg;

                    lastCloseBrace = closeBrace;
                    break;
                }
            }
        }

        if (lastCloseBrace + 1 < input.length())
        {
            ss << input.substr(lastCloseBrace + 1);
        }
        return FormatSimple(ss.str(), rest...);
    }
};

}  // namespace Utils
}  // namespace OpenOasis
