/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Datetime.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide timer、timespan、datetime utils.
 *
 ** ***********************************************************************************/
#pragma once
#include <chrono>
#include <string>


namespace OpenOasis
{
namespace Utils
{
using SysClock  = std::chrono::system_clock;
using TimePoint = std::chrono::time_point<SysClock>;


/// @brief Ratio used in `std::chrono::duration`, repersenting time precision.
enum class DurationRatio
{
    nanoseconds,   // 10^-9(s)
    mircoseconds,  // 10^-6(s)
    milliseconds,  // 10^-3(s)
    seconds,       // 1(s)
    minutes,       // 60(s)
    hours          // 3600(s)
};


/// @brief Timer used for routine timing with multipile precisions(up to nanosecond).
class Timer
{
private:
    TimePoint mBegin;
    TimePoint mNow;

public:
    Timer();

    ///////////////////////////////////////////////////////////////////////////////////
    // Member methods used for timing.
    //

    /// @brief Resets the begin and current time point for this timer.
    void reset(const TimePoint &tp);

    /// @brief Gets the current time point of this timer.
    TimePoint GetNow() const;

    /// @brief Gets the begin time point of this timer.
    TimePoint GetBegin() const;

    /// @brief Gets elapsed time from the begin time point with specified precision.
    /// @param ratio The time precision.
    /// @return Elapsed time calculated with specified time precision(unit).
    int64_t ElapsedTimeFromBegin(DurationRatio ratio = DurationRatio::milliseconds);

    /// @brief Gets elapsed time from last timed time with specified precision.
    /// @param ratio The time precision.
    /// @return Elapsed time calculated with specified time precision(unit).
    int64_t ElapsedTimeFromLast(DurationRatio ratio = DurationRatio::milliseconds);

    ///////////////////////////////////////////////////////////////////////////////////
    // Static methods used for `TimePoint` object operating.
    //

    /// @brief Updates time point with a duration at specified time precision.
    ///
    /// @param tp The time point to be updated.
    /// @param dt The duration.
    /// @param ratio The time precision (unit).
    static void Update(TimePoint &tp, double dt, DurationRatio ratio);

    /// @brief Gets duration between two time points with specified precision.
    /// Duration may be negative according to inputs.
    ///
    /// @param beg The begin time point.
    /// @param end The end time point.
    /// @param ratio The time precision.
    ///
    /// @return Duration from `beg` to `end` time point.
    static int64_t
    CalculateDuration(const TimePoint &beg, const TimePoint &end, DurationRatio ratio);
};


/// @brief `TimeSpan` represents a time interval with microsecond precision.
class TimeSpan
{
private:
    std::chrono::microseconds mDuration;

public:
    TimeSpan();
    TimeSpan(const TimeSpan &ts);
    TimeSpan(const std::chrono::microseconds &micro);

    ///////////////////////////////////////////////////////////////////////////////////
    // Member methods for getting total duration of the time span.
    //

    double GetTotalSeconds() const;
    double GetTotalDays() const;
    double GetTotalHours() const;
    double GetTotalMinutes() const;

    ///////////////////////////////////////////////////////////////////////////////////
    // Member methods for time span operating and comparing.
    //

    void Add(const TimeSpan &ts);
    void Add(double seconds);
    void Substract(const TimeSpan &ts);
    void Multiply(int factor);

    /// @brief Returns a new `TimeSpan` with the absolute value of the current object.
    TimeSpan Duration() const;

    ///////////////////////////////////////////////////////////////////////////////////
    // Static methods for time span operating.
    //

    /// @brief Compare two instances of `TimeSpan` and return an integer indicating
    /// whether the first value is shorter, equal to, or longer than the second.
    ///
    /// @param ts1 `TimeSpan` object.
    /// @param ts2 `TimeSpan` object.
    ///
    /// @return If ts1 is shorter than ts2, return -1; if longer, return +1;
    /// if equal, return 0.
    static int Compare(const TimeSpan &ts1, const TimeSpan &ts2);

    /// @brief Parses the `TimeSpan` object from the string format.
    /// The string format is asked as "%H:%M:%S".
    static TimeSpan FromString(const std::string &str);

    /// @brief Converts the current `TimeSpan` object to the equivalent string format.
    /// The string format is "%H:%M:%S".
    static std::string ToString(const TimeSpan &ts);
};


/// @brief `DateTime` represents the date and the time of a moment time.
/// The time stamp precision is microsecond.
/// The dates are on a 24-hour basis.
class DateTime
{
private:
    TimePoint mDatetime;

public:
    DateTime();
    DateTime(const TimePoint &time);
    DateTime(const DateTime &date);
    DateTime(int year, int month, int day, int hour = 0, int minute = 0, int sec = 0);

    ///////////////////////////////////////////////////////////////////////////////////
    // Member methods used for getting portion of the datetime.
    //

    int Microsecond() const;
    int Millisecond() const;
    int Year() const;
    int Month() const;
    int Day() const;
    int Hour() const;
    int Minute() const;
    int Second() const;

    int GetDayOfWeek() const;
    int GetDayOfYear() const;

    ///////////////////////////////////////////////////////////////////////////////////
    // Member methods used for date time operating.
    //

    void AddTimeSpan(const TimeSpan &value);
    void AddSeconds(double seconds);
    void AddDays(double days);

    /// @brief Gets the time point.
    TimePoint GetTimePoint() const;

    /// @brief Gets the time stamp calculated since 1970-01-01 00:00:00.
    int64_t GetTimeStampInMicroSecs() const;
    double  GetTimeStampInDays() const;

    /// @brief Gets the time(seconds) elapsed since midnight.
    double GetTimeOfDay() const;

    /// @brief Gets a new `DateTime` with same date as this but time set to zero.
    DateTime GetDate() const;

    ///////////////////////////////////////////////////////////////////////////////////
    // Static methods used for date time operating.
    //

    /// @brief Convert the `DateTime` object to string with specified format.
    static std::string
    ToString(const DateTime &obj, const std::string &fmt = "%Y-%m-%d %H:%M:%S");

    /// @brief Parses the `DateTime` object from the string format.
    static DateTime
    FromString(const std::string &str, const std::string &fmt = "%Y-%m-%d %H:%M:%S");

    /// @brief Gets a new `DateTime` with current date but time set to 0:0:0.
    static DateTime Today();

    /// @brief Gets a new `DateTime` with current date and time.
    static DateTime Now();

    /// @brief Gets a new `DateTime` with origin date time(1970-01-01).
    static DateTime Zero();

    /// @brief Returns the days in the specified year and month.
    static int DaysInMonth(int year, int month);

    /// @brief Compares two instances of `DateTime` and return an integer indicating
    /// whether the first value is earlier, equal to, or later than the second.
    ///
    /// @param t1 `DateTime` object.
    /// @param t2 `DateTime` object.
    ///
    /// @return If t1 is earlier than t2, return -1; if later, return +1;
    /// if equal, return 0.
    static int Compare(const DateTime &t1, const DateTime &t2);

    /// @brief Calculates the time interval between two `DateTime` instances.
    static TimeSpan GetTimeSpan(const DateTime &t1, const DateTime &t2);
};


}  // namespace Utils
}  // namespace OpenOasis