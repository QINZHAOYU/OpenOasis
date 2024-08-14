/** ***********************************************************************************
 *    @File      :  TimeBuffer.cpp
 *    @Brief     :  To provide temporal buffering functionalities.
 *
 ** ***********************************************************************************/
#include "TimeBuffer.h"
#include "Models/CommImp/Time.h"
#include "Models/CommImp/DevSupports/ExtensionMethods.h"
#include "Models/Utils/Exception.h"


namespace OpenOasis::CommImp::Temporal
{
using namespace DevSupports;
using namespace Utils;
using namespace std;


TimeBuffer::TimeBuffer()
{
    SetDoExtrapolate(true);
    Create();
}

TimeBuffer::TimeBuffer(const TimeBuffer &TimeBuffer)
{
    SetDoExtrapolate(true);
    Create();
}

void TimeBuffer::SetDoExtrapolate(bool value)
{
    mDoExtrapolate = value;
}

bool TimeBuffer::GetDoExtrapolate() const
{
    return mDoExtrapolate;
}

void TimeBuffer::Create()
{
    mDoExtendedDataVerification = true;
    mRelaxationFactor           = 1.0;

    mTimes  = make_shared<TimeSet>();
    mValues = make_shared<ValueSet2D>();
}

void TimeBuffer::AddValues(const shared_ptr<ITime> &time, const vector<real> &values)
{
    AddValuesToBuffer(time, values);
}

void TimeBuffer::AddValuesToBuffer(
    const shared_ptr<ITime> &time, const vector<real> &values)
{
    // Check if we can add the values
    if (mTimes->HasDurations())
    {
        if (time->GetDurationInDays() <= 0)
        {
            throw runtime_error(
                "Time without duration added to time set with durations.");
        }

        // Order of times: The end time is allowed to overlap with at most
        // Time::EpsilonForTimeCompare with the new time.
        if (mTimes->GetTimes().size() > 0
            && time->GetTimeStamp() + Time::EpsilonForTimeCompare
                   < ExtensionMethods::EndTimeStamp(mTimes->GetTimeHorizon()))
        {
            return;
            // throw runtime_error("Overlapping times out of order times in time
            // buffer.");
        }
    }
    else
    {
        if (time->GetDurationInDays() > 0)
        {
            throw runtime_error(
                "Time with duration added to time set without durations.");
        }

        // Order of times: There must at least be a difference of
        // Time.EpsilonForTimeCompare between two stamps.
        if (mTimes->GetCount() > 0
            && time->GetTimeStamp() - Time::EpsilonForTimeCompare
                   < mTimes->GetTimeHorizon()->GetTimeStamp())
        {
            return;
            // throw runtime_error("Overlapping times out of order times in time
            // buffer.");
        }
    }

    mTimes->AddTime(make_shared<Time>(time));  // save a copy of time

    for (int i = 0; i < (int)values.size(); ++i)
    {
        mValues->SetOrAddValue({mTimes->GetCount() - 1, i}, values[i]);
    }
}

real TimeBuffer::GetRelaxationFactor() const
{
    return mRelaxationFactor;
}

void TimeBuffer::SetRelaxationFactor(real value)
{
    mRelaxationFactor = value;
    if (mRelaxationFactor < 0 || mRelaxationFactor > 1)
    {
        throw runtime_error("ReleaxationFactor is out of range");
    }
}

shared_ptr<ITime> TimeBuffer::GetTimeAt(int timeStep) const
{
    if (mDoExtendedDataVerification)
    {
        CheckBuffer();
    }
    return (*mTimes)[timeStep];
}

void TimeBuffer::CheckBuffer() const
{
    if (mTimes->GetCount() != mValues->GetIndexCount({0}))
    {
        throw runtime_error("Different numbers of values and times in buffer");
    }

    if (mTimes->GetCount() == 0)
    {
        throw runtime_error("Buffer is empty");
    }
}

vector<real> TimeBuffer::GetValuesAt(int timeStep) const
{
    if (mDoExtendedDataVerification)
    {
        CheckBuffer();
    }

    vector<real> values;
    for (const auto &val : mValues->GetElementValuesForTime(timeStep))
    {
        values.push_back(any_cast<real>(val));
    }

    return values;
}

vector<real> TimeBuffer::GetValues(const shared_ptr<ITime> &requestedTime)
{
    if (mDoExtendedDataVerification)
    {
        CheckBuffer();
    }

    if (!mDoExtrapolate)
    {
        if (ExtensionMethods::End(requestedTime)->GetTimeStamp()
                > ExtensionMethods::End(mTimes->GetTimeHorizon())->GetTimeStamp()
                      + Time::EpsilonForTimeCompare
            || ExtensionMethods::Start(requestedTime)->GetTimeStamp()
                   < ExtensionMethods::Start(mTimes->GetTimeHorizon())->GetTimeStamp()
                         - Time::EpsilonForTimeCompare)
        {
            throw runtime_error("Extrapolation not allowed for this buffer");
        }
    }

    vector<real> returnValues;
    if (mValues->GetIndexCount({0}) != 0)
    {
        if (mTimes->HasDurations() && requestedTime->GetDurationInDays() > 0)
        {
            returnValues = MapFromTimeSpansToTimeSpan(requestedTime);
        }
        else if (
            mTimes->HasDurations()
            && requestedTime->GetDurationInDays() <= Time::EpsilonForTimeCompare)
        {
            returnValues = MapFromTimeSpansToTimeStamp(requestedTime);
        }
        else if (!mTimes->HasDurations() && requestedTime->GetDurationInDays() > 0)
        {
            returnValues = MapFromTimeStampsToTimeSpan(requestedTime);
        }
        else  // time stamps
        {
            returnValues = MapFromTimeStampsToTimeStamp(requestedTime);
        }
    }

    return returnValues;
}

vector<real>
TimeBuffer::MapFromTimeStampsToTimeStamp(const shared_ptr<ITime> &requestedTimeStamp)
{
    try
    {
        int  elementCount = mValues->GetIndexCount({0, 0});
        real tr           = requestedTimeStamp->GetTimeStamp();  // Requested TimeStamp

        vector<real> vr(elementCount);  // Values to return

        const auto &times = mTimes->GetTimes();
        if (times.size() == 1)
        {
            //-------------------------------------------------------------------------
            //    Buffered  TimesStamps:|          >tb0<
            //    Requested TimeStamp:  |    >tr<
            // or Requested TimeStamp:  |          >tr<
            // or Requested TimeStamp:  |                >tr<
            //                           -------------------------------------------> t
            // ------------------------------------------------------------------------
            if (tr > (times.front()->GetTimeStamp() + Time::EpsilonForTimeCompare)
                && !mDoExtrapolate)
            {
                throw runtime_error("Extrapolation not allowed");
            }

            for (int i = 0; i < elementCount; i++)
            {
                vr[i] = any_cast<real>(mValues->GetValue({0, i}));
            }
        }
        else if (tr <= times.front()->GetTimeStamp())
        {
            //-------------------------------------------------------------------------
            //  Buffered  TimesStamps:|          >tb0<   >tb1<   >tb2<  >tbN<
            //  Requested TimeStamp:  |    >tr<
            //                         ---------------------------------------------> t
            // ------------------------------------------------------------------------
            real tb0 = times[0]->GetTimeStamp();
            real tb1 = times[1]->GetTimeStamp();

            for (int i = 0; i < elementCount; i++)
            {
                real sbi0 = any_cast<real>(mValues->GetValue({0, i}));
                real sbi1 = any_cast<real>(mValues->GetValue({i, i}));
                vr[i] =
                    ((sbi0 - sbi1) / (tb0 - tb1)) * (tr - tb0) * (1 - mRelaxationFactor)
                    + sbi0;
            }
        }
        else if (tr > times.back()->GetTimeStamp())
        {
            //-------------------------------------------------------------------------
            //  Buffered  TimesStamps:|  >tb0<  >tb1<  >tb2<  >tbN_2<  >tbN_1<
            //  Requested TimeStamp:  |                                         >tr<
            //                         ---------------------------------------------> t
            // ------------------------------------------------------------------------
            int  size  = times.size();
            real tbN_2 = times[size - 2]->GetTimeStamp();
            real tbN_1 = times[size - 1]->GetTimeStamp();

            for (int i = 0; i < elementCount; i++)
            {
                real sbiN_2 = any_cast<real>(mValues->GetValue({size - 2, i}));
                real sbiN_1 = any_cast<real>(mValues->GetValue({size - 1, i}));
                vr[i]       = ((sbiN_1 - sbiN_2) / (tbN_1 - tbN_2)) * (tr - tbN_1)
                            * (1 - mRelaxationFactor)
                        + sbiN_1;
            }
        }
        else
        {
            //-------------------------------------------------------------------------
            //  Available TimesStamps: | >tb0< >tb1< >tbna<    >tnb< >tbN_1< >tbN_2<
            //  Requested TimeStamp:   |                   >tr<
            //                         ---------------------------------------------> t
            // ------------------------------------------------------------------------
            const auto &iter = find_if(begin(times), end(times), [&tr](const auto &t) {
                return t->GetTimeStamp() >= tr;
            });

            int iHigh = distance(begin(times), iter);

            real fraction =
                (tr - times[iHigh - 1]->GetTimeStamp())
                / (times[iHigh]->GetTimeStamp() - times[iHigh - 1]->GetTimeStamp());

            for (int i = 0; i < elementCount; i++)
            {
                real sbinA = any_cast<real>(mValues->GetValue({iHigh - 1, i}));
                real sbinB = any_cast<real>(mValues->GetValue({iHigh, i}));
                vr[i]      = sbinA + fraction * (sbinB - sbinA);
            }
        }

        return vr;
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("MapFromTimeStampsToTimeStamp Failed");
    }
}

vector<real>
TimeBuffer::MapFromTimeSpansToTimeSpan(const shared_ptr<ITime> &requestedTime)
{
    try
    {
        int          elementCount = mValues->GetIndexCount({0, 0});
        vector<real> vr(elementCount);  // Values to return

        // Begin time in requester time interval
        real trb = requestedTime->GetTimeStamp();
        // End time in requester time interval
        real tre = requestedTime->GetTimeStamp() + requestedTime->GetDurationInDays();
        // length of requested time interval
        real trl = tre - trb;

        const auto &times = mTimes->GetTimes();
        int         size  = (int)times.size();
        if (times.empty())
        {
            throw runtime_error("No times in buffer");
        }

        real tbb0 = times[0]->GetTimeStamp();

        // In the following the current abbreviations are used:
        // B: Buffer time
        // R: Requested time
        // I: Time included by this part of the code

        if (mDoExtrapolate)
        {
            //-------------------------------------------------------------------------
            // B                tbb0|-------tbe0|tbb1------|tbe1
            // R+I  trb|---|tre
            //-------------------------------------------------------------------------
            if (tre <= tbb0)
            {
                if (size >= 2 && mRelaxationFactor != 1)
                {
                    // Linear interpolation
                    real tbe0 = ExtensionMethods::End(times[0])->GetTimeStamp();
                    // real tbb1 = times[1]->GetTimeStamp();
                    real tbe1 = ExtensionMethods::End(times[1])->GetTimeStamp();

                    for (int i = 0; i < elementCount; i++)
                    {
                        real sbi0 = any_cast<real>(mValues->GetValue({0, i}));
                        real sbi1 = any_cast<real>(mValues->GetValue({1, i}));
                        vr[i]     = sbi0
                                - (1 - mRelaxationFactor) * (sbi1 - sbi0)
                                      * (tbe0 + tbb0 - tre - trb) / (tbe1 - tbb0);
                    }
                }
                else
                {
                    // Nearest value interpolation
                    for (int i = 0; i < elementCount; i++)
                    {
                        real sbi0 = any_cast<real>(mValues->GetValue({0, i}));
                        vr[i]     = sbi0;
                    }
                }

                return vr;
            }

            //-------------------------------------------------------------------------
            // B          tbb0|--------|---------|--------|
            // R     trb|----------------|tre
            // I        |-----|
            //-------------------------------------------------------------------------
            if (trb < tbb0)  // && tre > tbb0
            {
                real tbe0 = ExtensionMethods::End(times[0])->GetTimeStamp();
                if (size >= 2 && mRelaxationFactor != 1)
                {
                    real tbe1 = ExtensionMethods::End(times[1])->GetTimeStamp();

                    // Linear interpolation, use tbb0 as "endpoint" of interval
                    for (int i = 0; i < elementCount; i++)
                    {
                        real sbi0 = any_cast<real>(mValues->GetValue({0, i}));
                        real sbi1 = any_cast<real>(mValues->GetValue({1, i}));
                        vr[i] += ((tbb0 - trb) / trl)
                                 * (sbi0
                                    - (1 - mRelaxationFactor) * (sbi1 - sbi0)
                                          * (tbe0 - trb) / (tbe1 - tbb0));
                    }
                }
                else
                {
                    // Nearest value interpolation
                    for (int i = 0; i < elementCount; i++)
                    {
                        real sbi0 = any_cast<real>(mValues->GetValue({0, i}));
                        vr[i] += sbi0 * (tbb0 - trb) / trl;
                    }
                }
            }

            real tbeN0 = ExtensionMethods::End(times.back())->GetTimeStamp();

            //--------------------------------------------------------------------------
            // B     tbb0|---?----|-------|tbeN0
            // R+I                                trb|---|tre
            //--------------------------------------------------------------------------
            if (tbeN0 < trb)
            {
                if (size >= 2 && mRelaxationFactor != 1)
                {
                    // Linear interpolation
                    real tbeN1 = ExtensionMethods::End(times[size - 2])->GetTimeStamp();
                    real tbbN1 = times[size - 2]->GetTimeStamp();

                    for (int i = 0; i < elementCount; i++)
                    {
                        real sbiN0 = any_cast<real>(mValues->GetValue({size - 1, i}));
                        real sbiN1 = any_cast<real>(mValues->GetValue({size - 2, i}));
                        ;
                        vr[i] = sbiN0
                                + (1 - mRelaxationFactor) * (sbiN0 - sbiN1)
                                      * (trb + tre - tbeN0 - tbeN1) / (tbeN0 - tbbN1);
                    }
                }
                else
                {
                    // Nearest value interpolation
                    for (int i = 0; i < elementCount; i++)
                    {
                        real sbiN0 = any_cast<real>(mValues->GetValue({size - 1, i}));
                        vr[i]      = sbiN0;
                    }
                }

                return vr;
            }

            //--------------------------------------------------------------------------
            // B     |---?----|-------|tbeN0
            // R               trb|-------|tre
            // I                      |---|
            //--------------------------------------------------------------------------
            if (tbeN0 < tre)
            {
                if (size >= 2 && mRelaxationFactor != 1)
                {
                    real tbeN1 = ExtensionMethods::End(times[size - 2])->GetTimeStamp();
                    real tbbN1 = times[size - 2]->GetTimeStamp();
                    for (int i = 0; i < elementCount; i++)
                    {
                        real sbiN0 = any_cast<real>(mValues->GetValue({size - 1, i}));
                        real sbiN1 = any_cast<real>(mValues->GetValue({size - 2, i}));
                        vr[i] += ((tre - tbeN0) / (tre - trb))
                                 * (sbiN0
                                    + (1 - mRelaxationFactor) * (sbiN0 - sbiN1)
                                          * (tre - tbeN1) / (tbeN0 - tbbN1));
                    }
                }
                else
                {
                    for (int i = 0; i < elementCount; i++)
                    {
                        real sbiN0 = any_cast<real>(mValues->GetValue({size - 1, i}));
                        vr[i] += sbiN0 * ((tre - tbeN0) / (tre - trb));
                    }
                }
            }
        }

        int nstart = 0;
        int nend   = size - 1;

        // Narrow down the number of spans to investigate, based on the requested span.
        if (nend > 10)
        {
            // Assuming 4 spans in the buffer, use end-time in GetIntrval
            // will provide the right indices
            // spans                 |-0-|-1-|-2-|-3-|
            // endStamp intervals      0 | 1 | 2 | 3 | 4

            const auto &iter1 =
                find_if(begin(times), end(times), [&trb](const auto &t) {
                    return t->GetTimeStamp() >= trb;
                });
            nstart = distance(begin(times), iter1);

            const auto &iter2 =
                find_if(begin(times), end(times), [&tre](const auto &t) {
                    return ExtensionMethods::End(t)->GetTimeStamp() >= tre;
                });
            nend = distance(begin(times), iter2);
            nend = min(nend, size - 1);
        }

        for (int n = nstart; n <= nend; n++)
        {
            real tbbn = times[n]->GetTimeStamp();
            real tben = ExtensionMethods::End(times[n])->GetTimeStamp();

            //---------------------------------------------------------------------------
            // B:       tbbn|--------------------------|tben
            // R:     trb|-------------------------------------|tre
            // I:           |--------------------------|
            //---------------------------------------------------------------------------
            if (trb <= tbbn && tre >= tben)
            {
                for (int i = 0; i < elementCount; i++)
                {
                    real sbin = any_cast<real>(mValues->GetValue({n, i}));
                    vr[i] += sbin * (tben - tbbn) / (tre - trb);
                }
            }

            //---------------------------------------------------------------------------
            // B:      tbbn|-----------------------|tben
            // R+I:          trb|--------------|tre
            // --------------------------------------------------------------------------
            else if (tbbn <= trb && tre <= tben)  // cover all
            {
                for (int i = 0; i < elementCount; i++)
                {
                    vr[i] += any_cast<real>(mValues->GetValue({n, i}));
                }
            }

            //---------------------------------------------------------------------------
            // B:     tbbn|-----------------|tben
            // R:                 trb|--------------|tre
            // I:                    |------|
            // --------------------------------------------------------------------------
            else if (tbbn < trb && trb < tben && tre > tben)
            {
                for (int i = 0; i < elementCount; i++)
                {
                    real sbin = any_cast<real>(mValues->GetValue({n, i}));
                    vr[i] += sbin * (tben - trb) / (tre - trb);
                }
            }

            //---------------------------------------------------------------------------
            // B:           tbbn|-----------------|tben
            // R:      trb|--------------|tre
            // I:               |--------|
            // --------------------------------------------------------------------------
            else if (trb < tbbn && tre > tbbn && tre < tben)
            {
                for (int i = 0; i < elementCount; i++)
                {
                    real sbin = any_cast<real>(mValues->GetValue({n, i}));
                    vr[i] += sbin * (tre - tbbn) / (tre - trb);
                }
            }
        }

        return vr;
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("MapFromTimeSpansToTimeSpan Failed");
    }
}

vector<real>
TimeBuffer::MapFromTimeStampsToTimeSpan(const shared_ptr<ITime> &requestedTime)
{
    try
    {
        int          elementCount = mValues->GetIndexCount({0, 0});
        vector<real> vr(elementCount);  // Values to return

        // Begin time in requester time interval
        real trb = requestedTime->GetTimeStamp();
        // End time in requester time interval
        real tre = requestedTime->GetTimeStamp() + requestedTime->GetDurationInDays();
        // length of requested time interval
        // real trl = tre - trb;

        const auto &times = mTimes->GetTimes();
        int         size  = (int)times.size();

        //-----------------------------------------------------------------------------
        // This handles values within the time horizon of the buffer, i.e.
        // there must be at least two values in the buffer.

        int nstart = 1;
        int nend   = size - 1;

        // Narrow down the number of intervals to investigate,
        // based on the requested span.
        if (nend > 4)
        {
            const auto &iter1 =
                find_if(begin(times), end(times), [&trb](const auto &t) {
                    return t->GetTimeStamp() >= trb;
                });
            nstart = distance(begin(times), iter1);
            nstart = max(nstart, 1);

            const auto &iter2 =
                find_if(begin(times), end(times), [&tre](const auto &t) {
                    return ExtensionMethods::End(t)->GetTimeStamp() >= tre;
                });
            nend = distance(begin(times), iter2);
            nend = min(nend, size - 1);
        }

        // n corresponds to the n'th interval between the time stamps.
        for (int n = nstart; n <= nend; n++)
        {
            real tbn   = times[n - 1]->GetTimeStamp();
            real tbnp1 = times[n]->GetTimeStamp();

            //-------------------------------------------------------------------------
            //    B:        tbn|--------------------------|tbnp1
            //    R:     trb|-------------------------------------|tre
            //    I:           |--------------------------|
            // ------------------------------------------------------------------------
            if (trb <= tbn && tre >= tbnp1)
            {
                real factor = (tbnp1 - tbn) / (tre - trb);
                for (int i = 0; i < elementCount; i++)
                {
                    real sbin   = any_cast<real>(mValues->GetValue({n - 1, i}));
                    real sbinp1 = any_cast<real>(mValues->GetValue({n, i}));
                    vr[i] += 0.5 * (sbin + sbinp1) * factor;
                }
            }

            //-------------------------------------------------------------------------
            // B:       tbn|-----------------------|tbnp1
            // R+I:         trb|--------------|tre
            // ------------------------------------------------------------------------
            else if (tbn <= trb && tre <= tbnp1)  // cover all
            {
                real fraction = ((tre + trb) / 2 - tbn) / (tbnp1 - tbn);
                for (int i = 0; i < elementCount; i++)
                {
                    real sbin   = any_cast<real>(mValues->GetValue({n - 1, i}));
                    real sbinp1 = any_cast<real>(mValues->GetValue({n, i}));
                    vr[i] += sbin + (sbinp1 - sbin) * fraction;
                }
            }

            //-------------------------------------------------------------------------
            // B:      tbn|-----------------|tbnp1
            // R:                 trb|--------------|tre
            // I:                    |------|
            // ------------------------------------------------------------------------
            else if (tbn < trb && trb < tbnp1 && tre > tbnp1)
            {
                real fraction = ((tbnp1 - trb) / 2) / (tbnp1 - tbn);
                real factor   = (tbnp1 - trb) / (tre - trb);
                for (int i = 0; i < elementCount; i++)
                {
                    real sbin   = any_cast<real>(mValues->GetValue({n - 1, i}));
                    real sbinp1 = any_cast<real>(mValues->GetValue({n, i}));
                    vr[i] += (sbinp1 - (sbinp1 - sbin) * fraction) * factor;
                }
            }

            //-------------------------------------------------------------------------
            // B:            tbn|-----------------|tbnp1
            // R:      trb|--------------|tre
            // I:               |--------|
            // ------------------------------------------------------------------------
            else if (trb < tbn && tre > tbn && tre < tbnp1)
            {
                real fraction = ((tre - tbn) / 2) / (tbnp1 - tbn);
                real factor   = (tre - tbn) / (tre - trb);
                for (int i = 0; i < elementCount; i++)
                {
                    real sbin   = any_cast<real>(mValues->GetValue({n - 1, i}));
                    real sbinp1 = any_cast<real>(mValues->GetValue({n, i}));
                    vr[i] += (sbin + (sbinp1 - sbin) * fraction) * factor;
                }
            }
        }

        //-----------------------------------------------------------------------------
        // This handles values outside the time horizon of the buffer.

        //-----------------------------------------------------------------------------
        // In case of only one value in the buffer, regardless of its position relative
        // to R
        //    |     or     |     or   |               B
        //        |----------------|                  R
        //-----------------------------------------------------------------------------
        if (size == 1)
        {
            // TODO: Test if extrapolation is ok.
            for (int i = 0; i < elementCount; i++)
            {
                real sbi0 = any_cast<real>(mValues->GetValue({0, i}));
                vr[i]     = sbi0;
            }
        }
        else
        {
            // At least two stamp values in the buffer, so we can do extrapolation
            //-------------------------------------------------------------------------
            //  B:       tb0|-----tb1|---------|--------|
            //  R: trb|----------------|tre
            //  I:    |-----|
            //-------------------------------------------------------------------------
            real tb0   = times[0]->GetTimeStamp();
            real tb1   = times[1]->GetTimeStamp();
            real tbN_1 = times[size - 1]->GetTimeStamp();
            real tbN_2 = times[size - 2]->GetTimeStamp();

            if (trb < tb0 && tre > tb0)
            {
                real fraction =
                    (1 - mRelaxationFactor) * 0.5 * (tb0 - trb) / (tb1 - tb0);
                real factor = ((tb0 - trb) / (tre - trb));
                for (int i = 0; i < elementCount; i++)
                {
                    real sbi0 = any_cast<real>(mValues->GetValue({0, i}));
                    real sbi1 = any_cast<real>(mValues->GetValue({1, i}));
                    vr[i] += factor * (sbi0 - fraction * (sbi1 - sbi0));
                }
            }

            //-------------------------------------------------------------------------
            // B     |--------|---------|tbn_2---|tbn_1
            // R                        trb|----------------|tre
            // I                                 |----------|
            //-------------------------------------------------------------------------
            if (tre > tbN_1 && trb < tbN_1)
            {
                real factor = ((tre - tbN_1) / (tre - trb));
                real fraction =
                    (1 - mRelaxationFactor) * 0.5 * (tre - tbN_1) / (tbN_1 - tbN_2);
                for (int i = 0; i < elementCount; i++)
                {
                    real sbiN_1 = any_cast<real>(mValues->GetValue({size - 1, i}));
                    real sbiN_2 = any_cast<real>(mValues->GetValue({size - 2, i}));
                    vr[i] += factor * (sbiN_1 + fraction * (sbiN_1 - sbiN_2));
                }
            }

            //-------------------------------------------------------------------------
            // B   :   |--------|---------|tbn_2---|tbn_1
            // R+I :                                   trb|----------------|tre
            //-------------------------------------------------------------------------
            if (trb >= tbN_1)
            {
                real fraction = (1 - mRelaxationFactor) * (0.5 * (trb + tre) - tbN_1)
                                / (tbN_1 - tbN_2);
                for (int i = 0; i < elementCount; i++)
                {
                    real sbiN_1 = any_cast<real>(mValues->GetValue({size - 1, i}));
                    real sbiN_2 = any_cast<real>(mValues->GetValue({size - 2, i}));

                    vr[i] = sbiN_1 + (sbiN_1 - sbiN_2) * fraction;
                }
            }

            //-------------------------------------------------------------------------
            // B   :                  tb0|-----tb1|---------|--------| B
            // R+I :   trb|-------|tre
            //-------------------------------------------------------------------------
            if (tre <= tb0)
            {
                real fraction =
                    (1 - mRelaxationFactor) / (tb1 - tb0) * (tb0 - 0.5 * (trb + tre));
                for (int i = 0; i < elementCount; i++)
                {
                    real sbi0 = any_cast<real>(mValues->GetValue({0, i}));
                    real sbi1 = any_cast<real>(mValues->GetValue({1, i}));
                    vr[i]     = sbi0 - (sbi1 - sbi0) * fraction;
                }
            }
        }

        return vr;
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("MapFromTimeStampsToTimeSpan Failed");
    }
}

vector<real>
TimeBuffer::MapFromTimeSpansToTimeStamp(const shared_ptr<ITime> &requestedTimeStamp)
{
    try
    {
        int          elementCount = mValues->GetIndexCount({0, 0});
        vector<real> vr(elementCount);  // Values to return

        const auto &times = mTimes->GetTimes();
        int         size  = (int)times.size();

        real tr = requestedTimeStamp->GetTimeStamp();  // Requested TimeStamp

        if (size == 1)
        {
            //-------------------------------------------------------------------------
            //    Buffered TimesSpans:  |         |------|
            //    Requested TimeStamp:  |  >tr<
            // or Requested TimeStamp:  |           >tr<
            // or Requested TimeStamp:  |                   >tr<
            //                           -----------------------------------------> t
            // ------------------------------------------------------------------------

            // Check if tr is inside span
            if (!mDoExtrapolate)
            {
                if (times[0]->GetTimeStamp() - Time::EpsilonForTimeCompare > tr
                    || tr > ExtensionMethods::End(times.back())->GetTimeStamp()
                                + Time::EpsilonForTimeCompare)
                {
                    throw runtime_error("Extrapolation not allowed");
                }
            }

            for (int i = 0; i < elementCount; i++)  // For each element
            {
                vr[i] = any_cast<real>(mValues->GetValue({0, i}));
            }
        }

        //-----------------------------------------------------------------------------
        //  Buffered TimesSpans:  |          >tbb0<  ..........  >tbbN<
        //  Requested TimeStamp:  |    >tr<
        //                         -----------------------------------------> t
        // ----------------------------------------------------------------------------
        else if (tr <= times[0]->GetTimeStamp())
        {
            // Check if we are allowed to extrapolate
            if (!mDoExtrapolate)
            {
                if (tr < times[0]->GetTimeStamp() - Time::EpsilonForTimeCompare)
                {
                    throw runtime_error("Extrapolation not allowed");
                }

                // Very close to the first point, just provide that value
                for (int i = 0; i < elementCount; i++)  // For each element
                {
                    vr[i] = any_cast<real>(mValues->GetValue({0, i}));
                }
            }
            else
            {
                // Extrapolate from the first two values
                real tbb0     = times[0]->GetTimeStamp();
                real tbb1     = times[1]->GetTimeStamp();
                real fraction = (tr - tbb0) / (tbb0 - tbb1) * (1 - mRelaxationFactor);
                for (int i = 0; i < elementCount; i++)
                {
                    real sbi0 = any_cast<real>(mValues->GetValue({0, i}));
                    real sbi1 = any_cast<real>(mValues->GetValue({1, i}));
                    vr[i]     = (sbi0 - sbi1) * fraction + sbi0;
                }
            }
        }

        //-----------------------------------------------------------------------------
        //  Buffered TimesSpans:  |    >tbb0<   .................  >tbbN_1<
        //  Requested TimeStamp:  |                                           >tr<
        //                         -------------------------------------------------> t
        // ----------------------------------------------------------------------------
        else if (tr >= ExtensionMethods::End(times.back())->GetTimeStamp())
        {
            // Check if we are allowed to extrapolate
            if (!mDoExtrapolate)
            {
                if (tr > times.back()->GetTimeStamp() + Time::EpsilonForTimeCompare)
                {
                    throw runtime_error("Extrapolation not allowed");
                }

                // Very close to the last point, just provide that value
                for (int i = 0; i < elementCount; i++)
                {
                    vr[i] = any_cast<real>(mValues->GetValue({size - 1, i}));
                }
            }
            else
            {
                // Extrapolate from the last two values
                real tbeN_2 = ExtensionMethods::End(times[size - 2])->GetTimeStamp();
                real tbeN_1 = ExtensionMethods::End(times[size - 1])->GetTimeStamp();
                real fraction =
                    (tr - tbeN_1) / (tbeN_1 - tbeN_2) * (1 - mRelaxationFactor);
                for (int i = 0; i < elementCount; i++)
                {
                    real sbiN_2 = any_cast<real>(mValues->GetValue({size - 2, i}));
                    real sbiN_1 = any_cast<real>(mValues->GetValue({size - 1, i}));
                    vr[i]       = (sbiN_1 - sbiN_2) * fraction + sbiN_1;
                }
            }
        }

        //-----------------------------------------------------------------------------
        //  Availeble TimesSpans:  |    >tbb0<   ......................  >tbbN_1<
        //  Requested TimeStamp:   |                          >tr<
        //                         -------------------------------------------------> t
        // ----------------------------------------------------------------------------
        else
        {
            // Using end-time will provide the right indices
            // Example: assuming 4 spans in the buffer,
            // spans                 |-0-|-1-|-2-|-3-|
            // endStamp intervals      0 | 1 | 2 | 3 | 4
            const auto &iter = find_if(begin(times), end(times), [tr](const auto &t) {
                return t->GetTimeStamp() <= tr
                       && t->GetTimeStamp() + t->GetDurationInDays() >= tr;
            });
            int         interval = distance(begin(times), iter);

            for (int i = 0; i < elementCount; i++)
            {
                vr[i] = any_cast<real>(mValues->GetValue({interval, i}));
            }
        }

        return vr;
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("MapFromTimeSpansToTimeStamp Failed");
    }
}

int TimeBuffer::GetTimesCount() const
{
    return mTimes->GetCount();
}

int TimeBuffer::GetValuesCount() const
{
    return mValues->GetIndexCount({0, 0});
}

void TimeBuffer::ClearAfter(shared_ptr<ITime> time)
{
    real clearTimestamp = ExtensionMethods::Start(time)->GetTimeStamp();
    for (int i = 0; i < mTimes->GetCount(); i++)
    {
        if (clearTimestamp <= (*mTimes)[i]->GetTimeStamp())
        {
            // clear after current time.
            int numberOfValuesToRemove = mTimes->GetCount() - i;
            mTimes->RemoveTimeRange(i, numberOfValuesToRemove);

            for (int j = numberOfValuesToRemove; j >= 0; --j)
                mValues->RemoveValue({j + i});
        }
    }
}

void TimeBuffer::ClearBefore(shared_ptr<ITime> time)
{
    real clearTimestamp = ExtensionMethods::Start(time)->GetTimeStamp();

    while (!mTimes->GetTimes().empty())
    {
        if (mTimes->GetTimes().front()->GetTimeStamp() < clearTimestamp)
        {
            // Remove the expired time and values synchronously.
            mTimes->RemoveTime(0);
            mValues->RemoveValue({0});
        }
        else
        {
            break;
        }
    }
}

void TimeBuffer::Reset()
{
    mDoExtrapolate                = true;
    mRelaxationFactor             = 0;
    mLastBufferSizeMessageCounter = 0;
    mDoExtendedDataVerification   = false;

    mValues.reset();
    mTimes.reset();
}

vector<vector<real>> TimeBuffer::GetAllValues()
{
    vector<vector<real>> returnValues;
    for (int i = 0; i < mValues->GetIndexCount({0}); ++i)
    {
        vector<real> values;
        for (int j = 0; j < mValues->GetIndexCount({0, i}); ++j)
        {
            values.push_back(any_cast<real>(mValues->GetValue({i, j})));
        }

        returnValues.push_back(values);
    }
    return returnValues;
}

void TimeBuffer::SetOrAddValues(shared_ptr<ITime> time, vector<real> values)
{
    const auto &times = mTimes->GetTimes();
    const auto &iter  = find_if(begin(times), end(times), [&time](const auto &t) {
        return abs(t->GetTimeStamp() - time->GetTimeStamp())
               <= Time::EpsilonForTimeCompare;
    });

    int index = (iter != times.end()) ? -1 : distance(begin(times), iter);
    if (index < 0)
    {
        AddValues(time, values);  // add new time-values.
    }
    else
    {
        mValues->SetOrAddValue({index}, values);  // set existed values.
    }
}

shared_ptr<TimeSet> TimeBuffer::GetTimeSet() const
{
    return mTimes;
}

shared_ptr<ValueSet2D> TimeBuffer::GetValueSet() const
{
    return mValues;
}

bool TimeBuffer::GetDoExtendedDataVerification() const
{
    return mDoExtendedDataVerification;
}

void TimeBuffer::SetDoExtendedDataVerification(bool value)
{
    mDoExtendedDataVerification = value;
}


}  // namespace OpenOasis::CommImp::Temporal