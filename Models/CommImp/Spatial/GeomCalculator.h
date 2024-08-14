/** ***********************************************************************************
 *    Copyright (C) 2024, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  GeomCalculator.h
 *    @License   :  Apache-2.0
 *
 ** ***********************************************************************************/
#pragma once
#include "Geom.h"
#include "Mesh.h"


namespace OpenOasis::CommImp::Spatial
{
/// @brief Class for calculating plane geometric properties.
class GeomCalculator final
{
public:
    static constexpr real EPSILON = 1.0e-6;


    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for converting Mesh objects to Geom objects.
    //

    static Coordinate FromPoint(const Point &point, int foldedAxis = 2);

    static Point ToPoint(const Node &node, int foldedAxis = 2);

    static Line ToLine(const Mesh &mesh, int faceIndex, int foldedAxis = 2);

    static Polygon FaceToPolygon(const Mesh &mesh, int faceIndex, int foldedAxis = 2);

    static Polygon CellToPolygon(const Mesh &mesh, int cellIndex, int foldedAxis = 2);


    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for calculating geometric properties.
    //

    static real CalculatePointToLineDistance(const Line &line, const Point &point);

    static real CalculatePointsDistance(const Point &p1, const Point &p2);

    static real
    CalculatePointToPolylineDistance(const Polyline &polyline, const Point &point);

    static real CalculatePolygonSharedArea(const Polygon &p1, const Polygon &p2);

    static real CalculateLineSharedLength(const Line &l1, const Line &l2);

    static real CalculateLengthOfPolygon(const Polygon &polygon);

    static real CalculateAreaOfPolygon(const Polygon &polygon);

    static real CalculateLengthOfPolyline(const Polyline &polyline);

    static real CalculateLengthOfLine(const Line &line);

    static real
    CalculateLengthOfLineInsidePolygon(const Line &line, const Polygon &polygon);

    static real CalculateLengthOfPolylineInsidePolygon(
        const Polyline &polyline, const Polygon &polygon);

    static bool IsPointInLine(const Point &point, const Line &line);

    static bool IsLineIntersected(const Line &l1, const Line &l2);

    static bool IsPointInPolygon(const Point &point, const Polygon &polygon);

    static bool IsPointInExtent(const Point &point, const GeomExtent &extent);

    static bool IsExtentOverlap(const GeomExtent &e1, const GeomExtent &e2);


    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for generating geometric objects.
    //

    static Point GenerateLineCrossPoint(const Line &l1, const Line &l2);

    static Point GenerateMidpoint(const Line &line);

    static std::vector<Polygon> SplitPolygonToTriangles(const Polygon &polygon);

    static void UpdateExtent(GeomExtent &extent, const Point &point);

    static void UpdateExtent(GeomExtent &extent, const GeomExtent &other);


protected:
    /// @brief Finds a set of three concecutive points that form a triangle,
    /// that is not intersected by other parts of the polygon.
    static size_t FindTrianglePoints(const Polygon &polygon);

    /// @brief Determines if the triangle formed by  P(i-1), P(i) and P(i+1)
    /// from polygon are intersected by any of the other points.
    static bool IsTriangleIntersected(const Polygon &polygon, size_t i);

    /// @brief Calculates the intersection area of triangle polygon a and b.
    static real CalculateTriangleSharedArea(const Polygon &p1, const Polygon &p2);

    /// @brief Checks if two lines shared a point either as a
    /// real crossing point or as a shared end point or a end point of
    /// the one line being in the other line.
    static bool IsSharedPoint(const Line &l1, const Line &l2, Point &p);

    static bool IsValidePolygon(const Polygon &polygon);

    /// @brief Determines if a point is included in a lines interior
    /// and not an endpoint.
    static bool IsPointInLineInterior(const Point &point, const Line &line);

    /// @brief Calculates the intersection points of triangle xypolygon a and b.
    static void Intersect(
        const Polygon &p1, const Polygon &p2, Point &p, size_t &i, size_t &j,
        Polygon &sharedPolygon);

    /// @brief Retrieves the lineNumber line segment.
    static Line GenerateLineFromPolyline(const Polyline &polyline, size_t number);
    static Line GenerateLineFromPolygon(const Polygon &polygon, size_t number);

    /// @brief Decides if the angle at index point is convex or concave.
    static bool IsAngleConvex(const Polygon &polygon, size_t i);

    /// @brief Gets the next index in a circular list {0, ..., n-1}.
    static size_t IncrementModula(size_t i, size_t n);

    /// @brief Gets previous index in a circular list {0, ..., n-1}.
    static size_t DecrementModula(size_t i, size_t n);
};

}  // namespace OpenOasis::CommImp::Spatial