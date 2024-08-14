/** ***********************************************************************************
 *    Copyright (C) 2024, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  MeshCalculator.h
 *    @License   :  Apache-2.0
 *
 ** ***********************************************************************************/
#pragma once
#include "Mesh.h"


namespace OpenOasis::CommImp::Spatial
{
using namespace Numeric;

/// @brief Collections of mesh geometry functions.
class MeshCalculator final
{
public:
    static constexpr real EPSILON = 1.0e-6;

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for mesh node geometry calculations.
    //

    static Vector<real> ToVector(const Node &beg, const Node &end, int foldedAxis = -1);

    static real CalculateNodesDistance(const Node &node0, const Node &node1);

    static std::vector<size_t> CollectBoundaryNodeIndexes(const Mesh &mesh);


    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for mesh face geometry calculations.
    //

    /// @brief Sorts the nodes of a face in counterclockwise order.
    static std::vector<size_t> SortFaceNodes(size_t faceIdx, const Mesh &mesh);

    static std::vector<size_t> CollectBoundaryFaceIndexes(const Mesh &mesh);

    static Coordinate CalculateFaceCentroid(size_t faceIdx, const Mesh &mesh);

    static Vector<real> CalculateFaceNormal(size_t faceIdx, const Mesh &mesh);

    /// @brief Calculates the area of a face(for 2d mesh, the perimeter).
    static real CalculateFaceArea(size_t faceIdx, const Mesh &mesh);

    static real CalculateFacePerimeter(size_t faceIdx, const Mesh &mesh);

    static int ChooseFoldedAxis(size_t faceIdx, const Mesh &mesh);


    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for mesh cell geometry calculations.
    //

    static std::vector<size_t> GetCellNodeIndexes(size_t cellIdx, const Mesh &mesh);

    static std::vector<size_t>
    CollectBlockCellIndexes(const Mesh &mesh, const std::vector<size_t> &blockFaces);

    static Coordinate CalculateCellCentroid(size_t cellIdx, const Mesh &mesh);

    /// @brief Calculates the volume of a cell(for 2d mesh, the surface area).
    static real CalculateCellVolume(size_t cellIdx, const Mesh &mesh);

    static real CalculateCellSurfaceArea(size_t cellIdx, const Mesh &mesh);

    static std::vector<size_t> CollectBoundaryCellIndexes(const Mesh &mesh);


private:
    static Coordinate CalculateCentroid(
        const std::vector<size_t>              &nodeIdxs,
        const std::unordered_map<size_t, Node> &nodes);

    static bool Is2DMesh(const Mesh &mesh);
};

}  // namespace OpenOasis::CommImp::Spatial