/** ***********************************************************************************
 *    @File      :  JsonHandler.cpp
 *    @Brief     :  To provide unified json file read and write function.
 *
 ** ***********************************************************************************/
#include "JsonHandler.h"
#include "FilePathHelper.h"
#include "StringHelper.h"
#include <fstream>


namespace OpenOasis::Utils
{
using namespace std;

// ------------------------------------------------------------------------------------

JsonLoader::JsonLoader(const string &file)
{
    if (!FilePathHelper::FileExists(file))
    {
        throw invalid_argument(
            StringHelper::FormatSimple("Json file [{}] doesn't exist.", file));
    }

    if (!nlohmann::json::accept(ifstream(file)))
    {
        throw invalid_argument(
            StringHelper::FormatSimple("Invalid json file [{}].", file));
    }

    mFile = file;
    mJson = nlohmann::json::parse(ifstream(file));
}

nlohmann::json JsonLoader::GetJson() const
{
    return mJson;
}

string JsonLoader::GetFile() const
{
    return mFile;
}

vector<string> JsonLoader::GetKeys(const nlohmann::json &json) const
{
    vector<string> keys;
    for (const auto &it : json.items())
    {
        if (it.key() != "")
            keys.push_back(it.key());
    }

    return keys;
}

int JsonLoader::GetArraySize(const nlohmann::json &json) const
{
    return json.is_array() ? json.size() : 0;
}

// ------------------------------------------------------------------------------------

unsigned int JsonWriter::sTmpFileCount = 0;

JsonWriter::JsonWriter(const string &filePath)
{
    if (!FilePathHelper::FileExists(filePath))
    {
        try
        {
            if (FilePathHelper::DirectoryExists(filePath))
            {
                const auto &newFilePath = FilePathHelper::Combine(
                    filePath,
                    StringHelper::FormatSimple("Oasis_temp_{}.json", sTmpFileCount++));

                FilePathHelper::MakeFile(newFilePath);
                mFilePath = newFilePath;
            }
        }
        catch (...)
        {
            throw invalid_argument(StringHelper::FormatSimple(
                "File or directory [{}] does not exist.", filePath));
        }
    }
}

string JsonWriter::GetOutputFilePath() const
{
    return mFilePath;
}

void JsonWriter::Save()
{
    ofstream fout(mFilePath);
    fout << setw(4) << mJson << endl;
}

}  // namespace OpenOasis::Utils
