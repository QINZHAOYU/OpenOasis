/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IValueSet.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To represent a general multi-dimensional set of values.
 *
 ** ***********************************************************************************/
#pragma once
#include "IValueDefinition.h"
#include <any>
#include <typeinfo>
#include <vector>
#include <memory>


namespace OpenOasis
{
/// @brief Class represents a general(ordered) multi-dimensional set of values.
///
/// The size of each dimension can vary, depending on the indices provided, e.g.
/// In a 2D matrix, each row can have different lengths. For example,
/// assuming the data is stored as a double[][] matrix, then matrix[1].Size
/// need not equal to matrix[2].Size.
class IValueSet
{
public:
    ///////////////////////////////////////////////////////////////////////////////////
    // Generic value set with multi-dimensional.
    //

    /// @brief Definition of the values in the value set.
    ///
    /// The "IValueDefinition" should never be returned directly; The all implementing
    /// classes should return either an `IQuality`, an `IQuantity`, or a custom
    /// derived value definition interface.
    virtual std::shared_ptr<IValueDefinition> GetValueDefinition() const = 0;

    /// @brief Returns the number of possible indices (dimensions).
    /// @return number of indices, zero based.
    virtual int GetNumberOfIndices() const = 0;

    /// @brief Returns the length of the dimension specified.
    ///
    /// To get the size of the specified dimension, use zero-length int array as input
    /// argument. Length of indice must be a least one
    /// smaller than the `GetNumberOfIndices()`.
    ///
    /// @param indices Indices of the dimension to get the size of.
    /// @return Length of the specified dimension.
    virtual int GetIndexCount(const std::vector<int> &indices) const = 0;

    /// @brief Returns the value object specified by the given indices array.
    /// @param indices Indices of each dimension.
    /// @return The value object for the given indices.
    /// @throw The length of the array of indices is N, so that the index
    /// for each dimension is specified. Otherwise throw an exception.
    virtual std::any GetValue(const std::vector<int> &indices) const = 0;

    /// @brief Removes the values specified by the given indices.
    ///
    /// It is possible to remove not just a single value item, but also the
    /// whole set of values for the given indices.
    ///
    /// @param indices Indices of specified dimension.
    virtual void RemoveValue(const std::vector<int> &indices) = 0;

    /// @brief Sets or adds the value object specified by the given indices.
    /// @param indices Indices of each dimension.
    /// @param value Value object to be set or added.
    /// @throw The length of the array of indices is N, so that the index
    /// for each dimension is specified. Otherwise throw an exception.
    virtual void
    SetOrAddValue(const std::vector<int> &indices, const std::any &value) = 0;

    ///////////////////////////////////////////////////////////////////////////////////
    // Two-dimensional list of values.
    //
    // The first dimension represents time corresponding to the `ITimeSet`,
    // the second contains each elements value corresponding to the `IElementSet`.
    //

    /// @brief Whether this value set is a specified two-dimensional type.
    virtual bool IsValues2D() const = 0;

    /// @brief Gets the values, for all times, for the given elementIndex.
    /// If the data is spatial independent, @p elementIndex
    /// must be specified as 0.
    ///
    /// @param elementIndex Index of element in `IElementSet`.
    /// @return The timeseries values.
    /// @throw If the @p elementIndex is out of range, throw an exception.
    virtual std::vector<std::any>
    GetTimeSeriesValuesForElement(int elementIndex) const = 0;

    /// @brief Sets the values, for all times, for the given elementIndex.
    /// If the data is spatial independent, @p elementIndex
    /// must be specified as 0.
    ///
    /// @param elementIndex Index of element in `IElementSet`.
    /// @param values Timeseries values for given element.
    /// @throw If the @p elementIndex is out of range, throw an exception.
    virtual void SetTimeSeriesValuesForElement(
        int elementIndex, const std::vector<std::any> &values) = 0;

    /// @brief Sets the values, for all elements, for the given timeIndex.
    /// If data is time independent, @p timeIndex
    /// must be specified as 0.
    ///
    /// @param timeIndex Index of time in `ITimeSet`.
    /// @param values Element valueset at given time.
    /// @throw If @p timeIndex is out of range, throw an exception.
    virtual void
    SetElementValuesForTime(int timeIndex, const std::vector<std::any> &values) = 0;

    /// @brief Gets the values, for all elements, for the given timeIndex.
    /// If data is time independent, @p timeIndex
    /// must be specified as 0.
    ///
    /// @param timeIndex Index of time in `ITimeSet`.
    /// @return Element valueset at given time.
    /// @throw If @p timeIndex is out of range, throw an exception.
    virtual std::vector<std::any> GetElementValuesForTime(int timeIndex) const = 0;
};

}  // namespace OpenOasis
