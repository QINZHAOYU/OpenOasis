/** ***********************************************************************************
 *    Copyright (C) 2023, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Field.h
 *    @License   :  Apache-2.0
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Utils/CommMacros.h"
#include "Models/Utils/CommConstants.h"
#include "Vector.h"
#include "Tensor.h"
#include <vector>
#include <algorithm>


namespace OpenOasis::CommImp::Numeric
{
/// @brief The field type enum.
enum class FieldType
{
    SCALAR,
    VECTOR,
    TENSOR,
    NONE,
};


/// @brief The field domain enum.
enum class FieldDomain
{
    NODE,
    FACE,
    CELL,
    NONE,
};


/// @brief The abstract field class used to represent numeric field, such as
/// scalar, vector or tensor data.
template <typename T>
class Field
{
protected:
    FieldDomain    mDomain = FieldDomain::NONE;
    FieldType      mType   = FieldType::NONE;
    std::vector<T> mData;

protected:
    virtual ~Field() = default;

    Field() = default;
    Field(std::size_t size, T value) : mData(size, value){};
    Field(const std::vector<T> &data) : mData(data){};
    Field(const Field<T> &other) :
        mData(other.mData), mDomain(other.mDomain), mType(other.mType){};

public:
    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for field manipulation.
    //

    /// @brief Initializes the vector field with specified value.
    /// @param value The initial value.
    void Initialize(T value)
    {
        std::fill(mData.begin(), mData.end(), value);
    }

    /// @brief Resizes the field.
    /// @param size New size.
    void Resize(std::size_t size)
    {
        mData.resize(size);
        mData.shrink_to_fit();
    }

    /// @brief Cleans the field data.
    void Clean()
    {
        std::fill(mData.begin(), mData.end(), T(0));
    }

    /// @brief Clears the field.
    void Clear()
    {
        mData.clear();
    }

    /// @brief Iterates the field and invoke given @p func for each element.
    ///
    /// This function iterates the field elements and invoke
    /// the callback function @p func. The callback function takes
    /// field's element as its input.
    template <typename Callback>
    void ForEach(Callback func)
    {
        for (auto &elem : mData)
        {
            func(elem);
        }
    }

    /// @brief Sets the field data.
    void SetAt(std::size_t i, T value)
    {
        mData.at(i) = value;
    }

    /// @brief Sets the field data for the specified range.
    /// @param startIndex Start index of the range.
    /// @param endIndex End index of the range.
    /// @param other Source field.
    /// @param offset Index offset of the source field.
    void SetAt(
        std::size_t startIndex, std::size_t endIndex, const Field<T> &other,
        std::size_t offset = 0)
    {
        std::size_t j = offset;
        for (std::size_t i = startIndex; i < endIndex; ++i)
            mData.at(i) = other(j++);
    }

    /// @brief Returns a constant refrence to the raw data.
    const std::vector<T> &Raw() const
    {
        return mData;
    }

    /// @brief Returns the field size.
    std::size_t Size() const
    {
        return mData.size();
    }

    FieldType Type() const
    {
        return mType;
    }

    FieldDomain Domain() const
    {
        return mDomain;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Operators overrided for field manipulation.
    //

    const T &operator()(int i) const
    {
        return mData.at(i);
    }

    T &operator()(int i)
    {
        return mData.at(i);
    }

    void operator=(const Field<T> &other)
    {
        mData = other.mData;
    }

    void operator=(const T &value)
    {
        Initialize(value);
    }

    Field<T> operator+(const Field<T> &other) const
    {
        OO_ASSERT(other.Size() == Size());

        auto datas = mData;
        for (int i = 0; i < Size(); ++i)
        {
            datas[i] += other(i);
        }

        return datas;
    }

    void operator+=(const Field<T> &other)
    {
        OO_ASSERT(other.Size() == Size());

        for (int i = 0; i < Size(); ++i)
            mData[i] += other(i);
    }

    Field<T> operator-(const Field<T> &other) const
    {
        OO_ASSERT(other.Size() == Size());

        auto datas = mData;
        for (int i = 0; i < Size(); ++i)
        {
            datas[i] -= other(i);
        }

        return datas;
    }

    void operator-=(const Field<T> &other)
    {
        OO_ASSERT(other.Size() == Size());

        for (int i = 0; i < Size(); ++i)
            mData[i] -= other(i);
    }

    Field<T> operator*(double k) const
    {
        auto datas = mData;
        std::for_each(datas.begin(), datas.end(), [&k](auto &d) { d *= k; });

        return datas;
    }

    void operator*=(double k)
    {
        std::for_each(mData.begin(), mData.end(), [&k](auto &d) { d *= k; });
    }
};


/// @brief Scaler field.
template <typename T>
class ScalarField : public Field<T>
{
public:
    static_assert(
        std::is_arithmetic<T>::value,
        "ScalarField only can be instantiated with arithmetic types");

    virtual ~ScalarField() = default;

    ScalarField(std::size_t size, T val = 0, FieldDomain domain = FieldDomain::CELL) :
        Field<T>(size, val)
    {
        this->mDomain = domain;
        this->mType   = FieldType::SCALAR;
    }

    ScalarField(FieldDomain domain = FieldDomain::CELL)
    {
        this->mDomain = domain;
        this->mType   = FieldType::SCALAR;
    }
};


/// @brief Vector field.
template <typename T, std::size_t N = 3>
class VectorField : public Field<Vector<T, N>>
{
public:
    virtual ~VectorField() = default;

    VectorField(FieldDomain domain = FieldDomain::CELL)
    {
        this->mDomain = domain;
        this->mType   = FieldType::VECTOR;
    }

    VectorField(
        std::size_t size, const Vector<T, N> &val = {},
        FieldDomain domain = FieldDomain::CELL) :
        Field<Vector<T, N>>(size, val)
    {
        this->mDomain = domain;
        this->mType   = FieldType::VECTOR;
    }
};


/// @brief Tensor field.
template <typename T>
class TensorField : public Field<Tensor<T>>
{
public:
    virtual ~TensorField() = default;

    TensorField(FieldDomain domain = FieldDomain::CELL)
    {
        this->mDomain = domain;
        this->mType   = FieldType::TENSOR;
    }

    TensorField(
        std::size_t size, const Tensor<T> &val = {},
        FieldDomain domain = FieldDomain::CELL) :
        Field<Tensor<T>>(size, val)
    {
        this->mDomain = domain;
        this->mType   = FieldType::TENSOR;
    }
};


// Commonly used fields.

using ScalarFieldFp = ScalarField<Utils::real>;
using VectorFieldFp = VectorField<Utils::real>;
using TensorFieldFp = TensorField<Utils::real>;

}  // namespace OpenOasis::CommImp::Numeric
