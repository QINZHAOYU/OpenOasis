#include "ThirdPart/Catch2/catch.hpp"
#include "Models/Utils/CsvHandler.h"

using namespace OpenOasis::Utils;
using namespace std;


TEST_CASE("CsvLoader tests")
{
    const std::string &csv =
        "Date,Open,High,Low,Close,Volume,Adj Close\n"
        "2017-02-24 00:00:00,64.529999,64.800003,64.139999,64.620003,21705200,64.620003\n"
        "2017-02-23 00:00:00,64.419998,64.730003,64.190002,64.620003,20235200,64.620003\n"
        "2017-02-22 00:00:00,64.330002,64.389999,64.050003,64.360001,19259700,64.360001\n"
        "2017-02-21 00:00:00,64.610001,64.949997,64.449997,64.489998,19384900,64.489998\n"
        "2017-02-17 00:00:00,64.470001,64.690002,64.300003,64.620003,21234600,64.620003\n";

    SECTION("test0")
    {
        CsvLoader loader;

        loader.LoadByContent(csv);

        REQUIRE(loader.GetColumnCount() == 6);
        REQUIRE(loader.GetRowCount() == 5);

        const auto row2 = loader.GetRow<double>(1).value();
        REQUIRE(row2.size() == 6);
        REQUIRE(row2[2] == 64.190002);

        const auto col3 = loader.GetColumn<double>(2).value();
        REQUIRE(col3.size() == 5);
        REQUIRE(col3[2] == 64.050003);
    }

    SECTION("test1")
    {
        CsvLoader loader;

        loader.LoadByContent(csv, true, false);

        const auto dates = loader.GetColumnAsDateTime(0).value();
        REQUIRE(dates.size() == 5);
        REQUIRE(DateTime::ToString(dates[0]) == "2017-02-24 00:00:00.0");
    }
}


TEST_CASE("CsvWriter tests")
{
    const string path = "./test.csv";

    SECTION("column write")
    {
        CsvWriter writer(path);

        writer.InsertColumn<string>(0, "col0", {"1", "2", "3"});
        writer.InsertColumn<string>(1, "col1", {"1", "2", "3"});
        writer.InsertColumn<int>(2, "col2", {1, 2, 3});
        writer.InsertColumn<double>(3, "col3", {1.1, 2.2, 3.3});

        writer.SetRowLabel(-1, "id");
        writer.SetRowLabel(0, "row1");
        writer.SetRowLabel(1, "row2");
        writer.SetRowLabel(2, "row3");

        writer.Save();

        const auto file2 = writer.GetOutputFilePath();
        CsvLoader  loader;
        loader.LoadByFile(file2);
        REQUIRE(loader.GetColumnCount() == 4);

        const auto labels = loader.GetColumnLabels().value();
        REQUIRE(labels[0] == "col0");
    }

    SECTION("row write")
    {
        CsvWriter writer(path);

        writer.InsertRow<string>(0, "row0", {"1", "2", "3"});
        writer.InsertRow<string>(1, "row1", {"1", "2", "3"});
        writer.InsertRow<int>(2, "row2", {1, 2, 3});
        writer.InsertRow<double>(3, "row3", {1.1, 2.2, 3.3});

        writer.SetColumnLabel(0, "col1");
        writer.SetColumnLabel(1, "col2");
        writer.SetColumnLabel(2, "col3");

        writer.Save();

        const auto file2 = writer.GetOutputFilePath();
        CsvLoader  loader;
        loader.LoadByFile(file2);
        REQUIRE(loader.GetColumnCount() == 3);

        const auto labels = loader.GetColumnLabels().value();
        REQUIRE(labels[0] == "col1");
    }
}
