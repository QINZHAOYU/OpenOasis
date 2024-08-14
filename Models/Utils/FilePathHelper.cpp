/** ***********************************************************************************
 *    @File      :  FilePathHelper.cpp
 *    @Brief     :  To provide files and pathes checking、joining methods.
 *
 ** ***********************************************************************************/
#include "FilePathHelper.h"
#include <fstream>


namespace OpenOasis
{
namespace Utils
{
using namespace std;


// class FilePathHelper ---------------------------------------------------------

void FilePathHelper::MakeDirectory(const string &path)
{
    filesystem::create_directory(PathFromString(path));
}

void FilePathHelper::MakeFile(const string &path)
{
    ofstream file(PathFromString(path).generic_string(), ios::out);
}

bool FilePathHelper::FileExists(const string &path)
{
    return filesystem::is_regular_file(PathFromString(path));
}

bool FilePathHelper::DirectoryExists(const string &path)
{
    return filesystem::is_directory(PathFromString(path));
}

string FilePathHelper::Combine(const string &path1, const string &path2)
{
    return (PathFromString(path1) / PathFromString(path2)).generic_string();
}

bool FilePathHelper::IsPathRooted(const string &path)
{
    return PathFromString(path).has_root_path();
}

string FilePathHelper::GetFullPath(const string &path)
{
    return filesystem::absolute(PathFromString(path)).generic_string();
}

string FilePathHelper::GetFileName(const string &path)
{
    return filesystem::path(PathFromString(path)).filename().generic_string();
}

string FilePathHelper::GetDirectoryName(const string &path)
{
    return filesystem::path(PathFromString(path)).parent_path().generic_string();
}

string FilePathHelper::GetCurrentDirectory()
{
    return filesystem::current_path().generic_string();
}

// void FilePathHelper::CopyFile(const string &path1, const string &path2)
// {
//     filesystem::copy_file(PathFromString(path1), PathFromString(path2));
// }

// void FilePathHelper::RenamePath(const string &path1, const string &path2)
// {
//     filesystem::rename(PathFromString(path1), PathFromString(path2));
// }

filesystem::path FilePathHelper::PathFromString(const string &path)
{
    return filesystem::path(&path[0]);
}

}  // namespace Utils
}  // namespace OpenOasis
