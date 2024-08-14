/** ***********************************************************************************
 *    @File      :  TimeSet.cpp
 *    @Brief     :  A set of time stamps or time intervals.
 *
 ** ***********************************************************************************/
#include "TimeSet.h"
#include "Time.h"
#include "Models/Utils/Exception.h"
#include "Models/Utils/StringHelper.h"
#include <algorithm>


namespace OpenOasis::CommImp
{
using namespace Utils;
using namespace std;

TimeSet::TimeSet()
{
    mTimes                = {};
    mTimeHorizon          = make_shared<Time>();
    mOffsetFromUtcInHours = 8.;
    mHasDuration          = false;
}

TimeSet::TimeSet(const shared_ptr<ITimeSet> &timeSet)
{
    mOffsetFromUtcInHours = timeSet->GetOffsetFromUtcInHours();
    mHasDuration          = timeSet->HasDurations();

    mTimeHorizon = make_shared<Time>(timeSet->GetTimeHorizon());
    for (const auto &time : timeSet->GetTimes())
    {
        mTimes.emplace_back(make_shared<Time>(time));
    }
}

TimeSet::TimeSet(TimeSet &&timeSet)
{
    mOffsetFromUtcInHours = timeSet.mOffsetFromUtcInHours;
    mHasDuration          = timeSet.mHasDuration;
    mTimes                = timeSet.mTimes;
    mTimeHorizon          = timeSet.mTimeHorizon;
}

TimeSet::TimeSet(const vector<shared_ptr<ITime>> &times, double hourOffset)
{
    mOffsetFromUtcInHours = hourOffset;
    mTimes                = times;

    Sort();
    SetTimeHorizonFromTimes();

    mHasDuration = all_of(begin(times), end(times), [](const auto &time) {
        return time->GetDurationInDays() > Time::EpsilonForTimeCompare;
    });
}

vector<shared_ptr<ITime>> TimeSet::GetTimes() const
{
    return mTimes;
}

bool TimeSet::HasDurations() const
{
    return mHasDuration;
}

double TimeSet::GetOffsetFromUtcInHours() const
{
    return mOffsetFromUtcInHours;
}

shared_ptr<ITime> TimeSet::GetTimeHorizon() const
{
    return mTimeHorizon;
}

void TimeSet::AddTime(shared_ptr<ITime> time)
{
    if (!time)
    {
        return;
    }

    auto isOverlapped = any_of(begin(mTimes), end(mTimes), [&time](const auto &t) {
        return abs(t->GetTimeStamp() - time->GetTimeStamp())
               <= Time::EpsilonForTimeCompare;
    });
    if (isOverlapped)
    {
        return;
    }

    mHasDuration = HasDuration(time);
    mTimes.push_back(time);

    Sort();
    SetTimeHorizonFromTimes();
}

void TimeSet::RemoveTime(int index)
{
    if (index < 0 || index >= (int)mTimes.size())
    {
        throw IllegalArgumentException(StringHelper::FormatSimple(
            "Index [{}] of time to remove out of range [{}] .", index, mTimes.size()));
    }

    mTimes.erase(mTimes.begin() + index);
    SetTimeHorizonFromTimes();
}

void TimeSet::Sort()
{
    auto comp = [](const shared_ptr<ITime> &t1, const shared_ptr<ITime> &t2) {
        return t1->GetTimeStamp() <= t2->GetTimeStamp();
    };

    sort(begin(mTimes), end(mTimes), comp);
}

void TimeSet::RemoveTimeRange(int index, int count)
{
    if (!mTimes.empty())
    {
        index      = min(max(index, 0), (int)mTimes.size());
        count      = max(count, 0);
        size_t end = min(index + count, (int)mTimes.size());
        mTimes.erase(mTimes.begin() + index, mTimes.begin() + end);
    }
}

bool TimeSet::HasDuration(const shared_ptr<ITime> &time) const
{
    return time->GetDurationInDays() > 0.0;
}

void TimeSet::Reset()
{
    mTimes                = {};
    mTimeHorizon          = make_shared<Time>();
    mOffsetFromUtcInHours = 8.;
}

shared_ptr<ITime> &TimeSet::operator[](int timeIndex)
{
    return mTimes.at(timeIndex);
}

int TimeSet::GetCount() const
{
    return mTimes.size();
}

void TimeSet::SetTimeHorizonFromTimes()
{
    if (mTimes.empty())
    {
        mTimeHorizon = make_shared<Time>();
        return;
    }

    const auto &front = mTimes.front();
    const auto &back  = mTimes.back();

    auto end = back;
    if (mHasDuration)  // update the last time point with its duration.
    {
        end = make_shared<Time>(Time::ToDatetime(back), back->GetDurationInDays());
    }

    mTimeHorizon = make_shared<Time>(front, end);
}

}  // namespace OpenOasis::CommImp