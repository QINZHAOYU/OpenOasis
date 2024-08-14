/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  TimeSet.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  A set of time stamps or time intervals.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/ITimeSet.h"
#include "Models/Inc/ITime.h"


namespace OpenOasis
{
namespace CommImp
{
/// @brief Generic implementation of `ITimeSet`, set of time stamps or time intervals.
class TimeSet : public ITimeSet
{
protected:
    std::vector<std::shared_ptr<ITime>> mTimes;  // Sorted time stamps or time spans.

    // An instance of `ITime` with duration indicated total time span in mTimes.
    std::shared_ptr<ITime> mTimeHorizon;

    bool mHasDuration;

    double mOffsetFromUtcInHours;  // Time offset from UTC time (default 8 in china).

public:
    virtual ~TimeSet()
    {}

    TimeSet();
    TimeSet(const std::shared_ptr<ITimeSet> &timeSet);
    TimeSet(TimeSet &&timeSet);
    TimeSet(const std::vector<std::shared_ptr<ITime>> &times, double hourOffset = 8.);

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `ITimeSet`.
    //

    virtual std::vector<std::shared_ptr<ITime>> GetTimes() const override;

    virtual bool HasDurations() const override;

    virtual double GetOffsetFromUtcInHours() const override;

    virtual void AddTime(std::shared_ptr<ITime> time) override;

    virtual std::shared_ptr<ITime> GetTimeHorizon() const override;

    virtual void RemoveTime(int index) override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Local methods.
    //

    bool HasDuration(const std::shared_ptr<ITime> &time) const;

    void Reset();

    /// @brief Sorts the times from early to late.
    void Sort();

    /// @brief Removes time stamps or time spans from time set.
    /// @param index The start index of time to remove.
    /// @param count The number to remove.
    void RemoveTimeRange(int index, int count);

    /// @brief Set time horizon from times, duration used if `HasDurations()` is true.
    void SetTimeHorizonFromTimes();

    int GetCount() const;

    std::shared_ptr<ITime> &operator[](int timeIndex);
};

}  // namespace CommImp
}  // namespace OpenOasis