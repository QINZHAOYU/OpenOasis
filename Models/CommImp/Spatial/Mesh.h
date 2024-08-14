/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  MeshStructs.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Definitions of mesh related structures.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/CommImp/Numeric/Vector.h"
#include "Coordinate.h"
#include <vector>
#include <unordered_map>
#include <cmath>


namespace OpenOasis
{
namespace CommImp
{
namespace Spatial
{
/// @brief The mesh nodes data structure.
/// (Point type).
struct Node
{
    Coordinate coor;  // unit(m).

    // Indexes of mesh faces shared node.
    std::vector<size_t> faceIndexes;

    // Indexes of mesh cells shared node.
    std::vector<size_t> cellIndexes;
};


/// @brief The mesh faces data structure.
/// Polyline type for 1D or 2D mesh,
/// Polygon type for 3D mesh.
struct Face
{
    Coordinate centroid;

    // Perimeter of the face in (m).
    real perimeter = NAN;

    // Area of the face in (m^2).
    real area = NAN;

    // Unit normal vector.
    Numeric::Vector<real> normal;

    // Indexes of mesh nodes on the face
    // sorted counterclockwise.
    std::vector<size_t> nodeIndexes;

    // Indexes of mesh cells shared face.
    std::vector<size_t> cellIndexes;

    // Orientation of mesh cells to face.
    // Same or opposite to normal.
    // Corresponds to cellIndexes.
    std::vector<int> cellOwnable;
};


/// @brief The mesh cells data structure.
/// Polygon type for 1D and 2D mesh,
/// Polyhedron type for 3D mesh.
struct Cell
{
    Coordinate centroid;

    // Surface area of the cell in (m^2).
    real surface = NAN;

    // Volume of the cell in (m^3).
    real volume = NAN;

    // Indexes of mesh faces on the cell.
    std::vector<size_t> faceIndexes;

    // Indexes of neighboring mesh cells.
    std::vector<size_t> neighbors;

    // Indexes of sub-cells
    // after current mesh cell's refined.
    std::vector<size_t> subCells;
};


/// @brief Mesh structure.
struct Mesh
{
    // Mesh nodes set.
    std::unordered_map<size_t, Node> nodes;

    // Mesh faces set.
    std::unordered_map<size_t, Face> faces;

    // Mesh cells set.
    std::unordered_map<size_t, Cell> cells;
};

}  // namespace Spatial
}  // namespace CommImp
}  // namespace OpenOasis
