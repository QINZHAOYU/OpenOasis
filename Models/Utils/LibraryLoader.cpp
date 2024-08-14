/** ***********************************************************************************
 *    @File      :  LibraryLoader.cpp
 *    @Brief     :  To provide dynamic libraries loader.
 *
 ** ***********************************************************************************/
#include "LibraryLoader.h"

using namespace OpenOasis::Utils;
using namespace std;


// class LibraryLoader -------------------------------------------------------------
#ifdef WINDOWS

LibraryLoader::LibraryLoader() : mHMod(nullptr)
{}

LibraryLoader::~LibraryLoader()
{
    UnLoad();
}

bool LibraryLoader::Load(const string &libraryPath)
{
    mHMod = LoadLibraryA(libraryPath.data());
    if (mHMod == nullptr)
    {
        return false;
    }

    return true;
}

bool LibraryLoader::UnLoad()
{
    if (mHMod == nullptr)
    {
        return true;
    }

    auto status = FreeLibrary(mHMod);
    if (!status)
    {
        return false;
    }

    mHMod = nullptr;
    mFuncAddrMap.clear();
    return true;
}


// class LinuxLibraryLoader -----------------------------------------------------------
#elif defined(LINUX)

LibraryLoader::LibraryLoader() : mHMod(nullptr)
{}

LibraryLoader::~LibraryLoader()
{
    UnLoad();
}

bool LibraryLoader::Load(const string &libraryPath)
{
    mHMod = dlopen(libraryPath.c_str(), RTLD_LAZY);
    if (mHMod == nullptr)
    {
        return false;
    }

    return true;
}

bool LibraryLoader::UnLoad()
{
    if (mHMod == nullptr)
    {
        return true;
    }

    int status = dlclose(mHMod);
    if (status != 0)
    {
        return false;
    }

    mHMod = nullptr;
    mFuncAddrMap.clear();
    return true;
}


#endif