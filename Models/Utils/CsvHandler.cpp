/** ***********************************************************************************
 *    @File      :  CsvHandler.cpp
 *    @Brief     :  To provide unified csv file read and write function.
 *
 ** ***********************************************************************************/
#include "CsvHandler.h"
#include "FilePathHelper.h"
#include "StringHelper.h"


namespace OpenOasis::Utils
{
using namespace std;


// ------------------------------------------------------------------------------------

CsvLoader::CsvLoader(
    const string &filePath, bool hasColumnHeader, bool hasRowHeader, char delimiter,
    bool skipEmptyLine, char prefix)
{
    LoadByFile(
        filePath, hasColumnHeader, hasRowHeader, delimiter, skipEmptyLine, prefix);
}

void CsvLoader::LoadByFile(
    const string &filePath, bool hasColumnHeader, bool hasRowHeader, char delimiter,
    bool skipEmptyLine, char prefix)
{
    if (!FilePathHelper::FileExists(filePath))
    {
        throw invalid_argument(
            StringHelper::FormatSimple("File [{}] does not exist.", filePath));
    }

    mHasColumnHeader = hasColumnHeader;
    mHasRowHeader    = hasRowHeader;

    int colHeaderIdx = (hasColumnHeader) ? 0 : -1;
    int rowHeaderIdx = (hasRowHeader) ? 0 : -1;

    mCsv = rapidcsv::Document(
        filePath,
        rapidcsv::LabelParams(colHeaderIdx, rowHeaderIdx),
        rapidcsv::SeparatorParams(delimiter),
        rapidcsv::ConverterParams(),
        rapidcsv::LineReaderParams(skipEmptyLine, prefix, true));
}

void CsvLoader::LoadByContent(
    const string &content, bool hasColumnHeader, bool hasRowHeader, char delimiter,
    bool skipEmptyLine, char prefix)
{
    mHasColumnHeader = hasColumnHeader;
    mHasRowHeader    = hasRowHeader;

    int colHeaderIdx = (hasColumnHeader) ? 0 : -1;
    int rowHeaderIdx = (hasRowHeader) ? 0 : -1;

    stringstream sstream(content);

    mCsv = rapidcsv::Document(
        sstream,
        rapidcsv::LabelParams(colHeaderIdx, rowHeaderIdx),
        rapidcsv::SeparatorParams(delimiter),
        rapidcsv::ConverterParams(),
        rapidcsv::LineReaderParams(skipEmptyLine, prefix, true));
}

optional<vector<string>> CsvLoader::GetColumnLabels() const
{
    if (mHasColumnHeader)
        return mCsv.GetColumnNames();
    else
        return nullopt;
}

optional<vector<string>> CsvLoader::GetRowLabels() const
{
    if (mHasRowHeader)
        return mCsv.GetRowNames();
    else
        return nullopt;
}

int CsvLoader::GetColumnCount() const
{
    return mCsv.GetColumnCount();
}

int CsvLoader::GetRowCount() const
{
    return mCsv.GetRowCount();
}

optional<vector<DateTime>> CsvLoader::GetColumnAsDateTime(const string &label) const
{
    try
    {
        const auto &values = mCsv.GetColumn<string>(label);

        vector<DateTime> results(values.size());
        transform(begin(values), end(values), begin(results), [](const auto &val) {
            return DateTime::FromString(val);
        });

        return results;
    }
    catch (...)
    {
        return nullopt;
    }
}

optional<vector<DateTime>> CsvLoader::GetColumnAsDateTime(int idx) const
{
    try
    {
        const auto &values = mCsv.GetColumn<string>(idx);

        vector<DateTime> results(values.size());
        transform(begin(values), end(values), begin(results), [](const auto &val) {
            return DateTime::FromString(val);
        });

        return results;
    }
    catch (...)
    {
        return nullopt;
    }
}


// ------------------------------------------------------------------------------------

unsigned int CsvWriter::sTmpFileCount = 0;

CsvWriter::CsvWriter(const string &filePath, char delimiter)
{
    mFilePath = filePath;
    if (!FilePathHelper::FileExists(filePath))
    {
        try
        {
            auto parent = FilePathHelper::GetDirectoryName(filePath);
            auto name   = FilePathHelper::GetFileName(filePath);
            if (!FilePathHelper::DirectoryExists(parent))
            {
                FilePathHelper::MakeDirectory(parent);
            }

            if (FilePathHelper::DirectoryExists(parent))
            {
                mFilePath = FilePathHelper::Combine(parent, name);
                FilePathHelper::MakeFile(mFilePath);
            }
            else
            {
                throw exception();
            }
        }
        catch (...)
        {
            throw invalid_argument(StringHelper::FormatSimple(
                "File or directory [{}] does not exist.", filePath));
        }
    }

    FilePathHelper::MakeFile(mFilePath);
    mCsv = rapidcsv::Document(
        mFilePath, rapidcsv::LabelParams(0, 0), rapidcsv::SeparatorParams(delimiter));
}

void CsvWriter::Save()
{
    mCsv.Save(mFilePath);
}

string CsvWriter::GetOutputFilePath() const
{
    return mFilePath;
}

void CsvWriter::SetColumnLabel(int idx, const string &label)
{
    mCsv.SetColumnName(idx, label);
}

void CsvWriter::SetRowLabel(int idx, const string &label)
{
    mCsv.SetRowName(idx, label);
}


}  // namespace OpenOasis::Utils