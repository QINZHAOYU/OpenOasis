/** ***********************************************************************************
 *    Copyright (C) 2024, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Geom.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Definitions of geometry related structures.
 *
 ** ***********************************************************************************/
#pragma once
#include "Coordinate.h"
#include <vector>
#include <array>


namespace OpenOasis::CommImp::Spatial
{
using Point      = Coordinate;
using Line       = std::array<Point, 2>;
using Polyline   = std::vector<Point>;
using Polygon    = std::vector<Point>;
using Polyhedron = std::vector<Polygon>;


enum class GeomType
{
    Point,
    Line,
    Polyline,
    Polygon,
    Polyhedron
};

struct GeomExtent
{
    real xMin = 0.0;
    real xMax = 0.0;
    real yMin = 0.0;
    real yMax = 0.0;
    real zMin = 0.0;
    real zMax = 0.0;
};


}  // namespace OpenOasis::CommImp::Spatial