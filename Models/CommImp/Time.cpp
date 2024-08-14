/** ***********************************************************************************
 *    @File      :  Time.cpp
 *    @Brief     :  To support a time stamp as well as a time interval.
 *
 ** ***********************************************************************************/
#include "Time.h"
#include "Models/Utils/Exception.h"


namespace OpenOasis::CommImp
{
using namespace Utils;
using namespace std;

Time::Time()
{}

Time::Time(const shared_ptr<ITime> &time)
{
    SetTimeStamp(time->GetTimeStamp());
    SetDurationInDays(time->GetDurationInDays());
}

Time::Time(const shared_ptr<ITime> &start, const shared_ptr<ITime> &end)
{
    SetTimeStamp(start->GetTimeStamp());

    double days = end->GetTimeStamp() - start->GetTimeStamp();
    SetDurationInDays(days);
}

Time::Time(double timestampInDays, double durationInDays)
{
    SetTimeStamp(timestampInDays);
    SetDurationInDays(durationInDays);
}

Time::Time(const DateTime &start, const DateTime &end)
{
    double timestampInDay = start.GetTimeStampInDays();
    SetTimeStamp(timestampInDay);

    double durationInDays = DateTime::GetTimeSpan(start, end).GetTotalDays();
    SetDurationInDays(durationInDays);
}

Time::Time(const DateTime &start, double durationInDays)
{
    double timestampInDay = start.GetTimeStampInDays();
    SetTimeStamp(timestampInDay);
    SetDurationInDays(durationInDays);
}

double Time::GetTimeStamp() const
{
    return mTimeStamp;
}

void Time::SetTimeStamp(double value)
{
    mTimeStamp = max(0.0, value);
}

double Time::GetDurationInDays() const
{
    return mDurationInDays;
}

void Time::SetDurationInDays(double value)
{
    mDurationInDays = max(0.0, value);
}

DateTime Time::ToDatetime(const shared_ptr<ITime> &time)
{
    DateTime date(DateTime::Zero());
    date.AddDays(time->GetTimeStamp() * 24.0);

    return date;
}

unique_ptr<ITime> Time::ToITime(const DateTime &time)
{
    return make_unique<Time>(time);
}

int Time::CompareTo(const std::shared_ptr<IComparable> &obj) const
{
    if (!obj)
    {
        throw IllegalArgumentException("Invalid null object for ITime comparing.");
    }

    auto time = dynamic_pointer_cast<ITime>(obj);
    if (!time)
    {
        throw InvalidOperationException("Invalid comparable object with ITime.");
    }

    double dt = mTimeStamp - time->GetTimeStamp();
    if (dt > EpsilonForTimeCompare)
    {
        return 1;
    }
    else if (dt < -EpsilonForTimeCompare)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

string Time::ToString(const std::shared_ptr<ITime> &time)
{
    DateTime tp(DateTime::Zero());
    tp.AddDays(time->GetTimeStamp());

    auto datestr = DateTime::ToString(tp);
    return datestr;
}


}  // namespace OpenOasis::CommImp