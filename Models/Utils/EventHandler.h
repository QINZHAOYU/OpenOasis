/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  EventHandler.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To implement event handler.
 *
 ** ***********************************************************************************/
#pragma once
#include <functional>
#include <list>
#include <vector>
#include <memory>
#include <string>


namespace OpenOasis
{
namespace Utils
{
/// @brief Events handler, improved observer pattern.
template <typename Ret, typename... Args>
class EventHandler final
{
public:
    using FuncType = std::function<Ret(Args...)>;

private:
    std::list<std::shared_ptr<FuncType>> mFunctionList;  // event function list.

public:
    EventHandler() = default;

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for attaching, detaching and invoking events.
    //

    std::list<std::shared_ptr<FuncType>> GetFunctions() const
    {
        return mFunctionList;
    }

    bool IsEmpty()
    {
        return mFunctionList.empty() ? true : false;
    }

    void Attach(const FuncType &func)
    {
        if (IsExisted(func))
            return;
        mFunctionList.push_back(std::make_shared<FuncType>(func));
    }

    void Detach(const FuncType &func)
    {
        mFunctionList.remove_if(
            [this, &func](const auto &fptr) { return IsSame(*fptr, func); });
    }

    void Clear()
    {
        mFunctionList.clear();
    }

    void Invoke(Args... args)
    {
        // @todo : consider to invoke parallelly.
        for (auto &it : mFunctionList)
        {
            try
            {
                (*it)(std::forward<Args>(args)...);
            }
            catch (...)
            {}
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Operators overloading.
    //

    void operator+=(const FuncType &func)
    {
        Attach(func);
    }

    void operator-=(const FuncType &func)
    {
        Detach(func);
    }

    void operator()(Args &...args)
    {
        Invoke(std::forward<Args>(args)...);
    }

private:
    bool IsExisted(const FuncType &func)
    {
        auto itr = std::find_if(
            mFunctionList.begin(),
            mFunctionList.end(),
            [this, &func](const auto &fptr) { return IsSame(*fptr, func); });

        return (itr != mFunctionList.end()) ? true : false;
    }

    bool IsSame(const FuncType &func1, const FuncType &func2)
    {
        auto Func1Code = func1.target_type().hash_code();
        auto Func2Code = func2.target_type().hash_code();
        return Func1Code == Func2Code;
    }
};

}  // namespace Utils
}  // namespace OpenOasis
