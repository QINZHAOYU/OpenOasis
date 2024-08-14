/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  LibraryLoader.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide dynamic libraries loader.
 *
 ** ***********************************************************************************/
#pragma once
#include "CommMacros.h"
#include <string>
#include <map>
#include <functional>
#include <stdexcept>


namespace OpenOasis
{
namespace Utils
{
#ifdef WINDOWS

/// @brief Library loading and calling methods.
class LibraryLoader final
{
private:
    HMODULE                        mHMod;
    std::map<std::string, FARPROC> mFuncAddrMap;

public:
    LibraryLoader();
    ~LibraryLoader();

    bool Load(const std::string &libraryPath);
    bool UnLoad();

    template <typename T>
    std::function<T> GetFunction(const std::string &funcName)
    {
        if (mHMod == nullptr)
        {
            return nullptr;
        }

        auto it = mFuncAddrMap.find(funcName);
        if (it == mFuncAddrMap.end())
        {
            auto addr = GetProcAddress(mHMod, funcName.c_str());
            if (!addr)
            {
                return nullptr;
            }

            mFuncAddrMap.insert(std::make_pair(funcName, addr));
            it = mFuncAddrMap.find(funcName);
        }

        return std::function<T>((T *)(it->second));
    }

    template <typename T, typename... Args>
    typename std::result_of<std::function<T>(Args...)>::type
    RunFunction(const std::string &funcName, Args &&...args)
    {
        auto f = GetFunction<T>(funcName);
        if (f == nullptr)
        {
            std::string msg = "Invalid library function name " + funcName;
            throw std::logic_error(msg.c_str());
        }

        return f(std::forward<Args>(args)...);
    }
};

#elif defined(LINUX)

/// @brief Library loading and calling methods.
class LibraryLoader final
{
private:
    using Parser_t = void *;

    Parser_t                        mHMod;
    std::map<std::string, Parser_t> mFuncAddrMap;

public:
    LibraryLoader();
    ~LibraryLoader();

    bool Load(const std::string &libraryPath);
    bool UnLoad();

    template <typename T>
    std::function<T> GetFunction(const std::string &funcName)
    {
        if (mHMod == nullptr)
        {
            return nullptr;
        }

        auto it = mFuncAddrMap.find(funcName);
        if (it == mFuncAddrMap.end())
        {
            auto addr = dlsym(mHMod, funcName.c_str());
            if (!addr)
            {
                return nullptr;
            }

            mFuncAddrMap.insert(std::make_pair(funcName, addr));
            it = mFuncAddrMap.find(funcName);
        }

        return std::function<T>((T *)(it->second));
    }

    template <typename T, typename... Args>
    typename std::result_of<std::function<T>(Args...)>::type
    RunFunction(const std::string &funcName, Args &&...args)
    {
        auto f = GetFunction<T>(funcName);
        if (f == nullptr)
        {
            std::string msg = "Invalid library function name " + funcName;
            throw std::logic_error(msg.c_str());
        }

        return f(std::forward<Args>(args)...);
    }
};

#elif defined(MACOS)

/// @brief Library loading and calling methods.
class LibraryLoader final
{
public:
    LibraryLoader()
    {}
    ~LibraryLoader()
    {}

    bool Load(const std::string &libraryPath)
    {
        return false;
    }
    bool UnLoad()
    {
        return false;
    };

    template <typename T>
    std::function<T> GetFunction(const std::string &funcName)
    {
        throw std::runtime_error("Not implemented.");
    }

    template <typename T, typename... Args>
    typename std::result_of<std::function<T>(Args...)>::type
    RunFunction(const std::string &funcName, Args &&...args)
    {
        throw std::runtime_error("Not implemented.");
    }
};

#endif

}  // namespace Utils
}  // namespace OpenOasis