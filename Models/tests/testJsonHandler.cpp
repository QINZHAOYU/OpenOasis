#include "ThirdPart/Catch2/catch.hpp"
#include "Models/Utils/JsonHandler.h"
#include <iostream>

using namespace OpenOasis::Utils;
using namespace std;


TEST_CASE("JsonLoader tests")
{
    const char *json = R"(
    {
    "pi": 3.141,
    "happy": true,
    "name": "Niels",
    "nothing": null,
    "answer": {
        "everything": 42,
        "something": 22
    },
    "list": [
        1,
        0,
        2
    ],
    "object": {
        "currency": "USD",
        "value": 42.99
    },
    "test": [
        {"val": 1, "var": "temp"},
        {"val": 0, "var": "flow"}
    ],
    "objs": {
        "obj1": "objs",
        "obj2": "objs"
    }
    })";

    // JsonLoader2 loader;
    // loader.LoadByContent(json);

    // const auto &keys0 = loader.GetKeys({});  //
    // answer,happy,list,name,nothing,object,pi REQUIRE(keys0.size() == 9);
    // REQUIRE(keys0[0] == "answer");

    // const auto &keys1 = loader.GetKeys({keys0[0]});
    // REQUIRE(keys1.size() == 2);

    // const auto &keys2 = loader.GetKeys({keys0[1]});
    // REQUIRE(keys2.size() == 0);

    // bool isnull = loader.IsNull({}, "nothing");
    // REQUIRE(isnull);

    // double pi = loader.GetValue<double>({}, "pi").value();
    // REQUIRE(pi == 3.141);

    // double value = loader.GetValue<double>({"object"}, "value").value();
    // REQUIRE(value == 42.99);

    // auto res = loader.GetArray<int>({}, "list");
    // REQUIRE(res.has_value() == true);

    // auto arr = loader.GetArray<int>({}, "list").value();
    // REQUIRE(arr.size() == 3);
    // REQUIRE(arr[0] == 1);

    // auto size2 = loader.GetArraySize({"test"});
    // REQUIRE(size2 == 2);

    // auto arr2 = loader.GetValue<double>({"test"}, 1, "val");
    // REQUIRE(arr2.has_value() == true);
    // REQUIRE(arr2.value() == 0);

    // auto arr3 = loader.GetValue<string>({"test"}, 0, "var");
    // REQUIRE(arr3.has_value() == true);
    // REQUIRE(arr3.value() == "temp");

    // auto map2 = loader.GetMap({"objs"});
    // REQUIRE(map2.size() == 2);
    // REQUIRE(map2["obj1"] == "objs");
}


TEST_CASE("JsonWriter tests")
{
    const string path = "./";

    JsonWriter writer(path);

    writer.SetValue<double>({}, "pi", 3.1415926);
    writer.SetValue<bool>({}, "happy", true);
    writer.SetValue<string>({}, "name", "mike");
    writer.SetValue<int>({}, "nothing", 12);

    writer.SetValue<int>({"answer"}, "everything", 42);
    writer.SetValue<int>({"answer"}, "something", 22);
    writer.SetValue<string>({"answer"}, "desc", "field");

    writer.SetValue<string>({"key0", "key1"}, "key2", "value");

    writer.SetArray<int>({}, "array", {12, 13, 14});
    writer.SetArray<string>({}, "object", {"currency", "USD"});

    writer.SetArray<double>({}, "empty_list2", {});

    writer.Save();
}
