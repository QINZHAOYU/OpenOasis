#include "ThirdPart/Catch2/catch.hpp"
#include "Models/Utils/VectorHelper.h"
#include "Models/Utils/MapHelper.h"
#include <string>

using namespace OpenOasis::Utils;
using namespace std;

struct st1
{
    int  a;
    char b;
};

bool operator==(const st1 &obj1, const st1 &obj2)
{
    return (obj1.a == obj2.a) && (obj1.b == obj2.b);
}

TEST_CASE("VectorHelper tests")
{
    SECTION("test built-in types")
    {
        vector<int> vec1{1, 2, 3, 3, 4, 5, 3, 6};

        REQUIRE(VectorHelper::IndexOf(vec1, 3) == 2);
        REQUIRE(VectorHelper::IndexOf(vec1, 3, 3) == 3);
        REQUIRE(VectorHelper::IndexOf(vec1, 3, 8) == -1);
        REQUIRE_THROWS(VectorHelper::IndexOf(vec1, 3, 9));

        REQUIRE(VectorHelper::LastIndexOf(vec1, 3) == 6);
        REQUIRE(VectorHelper::LastIndexOf(vec1, 3, 2) == 3);
        REQUIRE(VectorHelper::LastIndexOf(vec1, 3, 3) == 3);
        REQUIRE(VectorHelper::LastIndexOf(vec1, 3, 5) == 2);
        REQUIRE(VectorHelper::LastIndexOf(vec1, 3, 8) == -1);
        REQUIRE_THROWS(VectorHelper::LastIndexOf(vec1, 3, 9));

        REQUIRE(VectorHelper::IsContained(vec1, 2) == true);
        REQUIRE(VectorHelper::IsContained(vec1, 10) == false);

        vector<int> vec2{1, 2, 3, 3};
        vector<int> vec3{1, 2, 3, 13};
        REQUIRE(VectorHelper::IsSubset(vec1, vec2) == true);
        REQUIRE(VectorHelper::IsSubset(vec1, vec3) == false);
        REQUIRE(VectorHelper::IsSubset(vec2, vec1) == false);
    }

    SECTION("test custom type")
    {
        vector<st1> vec1{{1, 'a'}, {2, 'b'}, {3, 'c'}, {3, 'c'}, {3, 'd'}, {4, 'd'}};

        REQUIRE(VectorHelper::IndexOf(vec1, st1{2, 'b'}) == 1);
        REQUIRE(VectorHelper::IndexOf(vec1, st1{3, 'c'}) == 2);
        REQUIRE(VectorHelper::IndexOf(vec1, st1{3, 'c'}, 3) == 3);
        REQUIRE(VectorHelper::LastIndexOf(vec1, st1{3, 'c'}) == 3);
        REQUIRE(VectorHelper::LastIndexOf(vec1, st1{3, 'c'}, 3) == 2);
    }
}


TEST_CASE("MapHelper tests")
{
    map<int, string> map1{{1, "a"}, {2, "b"}, {3, "c"}};
    REQUIRE(MapHelper::GetKeys(map1) == vector<int>{1, 2, 3});
    REQUIRE(MapHelper::GetKeys(map1) != vector<int>{1, 2, 3, 4});
    REQUIRE(MapHelper::GetValues(map1) == vector<string>{"a", "b", "c"});
    REQUIRE(MapHelper::GetValues(map1) != vector<string>{"a", "b", "c", "d"});

    unordered_map<int, string> map2{{1, "a"}, {2, "b"}, {3, "c"}};

    const vector<int> keys = {1, 2, 3};
    REQUIRE(MapHelper::GetKeys(map2).size() == keys.size());
    for (int val : MapHelper::GetKeys(map2))
        REQUIRE(VectorHelper::IsContained(keys, val));
    // REQUIRE(MapHelper::GetKeys(map2) == vector<int>{1, 2, 3});

    const vector<string> values = {"a", "b", "c"};
    REQUIRE(MapHelper::GetValues(map2).size() == values.size());
    for (const auto &val : MapHelper::GetValues(map2))
        REQUIRE(VectorHelper::IsContained(values, val));
    // REQUIRE(MapHelper::GetValues(map2) == vector<string>{"a", "b", "c"});
}