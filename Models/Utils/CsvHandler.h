/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  CsvHandler.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide unified csv file read and write function.
 *    For this' a simple csv parser, it's less fault tolerant for the csv file format
 *    and has fewer parsing interfaces for csv data. More specifically,
 *
 *    1. The empty line is illegal, or at most, allowing the end line to be empty.
 *    2. Lines of unequal length are allowed and can't be completed, e.g ',', etc.
 *    3. Spaces around numbers do not affect parsing.
 *
 ** ***********************************************************************************/
#pragma once
#include "ThirdPart/RapidCsv/rapidcsv.h"
#include "DateTime.h"
#include <optional>

namespace OpenOasis
{
namespace Utils
{
/// @brief CsvLoader class loads data from a specified csv file.
/// @note Row or column index used in this class is zero-based.
class CsvLoader
{
private:
    rapidcsv::Document mCsv;
    bool               mHasColumnHeader;
    bool               mHasRowHeader;

public:
    CsvLoader(){};
    CsvLoader(
        const std::string &file, bool hasColumnHeader = true, bool hasRowHeader = true,
        char delimiter = ',', bool skipEmptyLine = true, char skipPrefix = '#');

    void LoadByContent(
        const std::string &content, bool hasColumnHeader = true,
        bool hasRowHeader = true, char delimiter = ',', bool skipEmptyLine = true,
        char skipPrefix = '#');

    void LoadByFile(
        const std::string &file, bool hasColumnHeader = true, bool hasRowHeader = true,
        char delimiter = ',', bool skipEmptyLine = true, char skipPrefix = '#');

    std::optional<std::vector<std::string>> GetColumnLabels() const;
    std::optional<std::vector<std::string>> GetRowLabels() const;

    int GetColumnCount() const;
    int GetRowCount() const;

    std::optional<std::vector<DateTime>>
    GetColumnAsDateTime(const std::string &label) const;

    std::optional<std::vector<DateTime>> GetColumnAsDateTime(int idx) const;

    template <typename T>
    std::optional<std::vector<T>> GetColumn(const std::string &label) const
    {
        try
        {
            return mCsv.GetColumn<T>(label);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }

    template <typename T>
    std::optional<std::vector<T>> GetColumn(int idx) const
    {
        try
        {
            return mCsv.GetColumn<T>(idx);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }

    template <typename T>
    std::optional<std::vector<T>> GetRow(const std::string &label) const
    {
        try
        {
            return mCsv.GetRow<T>(label);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }

    template <typename T>
    std::optional<std::vector<T>> GetRow(int idx) const
    {
        try
        {
            return mCsv.GetRow<T>(idx);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }

    template <typename T>
    std::optional<T>
    GetCell(const std::string &colLabel, const std::string &rowLabel) const
    {
        try
        {
            return mCsv.GetCell<T>(colLabel, rowLabel);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }

    template <typename T>
    std::optional<T> GetCell(const std::string &colLabel, int rowIdx) const
    {
        try
        {
            return mCsv.GetCell<T>(colLabel, rowIdx);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }

    template <typename T>
    std::optional<T> GetCell(int colIdx, int rowIdx) const
    {
        try
        {
            return mCsv.GetCell<T>(colIdx, rowIdx);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }

    template <typename T>
    std::optional<T> GetCell(int colIdx, const std::string &rowLabel) const
    {
        try
        {
            return mCsv.GetCell<T>(colIdx, rowLabel);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }
};


/// @brief CsvWriter class receives data and outputs it to a given csv file.
/// @note Row or column index used in this class is zero-based.
class CsvWriter
{
private:
    std::string        mFilePath;
    rapidcsv::Document mCsv;

    static unsigned int sTmpFileCount;

public:
    CsvWriter(const std::string &filePath, char delimiter = ',');

    void Save();

    std::string GetOutputFilePath() const;

    void SetColumnLabel(int idx, const std::string &label);
    void SetRowLabel(int idx, const std::string &label);

    template <typename T>
    void InsertColumn(int idx, const std::string &label, const std::vector<T> &values)
    {
        mCsv.InsertColumn(idx, values, label);
    }

    template <typename T>
    void InsertRow(int idx, const std::string &label, const std::vector<T> &values)
    {
        mCsv.InsertRow(idx, values, label);
    }
};

}  // namespace Utils
}  // namespace OpenOasis
