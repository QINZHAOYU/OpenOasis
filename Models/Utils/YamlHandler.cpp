/** ***********************************************************************************
 *    @File      :  YamlHandler.cpp
 *    @Brief     :  To provide unified json file read and write function.
 *
 ** ***********************************************************************************/
#include "YamlHandler.h"
#include "FilePathHelper.h"
#include "StringHelper.h"


namespace OpenOasis::Utils
{
using namespace std;


// ------------------------------------------------------------------------------------

YamlLoader::YamlLoader(const string &filePath)
{
    LoadByFile(filePath);
}

void YamlLoader::LoadByFile(const string &filePath)
{
    if (!FilePathHelper::FileExists(filePath))
    {
        throw invalid_argument(
            StringHelper::FormatSimple("File [{}] doesn't existed.", filePath));
    }

    mYaml = ryml::parse_in_arena(ryml::to_csubstr(GetFileContents(filePath)));
}

void YamlLoader::LoadByContent(const string &content)
{
    mYaml = ryml::parse_in_arena(ryml::to_csubstr(content));
}

string YamlLoader::GetFileContents(const string &filename)
{
    FILE *fp = fopen(filename.c_str(), "rb");
    fseek(fp, 0, SEEK_END);

    string cc;
    long   sz = ftell(fp);
    cc.resize(static_cast<string::size_type>(sz));
    if (sz)
    {
        rewind(fp);
        if (fread(&(cc)[0], 1, cc.size(), fp) != cc.size())
        {
            throw runtime_error(
                StringHelper::FormatSimple("File [{}] loading failure.", filename));
        }
    }
    fclose(fp);

    return cc;
}

set<string> YamlLoader::MapKeys(const vector<string> &levels) const
{
    ryml::ConstNodeRef node = mYaml.crootref();
    try
    {
        node = GetNode(node, levels);
    }
    catch (...)
    {
        return {};
    }

    set<string> keys;
    for (ryml::ConstNodeRef n : node.children())
    {
        const auto &key = n.key();
        keys.insert(string(key.str, key.len));
    }

    return keys;
}

int YamlLoader::SeqSize(const vector<string> &levels) const
{
    ryml::ConstNodeRef node = mYaml.crootref();
    try
    {
        node = GetNode(node, levels);
    }
    catch (...)
    {
        return 0;
    }

    return (node.is_seq()) ? node.num_children() : 0;
}

optional<string> YamlLoader::GetMapValueInStr(
    const vector<string> &levels, const string &key, int index) const
{
    return GetNonDblValue<string>(levels, key, index);
}

optional<bool> YamlLoader::GetMapValueInBool(
    const vector<string> &levels, const string &key, int index) const
{
    return GetNonDblValue<bool>(levels, key, index);
}

optional<int> YamlLoader::GetMapValueInInt(
    const vector<string> &levels, const string &key, int index) const
{
    return GetNonDblValue<int>(levels, key, index);
}

optional<double> YamlLoader::GetMapValueInDbl(
    const vector<string> &levels, const string &key, int index) const
{
    return GetDblValue(levels, key, index);
}

unordered_map<string, string> YamlLoader::GetMap(const vector<string> &levels) const
{
    ryml::ConstNodeRef node = mYaml.rootref();
    try
    {
        node = GetNode(node, levels);

        unordered_map<string, string> map;
        for (ryml::ConstNodeRef const &n : node.children())
        {
            string key(n.key().str, n.key().size());
            string val(n.val().str, n.val().size());

            map[key] = val;
        }

        return map;
    }
    catch (...)
    {
        return {};
    }
}

unordered_map<string, string>
YamlLoader::GetMapInSeq(const vector<string> &levels, int idx) const
{
    ryml::ConstNodeRef node = mYaml.rootref();
    std::size_t        idx2 = idx;
    try
    {
        node = GetNode(node, levels);

        if (!node.is_seq() || node.num_children() <= idx2 || !node[idx2].is_map())
            return {};

        unordered_map<string, string> map;
        for (ryml::ConstNodeRef const &n : node[idx2].children())
        {
            string key(n.key().str, n.key().size());
            string val(n.val().str, n.val().size());

            map[key] = val;
        }

        return map;
    }
    catch (...)
    {
        return {};
    }
}

optional<string> YamlLoader::GetSeqValueInStr(
    const vector<string> &levels, int index, const string &key) const
{
    return GetNonDblValue<string>(levels, key, index);
}

optional<bool> YamlLoader::GetSeqValueInBool(
    const vector<string> &levels, int index, const string &key) const
{
    return GetNonDblValue<bool>(levels, key, index);
}

optional<int> YamlLoader::GetSeqValueInInt(
    const vector<string> &levels, int index, const string &key) const
{
    return GetNonDblValue<int>(levels, key, index);
}

optional<double> YamlLoader::GetSeqValueInDbl(
    const vector<string> &levels, int index, const string &key) const
{
    return GetDblValue(levels, key, index);
}

// ------------------------------------------------------------------------------------

}  // namespace OpenOasis::Utils
