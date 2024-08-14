/** ***********************************************************************************
 *    @File      :  Datetime.cpp
 *    @Brief     :  To provide timer、timespan、datetime utils.
 *
 ** ***********************************************************************************/
#include "DateTime.h"
#include <vector>
#include <regex>
#include <iomanip>
#include <sstream>


namespace OpenOasis::Utils
{
using namespace std;


// class Timer ------------------------------------------------------------------------

Timer::Timer()
{
    mBegin = SysClock::now();
    mNow   = mBegin;
}

void Timer::reset(const TimePoint &tp)
{
    mBegin = tp;
    mNow   = tp;
}

TimePoint Timer::GetNow() const
{
    return mNow;
}

TimePoint Timer::GetBegin() const
{
    return mBegin;
}

int64_t Timer::ElapsedTimeFromBegin(DurationRatio ratio)
{
    mNow = SysClock::now();
    return CalculateDuration(mBegin, mNow, ratio);
}

int64_t Timer::ElapsedTimeFromLast(DurationRatio ratio)
{
    auto now = SysClock::now();
    auto dur = CalculateDuration(mNow, now, ratio);

    mNow = now;
    return dur;
}

int64_t Timer::CalculateDuration(
    const TimePoint &beg, const TimePoint &end, DurationRatio ratio)
{
    int64_t dur = chrono::duration_cast<chrono::nanoseconds>(end - beg).count();

    switch (ratio)
    {
    case DurationRatio::milliseconds: return dur * 1.e-6;
    case DurationRatio::mircoseconds: return dur * 1.e-3;
    case DurationRatio::seconds: return dur * 1.e-9;
    case DurationRatio::hours: return (dur * 1.e-9) / 3600.0;
    case DurationRatio::minutes: return (dur * 1.e-9) / 60.0;
    case DurationRatio::nanoseconds: return dur;
    default: throw invalid_argument("Not supported DurationRatio.");
    }
}

void Timer::Update(TimePoint &tp, double dt, DurationRatio ratio)
{
    int64_t dt_nano = dt;
    switch (ratio)
    {
    case DurationRatio::seconds: dt_nano = dt * 1e9; break;
    case DurationRatio::milliseconds: dt_nano = dt * 1e6; break;
    case DurationRatio::mircoseconds: dt_nano = dt * 1e3; break;
    case DurationRatio::nanoseconds: break;
    case DurationRatio::minutes: dt_nano = 60 * dt * 1e9; break;
    case DurationRatio::hours: dt_nano = 3600 * dt * 1e9; break;
    }

    tp += chrono::duration_cast<SysClock::duration>(chrono::nanoseconds(dt_nano));
}


// class TimeSpan ---------------------------------------------------------------------

TimeSpan::TimeSpan()
{
    mDuration = chrono::microseconds(0);
}

TimeSpan::TimeSpan(const TimeSpan &ts)
{
    mDuration = ts.mDuration;
}

TimeSpan::TimeSpan(const chrono::microseconds &milli)
{
    mDuration = milli;
}

double TimeSpan::GetTotalDays() const
{
    int64_t dur = mDuration.count();
    double  val = dur / 24. / 3600. / 1.e6;

    return val;
}

double TimeSpan::GetTotalHours() const
{
    int64_t dur = mDuration.count();
    double  val = dur / 3600. / 1.e6;

    return val;
}

double TimeSpan::GetTotalMinutes() const
{
    int64_t dur = mDuration.count();
    double  val = dur / 60. / 1.e6;

    return val;
}

double TimeSpan::GetTotalSeconds() const
{
    int64_t dur = mDuration.count();
    double  val = dur / 1.e6;

    return val;
}

void TimeSpan::Add(const TimeSpan &ts)
{
    mDuration += ts.mDuration;
}

void TimeSpan::Add(double seconds)
{
    int64_t dur = seconds * 1.e6;
    mDuration += chrono::microseconds(dur);
}

void TimeSpan::Substract(const TimeSpan &ts)
{
    mDuration -= ts.mDuration;
}

void TimeSpan::Multiply(int factor)
{
    mDuration *= factor;
}

TimeSpan TimeSpan::Duration() const
{
    int64_t dur = abs(mDuration.count());
    return TimeSpan(chrono::microseconds(dur));
}

int TimeSpan::Compare(const TimeSpan &ts1, const TimeSpan &ts2)
{
    if (ts1.mDuration < ts2.mDuration)
    {
        return -1;
    }
    else if (ts1.mDuration > ts2.mDuration)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

TimeSpan TimeSpan::FromString(const string &str)
{
    regex                 reg(":");
    sregex_token_iterator iter(begin(str), end(str), reg, -1);

    vector<string> elems(iter, sregex_token_iterator());
    if (elems.empty() || elems.size() < 3)
    {
        throw runtime_error("Invalid time string: " + str + ", required \"%H:%M:%S\".");
    }

    double hours   = stod(elems[0]);
    double minutes = stod(elems[1]);
    double seconds = stod(elems[2]);

    int64_t dur = (hours * 3600. + minutes * 60. + seconds) * 1.e6;
    return TimeSpan(chrono::microseconds(dur));
}

string TimeSpan::ToString(const TimeSpan &ts)
{
    double dur        = ts.GetTotalSeconds();
    bool   isNegative = dur < 0.0;

    dur           = abs(dur);
    int64_t hours = dur / 3600;
    dur -= hours * 3600.0;

    int64_t minutes = dur / 60;
    dur -= minutes * 60.0;

    double seconds = dur;

    stringstream buff;
    if (isNegative)
        buff << "-";

    buff << setw(2) << setfill('0') << hours << ":" << setw(2) << setfill('0')
         << minutes << ":" << fixed << setprecision(6) << seconds;
    return buff.str();
}


// class DateTime ---------------------------------------------------------------------

DateTime::DateTime()
{
    mDatetime = TimePoint();
}

DateTime::DateTime(const TimePoint &tp)
{
    mDatetime = tp;
}

DateTime::DateTime(const DateTime &date)
{
    mDatetime = date.mDatetime;
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int sec)
{
    tm     t{sec, minute, hour, day, month - 1, year - 1900};
    time_t tt = mktime(&t);

    tm     tmp = *localtime(&tt);
    time_t tt2 = mktime(&tmp);
    mDatetime  = SysClock::from_time_t(tt2);
}

int DateTime::Year() const
{
    time_t tt = SysClock::to_time_t(mDatetime);
    tm    *t  = localtime(&tt);

    return t->tm_year + 1900;
}

int DateTime::Month() const
{
    time_t tt = SysClock::to_time_t(mDatetime);
    tm    *t  = localtime(&tt);

    return t->tm_mon + 1;
}

int DateTime::Day() const
{
    time_t tt = SysClock::to_time_t(mDatetime);
    tm    *t  = localtime(&tt);

    return t->tm_mday;
}

int DateTime::Hour() const
{
    time_t tt = SysClock::to_time_t(mDatetime);
    tm    *t  = localtime(&tt);

    return t->tm_hour;
}

int DateTime::Minute() const
{
    time_t tt = SysClock::to_time_t(mDatetime);
    tm    *t  = localtime(&tt);

    return t->tm_min;
}

int DateTime::Second() const
{
    time_t tt = SysClock::to_time_t(mDatetime);
    tm    *t  = localtime(&tt);

    return t->tm_sec;
}

int DateTime::Millisecond() const
{
    time_t tt = SysClock::to_time_t(mDatetime);
    tm    *t  = localtime(&tt);

    time_t    tt2 = mktime(t);
    TimePoint tmp = SysClock::from_time_t(tt2);

    int64_t milli =
        chrono::duration_cast<chrono::milliseconds>(mDatetime - tmp).count();
    return milli;
}

int DateTime::Microsecond() const
{
    time_t tt = SysClock::to_time_t(mDatetime);
    tm    *t  = localtime(&tt);

    time_t    tt2 = mktime(t);
    TimePoint tmp = SysClock::from_time_t(tt2);

    int64_t milli =
        chrono::duration_cast<chrono::milliseconds>(mDatetime - tmp).count();
    int64_t macro =
        chrono::duration_cast<chrono::microseconds>(mDatetime - tmp).count();
    return macro - milli * 1e3;
}

int DateTime::GetDayOfWeek() const
{
    time_t tt = SysClock::to_time_t(mDatetime);
    tm    *t  = localtime(&tt);

    return t->tm_wday;
}

int DateTime::GetDayOfYear() const
{
    time_t tt = SysClock::to_time_t(mDatetime);
    tm    *t  = localtime(&tt);

    return t->tm_yday;
}

void DateTime::AddDays(double days)
{
    int64_t dur = days * 24.0 * 3600.0 * 1.e6;
    mDatetime += chrono::microseconds(dur);
}

void DateTime::AddSeconds(double seconds)
{
    int64_t dur = seconds * 1.e6;
    mDatetime += chrono::microseconds(dur);
}

void DateTime::AddTimeSpan(const TimeSpan &value)
{
    double dur = value.GetTotalSeconds();
    AddSeconds(dur);
}

TimePoint DateTime::GetTimePoint() const
{
    return mDatetime;
}

int64_t DateTime::GetTimeStampInMicroSecs() const
{
    return chrono::duration_cast<chrono::microseconds>(mDatetime.time_since_epoch())
        .count();
}

double DateTime::GetTimeStampInDays() const
{
    return (double)GetTimeStampInMicroSecs() / 24.0 / 3600.0 / 1.e6;
}

double DateTime::GetTimeOfDay() const
{
    DateTime date = GetDate();
    TimeSpan dur  = GetTimeSpan(date, *this);

    return dur.GetTotalSeconds();
}

DateTime DateTime::GetDate() const
{
    time_t tt = SysClock::to_time_t(mDatetime);
    tm    *t  = localtime(&tt);

    return DateTime(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
}

DateTime DateTime::Today()
{
    return DateTime(SysClock::now()).GetDate();
}

DateTime DateTime::Now()
{
    return DateTime(SysClock::now());
}

DateTime DateTime::Zero()
{
    return DateTime();
}

int DateTime::DaysInMonth(int year, int month)
{
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int d;

    if (2 == month)
    {
        bool cond1 = (0 == year % 4) && (0 != year % 100);
        bool cond2 = (0 == year % 400);
        d          = (cond1 || cond2) ? 29 : 28;
    }
    else
    {
        d = days[month - 1];
    }

    return d;
}

int DateTime::Compare(const DateTime &t1, const DateTime &t2)
{
    if (t2.GetTimeStampInMicroSecs() > t1.GetTimeStampInMicroSecs())
    {
        return -1;
    }
    else if (t2.GetTimeStampInMicroSecs() < t1.GetTimeStampInMicroSecs())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

string DateTime::ToString(const DateTime &obj, const string &fmt)
{
    const auto &tp = obj.GetTimePoint();

    chrono::seconds seconds_diff(
        chrono::duration_cast<chrono::seconds>(tp.time_since_epoch()));
    time_t tt(SysClock::to_time_t(SysClock::time_point(seconds_diff)));

    char temp[30];
    if (!strftime(temp, 30, fmt.c_str(), localtime(&tt)))
        return "null";

    double subseconds = (tp.time_since_epoch() - seconds_diff).count();
    return string(temp) + "." + to_string((int)subseconds);
}

DateTime DateTime::FromString(const string &str, const string &fmt)
{
    stringstream ss{str};
    tm           t;
    ss >> get_time(&t, fmt.c_str());

    t.tm_isdst = -1;
    time_t tt  = mktime(&t);
    if (tt == -1)
    {
        throw runtime_error("DateTime transform error.");
    }
    return SysClock::from_time_t(tt);
}

TimeSpan DateTime::GetTimeSpan(const DateTime &t1, const DateTime &t2)
{
    int64_t dur = t2.GetTimeStampInMicroSecs() - t1.GetTimeStampInMicroSecs();
    return TimeSpan(chrono::microseconds(dur));
}


}  // namespace OpenOasis::Utils