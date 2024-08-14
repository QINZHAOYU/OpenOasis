/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ValueSet2D.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To represent an ordered two-dimensional list of values.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IValueSet.h"
#include "Models/Inc/IQuantity.h"
#include "Models/Inc/IQuality.h"
#include "Models/Utils/CommConstants.h"


namespace OpenOasis
{
namespace CommImp
{
using namespace Utils;

/// @brief Common implementation of `IValueSet` in two dimension (time-space).
class ValueSet2D : public IValueSet
{
protected:
    std::vector<std::vector<std::any>> mValues2D;
    std::shared_ptr<IValueDefinition>  mValueDef;

public:
    virtual ~ValueSet2D()
    {}
    ValueSet2D()
    {}

    ValueSet2D(std::shared_ptr<IQuantity> valueDef);
    ValueSet2D(const std::shared_ptr<IValueSet> &valueSet);
    ValueSet2D(ValueSet2D &&obj);

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IValueSet`.
    //

    virtual std::shared_ptr<IValueDefinition> GetValueDefinition() const override;

    virtual std::any GetValue(const std::vector<int> &indices) const override;

    virtual void
    SetOrAddValue(const std::vector<int> &indices, const std::any &value) override;

    virtual void RemoveValue(const std::vector<int> &indices) override;

    virtual int GetNumberOfIndices() const override;

    virtual int GetIndexCount(const std::vector<int> &indices) const override;

    virtual bool IsValues2D() const override;

    virtual std::vector<std::any>
    GetTimeSeriesValuesForElement(int elementIndex) const override;

    virtual void SetTimeSeriesValuesForElement(
        int elementIndex, const std::vector<std::any> &values) override;

    virtual std::vector<std::any> GetElementValuesForTime(int timeIndex) const override;

    virtual void SetElementValuesForTime(
        int timeIndex, const std::vector<std::any> &values) override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Local methods for convenience.
    //

    int GetElementsCount(int timeIndex) const;

    int GetTimesCount() const;

    void SetValueDefinition(std::shared_ptr<IValueDefinition> value);

protected:
    // To check if given indices out of the valueset dimensions.
    void CheckIndicesOutOfDimension(const std::vector<int> &indices) const;

    // To check if each dimension were specified.
    void CheckAllDimensionSpecified(const std::vector<int> &indices) const;

    std::vector<std::vector<std::any>>
    GetValues(const std::shared_ptr<IValueSet> &valueSet) const;

    void AddValue(const std::vector<int> &indices, const std::any &value);

    void CheckTimeIndex(int timeIndex) const;

    void CheckElementIndex(int timeIndex, int elementIndex) const;

    // To check if the value type match the value set while adding/setting value in.
    virtual bool IsValidValueType(const std::any &value) const;
};


/// @brief Two-dimensional value set contains integer data.
class ValueSetInt : public ValueSet2D
{
public:
    virtual ~ValueSetInt() = default;
    ValueSetInt(
        const std::vector<std::vector<int>> &values2D,
        std::shared_ptr<IQuantity>           valueDef);

private:
    bool IsValidValueType(const std::any &value) const override;
};


/// @brief Two-dimensional value set contains float-point data.
class ValueSetFP : public ValueSet2D
{
public:
    virtual ~ValueSetFP() = default;
    ValueSetFP(
        const std::vector<std::vector<real>> &values2D,
        std::shared_ptr<IQuantity>            valueDef);

private:
    bool IsValidValueType(const std::any &value) const override;
};

}  // namespace CommImp
}  // namespace OpenOasis