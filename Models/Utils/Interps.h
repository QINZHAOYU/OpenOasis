/** ***********************************************************************************
 *    Copyright (C) 2024, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Interps.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Provides interpolation functions.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Utils/CommConstants.h"
#include <optional>
#include <vector>
#include <tuple>
#include <functional>
#include <string>
#include <unordered_map>


namespace OpenOasis::Utils
{
using InterpFunctionType = std::function<std::optional<real>(
    const real *, const real *, size_t, real, bool, real)>;


std::optional<real> InterpByNearest(
    const real *xArr, const real *yArr, size_t n, real x, bool isExtrap = true,
    real epsilon = Utils::FP_EPSILON);

std::optional<real> InterpByLinear(
    const real *xArr, const real *yArr, size_t n, real x, bool isExtrap = true,
    real epsilon = Utils::FP_EPSILON);


class InterpolationAssembly final
{
private:
    static std::unordered_map<std::string, InterpFunctionType> mInterps;

public:
    static InterpFunctionType GetInterpFunction(const std::string &interpName);
};


}  // namespace OpenOasis::Utils