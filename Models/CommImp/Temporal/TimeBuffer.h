/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  TimeBuffer.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide temporal buffering functionalities.
 *
 *    The TimeBuffer class provides buffering functionalities that will store values
 *    needed for a particular output item in memory and functionality that will
 *    interpolate, extrapolate and aggregate values from these values.
 *
 *    The content of the TimeBuffer is lists of corresponding times and ValueSets,
 *    where times can be TimeStamps or TimeSpans and the ValueSets are double[].
 *
 *    TimeBuffer objects may not contain mixtures of TimeSpans and TimeStamps.
 *    The number of Times (TimeSpans or TimeStamps) must equal the number of value
 *    sets (double[]) in the TimeBuffer.
 *
 *    When time spans are put in the buffer, it is assumed that there aren't holes
 *    between the spans, i.e. a time setup of:
 *
 *    |----|----|   |----|----|
 *    a    b    c   d    e    f
 *
 *    where data is missing between c and d isn't allowed (a value of 0 is assumed
 *    in such an interval).
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/CommImp/TimeSet.h"
#include "Models/CommImp/ValueSet2D.h"


namespace OpenOasis
{
namespace CommImp
{
namespace Temporal
{
using Utils::real;

/// @brief The TimeBuffer class provides temporal buffering functionality.
class TimeBuffer
{
protected:
    bool mDoExtrapolate = true;

    std::shared_ptr<ValueSet2D> mValues = nullptr;

    std::shared_ptr<TimeSet> mTimes = nullptr;

    real mRelaxationFactor = 0;

    int mLastBufferSizeMessageCounter = 0;

    bool mDoExtendedDataVerification = false;

public:
    virtual ~TimeBuffer()
    {}

    TimeBuffer();
    TimeBuffer(const TimeBuffer &TimeBuffer);

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for getting and setting the buffer extrapolation flag.
    //
    // Default is true. If false and a requested time is outside the available spans,
    // an exception is thrown.

    void SetDoExtrapolate(bool value);

    bool GetDoExtrapolate() const;

    /// @brief Adds corresponding values for time and values to the TimeBuffer.
    /// The AddValues method will internally make a copy of the added times and values.
    /// The reason for doing this is that the times and values arguments are reference,
    /// and the corresponding values could be changed by the owner of the classes.
    ///
    /// @param time Description of the time parameter.
    /// @param valueSet Description of the values parameter.
    void AddValues(const std::shared_ptr<ITime> &time, const std::vector<real> &values);

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for getting and setting the relaxation factor(must be in [0, 1]).
    //
    // Relaxation parameter is used when doing extrapolation.
    // A value of 1 results in nearest extrapolation whereas
    // a value 0 results in linear extrapolation.

    /// @brief Returns the values that corresponds to the requestedTime. The values
    /// may be found by interpolation, extrapolation and/or aggregation.
    std::vector<real> GetValues(const std::shared_ptr<ITime> &requestedTime);

    void SetRelaxationFactor(real value);

    real GetRelaxationFactor() const;

    std::shared_ptr<ITime> GetTimeAt(int timeStep) const;

    std::vector<real> GetValuesAt(int timeStep) const;

    std::shared_ptr<TimeSet> GetTimeSet() const;

    std::shared_ptr<ValueSet2D> GetValueSet() const;

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for getting and setting the data extension flag.
    //

    bool GetDoExtendedDataVerification() const;

    void SetDoExtendedDataVerification(bool value);

    /// @brief Checks weather the contents of the buffer is valid.
    void CheckBuffer() const;

    /// @brief Clears all records in the buffer associated to time that is earlier
    /// than the time specified.
    void ClearBefore(std::shared_ptr<ITime> time);

    /// @brief Clears all times and values in the buffer at/or later than the
    /// specified time.
    /// If the specified time is span, the start time is used.
    void ClearAfter(std::shared_ptr<ITime> time);

    void Reset();

    std::vector<std::vector<real>> GetAllValues();

    void SetOrAddValues(std::shared_ptr<ITime> time, std::vector<real> values);

    /// @brief Number of time streps in the buffer.
    int GetTimesCount() const;

    /// @brief The number of values in the valuesets contained in the buffer.
    int GetValuesCount() const;

protected:
    /// @brief Adds values to the buffer, check the time is increasing,
    /// not overlapping(spans), and has durations matching the TimeSet.
    void AddValuesToBuffer(
        const std::shared_ptr<ITime> &time, const std::vector<real> &values);

    void Create();

    /// @brief A ValueSet corresponding to a TimeSpan is calculated using interpolation
    /// or extrapolation in corresponding lists of ValueSets and TimeStamps.
    std::vector<real>
    MapFromTimeStampsToTimeSpan(const std::shared_ptr<ITime> &requestedTime);

    /// @brief A ValueSet for a time stamp is calculated using interpolation
    /// or extrapolation in corresponding lists of ValueSets and TimeSpans.
    std::vector<real>
    MapFromTimeSpansToTimeStamp(const std::shared_ptr<ITime> &requestedTimeStamp);

    /// @brief A ValueSet corresponding to a TimeSpan is calculated using interpolation
    /// or extrapolation in corresponding lists of ValueSets and TimeSpans.
    std::vector<real>
    MapFromTimeSpansToTimeSpan(const std::shared_ptr<ITime> &requestedTime);

    /// @brief A ValueSet corresponding to Time Stamp is calculated using interpolation
    /// or extrapolation in corresponding lists of ValueSets and TimeStamps.
    std::vector<real>
    MapFromTimeStampsToTimeStamp(const std::shared_ptr<ITime> &requestedTimeStamp);
};

}  // namespace Temporal
}  // namespace CommImp
}  // namespace OpenOasis