/** ***********************************************************************************
 *    @File      :  StringHelper.cpp
 *    @Brief     :  To provide help for converting, formatting, splitting,
 *                  and merging wide and narrow strings.
 *
 ** ***********************************************************************************/
#include "StringHelper.h"
#include <codecvt>
#include <locale>


namespace OpenOasis::Utils
{
using namespace std;

string StringHelper::ToLower(const string &source)
{
    string copy(source);
    transform(
        begin(source), end(source), begin(copy), [](char c) { return tolower(c); });
    return copy;
}

string StringHelper::ToUpper(const string &source)
{
    string copy(source);
    transform(
        begin(source), end(source), begin(copy), [](char c) { return toupper(c); });

    return copy;
}

string StringHelper::TrimStart(const string &source, const string &trimChars)
{
    string copy(source);
    return copy.erase(0, source.find_first_not_of(trimChars));
}

string StringHelper::TrimEnd(const string &source, const string &trimChars)
{
    string copy(source);
    return copy.erase(source.find_last_not_of(trimChars) + 1);
}

string StringHelper::Trim(const string &source, const string &trimChars)
{
    string copy(source);
    return TrimStart(TrimEnd(copy, trimChars), trimChars);
}

string
StringHelper::Replace(const string &source, const string &find, const string &replace)
{
    string copy(source);
    size_t pos = 0;
    while ((pos = copy.find(find, pos)) != string::npos)
    {
        copy.replace(pos, find.length(), replace);
        pos += replace.length();
    }
    return copy;
}

bool StringHelper::StartsWith(const string &source, const string &value)
{
    if (source.length() < value.length())
        return false;
    else
        return source.compare(0, value.length(), value) == 0;
}

bool StringHelper::EndsWith(const string &source, const string &value)
{
    if (source.length() < value.length())
        return false;
    else
        return source.compare(source.length() - value.length(), value.length(), value)
               == 0;
}

vector<string> StringHelper::Split(const string &source, char delimiter)
{
    vector<string> output;
    istringstream  ss(source);
    string         nextItem;
    while (getline(ss, nextItem, delimiter))
    {
        if (!nextItem.empty())
            output.push_back(nextItem);
    }

    return output;
}

bool StringHelper::IsEmptyOrWhiteSpace(const string &source)
{
    if (source.length() == 0)
        return true;
    else
    {
        for (size_t index = 0; index < source.length(); index++)
        {
            if (!isspace(source[index]))
                return false;
        }

        return true;
    }
}

string StringHelper::WStringToString(const wstring &wstr)
{
    return wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(wstr);
}

wstring StringHelper::StringToWString(const string &str)
{
    return wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(str);
}

}  // namespace OpenOasis::Utils