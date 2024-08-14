#include "ThirdPart/Catch2/catch.hpp"
#include "Models/Utils/DateTime.h"
#include <thread>

using namespace OpenOasis::Utils;
using namespace std;


TEST_CASE("Timer tests")
{
    Timer tp;

    this_thread::sleep_for(chrono::seconds(3));
    REQUIRE(tp.ElapsedTimeFromBegin() >= 3000.0);

    this_thread::sleep_for(chrono::milliseconds(3));
    REQUIRE(tp.ElapsedTimeFromLast() >= 3.0);
    REQUIRE(tp.ElapsedTimeFromBegin() >= 3003.0);

    TimePoint now      = tp.GetNow();
    TimePoint now_copy = tp.GetNow();

    Timer::Update(now, 3.12, DurationRatio::seconds);
    REQUIRE(
        Timer::CalculateDuration(now_copy, now, DurationRatio::milliseconds) == 3120);
    REQUIRE(
        Timer::CalculateDuration(now, now_copy, DurationRatio::milliseconds) == -3120);
}


TEST_CASE("TimeSpan tests")
{
    TimeSpan ts;
    REQUIRE(ts.GetTotalSeconds() == Approx(0.0).epsilon(0.01));

    ts.Add(12.2);
    REQUIRE(ts.GetTotalSeconds() == Approx(12.2).margin(1.e-3));
    REQUIRE(ts.GetTotalMinutes() == Approx(12.2 / 60.0).margin(1.e-4));
    REQUIRE(ts.GetTotalHours() == Approx(12.2 / 3600.0).margin(1.e-7));
    REQUIRE(ts.GetTotalDays() == Approx(12.2 / 24.0 / 3600.0).margin(1.e-8));

    ts.Multiply(2);
    REQUIRE(ts.GetTotalSeconds() == Approx(12.2 * 2).margin(1.e-3));

    ts.Multiply(-1);
    REQUIRE(ts.GetTotalSeconds() == Approx(-12.2 * 2).margin(1.e-3));
    CHECK(TimeSpan::ToString(ts) == "-00:00:24.400000");

    TimeSpan ts_copy = ts.Duration();
    REQUIRE(ts_copy.GetTotalSeconds() == Approx(12.2 * 2).margin(1.e-3));
    CHECK(TimeSpan::Compare(ts, ts_copy) == -1);

    ts.Multiply(-1);
    CHECK(TimeSpan::Compare(ts, ts_copy) == 0);

    TimeSpan ts2(chrono::milliseconds(1000));
    REQUIRE(ts2.GetTotalSeconds() == Approx(1.0).margin(1.e-3));
    CHECK(TimeSpan::Compare(ts, ts2) == 1);

    string   time = "1:02:3.1";
    TimeSpan ts3  = TimeSpan::FromString(time);
    REQUIRE(ts3.GetTotalHours() == Approx(1. + 2. / 60. + 3.1 / 3600.0).margin(1.e-3));
    CHECK(TimeSpan::ToString(ts3) == "01:02:3.100000");

    TimeSpan ts4 = TimeSpan::FromString("01:02:3.100000");
    CHECK(TimeSpan::Compare(ts3, ts4) == 0);
}

TEST_CASE("DateTime tests")
{
    SECTION("test member functions")
    {
        int timeZoneOffset = 0;

        DateTime dt;
        REQUIRE(dt.Year() == 1970);
        REQUIRE(dt.Month() == 1);
        REQUIRE(dt.Day() == 1);
        REQUIRE(dt.Minute() == 0);
        REQUIRE(dt.Second() == 0);
        REQUIRE(dt.Millisecond() == 0);

        timeZoneOffset = 8 - dt.Hour();
        dt.AddSeconds(timeZoneOffset * 3600);
        REQUIRE_THAT(DateTime::ToString(dt), Catch::Contains("1970-01-01 08:00:00.0"));

        DateTime dt2(2023, 2, 15, 7);
        REQUIRE(dt2.Year() == 2023);
        REQUIRE(dt2.Month() == 2);
        REQUIRE(dt2.Day() == 15);
        REQUIRE(dt2.Hour() == 7);
        REQUIRE(dt2.Minute() == 0);
        REQUIRE(dt2.Second() == 0);
        REQUIRE(dt2.Millisecond() == 0);
        REQUIRE_THAT(DateTime::ToString(dt2), Catch::Contains("2023-02-15 07:00:00.0"));

        dt.AddDays(1.5);
        REQUIRE_THAT(DateTime::ToString(dt), Catch::Contains("1970-01-02 20:00:00.0"));
        REQUIRE(dt.Day() == 2);

        dt.AddSeconds(12.2);
        REQUIRE_THAT(
            DateTime::ToString(dt), Catch::Contains("1970-01-02 20:00:12.200"));
        REQUIRE(dt.Second() == 12);

        dt.AddSeconds(3600.0);
        REQUIRE_THAT(
            DateTime::ToString(dt), Catch::Contains("1970-01-02 21:00:12.200"));
        REQUIRE(dt.Second() == 12);

        int64_t timestamp = (dt.Day() - 1) * 24. * 3600. * 1e6
                            + (dt.Hour() - 8 + timeZoneOffset) * 3600. * 1.e6
                            + dt.Minute() * 60. * 1e6 + dt.Second() * 1e6
                            + dt.Millisecond() * 1e3 + dt.Microsecond();
        REQUIRE(dt.GetTimeStampInMicroSecs() == timestamp);

        DateTime date = dt.GetDate();
        REQUIRE(date.Year() == 1970);
        REQUIRE(date.Month() == 1);
        REQUIRE(date.Day() == 2);
        REQUIRE(date.Hour() == 0);
        REQUIRE(date.Minute() == 0);
        REQUIRE(date.Second() == 0);

        double time = dt.Hour() * 3600.0 + dt.Minute() * 60. + dt.Second()
                      + dt.Millisecond() / 1.e3;
        REQUIRE(dt.GetTimeOfDay() == Approx(time).margin(1.e-3));
    }

    SECTION("test static methods")
    {
        DateTime dt1   = DateTime::Today();
        string   dtStr = DateTime::ToString(dt1, "%m-%d-%Y %H:%M:%S");
        // CHECK(dtStr == "02-15-2023 00:00:00.000");

        DateTime dt2 = DateTime::Now();
        REQUIRE(DateTime::Compare(dt1, dt2) == -1);
        REQUIRE(DateTime::Compare(dt2, dt1) == 1);
        REQUIRE(DateTime::Compare(dt2, dt2) == 0);

        DateTime dt3 = DateTime::Zero();
        REQUIRE(dt3.Year() == 1970);
        REQUIRE(dt3.Month() == 1);
        REQUIRE(dt3.Day() == 1);

        DateTime dt4 = DateTime::FromString(dtStr, "%m-%d-%Y %H:%M:%S");
        REQUIRE(DateTime::Compare(dt1, dt4) == 0);

        DateTime dt5(dt4);
        dt5.AddSeconds(6.1);
        TimeSpan ts = DateTime::GetTimeSpan(dt4, dt5);
        REQUIRE(ts.GetTotalSeconds() == Approx(6.1).margin(1.e-3));
    }
}
