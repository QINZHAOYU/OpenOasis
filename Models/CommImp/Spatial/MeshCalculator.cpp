/** ***********************************************************************************
 *    @File      :  MeshCalculator.cpp
 *    @Brief     :  Collections of mesh geometry functions.
 *
 ** ***********************************************************************************/
#include "MeshCalculator.h"
#include "Models/Utils/Exception.h"
#include <set>
#include <algorithm>


namespace OpenOasis::CommImp::Spatial
{
using namespace Utils;
using namespace std;


Vector<real> MeshCalculator::ToVector(const Node &beg, const Node &end, int foldedAxis)
{
    const auto &n0 = beg.coor;
    const auto &n1 = end.coor;

    Vector<real> vec = {n1.x - n0.x, n1.y - n0.y, n1.z - n0.z};
    if (foldedAxis > -1 && foldedAxis < 3)
    {
        vec.SetAt(foldedAxis, 0.0);
    }

    return vec;
}

real MeshCalculator::CalculateNodesDistance(const Node &node0, const Node &node1)
{
    const auto &vec = ToVector(node0, node1);
    return vec.Magnitude();
}

vector<size_t> MeshCalculator::GetCellNodeIndexes(size_t cellIdx, const Mesh &mesh)
{
    set<size_t> nodeIdxs;

    for (size_t faceIdx : mesh.cells.at(cellIdx).faceIndexes)
    {
        for (size_t nodeIdx : mesh.faces.at(faceIdx).nodeIndexes)
        {
            nodeIdxs.emplace(nodeIdx);
        }
    }

    vector<size_t> idxs(nodeIdxs.begin(), nodeIdxs.end());
    return idxs;
}

vector<size_t> MeshCalculator::SortFaceNodes(size_t faceIdx, const Mesh &mesh)
{
    const auto    &nodeIndexes = mesh.faces.at(faceIdx).nodeIndexes;
    vector<size_t> sortIndexes(nodeIndexes);

    auto center = CalculateFaceCentroid(faceIdx, mesh);
    Node origin = {center};

    vector<Vector<real>> nodeVecs;
    for (size_t idx : nodeIndexes)
    {
        nodeVecs.emplace_back(ToVector(origin, mesh.nodes.at(idx)));
    }

    Vector<real> normal = mesh.faces.at(faceIdx).normal;
    if (normal.IsZero())
        normal = CalculateFaceNormal(faceIdx, mesh);

    for (size_t i = 0; i < sortIndexes.size() - 1; i++)
    {
        for (size_t j = i + 1; j < sortIndexes.size(); j++)
        {
            Vector<real> cross = nodeVecs[i] & nodeVecs[j];
            if (cross * normal > 0)
            {
                swap(sortIndexes[i], sortIndexes[j]);
            }
        }
    }

    return sortIndexes;
}

Coordinate MeshCalculator::CalculateFaceCentroid(size_t faceIdx, const Mesh &mesh)
{
    return CalculateCentroid(mesh.faces.at(faceIdx).nodeIndexes, mesh.nodes);
}

Coordinate MeshCalculator::CalculateCellCentroid(size_t cellIdx, const Mesh &mesh)
{
    return CalculateCentroid(GetCellNodeIndexes(cellIdx, mesh), mesh.nodes);
}

vector<size_t> MeshCalculator::CollectBoundaryFaceIndexes(const Mesh &mesh)
{
    vector<size_t> indexes;
    for (const auto &face : mesh.faces)
    {
        if (face.second.cellIndexes.size() == 1)
            indexes.push_back(face.first);
    }

    return indexes;
}

vector<size_t> MeshCalculator::CollectBoundaryCellIndexes(const Mesh &mesh)
{
    vector<size_t> indexes;
    for (const auto &cell : mesh.cells)
    {
        auto faceCount = cell.second.faceIndexes.size();
        auto neighbors = cell.second.neighbors.size();

        if (faceCount > neighbors)
            indexes.push_back(cell.first);
    }

    return indexes;
}

vector<size_t> MeshCalculator::CollectBoundaryNodeIndexes(const Mesh &mesh)
{
    vector<size_t> indexes;

    set<size_t> nodeIdxs;
    for (auto &fIdx : CollectBoundaryFaceIndexes(mesh))
    {
        for (auto &nIdx : mesh.faces.at(fIdx).nodeIndexes)
            nodeIdxs.insert(nIdx);
    }

    indexes.assign(nodeIdxs.begin(), nodeIdxs.end());
    return indexes;
}

vector<size_t> MeshCalculator::CollectBlockCellIndexes(
    const Mesh &mesh, const vector<size_t> &blockFaces)
{
    throw runtime_error("Not implemented yet.");
}

Vector<real> MeshCalculator::CalculateFaceNormal(size_t faceIdx, const Mesh &mesh)
{
    const auto &nodeIdxs = mesh.faces.at(faceIdx).nodeIndexes;
    const auto &nodes    = mesh.nodes;

    // 2D mesh, calculate the normal vector on the xy plane.
    if (Is2DMesh(mesh))
    {
        auto vec = ToVector(nodes.at(nodeIdxs[0]), nodes.at(nodeIdxs[1]), 2);
        auto n   = Vector<real>{-vec(1), vec(0), 0.0};
        n.Normalize();

        return n;
    }
    // 3D mesh, calculate the normal vector of the face.
    else
    {
        const auto &v1 = ToVector(nodes.at(nodeIdxs[1]), nodes.at(nodeIdxs[0]));
        const auto &v2 = ToVector(nodes.at(nodeIdxs[1]), nodes.at(nodeIdxs[2]));
        auto        n  = v1 & v2;
        n.Normalize();

        return n;
    }
}

real MeshCalculator::CalculateFaceArea(size_t faceIdx, const Mesh &mesh)
{
    const auto &nodeIdxs = mesh.faces.at(faceIdx).nodeIndexes;
    const auto &nodes    = mesh.nodes;
    size_t      size     = nodeIdxs.size();

    // 2D mesh.
    if (Is2DMesh(mesh))
    {
        real area = mesh.faces.at(faceIdx).perimeter;
        if (isnan(area))
            area = CalculateFacePerimeter(faceIdx, mesh);

        return area;
    }
    // 3D mesh.
    else
    {
        // calculate face area by Shoelace Theorem in 3d.
        // area = 1/2 * {
        //    cos(n, z) * sum(x_i * y_i+1 - x_i+1 * y_i) +
        //    cos(n, x) * sum(y_i * z_i+1 - y_i+1 * z_i) +
        //    cos(n, y) * sum(z_i * x_i+1 - z_i+1 * x_i)
        // }

        Vector<real> normal = mesh.faces.at(faceIdx).normal;
        if (normal.IsZero())
            normal = CalculateFaceNormal(faceIdx, mesh);

        real area = 0.0;
        for (size_t i = 0; i < size - 1; ++i)
        {
            const auto &c0 = nodes.at(nodeIdxs[i]).coor;
            const auto &c1 = nodes.at(nodeIdxs[i + 1]).coor;

            area += normal(2) * (c0.x * c1.y - c0.y * c1.x);
            area += normal(0) * (c0.y * c1.z - c0.z * c1.y);
            area += normal(1) * (c0.z * c1.x - c0.x * c1.z);
        }

        return abs(area) / 2.0;
    }
}

real MeshCalculator::CalculateFacePerimeter(size_t faceIdx, const Mesh &mesh)
{
    const auto &nodeIdxs = mesh.faces.at(faceIdx).nodeIndexes;
    const auto &nodes    = mesh.nodes;
    size_t      n        = nodeIdxs.size();

    double len = 0;
    for (size_t i = 0; i < n - 1; i++)
    {
        len += CalculateNodesDistance(nodes.at(nodeIdxs[i]), nodes.at(nodeIdxs[i + 1]));
    }

    if (!Is2DMesh(mesh))
    {
        len += CalculateNodesDistance(nodes.at(nodeIdxs[n - 1]), nodes.at(nodeIdxs[0]));
    }

    return len;
}

real MeshCalculator::CalculateCellVolume(size_t cellIdx, const Mesh &mesh)
{
    const auto &nodeIdxs = GetCellNodeIndexes(cellIdx, mesh);
    const auto &nodes    = mesh.nodes;

    // 2D mesh cell.
    if (Is2DMesh(mesh))
    {
        real vol = mesh.cells.at(cellIdx).surface;
        if (isnan(vol))
            vol = CalculateCellSurfaceArea(cellIdx, mesh);

        return vol;
    }
    // tetrahedral cell.
    else if (nodeIdxs.size() == 4)
    {
        const auto &v1 = ToVector(nodes.at(nodeIdxs[0]), nodes.at(nodeIdxs[1]));
        const auto &v2 = ToVector(nodes.at(nodeIdxs[0]), nodes.at(nodeIdxs[2]));
        const auto &v3 = ToVector(nodes.at(nodeIdxs[0]), nodes.at(nodeIdxs[3]));

        real vol = ((v1 & v2) * v3) / 6.0;
        return vol;
    }
    else
    {
        throw NotImplementedException();
    }
}

real MeshCalculator::CalculateCellSurfaceArea(size_t cellIdx, const Mesh &mesh)
{
    real area = 0.0;

    for (size_t faceIdx : mesh.cells.at(cellIdx).faceIndexes)
    {
        real val = mesh.faces.at(faceIdx).area;
        if (isnan(val))
            val = CalculateFaceArea(faceIdx, mesh);

        area += val;
    }

    return area;
}

Coordinate MeshCalculator::CalculateCentroid(
    const vector<size_t> &nodeIdxs, const unordered_map<size_t, Node> &nodes)
{
    if (nodeIdxs.empty())
        return {};

    real sumX = 0., sumY = 0., sumZ = 0.;
    for (int idx : nodeIdxs)
    {
        sumX += nodes.at(idx).coor.x;
        sumY += nodes.at(idx).coor.y;
        sumZ += nodes.at(idx).coor.z;
    }

    int num = int(nodeIdxs.size());
    return {sumX / num, sumY / num, sumZ / num};
}

bool MeshCalculator::Is2DMesh(const Mesh &mesh)
{
    for (const auto &face : mesh.faces)
    {
        if (face.second.nodeIndexes.size() == 2)
        {
            return true;
        }
    }

    return false;
}

int MeshCalculator::ChooseFoldedAxis(size_t faceIdx, const Mesh &mesh)
{
    const auto &face = mesh.faces.at(faceIdx);

    auto normal = face.normal;
    if (normal.IsZero())
        normal = CalculateFaceNormal(faceIdx, mesh);

    return normal.AbsMinIndex();
}

}  // namespace OpenOasis::CommImp::Spatial