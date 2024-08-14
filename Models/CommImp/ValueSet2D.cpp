/** ***********************************************************************************
 *    @File      :  ValueSet2D.cpp
 *    @Brief     :  To represent an ordered two-dimensional list of values.
 *
 ** ***********************************************************************************/
#include "ValueSet2D.h"
#include "Quantity.h"
#include "Models/Utils/Exception.h"
#include "Models/Utils/StringHelper.h"
#include <algorithm>
#include <functional>


namespace OpenOasis::CommImp
{
using namespace Utils;
using namespace std;


// class ValueSet2D--------------------------------------------------------------------

ValueSet2D::ValueSet2D(shared_ptr<IQuantity> valueDef)
{
    mValueDef = valueDef;
}

ValueSet2D::ValueSet2D(const shared_ptr<IValueSet> &valueSet)
{
    mValues2D = GetValues(valueSet);

    auto valDef = dynamic_pointer_cast<IQuantity>(valueSet->GetValueDefinition());
    mValueDef   = make_shared<Quantity>(valDef);
}

ValueSet2D::ValueSet2D(ValueSet2D &&obj)
{
    mValueDef = obj.mValueDef;
    mValues2D = obj.mValues2D;
}

shared_ptr<IValueDefinition> ValueSet2D::GetValueDefinition() const
{
    return mValueDef;
}

any ValueSet2D::GetValue(const std::vector<int> &indices) const
{
    CheckAllDimensionSpecified(indices);

    int tIndex = indices[0], eIndex = indices[1];
    CheckTimeIndex(tIndex);
    CheckElementIndex(tIndex, eIndex);

    return mValues2D.at(tIndex).at(eIndex);
}

void ValueSet2D::SetOrAddValue(const std::vector<int> &indices, const any &value)
{
    CheckAllDimensionSpecified(indices);

    int tIndex = indices[0], eIndex = indices[1];

    if (tIndex < 0 || eIndex < 0)
    {
        throw IllegalArgumentException("Negative time or element index.");
    }

    if (tIndex > GetTimesCount())
    {
        throw IllegalArgumentException(StringHelper::FormatSimple(
            "Time index [{}] far exceed valueset time range [{}] .",
            tIndex,
            GetTimesCount()));
    }

    if (tIndex < GetTimesCount())
    {
        if (!IsValidValueType(value))
        {
            throw IllegalArgumentException(StringHelper::FormatSimple(
                "The set value type [{}] doesn't match the valueset [{}] .",
                value.type().name(),
                mValues2D[tIndex][eIndex].type().name()));
        }

        if (eIndex < GetElementsCount(tIndex))
        {
            mValues2D[tIndex][eIndex] = value;
        }
        else
        {
            while (eIndex >= GetElementsCount(tIndex))
                mValues2D[tIndex].push_back(value);
        }
    }
    else
    {
        AddValue(indices, value);
    }
}

int ValueSet2D::GetNumberOfIndices() const
{
    // Two-dimensional valueset.
    return 2;
}

int ValueSet2D::GetIndexCount(const std::vector<int> &indices) const
{
    // Check parameter validity.
    CheckIndicesOutOfDimension(indices);

    if (indices.size() == 1)
        return mValues2D.size();

    // Query the length of first time-dimension.
    int tIndex = indices[0];
    if (tIndex >= GetTimesCount())
    {
        throw IllegalArgumentException(StringHelper::FormatSimple(
            "The first query index [{}] out of range [{}] .", tIndex, GetTimesCount()));
    }

    // Query the length of specified index in second element-dimension.
    int eIndex = indices[1];
    CheckElementIndex(tIndex, eIndex);

    return mValues2D[tIndex].size();
}

void ValueSet2D::AddValue(const vector<int> &indices, const any &value)
{
    // Add given value at new time index.
    int tIndex = GetTimesCount();
    int eIndex = indices[1];

    try
    {
        if (!IsValidValueType(value))
        {
            throw IllegalArgumentException(StringHelper::FormatSimple(
                "The added value type [{}] doesn't match the valueset [{}] .",
                value.type().name(),
                mValues2D[tIndex][eIndex].type().name()));
        }

        mValues2D.push_back(vector<any>(eIndex + 1));

        auto &values   = mValues2D[tIndex];
        values[eIndex] = value;
    }
    catch (const bad_any_cast &e)
    {
        throw InvalidOperationException(
            "The added value doesn't match the specified dimension.");
    }
}

void ValueSet2D::RemoveValue(const vector<int> &indices)
{
    if (mValues2D.empty())
        return;

    // Check paramter validity.
    CheckIndicesOutOfDimension(indices);

    // Remove a time.
    int tIndex = indices[0];
    CheckTimeIndex(tIndex);

    if (indices.size() == 1)
    {
        mValues2D.erase(mValues2D.begin() + tIndex);
        return;
    }

    // Remove a element.
    int eIndex = indices[1];
    CheckElementIndex(tIndex, eIndex);

    auto &source = mValues2D[tIndex];
    source.erase(source.begin() + eIndex);
}

bool ValueSet2D::IsValues2D() const
{
    // Time(ITimeSet)-Space(IElementSet) value set.
    return true;
}

vector<any> ValueSet2D::GetTimeSeriesValuesForElement(int elementIndex) const
{
    vector<any> values;
    for (size_t tIndex = 0; tIndex < mValues2D.size(); tIndex++)
    {
        values.push_back(mValues2D.at(tIndex).at(elementIndex));
    }

    return values;
}

void ValueSet2D::SetTimeSeriesValuesForElement(
    int elementIndex, const vector<any> &values)
{
    if ((int)values.size() != GetTimesCount())
    {
        throw IllegalArgumentException(
            "Invalid timeseries values length out of current valueset.");
    }

    for (int t = 0; t < GetTimesCount(); ++t)
    {
        if (!IsValidValueType(values[t]))
        {
            throw IllegalArgumentException(
                "The added value type doesn't match the value set.");
        }
        mValues2D[t][elementIndex] = values[t];
    }
}

vector<any> ValueSet2D::GetElementValuesForTime(int timeIndex) const
{
    CheckTimeIndex(timeIndex);
    return mValues2D.at(timeIndex);
}

void ValueSet2D::SetElementValuesForTime(int timeIndex, const vector<any> &values)
{
    CheckTimeIndex(timeIndex);

    if (values.size() != mValues2D[timeIndex].size())
    {
        throw IllegalArgumentException(
            "Invalid elements values length out of current valueset.");
    }

    if (!all_of(
            begin(values),
            end(values),
            bind(&ValueSet2D::IsValidValueType, this, placeholders::_1)))
    {
        throw IllegalArgumentException(
            "The set element value type doesn't match the valueset.");
    }

    mValues2D[timeIndex] = values;
}

int ValueSet2D::GetTimesCount() const
{
    if (mValues2D.empty())
        return 0;

    return mValues2D.size();
}

int ValueSet2D::GetElementsCount(int timeIndex) const
{
    if (timeIndex < 0 || timeIndex >= GetTimesCount())
        return 0;

    if (mValues2D[timeIndex].empty())
        return 0;

    return mValues2D[timeIndex].size();
}

void ValueSet2D::SetValueDefinition(shared_ptr<IValueDefinition> value)
{
    mValueDef = dynamic_pointer_cast<IQuantity>(value);
}

vector<vector<any>> ValueSet2D::GetValues(const shared_ptr<IValueSet> &valueSet) const
{
    if (!valueSet->IsValues2D())
        return {};

    int times    = valueSet->GetIndexCount({0});
    int elements = valueSet->GetIndexCount({0, 0});

    vector<vector<any>> data(times);
    for (int t = 0; t < times; ++t)
    {
        vector<any> ts(elements);
        for (int i = 0; i < elements; ++i)
        {
            ts[i] = valueSet->GetValue({t, i});
        }

        data[t] = ts;
    }

    return data;
}

bool ValueSet2D::IsValidValueType(const any &value) const
{
    return true;
}

void ValueSet2D::CheckIndicesOutOfDimension(const vector<int> &indices) const
{
    if (indices.empty())
    {
        throw IllegalArgumentException("Empty indices specified.");
    }

    if (indices.size() > 2)
    {
        throw ArgumentOutOfRangeException(
            "The given indices were out of the value set dimensions(2).");
    }
}

void ValueSet2D::CheckAllDimensionSpecified(const vector<int> &indices) const
{
    if (indices.size() != 2)
    {
        throw ArgumentOutOfRangeException(
            "Invalid given indices exceeded or omitted the value set dimensions(2).");
    }
}

void ValueSet2D::CheckTimeIndex(int timeIndex) const
{
    if (timeIndex < 0)
    {
        throw IllegalArgumentException(
            "Invalid timeindex (" + to_string(timeIndex)
            + "), negative index not allowed.");
    }

    int size = GetTimesCount();
    if (timeIndex >= size)
    {
        throw IllegalArgumentException(
            "Invalid timeindex (" + to_string(timeIndex) + "), only " + to_string(size)
            + " times available.");
    }
}

void ValueSet2D::CheckElementIndex(int timeIndex, int elementIndex) const
{
    if (elementIndex < 0)
    {
        throw IllegalArgumentException(
            "Invalid elementindex (" + to_string(elementIndex)
            + "), negative index nog allowed.");
    }

    int size = GetElementsCount(timeIndex);
    if (elementIndex >= size)
    {
        throw IllegalArgumentException(
            "Invalid elementindex (" + to_string(elementIndex) + "), only "
            + to_string(size) + " elements available.");
    }
}


// class ValueSetInt-------------------------------------------------------------------

ValueSetInt::ValueSetInt(
    const vector<vector<int>> &values2D, shared_ptr<IQuantity> valueDef)
{
    for (const auto &arr : values2D)
    {
        vector<any> tmp;
        for (const auto &val : arr)
        {
            tmp.emplace_back(val);
        }

        mValues2D.emplace_back(tmp);
    }

    mValueDef = valueDef;
}

bool ValueSetInt::IsValidValueType(const any &value) const
{
    return value.type() == typeid(int);
}


// class ValueSetFP-------------------------------------------------------------------

ValueSetFP::ValueSetFP(
    const vector<vector<real>> &values2D, shared_ptr<IQuantity> valueDef)
{
    for (const auto &arr : values2D)
    {
        vector<any> tmp;
        for (const auto &val : arr)
        {
            tmp.emplace_back(val);
        }

        mValues2D.emplace_back(tmp);
    }

    mValueDef = valueDef;
}

bool ValueSetFP::IsValidValueType(const any &value) const
{
    return value.type() == typeid(real);
}


}  // namespace OpenOasis::CommImp