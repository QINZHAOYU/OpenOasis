/** ***********************************************************************************
 *    Copyright (C) 2023, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  MultiIndexMapper.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Customized unordered_map using multiple indexes as key.
 *
 ** ***********************************************************************************/
#pragma once
#include <unordered_map>
#include <array>


namespace OpenOasis::Utils
{
/// @brief Customized unordered_map using multiple indexes as key.
/// @tparam N The number of indexes.
/// @note Indexes have order, different order means different key.
template <size_t N = 2>
class MultiIndex
{
public:
    static_assert(N > 1, "Using std::unordered_map<size_t, T> instead.");

    MultiIndex() = default;
    MultiIndex(const std::array<size_t, N> &lst) : indexes(lst)
    {}

    std::array<size_t, N> indexes;
};

template <size_t N = 2>
struct MultiEqualFunc
{
    bool operator()(const MultiIndex<N> &lhs, const MultiIndex<N> &rhs) const
    {
        for (size_t i = 0; i < N; ++i)
        {
            if (lhs.indexes[i] != rhs.indexes[i])
                return false;
        }
        return true;
    }
};

template <size_t N = 2>
struct MultiHashFunc
{
    size_t operator()(const MultiIndex<N> &key) const
    {
        size_t res = 0;
        for (size_t i = 0; i < N; ++i)
        {
            res += std::hash<size_t>()(key.indexes[i]) / N;
        }
        return res;
    }
};

template <typename T, size_t N = 2>
using MultiIndexMap =
    std::unordered_map<MultiIndex<N>, T, MultiHashFunc<N>, MultiEqualFunc<N>>;


/// @brief Customized unordered_map using a tuple of indexes as key.
/// @tparam N The number of indexes.
/// @note Indexes have no order, different orders mean one same key.
template <size_t N = 2>
class TupleIndex
{
public:
    static_assert(N > 1, "Using std::unordered_map<size_t, T> instead.");

    TupleIndex() = default;
    TupleIndex(const std::array<size_t, N> &lst) : indexes(lst)
    {}

    std::array<size_t, N> indexes;
};

template <size_t N = 2>
struct TupleEqualFunc
{
    bool operator()(const TupleIndex<N> &lhs, const TupleIndex<N> &rhs) const
    {
        for (size_t i = 0; i < N; ++i)
        {
            size_t j = 0;
            for (; j < N; ++j)
            {
                if (lhs.indexes[i] == rhs.indexes[j])
                    break;
            }

            if (j >= N)
                return false;
        }
        return true;
    }
};

template <size_t N = 2>
struct TupleHashFunc
{
    size_t operator()(const TupleIndex<N> &key) const
    {
        size_t res = 0;
        for (size_t i = 0; i < N; ++i)
        {
            res += std::hash<size_t>()(key.indexes[i]) / N;
        }
        return res;
    }
};

template <typename T, size_t N = 2>
using TupleIndexMap =
    std::unordered_map<TupleIndex<N>, T, TupleHashFunc<N>, TupleEqualFunc<N>>;

}  // namespace OpenOasis::Utils