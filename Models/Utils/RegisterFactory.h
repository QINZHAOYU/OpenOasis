/** ***********************************************************************************
 *    Copyright (C) 2024, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  RegisterFactory.h
 *    @License   :  Apache-2.0
 *
 ** ***********************************************************************************/
#pragma once
#include "StringHelper.h"
#include <functional>
#include <map>
#include <iostream>
#include <memory>
#include <type_traits>


namespace OpenOasis::Utils
{
/// @brief A singleton factory class to register and produce objects of a given class.
/// @tparam `CLS` The interface class to be registered.
/// @note Each class can only be registered once.
#define RegisterFactory(CLS)                                                           \
    class CLS##Register                                                                \
    {                                                                                  \
    public:                                                                            \
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<CLS, T>>>  \
        struct Register                                                                \
        {                                                                              \
            Register(const std::string &clsName)                                       \
            {                                                                          \
                auto regFunc = []() { return std::make_shared<T>(); };                 \
                CLS##Register::Get().mRegistry.emplace(clsName, regFunc);              \
            }                                                                          \
                                                                                       \
            template <typename... Args>                                                \
            Register(const std::string &clsName, Args... args)                         \
            {                                                                          \
                auto regFunc = [&]() { return std::make_shared<T>(args...); };         \
                CLS##Register::Get().mRegistry.emplace(clsName, regFunc);              \
            }                                                                          \
        };                                                                             \
                                                                                       \
        static std::shared_ptr<CLS> Produce(const std::string &clsName)                \
        {                                                                              \
            if (mRegistry.find(clsName) == mRegistry.end())                            \
                throw std::invalid_argument(StringHelper::FormatSimple(                \
                    "Class [{}] hasn't been registered.", clsName));                   \
            return mRegistry[clsName]();                                               \
        }                                                                              \
                                                                                       \
    private:                                                                           \
        CLS##Register(){};                                                             \
        CLS##Register(const CLS##Register &) = delete;                                 \
        CLS##Register(CLS##Register &&)      = delete;                                 \
                                                                                       \
        static CLS##Register &Get()                                                    \
        {                                                                              \
            static CLS##Register instance;                                             \
            return instance;                                                           \
        }                                                                              \
                                                                                       \
        static std::map<std::string, std::function<std::shared_ptr<CLS>()>> mRegistry; \
    };


#define REGISTER_CLS_VNAME(T) reg_cls_##T##_
#define REGISTER_CLS(CLS, T, id, ...)                                                  \
    static CLS##Register::Register<T> REGISTER_CLS_VNAME(T)(id, ##__VA_ARGS__);

}  // namespace OpenOasis::Utils