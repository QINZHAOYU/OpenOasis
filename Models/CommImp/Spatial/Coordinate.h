/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Coordinate.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  The Coordinate struct contains a (x, y, z) coordinate.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Utils/CommConstants.h"


namespace OpenOasis::CommImp::Spatial
{
using Utils::real;

/// @brief The Coordinate struct contains a (x, y, z) coordinate.
struct Coordinate
{
    real x = 0.;
    real y = 0.;
    real z = 0.;

    bool Equals(real coorX, real coorY, real coorZ) const;
    bool Equals(const Coordinate &coor) const;
};

}  // namespace OpenOasis::CommImp::Spatial