#include "ThirdPart/Catch2/catch.hpp"
#include "Models/Utils/StringHelper.h"

using namespace OpenOasis::Utils;
using namespace std;


TEST_CASE("StringHelper tests")
{
    string str1 = "abCdeF12hm";
    string str2 = "ab杭州";
    REQUIRE(StringHelper::ToLower(str1) == "abcdef12hm");
    REQUIRE(StringHelper::ToUpper(str1) == "ABCDEF12HM");
    REQUIRE(StringHelper::ToUpper("aB") == "AB");
    REQUIRE(StringHelper::ToUpper(str2) == "AB杭州");

    string str3 = "\nab cd\t";
    REQUIRE(StringHelper::TrimStart(str3) == "ab cd\t");
    REQUIRE(StringHelper::TrimStart(str3, " ") == "\nab cd\t");
    REQUIRE(StringHelper::TrimEnd(str3) == "\nab cd");
    REQUIRE(StringHelper::TrimEnd(str3, "\n") == "\nab cd\t");
    REQUIRE(StringHelper::Trim(str3) == "ab cd");
    REQUIRE(StringHelper::Trim(str3, " ") == "\nab cd\t");
    REQUIRE(StringHelper::Trim(str3, "\n") == "ab cd\t");
    REQUIRE(StringHelper::Trim(str3, "\t") == "\nab cd");

    REQUIRE(StringHelper::StartsWith(str3, "\n") == true);
    REQUIRE(StringHelper::EndsWith(str3, "\t") == true);
    REQUIRE(StringHelper::StartsWith(str3, "\t") == false);
    REQUIRE(StringHelper::EndsWith(str3, "\n") == false);
    REQUIRE(StringHelper::EndsWith(str2, "州") == true);

    string str4 = "ab-cd++eef";
    REQUIRE(StringHelper::Replace(str4, "-", "/") == "ab/cd++eef");
    REQUIRE(StringHelper::Replace(str4, "++", ">>") == "ab-cd>>eef");
    REQUIRE(StringHelper::Replace(str2, "杭州", "合肥") == "ab合肥");

    vector<string> res;
    string         str5 = "a- - -b";
    REQUIRE(StringHelper::Split(str4, '-').size() == 2);
    REQUIRE(StringHelper::Split(str4, '+').size() == 2);
    REQUIRE(StringHelper::Split(str4, '+')[1] == "eef");
    REQUIRE(StringHelper::Split(str2, 'b')[1] == "杭州");
    REQUIRE(StringHelper::Split(str5, '-').size() == 4);
    REQUIRE(StringHelper::Split(str5, '-')[1] == " ");

    wstring str6 = L"太平洋";
    REQUIRE(StringHelper::WStringToString(str6) == "太平洋");
    REQUIRE(StringHelper::StringToWString(str2) == L"ab杭州");

    REQUIRE(StringHelper::ToString(1) == "1");
    REQUIRE(StringHelper::ToString(10.2) == "10.2");
    REQUIRE(StringHelper::FromString<int>("1") == 1);
    REQUIRE(StringHelper::FromString<double>("10.2") == Approx(10.2));

    REQUIRE(StringHelper::FormatSimple("id-{}", "obj") == "id-obj");
    REQUIRE(StringHelper::FormatSimple("id-{}, id2-{}", "obj") == "id-obj, id2-obj");
    REQUIRE(StringHelper::FormatSimple("value-{}", 10.2) == "value-10.2");
    REQUIRE(
        StringHelper::FormatSimple("id-{} value-{}.", "id1", 10.2)
        == "id-id1 value-10.2.");
    REQUIRE(
        StringHelper::FormatSimple("id-{} value-{}, {}.", "id1", 22, 10.2)
        == "id-id1 value-22, 10.2.");
    REQUIRE(
        StringHelper::FormatSimple("id-{} value-{}, {}.", "id1", 22)
        == "id-id1 value-22, 22.");
    CHECK(
        StringHelper::FormatSimple("id-{} value-{}, {}.", "id1", 22, 10.2, 11)
        == "id-id1 value-22, 10.2.");
    REQUIRE(
        StringHelper::FormatSimple("value: {}, {}, {}", vector<int>{11, 22, 33})
        == "value: 11, 22, 33");
    REQUIRE(
        StringHelper::FormatSimple("value: {}, {}, {}", vector<int>{11, 22})
        == "value: 11, 22, 22");
    CHECK(
        StringHelper::FormatSimple("value: {}, {}, {}", vector<int>{11, 22, 33, 44})
        == "value: 11, 22, 33");

    REQUIRE(StringHelper::StringToWString(str2) == L"ab杭州");
    REQUIRE(StringHelper::WStringToString(L"ab杭州") == str2);
}