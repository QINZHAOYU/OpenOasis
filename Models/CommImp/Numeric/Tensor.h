/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Tensor.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Define Tensor template class for numerical calculation.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Utils/CommMacros.h"
#include <array>


namespace OpenOasis
{
namespace CommImp
{
namespace Numeric
{
template <typename T, std::size_t N>
class Vector;

template <typename T>
class Tensor
{
private:
    std::array<T, 9> mElement;

public:
    static_assert(std::is_arithmetic<T>::value, "Tensor requires arithmetic types");

    Tensor()
    {
        mElement.fill(static_cast<T>(0));
    }

    Tensor(const Tensor &other)
    {
        mElement = other.mElement;
    }

    template <typename... Args>
    Tensor(Args... args)
    {
        static_assert(sizeof...(args) == 9, "Invalid number of parameters.");

        SetAt(0, args...);
    }

    template <typename U>
    Tensor(const std::initializer_list<U> &lst)
    {
        Set(lst);
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for tensor data setting.
    //

    void Set(const Tensor &other)
    {
        for (std::size_t i = 0; i < 9; ++i)
        {
            std::size_t j = i % 3;

            mElement[i] = other(i / 3, j);
        }
    }

    template <typename... Args>
    void SetAt(std::size_t i, T v, Args... args)
    {
        mElement.at(i) = v;

        SetAt(i + 1, args...);
    }

    void SetAt(std::size_t i, T v)
    {
        mElement.at(i) = v;
    }

    void SetAt(std::size_t i, std::size_t j, T v)
    {
        mElement.at(i * 3 + j) = v;
    }

    void SetAt(std::size_t i, const Vector<T, 3> &vec)
    {
        for (std::size_t j = 0; j < 3; ++j)
            mElement.at(i * 3 + j) = vec(j);
    }

    template <typename U>
    void Set(const std::initializer_list<U> &lst)
    {
        OO_ASSERT(lst.size() >= 9);

        std::size_t i = 0;
        for (const auto &v : lst)
            mElement.at(i++) = static_cast<T>(v);
    }

    constexpr std::size_t Size() const
    {
        return 3;
    }

    T &operator()(std::size_t i, std::size_t j)
    {
        return mElement.at(i * 3 + j);
    }

    const T &operator()(std::size_t i, std::size_t j) const
    {
        return mElement.at(i * 3 + j);
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for tensor attributes query.
    //

    T Sum() const
    {
        return std::accumulate(mElement.begin(), mElement.end(), T(0));
    }

    T Avg() const
    {
        return Sum() / static_cast<T>(9);
    }

    T Min() const
    {
        return std::min_element(mElement.begin(), mElement.end())->operator*();
    }

    /// @brief Return the component of the tensor hasing the minimum absolute value.
    T AbsMin() const
    {
        return std::min_element(
                   mElement.begin(),
                   mElement.end(),
                   [](T a, T b) { return abs(a) < abs(b); })
            ->
            operator*();
    }

    T Max() const
    {
        return std::max_element(mElement.begin(), mElement.end())->operator*();
    }

    /// @brief Return the component of the tensor hasing the maximum absolute value.
    T AbsMax() const
    {
        return std::max_element(
                   mElement.begin(),
                   mElement.end(),
                   [](T a, T b) { return abs(a) > abs(b); })
            ->
            operator*();
    }

    /// @brief Return the tensor's magnitude.
    T Magnitude() const
    {
        return std::sqrt(DDot(*this));
    }

    bool IsEqual(const Tensor &other) const
    {
        for (std::size_t i = 0; i < 9; ++i)
        {
            if (abs(mElement[i] - other(i / 3, i % 3)) > T(1e-10))
                return false;
        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for tensor operations.
    //

    void Normalize()
    {
        T len = Magnitude();
        if (len <= T(0))
            return;

        Div(len);
    }

    void Add(T v)
    {
        for (T &val : mElement)
            val += v;
    }

    void Add(const Tensor &other)
    {
        for (std::size_t i = 0; i < 9; ++i)
            mElement[i] += other(i / 3, i % 3);
    }

    void Sub(T v)
    {
        for (T &val : mElement)
            val -= v;
    }

    void Sub(const Tensor &other)
    {
        for (std::size_t i = 0; i < 9; ++i)
            mElement[i] -= other(i / 3, i % 3);
    }

    void Mul(T v)
    {
        for (T &val : mElement)
            val *= v;
    }

    void Div(T v)
    {
        OO_ASSERT(v != T(0));

        for (T &val : mElement)
            val /= v;
    }

    Vector<T, 3> Dot(const Vector<T, 3> &other) const
    {
        Vector<T, 3> ret;

        for (std::size_t i = 0; i < 3; ++i)
        {
            ret(i) = mElement[i * 3] * other(0) + mElement[i * 3 + 1] * other(1)
                     + mElement[i * 3 + 2] * other(2);
        }

        return ret;
    }

    T DDot(const Tensor &other) const
    {
        T ret = 0;

        for (std::size_t i = 0; i < 3; ++i)
        {
            for (std::size_t j = 0; j < 3; ++j)
            {
                ret += mElement[i * 3 + j] * other(j, i);
            }
        }

        return ret;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Override operators for tensor.
    //

    void operator=(const Tensor &other)
    {
        mElement = other.mElement;
    }

    Tensor operator+(const Tensor &other) const
    {
        Tensor ret(*this);
        ret.Add(other);

        return ret;
    }

    Tensor operator+(T v) const
    {
        Tensor ret(*this);
        ret.Add(v);

        return ret;
    }

    Tensor &operator+=(const Tensor &other)
    {
        Add(other);
        return *this;
    }

    Tensor &operator+=(T v)
    {
        Add(v);
        return *this;
    }

    Tensor operator-(const Tensor &other) const
    {
        Tensor ret(*this);
        ret.Sub(other);

        return ret;
    }

    Tensor operator-(T v) const
    {
        Tensor ret(*this);
        ret.Sub(v);

        return ret;
    }

    Tensor &operator-=(const Tensor &other)
    {
        Sub(other);
        return *this;
    }

    Tensor &operator-=(T v)
    {
        Sub(v);
        return *this;
    }

    Vector<T, 3> operator*(const Vector<T, 3> &other) const
    {
        return Dot(other);
    }

    T operator*(const Tensor &other) const
    {
        return DDot(other);
    }
};

}  // namespace Numeric
}  // namespace CommImp
}  // namespace OpenOasis