/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  CommConstants.hpp
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide common constants.
 *
 ** ***********************************************************************************/
#pragma once
#include <limits>
#include <cmath>

namespace OpenOasis::Utils
{

// Commonly used constants ------------------------------------------------------------

const int          _INT_MAX       = (std::numeric_limits<int>::max)();
const int          _INT_MIN       = (std::numeric_limits<int>::min)();
const unsigned int _UINT_MAX      = (std::numeric_limits<unsigned int>::max)();
const unsigned int _UINT_MIN      = (std::numeric_limits<unsigned int>::min)();
const double       _DBL_MAX       = (std::numeric_limits<double>::max)();
const double       _DBL_MIN       = (std::numeric_limits<double>::min)();
const double       _DBL_EPSILON   = (std::numeric_limits<double>::epsilon)();
const float        _FLOAT_MAX     = (std::numeric_limits<float>::max)();
const float        _FLOAT_MIN     = (std::numeric_limits<float>::min)();
const float        _FLOAT_EPSILON = (std::numeric_limits<float>::epsilon)();


// Single and double precision switches -----------------------------------------------

#ifdef USE_SP
typedef float real;
const real    FP_EPSILON = _FLOAT_EPSILON;
const real    FP_MAX     = _FLOAT_MAX;
const real    FP_MIN     = _FLOAT_MIN;
#else
typedef double real;
const real     FP_EPSILON = _DBL_EPSILON;
const real     FP_MAX     = _DBL_MAX;
const real     FP_MIN     = _DBL_MIN;
#endif

#define FP(a) static_cast<real>(a)
#define IsNan(a) (isnan(a))
#define IsEql(a, b) (fabs(a - b) <= FP_EPSILON)
#define IsSim(a, b, delta) (fabs(a - b) <= fabs(delta))
#define IsBigger(a, b) (a - b > FP_EPSILON)
#define IsNoLess(a, b) (a - b >= FP_EPSILON)

}  // namespace OpenOasis::Utils