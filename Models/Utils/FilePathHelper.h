/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  FilePathHelper.hpp
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide files and pathes checking、joining methods.
 *
 ** ***********************************************************************************/
#pragma once
#include <string>
#include <filesystem>


namespace OpenOasis
{
namespace Utils
{
/// @brief File and path operation helper.
class FilePathHelper
{
public:
    /// @brief Creates a directory if not existed.
    /// It would not check if the path is a directory or a file.
    static void MakeDirectory(const std::string &path);

    /// @brief Creates a new empty file.
    /// It would not create the file if the path is a directory.
    static void MakeFile(const std::string &path);

    /// @brief Checks if a file existed.
    /// If it's a directory path, the checking result will be false.
    static bool FileExists(const std::string &path);

    /// @brief Checks if a directory existed.
    /// If it's a file path, the checking result will be false.
    static bool DirectoryExists(const std::string &path);

    /// @brief Combines two path strings.
    static std::string Combine(const std::string &path1, const std::string &path2);

    static bool IsPathRooted(const std::string &path);

    /// @brief Gets the full path of a directory or a file.
    static std::string GetFullPath(const std::string &path);

    /// @brief Gets the file name.
    /// If it's a directory path, return an empty string.
    static std::string GetFileName(const std::string &path);

    /// @brief Gets the directory name.
    /// It would not try to convert any part of the input path string.
    static std::string GetDirectoryName(const std::string &path);

    static std::string GetCurrentDirectory();

    // static void CopyFile(const std::string &from, const std::string &to);

    // static void RenamePath(const std::string &from, const std::string &to);

private:
    static std::filesystem::path PathFromString(const std::string &path);
};

}  // namespace Utils
}  // namespace OpenOasis
