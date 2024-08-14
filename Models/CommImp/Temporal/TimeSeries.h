/** ***********************************************************************************
 *    Copyright (C) 204, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  TimeSeries.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide timeseries functionalities.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Utils/CommConstants.h"
#include "Models/Utils/Interps.h"
#include <functional>


namespace OpenOasis::CommImp::Temporal
{
using namespace Utils;

class TimeSeries
{
private:
    std::string       mId;
    std::vector<real> mTimestamps;
    std::vector<real> mValues;

public:
    TimeSeries(const std::string &id, const std::vector<double> &times, real value = 0);
    TimeSeries(
        const std::string &id, const std::vector<double> &times,
        const std::vector<real> &values);
    TimeSeries(const TimeSeries &other);

    const std::string         &GetId() const;
    const std::vector<double> &GetTimestamps() const;
    const std::vector<real>   &GetValues() const;

    const real GetValue(
        double timestamp, std::string interpMethod = "linear", bool isExtrap = true,
        std::function<real(real)> process = nullptr, real eps = FP_EPSILON) const;

    static TimeSeries ResampleTo(
        const TimeSeries &timeseries, double startTime, double endTime, double step,
        std::string interpMethod = "linear");
};

}  // namespace OpenOasis::CommImp::Temporal
