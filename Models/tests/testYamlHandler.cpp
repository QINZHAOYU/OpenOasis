#include "ThirdPart/Catch2/catch.hpp"
#include "Models/Utils/YamlHandler.h"
#include <iostream>

using namespace OpenOasis::Utils;
using namespace std;


TEST_CASE("YamlLoader tests")
{
    const char yaml[] = R"(
actors:
  - {name: Junior, value: 4.0}
  - {name: Middle, value: 16.0}
  - {name: Senior, value: 32.0}
  - {name: Dark, value: 48.0}
species: [a, b, c]
scalar: hello world
dict:
  item: "foo bar"
  boolean: TRUE
  integer: 123
  point: 2.75
arr: [1, 2, 3]
dic: {a: 1, b: 2}    
)";

    YamlLoader loader;
    loader.LoadByContent(yaml);

    set<string> keys = loader.MapKeys({});
    CHECK(keys.size() == 6);

    auto value = loader.GetSeqValueInDbl({"actors"}, 1, "value");
    REQUIRE(value.has_value());
    REQUIRE(value.value() == 16.0);

    auto val = loader.GetMapValueInDbl({}, "arr", 1).value();
    REQUIRE(val == 2.);

    set<string> keys2 = loader.MapKeys({"dict"});
    REQUIRE(keys2.size() == 4);

    int size1 = loader.SeqSize({"actors"});
    REQUIRE(size1 == 4);

    string value1 = loader.GetMapValueInStr({}, "scalar").value();
    CHECK(value1 == "hello world");

    auto res0 = loader.GetMapValueInBool({"dict"}, "boolean");
    REQUIRE(res0.has_value());

    bool state = loader.GetMapValueInBool({"dict"}, "boolean").value();
    CHECK(state == true);

    auto state2 = loader.GetMapValueInStr({"dict"}, "item").value();
    CHECK(state2 == "foo bar");

    int value2 = loader.GetMapValueInInt({"dict"}, "integer").value();
    REQUIRE(value2 == 123);

    double value3 = loader.GetMapValueInDbl({"dict"}, "point").value();
    REQUIRE(value3 == 2.75);

    auto res = loader.GetSeq<int>({"arr"});
    REQUIRE(!res.empty());
    REQUIRE(res.size() == 3);
    REQUIRE(res[0] == 1);

    auto res3 = loader.GetMapValueInInt({"dic"}, "a").value();
    REQUIRE(res3 == 1);

    auto map = loader.GetMap({"dic"});
    REQUIRE(map["a"] == "1");
}
