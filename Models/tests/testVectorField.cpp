#include "ThirdPart/Catch2/catch.hpp"
#include "Models/CommImp/Numeric/Field.h"

using namespace OpenOasis::CommImp::Numeric;
using namespace std;


TEST_CASE("Vector template test")
{
    SECTION("constructor test")
    {
        Vector<int, 3> vec0;
        for (int i = 0; i < 3; i++)
        {
            REQUIRE(vec0(i) == 0);
        }

        Vector<float, 2> vec1(1.0f, 2.0f);
        REQUIRE(vec1(0) == 1.0f);
        REQUIRE(vec1(1) == 2.0f);

        Vector<double, 3> vec2 = {1.0, 2.0, 3.0};
        REQUIRE(vec2(2) == 3.0);

        Vector<double, 3> vec3(vec2);
        REQUIRE(vec3(2) == vec3(2));

        Vector<double, 3> vec11 = {1.0, 2.0, 3.0};
        Vector<double, 3> vec22 = {1.1, 2.1, 3.1};

        vec11.SetAt(1, 2.2, 2.3);
        REQUIRE(vec11(0) == 1.0);
        REQUIRE(vec11(2) == 2.3);

        vec22.Set(vec11);
        REQUIRE(vec22(0) == 1.0);
        REQUIRE(vec22(2) == 2.3);

        vec11.Set(Vector<double, 3>(11.0, 12.0, 13.0));
        REQUIRE(vec11(0) == 11.0);
        REQUIRE(vec11(2) == 13.0);
    }

    SECTION("operator test")
    {
        Vector<double, 3> vec1 = {1.0, 2.0, 3.0};
        Vector<double, 3> vec2 = {1.1, 2.1, 3.1};

        auto vec3 = vec1 + vec2;
        REQUIRE(vec3(0) == 2.1);
        REQUIRE(vec3(2) == 6.1);

        auto vec4 = vec3 + 1.0;
        REQUIRE(vec4(0) == 3.1);
        REQUIRE(vec4(2) == 7.1);
        REQUIRE(vec3(0) == 2.1);

        vec3 += vec1;
        REQUIRE(vec3(0) == 3.1);
        REQUIRE(vec3(2) == 9.1);

        auto vec5 = vec3 - vec2;
        REQUIRE(vec5(0) == 2.0);
        REQUIRE(vec5(2) == 6.0);

        double res = vec1 * vec2;
        REQUIRE(res == vec1.Dot(vec2));
        REQUIRE(vec1(0) == 1.0);
        REQUIRE(vec2(0) == 1.1);

        auto vec6 = vec1 & vec2;
        REQUIRE(vec6(0) == vec1.Cross(vec2)(0));
        REQUIRE(vec6(1) == vec1.Cross(vec2)(1));
        REQUIRE(vec1(0) == 1.0);
        REQUIRE(vec2(0) == 1.1);

        vec1 &= vec2;
        REQUIRE(vec1(0) == vec6(0));
        REQUIRE(vec1(1) == vec6(1));

        vec1.Set({1.0, 2.0, 3.0});
        vec2.Set({1.1, 2.1, 3.1});
        Tensor<double> t = vec1 ^ vec2;
        REQUIRE(t(0, 0) == 1.1);
        REQUIRE(t(1, 1) == 4.2);
        REQUIRE(t(2, 2) == 9.3);
    }
}


TEST_CASE("VectorField template test")
{
    VectorField<double, 3> vecField(12);
    const auto            &vec0 = vecField(3);
    REQUIRE(vec0(0) == 0.0);
    REQUIRE(vec0(1) == 0.0);

    vecField.SetAt(3, Vector<double, 3>(1.0, 2.0, 3.0));
    const auto &vec1 = vecField(3);
    REQUIRE(vec1(0) == 1.0);
    REQUIRE(vec1(1) == 2.0);

    vecField.SetAt(4, Vector<double, 3>(11.0, 21.0, 31.0));
    vecField.SetAt(5, Vector<double, 3>(-1.0, -2.0, -3.0));

    vecField.Resize(6);
    REQUIRE(vecField.Size() == 6);
    const auto &vec3 = vecField(3);
    REQUIRE(vec3(0) == 1.0);
    REQUIRE(vec3(1) == 2.0);

    auto func1 = [](Vector<double, 3> &elem) { elem += 1.0; };
    // auto func2 = [](const Vector<double, 3> &elem) { printf("%f\n", elem(0)); };

    vecField.ForEach(func1);
    const auto &vec4 = vecField(3);
    REQUIRE(vec4(0) == 2.0);
    REQUIRE(vec4(1) == 3.0);

    // vecField.ForEach(func2);
}
