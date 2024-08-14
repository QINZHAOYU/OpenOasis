/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ITime.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Time interface to support a time stamp as well as a time interval.
 *
 ** ***********************************************************************************/
#pragma once


namespace OpenOasis
{
/// @brief Time interface to support a time stamp as well as a time interval.
class ITime
{
public:
    /// @brief Time stamp as days since 08:00::00 January 1, 1970.
    virtual double GetTimeStamp() const = 0;

    /// @brief Duration in days for time interval. 0 if time is a time stamp.
    virtual double GetDurationInDays() const = 0;
};

}  // namespace OpenOasis
