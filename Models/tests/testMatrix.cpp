#include "ThirdPart/Catch2/catch.hpp"
#include "Models/CommImp/Numeric/Matrix.h"

using namespace OpenOasis::CommImp::Numeric;
using namespace std;


TEST_CASE("Matrix template test")
{
    SECTION("constructor test")
    {
        Matrix<double> mat(3, 3);
    }
}