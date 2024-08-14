/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ITimeSet.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  A set of time stamps or time intervals, used to indicate when an
 *                  output item has values and can provide values, and when an input
 *                  item does or may require values.
 *
 ** ***********************************************************************************/
#pragma once
#include "ITime.h"
#include <vector>
#include <memory>


namespace OpenOasis
{
/// @brief Set of time stamps or time intervals, used to indicate when an output has
/// and can provide values, and when an input item does or may require values.
class ITimeSet
{
public:
    /// @brief Time stamps or spans as available in of an output item,
    /// or as required by an input item. Times should be ordered.
    ///
    /// Specific values:
    /// + `TimeSet::GetTimes().size() == 0`, in case of input:  time dependent item,
    ///   but currently no values required.
    /// + `TimeSet::GetTimes().size() == 0`, in case of output: time dependent item,
    ///   but no values available yet or required yet.
    virtual std::vector<std::shared_ptr<ITime>> GetTimes() const = 0;

    /// @brief Removes the `ITime` element with specified index.
    /// After removing time, the duration must be updated automatically.
    virtual void RemoveTime(int index) = 0;

    /// @brief Adds a `ITime` to this time set.
    ///
    /// Usually, a timeset item is ordered by timestamp, it means the insertion will
    /// trigger interval sorting.
    /// After adding time, the duration must be updated automatically.
    virtual void AddTime(std::shared_ptr<ITime> time) = 0;

    /// @brief Whether each `ITime` have duration.
    /// In this case,duration value greater than zero is expected for each
    /// `ITime` in the `GetTimes()` list.
    virtual bool HasDurations() const = 0;

    /// @brief Time zone offset from UTC, expressed in the number of hours.
    ///
    /// Since some of the world's time zone differ half an hour from their
    /// neighbour the value is specified as a double.
    virtual double GetOffsetFromUtcInHours() const = 0;

    /// @brief Gets the time horizon for what timespan it may require/provide values.
    ///
    /// The providers of this input can assume that the input item never go back
    /// further in time than the time horizon's begin time:
    /// `GetTimeHorizon()->GetTimeStamp()`. Also, it will never go further ahead
    /// than the time horizon's end time::
    /// `GetTimeHorizon()->GetTimeStamp() + GetTimeHorizon()->GetDurationInDays()`.
    ///
    /// For an output item, thus for an adapted output, the time horizon
    /// indicates in what time span the item can provide values. Specific values:
    /// `GetTimeHorizon()->GetTimeStamp()` == - Infinity: far back in time;
    /// `GetTimeHorizon()->GetDurationInDays()` == + Infinity: far in the future.
    virtual std::shared_ptr<ITime> GetTimeHorizon() const = 0;
};

}  // namespace OpenOasis
