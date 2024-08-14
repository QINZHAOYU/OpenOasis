/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Vector.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Define Vector template class for numerical calculation.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Utils/CommMacros.h"
#include "Tensor.h"
#include <array>
#include <algorithm>
#include <numeric>


namespace OpenOasis
{
namespace CommImp
{
namespace Numeric
{
template <typename T, size_t N = 3>
class Vector
{
public:
    static_assert(N > 0, "Size of static-sized vector should be greater than zero.");
    static_assert(
        std::is_arithmetic<T>::value,
        "Vector only can be instantiated with arithmetic types");

private:
    std::array<T, N> mElement;

public:
    virtual ~Vector() = default;

    Vector()
    {
        mElement.fill(static_cast<T>(0));
    }

    Vector(const Vector &other)
    {
        mElement = other.mElement;
    }

    Vector(const std::array<T, N> &other)
    {
        mElement = other;
    }

    template <typename... Args>
    Vector(Args... args)
    {
        static_assert(sizeof...(args) == N, "Invalid number of parameters.");

        SetAt(0, args...);
    }

    template <typename U>
    Vector(const std::initializer_list<U> &lst)
    {
        Set(lst);
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for vector data setting.
    //

    template <typename U>
    void Set(const std::initializer_list<U> &lst)
    {
        OO_ASSERT(lst.size() >= N);

        size_t i = 0;
        for (const auto &val : lst)
            mElement.at(i++) = static_cast<T>(val);
    }

    void Set(const Vector &other)
    {
        for (size_t i = 0; i < N; ++i)
        {
            mElement[i] = other(i);
        }
    }

    template <typename... Args>
    void SetAt(size_t i, T v, Args... args)
    {
        mElement.at(i) = v;

        SetAt(i + 1, args...);
    }

    void SetAt(size_t i, T v)
    {
        mElement.at(i) = v;
    }

    constexpr size_t Size() const
    {
        return N;
    }

    T &operator()(size_t i)
    {
        return mElement.at(i);
    }

    const T &operator()(size_t i) const
    {
        return mElement.at(i);
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for vector attributes query.
    //

    T Sum() const
    {
        return std::accumulate(mElement.begin(), mElement.end(), T(0));
    }

    T Avg() const
    {
        return Sum() / static_cast<T>(N);
    }

    T Min() const
    {
        return std::min_element(mElement.begin(), mElement.end())->operator*();
    }

    size_t MinIndex() const
    {
        return std::min_element(mElement.begin(), mElement.end()) - mElement.begin();
    }

    /// @brief Return the component of the vector hasing the minimum absolute value.
    T AbsMin() const
    {
        return std::min_element(
                   mElement.begin(),
                   mElement.end(),
                   [](T a, T b) { return abs(a) < abs(b); })
            ->
            operator*();
    }

    size_t AbsMinIndex() const
    {
        return std::min_element(
                   mElement.begin(),
                   mElement.end(),
                   [](T a, T b) { return abs(a) < abs(b); })
               - mElement.begin();
    }

    T Max() const
    {
        return std::max_element(mElement.begin(), mElement.end())->operator*();
    }

    size_t MaxIndex() const
    {
        return std::max_element(mElement.begin(), mElement.end()) - mElement.begin();
    }

    /// @brief Return the component of the vector hasing the maximum absolute value.
    T AbsMax() const
    {
        return std::max_element(
                   mElement.begin(),
                   mElement.end(),
                   [](T a, T b) { return abs(a) < abs(b); })
            ->
            operator*();
    }

    size_t AbsMaxIndex() const
    {
        return std::max_element(
                   mElement.begin(),
                   mElement.end(),
                   [](T a, T b) { return abs(a) < abs(b); })
               - mElement.begin();
    }

    T Magnitude() const
    {
        return std::sqrt(Dot(*this));
    }

    bool IsEqual(const Vector &other) const
    {
        return std::equal(mElement.begin(), mElement.end(), other.mElement.begin());
    }

    bool IsZero() const
    {
        return IsEqual(Vector());
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for vector operations.
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

    void Add(const Vector &other)
    {
        OO_ASSERT(other.Size() == N);

        for (size_t i = 0; i < N; ++i)
            mElement[i] += other(i);
    }

    void Sub(T v)
    {
        for (T &val : mElement)
            val -= v;
    }

    void Sub(const Vector &other)
    {
        OO_ASSERT(other.Size() == N);

        for (size_t i = 0; i < N; ++i)
            mElement[i] -= other(i);
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

    T Dot(const Vector &other) const
    {
        OO_ASSERT(other.Size() == N);

        return std::inner_product(
            mElement.begin(), mElement.end(), other.mElement.begin(), T(0));
    }

    Vector Cross(const Vector &other) const
    {
        OO_ASSERT(other.Size() == 3);
        OO_ASSERT(N == 3);

        return Vector(
            mElement[1] * other(2) - mElement[2] * other(1),
            mElement[2] * other(0) - mElement[0] * other(2),
            mElement[0] * other(1) - mElement[1] * other(0));
    }

    Tensor<T> Dyadic(const Vector &other) const
    {
        OO_ASSERT(other.Size() == 3);
        OO_ASSERT(N == 3);

        Tensor<T> ret;
        ret.SetAt(0, other * mElement[0]);
        ret.SetAt(1, other * mElement[1]);
        ret.SetAt(2, other * mElement[2]);

        return ret;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Override operators for vector.
    //

    void operator=(const Vector &other)
    {
        mElement = other.mElement;
    }

    Vector operator+(const Vector &other) const
    {
        Vector ret(*this);
        ret.Add(other);

        return ret;
    }

    Vector operator+(T v) const
    {
        Vector ret(*this);
        ret.Add(v);

        return ret;
    }

    Vector &operator+=(const Vector &other)
    {
        Add(other);
        return *this;
    }

    Vector &operator+=(T v)
    {
        Add(v);
        return *this;
    }

    Vector operator-(const Vector &other) const
    {
        Vector ret(*this);
        ret.Sub(other);

        return ret;
    }

    Vector operator-(T v) const
    {
        Vector ret(*this);
        ret.Sub(v);

        return ret;
    }

    Vector &operator-=(const Vector &other)
    {
        Sub(other);
        return *this;
    }

    Vector &operator-=(T v)
    {
        Sub(v);
        return *this;
    }

    /// @brief Override `*` operator for vector multiplication.
    Vector operator*(T v) const
    {
        Vector ret(*this);
        ret.Mul(v);

        return ret;
    }

    /// @brief Override `*` operator for dot product.
    T operator*(const Vector &other) const
    {
        return Dot(other);
    }

    Vector &operator*=(T v)
    {
        Mul(v);
        return *this;
    }

    /// @brief Override `&` operator for cross product.
    Vector operator&(const Vector &other) const
    {
        return Cross(other);
    }

    Vector &operator&=(const Vector &other)
    {
        Set(Cross(other));
        return *this;
    }

    /// @brief Override `^` operator for dyadic product.
    Tensor<T> operator^(const Vector &other) const
    {
        return Dyadic(other);
    }

    bool operator==(const Vector &other) const
    {
        return IsEqual(other);
    }

    bool operator!=(const Vector &other) const
    {
        return !IsEqual(other);
    }
};

}  // namespace Numeric
}  // namespace CommImp
}  // namespace OpenOasis