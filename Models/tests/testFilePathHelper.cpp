#include "ThirdPart/Catch2/catch.hpp"
#include "Models/Utils/FilePathHelper.h"
#include "Models/Utils/CommMacros.h"
#include <locale>

using namespace OpenOasis::Utils;
using namespace std;


TEST_CASE("FilePathHelper tests")
{
#ifdef LINUX
    std::locale::global(std::locale(""));
    CHECK(2 == 2);
#elif defined(WINDOWS)
    setlocale(LC_ALL, ".65001");
#endif

    string newDir  = "./temprary/";
    string newDir2 = "./目录/";
    FilePathHelper::MakeDirectory(newDir);
    FilePathHelper::MakeDirectory(newDir2);
    REQUIRE(FilePathHelper::DirectoryExists(newDir) == true);
    REQUIRE(FilePathHelper::DirectoryExists("./haha/") == false);
    CHECK(FilePathHelper::DirectoryExists(newDir2) == true);
    // CHECK(FilePathHelper::GetFullPath(newDir) == "");

    string file = "./temprary/test.txt";
    // string file2 = "./temprary/杭州.txt";
    FilePathHelper::MakeFile(file);
    // FilePathHelper::MakeFile(file2);
    REQUIRE(FilePathHelper::FileExists(file) == true);
    REQUIRE(FilePathHelper::FileExists("./temprary/test.doc") == false);
    REQUIRE(FilePathHelper::GetFileName(file) == "test.txt");
    // REQUIRE(FilePathHelper::FileExists(file2) == true);
    // REQUIRE(FilePathHelper::GetFileName(file2) == "杭州.txt");

    FilePathHelper::MakeDirectory("./temprary/test2.txt");
    FilePathHelper::MakeFile("./temprary/haha");
    FilePathHelper::MakeFile("./temprary/haha2/");
    FilePathHelper::MakeFile("./temprary/haha2/test.txt");

    REQUIRE(FilePathHelper::DirectoryExists(file) == false);
    REQUIRE(FilePathHelper::FileExists(newDir) == false);
    // CHECK(FilePathHelper::GetFullPath(file) == "");
    CHECK(FilePathHelper::GetFileName(newDir) == "");
    CHECK(FilePathHelper::GetDirectoryName(newDir) == "./temprary");
    // CHECK(FilePathHelper::GetCurrentDirectory() == "");
    CHECK(FilePathHelper::GetDirectoryName("./temprary/test2.txt") == "./temprary");

    string subDir = "subDir";
    CHECK(FilePathHelper::Combine(newDir, subDir) == "./temprary/subDir");
    // CHECK(FilePathHelper::Combine(newDir2, "file.txt") == "./目录/file.txt");
    CHECK(FilePathHelper::IsPathRooted(newDir) == false);
    CHECK(FilePathHelper::IsPathRooted(FilePathHelper::GetFullPath(newDir)) == true);

    // FilePathHelper::CreateDirectory("./temprary/dir");
    // FilePathHelper::CopyFile(
    //     FilePathHelper::GetFullPath(file),
    //     FilePathHelper::GetFullPath("./temprary/dir/"));
    // FilePathHelper::CopyFile(file2, newDir2);
    // CHECK(FilePathHelper::FileExists(FilePathHelper::Combine(newDir2, file2)) ==
    // true);

    // FilePathHelper::RenamePath(newDir2, "folder");
    // CHECK(FilePathHelper::DirectoryExists("./folder/") == true);
}