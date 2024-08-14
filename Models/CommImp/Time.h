/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Time.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To support a time stamp as well as a time interval.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/ITime.h"
#include "Models/Inc/AdditionalControl/IComparable.h"
#include "Models/Utils/DateTime.h"
#include <memory>


namespace OpenOasis
{
namespace CommImp
{
using namespace AdditionalControl;
using namespace Utils;

/// @brief Generic implementation of `ITime`.
class Time : public ITime, public IComparable
{
public:
    static constexpr double EpsilonForTimeCompare = 1.e-6 * 1. / 24.0 / 3600.0;  // μs.

protected:
    double mDurationInDays = 0.;  // non-negative duration.
    double mTimeStamp      = 0.;  // Timestamp calculated in days since 1970/1/1 8:0:0.

public:
    virtual ~Time()
    {}

    Time();
    Time(const std::shared_ptr<ITime> &time);
    Time(const std::shared_ptr<ITime> &start, const std::shared_ptr<ITime> &end);
    Time(double timestampInDays, double durationInDays = 0);
    Time(const DateTime &start, const DateTime &end);
    Time(const DateTime &start, double durationInDays = 0);

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited form `ITime`.
    //

    double GetDurationInDays() const override;

    double GetTimeStamp() const override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited form `IComparable`.
    //

    int CompareTo(const std::shared_ptr<IComparable> &obj) const override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Static methods.
    //

    static std::string ToString(const std::shared_ptr<ITime> &time);

    static std::unique_ptr<ITime> ToITime(const DateTime &time);

    /// @brief Convert `ITime` to `Datetime` without duration(unused).
    static DateTime ToDatetime(const std::shared_ptr<ITime> &time);

protected:
    void SetTimeStamp(double value);
    void SetDurationInDays(double value);
};

}  // namespace CommImp
}  // namespace OpenOasis