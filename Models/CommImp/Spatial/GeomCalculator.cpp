/** ***********************************************************************************
 *    @File      :  GeomCalculator.cpp
 *    @Brief     :  A collection of general geometry functions.
 *
 ** ***********************************************************************************/
#include "GeomCalculator.h"
#include "MeshCalculator.h"
#include <stdexcept>


namespace OpenOasis::CommImp::Spatial
{
using namespace Utils;
using namespace std;


Coordinate GeomCalculator::FromPoint(const Point &point, int foldedAxis)
{
    real x, y, z;
    if (foldedAxis == 0)
    {
        x = 0.0;
        y = point.x;
        z = point.y;
    }
    else if (foldedAxis == 1)
    {
        x = point.x;
        y = 0.0;
        z = point.y;
    }
    else if (foldedAxis == 2)
    {
        x = point.x;
        y = point.y;
        z = 0.0;
    }
    else
    {
        throw runtime_error(
            "Invalid folded axis while converting Point to Coordinate.");
    }
    return Coordinate{x, y, z};
}

Point GeomCalculator::ToPoint(const Node &node, int foldedAxis)
{
    real x, y;
    if (foldedAxis == 0)
    {
        x = node.coor.y;
        y = node.coor.z;
    }
    else if (foldedAxis == 1)
    {
        x = node.coor.x;
        y = node.coor.z;
    }
    else if (foldedAxis == 2)
    {
        x = node.coor.x;
        y = node.coor.y;
    }
    else
    {
        throw runtime_error("Invalid folded axis while converting Node to Point.");
    }
    return Point{x, y, 0.0};
}


Line GeomCalculator::ToLine(const Mesh &mesh, int faceIndex, int foldedAxis)
{
    const Face &face = mesh.faces.at(faceIndex);
    if (face.nodeIndexes.size() != 2)
    {
        throw runtime_error("Only 2D Mesh Face can be converting to Line.");
    }

    Point p1 = ToPoint(mesh.nodes.at(face.nodeIndexes.at(0)), foldedAxis);
    Point p2 = ToPoint(mesh.nodes.at(face.nodeIndexes.at(0)), foldedAxis);

    return Line{p1, p2};
}

Polygon GeomCalculator::FaceToPolygon(const Mesh &mesh, int faceIndex, int foldedAxis)
{
    const Face &face = mesh.faces.at(faceIndex);
    if (face.nodeIndexes.size() < 3)
    {
        throw runtime_error("Only 3D Mesh Face can be converting to Polygon.");
    }

    vector<Point> points;
    for (int i = 0; i < face.nodeIndexes.size(); i++)
    {
        auto point = ToPoint(mesh.nodes.at(face.nodeIndexes.at(i)), foldedAxis);
        points.emplace_back(point);
    }

    return Polygon{points};
}

Polygon GeomCalculator::CellToPolygon(const Mesh &mesh, int cellIndex, int foldedAxis)
{
    const Cell &cell = mesh.cells.at(cellIndex);

    if (mesh.faces.at(cell.faceIndexes.at(0)).nodeIndexes.size() != 2)
    {
        throw runtime_error("Only 2D Mesh Cell can be converting to Polygon.");
    }

    vector<Point> points;
    for (auto nodeIndex : MeshCalculator::GetCellNodeIndexes(cellIndex, mesh))
    {
        auto point = ToPoint(mesh.nodes.at(nodeIndex), foldedAxis);
        points.emplace_back(point);
    }

    return Polygon{points};
}

real GeomCalculator::CalculatePointsDistance(const Point &p1, const Point &p2)
{
    real dx = p2.x - p1.x;
    real dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

bool GeomCalculator::IsLineIntersected(const Line &l1, const Line &l2)
{
    const Point &l1p1 = l1[0], &l1p2 = l1[1];
    const Point &l2p1 = l2[0], &l2p2 = l2[1];

    // Fast rejection test.
    if (max(l1p1.x, l1p2.x) <= min(l2p1.x, l2p2.x)
        || max(l2p1.y, l2p2.y) <= min(l1p1.y, l1p2.y)
        || max(l2p1.x, l2p2.x) <= min(l1p1.x, l1p2.x)
        || max(l1p1.y, l1p2.y) <= min(l2p1.y, l2p2.y))
    {
        return false;
    }

    // Straddle test.
    real l1det1 =
        (l1p1.x - l2p2.x) * (l2p1.y - l2p2.y) - (l1p1.y - l2p2.y) * (l2p1.x - l2p2.x);
    real l1det2 =
        (l1p2.x - l2p2.x) * (l2p1.y - l2p2.y) - (l1p2.y - l2p2.y) * (l2p1.x - l2p2.x);
    real l2det1 =
        (l2p1.x - l1p2.x) * (l1p1.y - l1p2.y) - (l2p1.y - l1p2.y) * (l1p1.x - l1p2.x);
    real l2det2 =
        (l2p2.x - l1p2.x) * (l1p1.y - l1p2.y) - (l2p2.y - l1p2.y) * (l1p1.x - l1p2.x);

    if (l1det1 * l1det2 >= EPSILON || l2det1 * l2det2 >= EPSILON)
    {
        return false;  // line segments not cross.
    }
    if (abs(l1det1 * l1det2) <= EPSILON && abs(l2det1 * l2det2) <= EPSILON)
    {
        return false;  // lines are parrallel.
    }

    return true;
}

Point GeomCalculator::GenerateLineCrossPoint(const Line &l1, const Line &l2)
{
    if (!IsLineIntersected(l1, l2))
    {
        throw runtime_error("Line segments have no intersection point.");
    }

    const Point &l1p1 = l1[0], &l1p2 = l1[1];
    const Point &l2p1 = l2[0], &l2p2 = l2[1];

    real area1 =
        (l1p2.x - l1p1.x) * (l2p1.y - l1p1.y) - (l1p2.y - l1p1.y) * (l2p1.x - l1p1.x);
    real area2 =
        (l1p2.x - l1p1.x) * (l2p2.y - l1p1.y) - (l1p2.y - l1p1.y) * (l2p2.x - l1p1.x);

    real lambda = abs(area1) / abs(area2);
    real x      = l2p1.x + lambda / (lambda + 1) * (l2p2.x - l2p1.x);
    real y      = l2p1.y + lambda / (lambda + 1) * (l2p2.y - l2p1.y);

    return Point{x, y, 0.0};
}

Point GeomCalculator::GenerateMidpoint(const Line &line)
{
    const Point &p1 = line[0], &p2 = line[1];

    real x = (p1.x + p2.x) / 2.;
    real y = (p1.y + p2.y) / 2.;
    return Point{x, y, 0.0};
}

real GeomCalculator::CalculatePointToLineDistance(const Line &line, const Point &point)
{
    const Point p1 = line[0], &p2 = line[1];

    // check if the line is a point actually.
    if (p1.Equals(p2))
    {
        return CalculatePointsDistance(point, p1);
    }

    // Point projected on line before P1, use distance to P1.
    real p2p1dotpp1 =
        (p2.x - p1.x) * (point.x - p1.x) + (p2.y - p1.y) * (point.y - p1.y);
    if (p2p1dotpp1 <= EPSILON)
    {
        return CalculatePointsDistance(point, p1);
    }

    // Point projected on line after P2, use distance to P2.
    real p2p1dotpp2 =
        (p2.x - p1.x) * (point.x - p2.x) + (p2.y - p1.y) * (point.y - p2.y);
    if (p2p1dotpp2 >= EPSILON)
    {
        return CalculatePointsDistance(point, p2);
    }

    // Point projected between P1 and P2, calculate distance to projection point.
    real p2p1crosspp1 =
        (p2.x - p1.x) * (point.y - p1.y) - (p2.y - p1.y) * (point.x - p1.x);
    real lineLen = CalculatePointsDistance(p1, p2);
    return abs(p2p1crosspp1 / lineLen);
}

Line GeomCalculator::GenerateLineFromPolyline(const Polyline &polyline, size_t i)
{
    if (polyline.empty() || i >= polyline.size() || i < 0)
    {
        throw runtime_error("The line index exceeds the Polyline size.");
    }

    return Line{polyline.at(i - 1), polyline.at(i)};
}

Line GeomCalculator::GenerateLineFromPolygon(const Polygon &polygon, size_t i)
{
    if (polygon.empty() || i >= polygon.size())
    {
        throw runtime_error("The line index exceeds the Polygon size.");
    }

    size_t j = IncrementModula(i, polygon.size());
    return Line{polygon.at(i), polygon.at(j)};
}

real GeomCalculator::CalculatePointToPolylineDistance(
    const Polyline &polyline, const Point &point)
{
    if (polyline.empty())
    {
        return 0;
    }

    Line line = GenerateLineFromPolyline(polyline, 1);
    real dist = CalculatePointToLineDistance(line, point);

    size_t i = 1;
    while (i < polyline.size() - 1)
    {
        line = GenerateLineFromPolyline(polyline, i++);
        dist = min(dist, CalculatePointToLineDistance(line, point));
    }

    return dist;
}

real GeomCalculator::CalculateLengthOfPolylineInsidePolygon(
    const Polyline &polyline, const Polygon &polygon)
{
    real lengthInside = 0;
    for (size_t i = 1; i < polyline.size() - 1; i++)
    {
        auto line = GenerateLineFromPolyline(polyline, i);
        lengthInside += CalculateLengthOfLineInsidePolygon(line, polygon);
    }
    return lengthInside;
}

bool GeomCalculator::IsPointInLine(const Point &point, const Line &line)
{
    return (CalculatePointToLineDistance(line, point) <= EPSILON) ? true : false;
}

real GeomCalculator::CalculateAreaOfPolygon(const Polygon &polygon)
{
    // The area of the polygon is:
    //  S = 0.5 * |∑(x_i * y_i+1 - x_i+1 * y_i)|, for i = 0, ...n
    //
    // Among them, x_n+1 = x_1, y_n+1 = y_1.
    //

    if (polygon.empty() || polygon.size() < 3)
        return 0.0;

    auto size = polygon.size();
    real area = 0.0;
    for (size_t i = 0; i < size; ++i)
    {
        size_t j = IncrementModula(i, size);
        area += polygon.at(i).x * polygon.at(j).y - polygon.at(j).x * polygon.at(i).y;
    }

    area *= 0.5;
    return abs(area);
}

real GeomCalculator::CalculateLengthOfPolyline(const Polyline &polyline)
{
    real length = 0;
    for (size_t i = 1; i < polyline.size(); i++)
    {
        length += CalculatePointsDistance(polyline.at(i - 1), polyline.at(i));
    }

    return length;
}

real GeomCalculator::CalculateLengthOfLine(const Line &line)
{
    const Point &p1 = line[0], &p2 = line[1];
    return CalculatePointsDistance(p1, p2);
}

real GeomCalculator::CalculateLengthOfPolygon(const Polygon &polygon)
{
    if (polygon.empty())
    {
        return 0;
    }

    real length = 0;
    for (size_t i = 0; i < polygon.size(); i++)
    {
        size_t j = IncrementModula(i, polygon.size());
        length += CalculatePointsDistance(polygon.at(i), polygon.at(j));
    }

    return length;
}

bool GeomCalculator::IsValidePolygon(const Polygon &polygon)
{
    if (polygon.empty() || polygon.size() < 3)
    {
        return false;
    }

    if (CalculateAreaOfPolygon(polygon) <= EPSILON)
    {
        throw false;
    }

    for (size_t i = 1; i < polygon.size() - 1; i++)
    {
        auto line1 = GenerateLineFromPolygon(polygon, i);
        if (CalculateLengthOfLine(line1) <= EPSILON)
        {
            throw false;
        }

        for (size_t j = 0; j < i; ++j)
        {
            auto line2 = GenerateLineFromPolygon(polygon, j);
            if (IsLineIntersected(line1, line2))
            {
                throw false;
            }
        }
    }

    return true;
}

bool GeomCalculator::IsPointInPolygon(const Point &point, const Polygon &polygon)
{
    if (!IsValidePolygon(polygon))
    {
        return false;
    }

    bool isInside = false;

    real x1, y1, x2, y2;
    auto size = polygon.size();
    for (size_t i = 0; i < size; i++)
    {
        size_t j = IncrementModula(i, size);
        x1       = polygon.at(i).x;
        y1       = polygon.at(i).y;
        x2       = polygon.at(j).x;
        y2       = polygon.at(j).y;

        if (point.y - min(y1, y2) >= EPSILON)
        {
            if (max(y1, y2) - point.y >= EPSILON)
            {
                if (max(x1, x2) - point.x >= EPSILON)
                {
                    if (abs(y1 - y2) > EPSILON)
                    {
                        real xinter = (point.y - y1) * (x2 - x1) / (y2 - y1) + x1;
                        if (abs(x1 - x2) <= EPSILON || xinter - point.x >= EPSILON)
                        {
                            isInside = !isInside;
                        }
                    }
                }
            }
        }
    }

    return isInside;
}

real GeomCalculator::CalculatePolygonSharedArea(const Polygon &p1, const Polygon &p2)
{
    const auto &triangleListA = SplitPolygonToTriangles(p1);
    const auto &triangleListB = SplitPolygonToTriangles(p2);

    real area = 0;
    for (size_t ia = 0; ia < triangleListA.size(); ia++)
    {
        const auto &triangleA = triangleListA[ia];

        for (size_t ib = 0; ib < triangleListB.size(); ib++)
        {
            const auto &triangleB = triangleListB[ib];
            area = area + CalculateTriangleSharedArea(triangleA, triangleB);
        }
    }

    return area;
}

real GeomCalculator::CalculateTriangleSharedArea(
    const Polygon &triangleA, const Polygon &triangleB)
{
    try
    {
        size_t i = 1;  // Index for "next" node in polygon a.

        // Index for "next" node in polygon b.
        // -1 indicates that the first has not yet been found.
        size_t j = -1;

        real    area = 0;      // Intersection area.
        Polygon interPolygon;  // Intersection polygon.

        // Latest intersection node found.
        Point pInter(triangleA.at(0));
        Intersect(triangleA, triangleB, pInter, i, j, interPolygon);

        // First intersection point between triangles.
        Point pFirst = pInter;
        if (j != -1)
        {
            bool complete = false;
            int  count    = 0;
            while (!complete)
            {
                // coordinates for vectors pointing to next triangleA and triangleB
                // point respectively.
                real vax = triangleA.at(i).x - pInter.x;
                real vay = triangleA.at(i).y - pInter.y;
                real vbx = triangleB.at(j).x - pInter.x;
                real vby = triangleB.at(j).y - pInter.y;

                // The sideOf tells if the vb vector or the va vector is the one
                // pointing "left" If sideOf is positive, vb is pointing left, otherwise
                // va is pointing left The "left" vector is the one that is inside the
                // polygon.
                real sideOf = vax * vby - vay * vbx;

                // Make sure to get out of EPSILON reach from the original point, hence
                // 2*EPSILON
                if (sideOf < 0
                    && IsPointInPolygon(
                        Point{
                            pInter.x + 2 * EPSILON * vax, pInter.y + 2 * EPSILON * vay},
                        triangleB))
                {
                    Intersect(triangleA, triangleB, pInter, i, j, interPolygon);
                }
                else if (
                    sideOf > 0
                    && IsPointInPolygon(
                        Point{
                            pInter.x + 2 * EPSILON * vbx, pInter.y + 2 * EPSILON * vby},
                        triangleA))
                {
                    Intersect(triangleB, triangleA, pInter, j, i, interPolygon);
                }
                // can be true if the point is on the edge of the triangleB.
                else if (IsPointInPolygon(
                             Point{
                                 pInter.x + 2 * EPSILON * vax,
                                 pInter.y + 2 * EPSILON * vay},
                             triangleB))
                {
                    Intersect(triangleA, triangleB, pInter, i, j, interPolygon);
                }
                // can be true if the point is on the edge of the triangleA
                // Should never happen, since above test basically does the same.
                else if (IsPointInPolygon(
                             Point{
                                 pInter.x + 2 * EPSILON * vbx,
                                 pInter.y + 2 * EPSILON * vby},
                             triangleA))
                {
                    Intersect(triangleB, triangleA, pInter, j, i, interPolygon);
                }
                // triangleA and triangleB only touches one another but do not
                // intersect
                else
                {
                    return 0.0;
                }

                if (!interPolygon.empty())
                {
                    complete = (CalculatePointsDistance(pInter, pFirst) < EPSILON);
                }

                count++;
                if (count > 20)
                {
                    throw runtime_error("Failed to find intersection polygon");
                }
            }

            area = CalculateAreaOfPolygon(interPolygon);
        }
        else
        {
            // internal point in triangle a
            const auto &pointsA = triangleA;
            real        xPa     = (pointsA[0].x + pointsA[1].x + pointsA[2].x) / 3.0;
            real        yPa     = (pointsA[0].y + pointsA[1].y + pointsA[2].y) / 3.0;

            Point pa{xPa, yPa};

            // internal point in triangle b
            const auto &pointsB = triangleB;
            real        xPb     = (pointsB[0].x + pointsB[1].x + pointsB[2].x) / 3.0;
            real        yPb     = (pointsB[0].y + pointsB[1].y + pointsB[2].y) / 3.0;

            Point pb{xPb, yPb};

            // triangleA is completely inside triangleB
            if (IsPointInPolygon(pa, triangleB) || IsPointInPolygon(pb, triangleA))
            {
                real area1 = CalculateAreaOfPolygon(triangleA);
                real area2 = CalculateAreaOfPolygon(triangleB);
                area       = min(area1, area2);
            }
            else  // triangleA and triangleB do dot intersect
            {
                area = 0;
            }
        }

        return area;
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("GetTriangleSharedArea failed");
    }
}

vector<Polygon> GeomCalculator::SplitPolygonToTriangles(const Polygon &polygon)
{
    if (!IsValidePolygon(polygon))
    {
        throw runtime_error("Invalid Polygon object.");
    }

    Polygon         localPolygon(polygon);
    vector<Polygon> triangleList;

    while (localPolygon.size() > 3)
    {
        auto i    = FindTrianglePoints(localPolygon);
        auto n    = localPolygon.size();
        auto prev = (i == 0) ? n - 1 : i - 1;
        auto next = (i == n - 1) ? 0 : i + 1;

        Polygon triangle;
        triangle.push_back(localPolygon[prev]);
        triangle.push_back(localPolygon[i]);
        triangle.push_back(localPolygon[next]);

        triangleList.push_back(triangle);
        localPolygon.erase(localPolygon.begin() + i);
    }

    triangleList.push_back(localPolygon);
    return triangleList;
}

bool GeomCalculator::IsAngleConvex(const Polygon &polygon, size_t i)
{
    // Acute angles are convex.

    if (polygon.empty())
    {
        return false;
    }

    auto prevIndex = DecrementModula(i, polygon.size());
    auto nextIndex = IncrementModula(i, polygon.size());

    const Point &p1 = polygon[prevIndex], &p2 = polygon[nextIndex], &p = polygon[i];
    real         dot = (p1.x - p.x) * (p2.x - p.x) + (p1.y - p.y) * (p2.y - p.y);

    return (dot >= EPSILON) ? true : false;
}

size_t GeomCalculator::FindTrianglePoints(const Polygon &polygon)
{
    bool   found = false;
    size_t i     = 0;

    while (i < polygon.size() && !found)
    {
        if (IsAngleConvex(polygon, i) && !IsTriangleIntersected(polygon, i))
        {
            found = true;
        }
        else
        {
            i++;
        }
    }

    return i;
}

bool GeomCalculator::IsTriangleIntersected(const Polygon &polygon, size_t index)
{
    const auto &points    = polygon;
    auto        prevIndex = DecrementModula(index, points.size());
    auto        nextIndex = IncrementModula(index, points.size());

    const Point &p1 = points[prevIndex], &p = points[index], &p2 = points[nextIndex];

    Polygon triangulation;
    triangulation.push_back(p1);
    triangulation.push_back(p);
    triangulation.push_back(p2);

    size_t i           = 0;
    bool   insertected = false;
    while (i < points.size() - 1 && !insertected)
    {
        bool skip = (i == index || i == prevIndex || i == nextIndex);
        if (!skip && IsPointInPolygon(points[i], triangulation))
        {
            return true;
        }
        else
        {
            i++;
        }
    }

    return insertected;
}

real GeomCalculator::CalculateLineSharedLength(const Line &lineA, const Line &lineB)
{
    const Point &p1A = lineA.at(0), &p2A = lineA[1];
    const Point &p1B = lineB.at(0), &p2B = lineB[1];

    if (abs(p2A.x - p1A.x) < EPSILON && abs(p2B.x - p1B.x) < EPSILON
        && abs(p1A.x - p1B.x) < EPSILON)
    {
        real YP1A = min(p1A.y, p2A.y);
        real YP2A = max(p1A.y, p2A.y);
        real YP1B = min(p1B.y, p2B.y);
        real YP2B = max(p1B.y, p2B.y);

        real YP1 = max(YP1A, YP1B);
        real YP2 = min(YP2A, YP2B);
        if (YP1 < YP2)
        {
            return YP2 - YP1;
        }
    }
    else if (abs(p2A.x - p1A.x) < EPSILON || abs(p2B.x - p1B.x) < EPSILON)
    {
        return 0;
    }
    else
    {
        Point P1A, P2A;
        if (p1A.x < p2A.x)
        {
            P1A = p1A;
            P2A = p2A;
        }
        else
        {
            P1A = p2A;
            P2A = p1A;
        }
        Point P1B, P2B;
        if (p1B.x < p2B.x)
        {
            P1B = p1B;
            P2B = p2B;
        }
        else
        {
            P1B = p2B;
            P2B = p1B;
        }

        real alphaA = (P2A.y - P1A.y) / (P2A.x - P1A.x);
        real betaA  = -alphaA * P2A.x + P2A.y;
        real alphaB = (P2B.y - P1B.y) / (P2B.x - P1B.x);
        real betaB  = -alphaA * P2B.x + P2B.y;
        if (abs(alphaA - alphaB) < EPSILON && abs(betaA - betaB) < EPSILON)
        {
            real x1 = max(P1A.x, P1B.x);
            real x2 = min(P2A.x, P2B.x);
            if (x1 < x2)
            {
                Line line{x1, alphaA * x1 + betaA, x2, alphaA * x2 + betaA};

                return CalculateLengthOfLine(line);
            }
        }
    }

    return 0.0;
}

real GeomCalculator::CalculateLengthOfLineInsidePolygon(
    const Line &line, const Polygon &polygon)
{
    vector<Line> lineList{line};
    for (size_t i = 0; i < polygon.size(); i++)  // All lines in the polygon
    {
        for (size_t n = 0; n < lineList.size(); n++)
        {
            if (lineList.size() > 1000)
            {
                throw runtime_error("Line has been cut in more than 1000 pieces.");
            }

            Line line = GenerateLineFromPolygon(polygon, i);
            if (IsLineIntersected(lineList[n], line))
            {
                // Split the intersecting line into two lines
                Point IntersectionPoint(GenerateLineCrossPoint(lineList[n], line));

                Line tempVar{IntersectionPoint, lineList[n][1]};
                lineList.push_back(tempVar);

                lineList[n][1] = IntersectionPoint;
                break;
            }
        }
    }

    for (size_t i = 0; i < lineList.size(); i++)
    {
        if (lineList.size() > 1000)
        {
            throw runtime_error("Line has been cuttes in more than 100 pieces.");
        }
        for (size_t j = 0; j < polygon.size(); j++)
        {
            Line line = GenerateLineFromPolygon(polygon, j);
            if (IsPointInLineInterior(line[0], lineList[i]))
            {
                Line tempVar2{line[0], lineList[i][1]};
                lineList.push_back(tempVar2);

                lineList[i][1] = line[0];
            }
        }
    }

    real lengthInside = 0;
    for (size_t i = 0; i < lineList.size(); i++)
    {
        real sharedLength = 0;
        for (size_t j = 0; j < polygon.size(); j++)
        {
            Line line = GenerateLineFromPolygon(polygon, j);
            sharedLength += CalculateLineSharedLength(lineList[i], line);
        }
        if (sharedLength > EPSILON)
        {
            lengthInside += sharedLength / 2;
        }
        else if (IsPointInPolygon(GenerateMidpoint(lineList[i]), polygon))
        {
            lengthInside += CalculateLengthOfLine(lineList[i]) / 2;
        }
    }
    return lengthInside;
}

bool GeomCalculator::IsSharedPoint(
    const Line &LineA, const Line &LineB, Point &intersectionPoint)
{
    if (IsLineIntersected(LineA, LineB))
    {
        intersectionPoint = GenerateLineCrossPoint(LineA, LineB);
        return true;
    }

    if (IsPointInLine(LineA[1], LineB))
    {
        intersectionPoint = LineA[1];
        return true;
    }

    if (IsPointInLine(LineB[1], LineA))
    {
        intersectionPoint = LineB[1];
        return true;
    }
    if (IsPointInLine(LineB[0], LineA))
    {
        intersectionPoint = LineB[0];
        return true;
    }
    if (IsPointInLine(LineA[0], LineB))
    {
        intersectionPoint = LineA[0];
        return true;
    }

    return false;
}

bool GeomCalculator::IsPointInLineInterior(const Point &point, const Line &line)
{
    bool result = false;
    if (line[0].x - line[1].x != 0)  // line is not vertical
    {
        if ((point.x > min(line[0].x, line[1].x))
            && (point.x < max(line[0].x, line[1].x)))
        {
            if (abs(point.y - line[0].y
                    - (line[1].y - line[0].y) / (line[0].x - line[1].x)
                          * (line[0].x - point.x))
                < EPSILON * EPSILON)
            {
                result = true;
            }
        }
    }
    else  // line is vertical
    {
        if (line[0].x == point.x)
        {
            if ((point.y > min(line[0].y, line[1].y))
                && (point.y < max(line[0].y, line[1].y)))
            {
                result = true;
            }
        }
    }
    return result;
}

void GeomCalculator::Intersect(
    const Polygon &triangleA, const Polygon &triangleB, Point &p, size_t &i, size_t &j,
    Polygon &sharedPolygon)
{
    Line lineA;
    Line lineB;

    size_t im1    = DecrementModula(i, 3);  // "i-1"
    int    count1 = 0;
    bool   found  = false;

    while ((count1 < 3) && (!found))
    {
        lineA = GenerateLineFromPolygon(triangleA, im1);
        if (count1 == 0)
        {
            lineA[0] = p;
        }

        real   MinDist  = -1;  // Distance used when a line is crossed more than once.
        size_t jm1      = 0;   // "j-1"
        size_t jm1Store = -1;
        while (jm1 < 3)
        {
            lineB = GenerateLineFromPolygon(triangleB, jm1);
            found = IsSharedPoint(lineA, lineB, p);

            real Dist = CalculatePointsDistance(lineA[0], p);
            if (Dist < EPSILON)
            {
                found = false;
            }

            if (found)
            {
                if ((MinDist < 0) || (Dist < MinDist))
                {
                    MinDist  = Dist;
                    jm1Store = jm1;
                }
            }
            jm1++;
        }

        if (jm1Store > -1)
        {
            lineB = GenerateLineFromPolygon(triangleB, jm1Store);
            found = IsSharedPoint(lineA, lineB, p);

            Point HelpCoordinate(p);
            Point HelpNode(HelpCoordinate);
            sharedPolygon.push_back(HelpNode);

            j = IncrementModula(jm1Store, 3);
        }

        if (!found)
        {
            count1++;
            im1 = IncrementModula(im1, 3);
            i   = IncrementModula(i, 3);
            if (j != -1)
            {
                Point HelpCoordinate(lineA[1]);
                Point HelpNode(HelpCoordinate);
                sharedPolygon.push_back(HelpNode);
            }
        }
    }

    lineA = GenerateLineFromPolygon(triangleA, DecrementModula(i, 3));
    if (CalculatePointsDistance(p, lineA[1]) < EPSILON)
    {
        i = IncrementModula(i, 3);
    }

    lineB = GenerateLineFromPolygon(triangleB, DecrementModula(j, 3));
    if (CalculatePointsDistance(p, lineB[1]) < EPSILON)
    {
        j = IncrementModula(j, 3);
    }
}

size_t GeomCalculator::IncrementModula(size_t i, size_t n)
{
    i++;
    i %= n;
    return i;
}

size_t GeomCalculator::DecrementModula(size_t i, size_t n)
{
    i--;
    if (i < 0)
    {
        i = n - 1;
    }
    return i;
}

bool GeomCalculator::IsPointInExtent(const Point &point, const GeomExtent &extent)
{
    return point.x >= extent.xMin && point.x <= extent.xMax && point.y >= extent.yMin
           && point.y <= extent.yMax;
}

bool GeomCalculator::IsExtentOverlap(const GeomExtent &e1, const GeomExtent &e2)
{
    return e1.xMax > e2.xMin && e1.xMin < e2.xMax && e1.yMax > e2.yMin
           && e1.yMin < e2.yMax && e1.zMax > e2.zMin && e1.zMin < e2.zMax;
}

void GeomCalculator::UpdateExtent(GeomExtent &extent, const Point &point)
{
    extent.xMin = min(extent.xMin, point.x);
    extent.xMax = max(extent.xMax, point.x);
    extent.yMin = min(extent.yMin, point.y);
    extent.yMax = max(extent.yMax, point.y);
    extent.zMin = min(extent.zMin, point.z);
    extent.zMax = max(extent.zMax, point.z);
}

void GeomCalculator::UpdateExtent(GeomExtent &extent, const GeomExtent &other)
{
    extent.xMin = min(extent.xMin, other.xMin);
    extent.xMax = max(extent.xMax, other.xMax);
    extent.yMin = min(extent.yMin, other.yMin);
    extent.yMax = max(extent.yMax, other.yMax);
    extent.zMin = min(extent.zMin, other.zMin);
    extent.zMax = max(extent.zMax, other.zMax);
}

}  // namespace OpenOasis::CommImp::Spatial