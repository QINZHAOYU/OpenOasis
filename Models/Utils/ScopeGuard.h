/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ScopeGuard.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Implement a scope guard to keep resource released.
 *
 ** ***********************************************************************************/
#pragma once
#include <type_traits>


namespace OpenOasis
{
namespace Utils
{
/// @brief A scope guard to keep resource released.
template <typename F>
class ScopeGuard
{
public:
    explicit ScopeGuard(F &&f) : mFunc(std::move(f)), mDismiss(false)
    {}
    explicit ScopeGuard(const F &f) : mFunc(f), mDismiss(false)
    {}

    ScopeGuard(ScopeGuard &&rhs) : mFunc(std::move(rhs.mFunc)), mDismiss(rhs.mDismiss)
    {
        rhs.Dismiss();
    }

    ~ScopeGuard()
    {
        if (!mDismiss && mFunc != nullptr)
        {
            mFunc();
        }
    }

    void Dismiss()
    {
        mDismiss = true;
    }

private:
    ScopeGuard();
    ScopeGuard(const ScopeGuard &);
    ScopeGuard &operator=(const ScopeGuard &);

private:
    F    mFunc;
    bool mDismiss;
};


template <typename F>
ScopeGuard<typename std::decay<F>::type> MakeGuard(F &&f)
{
    using scope_t = typename std::decay<F>::type;
    return ScopeGuard<scope_t>(std::forward<F>(f));
}

}  // namespace Utils
}  // namespace OpenOasis