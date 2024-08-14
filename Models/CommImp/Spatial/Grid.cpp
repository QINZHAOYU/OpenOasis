/** ***********************************************************************************
 *    @File      :  Grid.cpp
 *    @Brief     :  Grid used for numerical calculation.
 *
 ** ***********************************************************************************/
#include "Grid.h"
#include "MeshCalculator.h"


namespace OpenOasis::CommImp::Spatial
{
using namespace Utils;
using namespace std;


// ------------------------------------------------------------------------------------

Grid::Grid(
    const unordered_map<size_t, Coordinate>     &nodeCoords,
    const unordered_map<size_t, Coordinate>     &faceCoords,
    const unordered_map<size_t, Coordinate>     &cellCoords,
    const unordered_map<size_t, vector<size_t>> &faceNodes,
    const unordered_map<size_t, vector<size_t>> &cellFaces)
{
#pragma omp parallel sections
    {
#pragma omp section
        {
            for (const auto &node : nodeCoords)
            {
                mMesh.nodes[node.first]      = {};
                mMesh.nodes[node.first].coor = node.second;
            }
        }
#pragma omp section
        {
            for (const auto &face : faceCoords)
            {
                size_t i                   = face.first;
                mMesh.faces[i]             = {};
                mMesh.faces[i].centroid    = face.second;
                mMesh.faces[i].nodeIndexes = faceNodes.at(i);
            }
        }
#pragma omp section
        {
            for (const auto &cell : cellCoords)
            {
                size_t i                   = cell.first;
                mMesh.cells[i]             = {};
                mMesh.cells[i].centroid    = cellCoords.at(i);
                mMesh.cells[i].faceIndexes = cellFaces.at(i);
            }
        }
    }
}

Grid::Grid(const Mesh &mesh) : mMesh(mesh)
{}

Grid::Grid(Mesh &&mesh) : mMesh(move(mesh))
{}

int Grid::GetVersion() const
{
    return mVersion;
}

const Mesh &Grid::GetMesh() const
{
    return mMesh;
}

void Grid::RefineCell(size_t cellIndex)
{
    // TODO: Implement refine cell.
}

void Grid::RelaxCell(size_t cellIndex)
{
    // TODO: Implement relax cell.
}

void Grid::Activate()
{
    // Complete mesh topological connections.
    CollectCellsSharedNode();
    CollectFacesSharedNode();
    CollectCellsSharedFace();
    CollectCellNeighbors();
    CollectFaceCellSides();

    // Sort node counterclockwise.
    SortNodes();

    // Activate mesh face structures orderly.
    CalculateFaceNormal();
    CalculateFaceArea();
    CalculateFacePerimeter();

    // Activate mesh cell structures.
    CalculateCellSurface();
    CalculateCellVolume();

    // Check mesh validation.
    CheckMesh();
}

void Grid::CollectCellsSharedNode()
{
#pragma omp parallel for schedule(dynamic)
    for (auto nIdx = 0; nIdx < GetNumNodes(); nIdx++)
    {
        mMesh.nodes[nIdx].cellIndexes.clear();
    }

    for (auto cIdx = 0; cIdx < GetNumCells(); cIdx++)
    {
        const auto &nodeIdxs = MeshCalculator::GetCellNodeIndexes(cIdx, mMesh);
        for (auto nIdx : nodeIdxs)
            mMesh.nodes[nIdx].cellIndexes.push_back(cIdx);
    }
}

void Grid::CollectFacesSharedNode()
{
#pragma omp parallel for schedule(dynamic)
    for (auto nIdx = 0; nIdx < GetNumNodes(); nIdx++)
    {
        mMesh.nodes[nIdx].faceIndexes.clear();
    }

    for (auto fIdx = 0; fIdx < GetNumFaces(); fIdx++)
    {
        const auto &nodeIndexes = mMesh.faces[fIdx].nodeIndexes;
        for (auto nIdx : nodeIndexes)
        {
            mMesh.nodes[nIdx].faceIndexes.push_back(fIdx);
        }
    }
}

void Grid::CollectCellsSharedFace()
{
#pragma omp parallel for schedule(dynamic)
    for (auto fIdx = 0; fIdx < GetNumFaces(); fIdx++)
    {
        mMesh.faces[fIdx].cellIndexes.clear();
    }

    for (auto cIdx = 0; cIdx < GetNumCells(); cIdx++)
    {
        const auto &faceIndexes = mMesh.cells[cIdx].faceIndexes;
        for (auto fIdx : faceIndexes)
        {
            mMesh.faces[fIdx].cellIndexes.push_back(cIdx);
        }
    }
}

void Grid::CollectCellNeighbors()
{
#pragma omp parallel for schedule(dynamic)
    for (auto cIdx = 0; cIdx < GetNumCells(); cIdx++)
    {
        mMesh.cells[cIdx].neighbors.clear();
    }

    for (auto fIdx = 0; fIdx < GetNumFaces(); fIdx++)
    {
        const auto &cellIndexes = mMesh.faces[fIdx].cellIndexes;
        if (cellIndexes.size() != 2)
            continue;

        auto c1 = cellIndexes[0], c2 = cellIndexes[1];
        mMesh.cells[c1].neighbors.push_back(c2);
        mMesh.cells[c2].neighbors.push_back(c1);
    }
}

void Grid::SortNodes()
{
#pragma omp parallel for schedule(dynamic)
    for (auto fIdx = 0; fIdx < GetNumFaces(); fIdx++)
    {
        const auto &nodeIndexes = MeshCalculator::SortFaceNodes(fIdx, mMesh);

        mMesh.faces[fIdx].nodeIndexes = nodeIndexes;
    }
}

void Grid::CollectFaceCellSides()
{
#pragma omp parallel for schedule(dynamic)
    for (auto fIdx = 0; fIdx < GetNumFaces(); fIdx++)
    {
        auto &face = mMesh.faces[fIdx];
        face.cellOwnable.clear();

        const auto &fPoint = face.centroid;
        const auto &cPoint = mMesh.cells[face.cellIndexes[0]].centroid;

        auto vec = MeshCalculator::ToVector(Node{fPoint}, Node{cPoint});
        auto res = vec * face.normal;
        auto dir = (res < 0) ? 1 : -1;

        face.cellOwnable.push_back(dir);
        if (face.cellIndexes.size() == 2)
        {
            face.cellOwnable.push_back(-dir);
        }
    }
}

void Grid::CalculateFaceNormal()
{
#pragma omp parallel for schedule(dynamic)
    for (auto fIdx = 0; fIdx < GetNumFaces(); fIdx++)
    {
        auto normal = MeshCalculator::CalculateFaceNormal(fIdx, mMesh);

        mMesh.faces[fIdx].normal = normal;
    }
}

void Grid::CalculateFaceArea()
{
#pragma omp parallel for schedule(dynamic)
    for (auto fIdx = 0; fIdx < GetNumFaces(); fIdx++)
    {
        auto area = MeshCalculator::CalculateFaceArea(fIdx, mMesh);

        mMesh.faces[fIdx].area = area;
    }
}

void Grid::CalculateFacePerimeter()
{
#pragma omp parallel for schedule(dynamic)
    for (auto fIdx = 0; fIdx < GetNumFaces(); fIdx++)
    {
        auto perimeter = MeshCalculator::CalculateFacePerimeter(fIdx, mMesh);

        mMesh.faces[fIdx].perimeter = perimeter;
    }
}

void Grid::CalculateCellSurface()
{
#pragma omp parallel for schedule(dynamic)
    for (auto cIdx = 0; cIdx < GetNumCells(); cIdx++)
    {
        auto surface = MeshCalculator::CalculateCellSurfaceArea(cIdx, mMesh);

        mMesh.cells[cIdx].surface = surface;
    }
}

void Grid::CalculateCellVolume()
{
#pragma omp parallel for schedule(dynamic)
    for (auto cIdx = 0; cIdx < GetNumCells(); cIdx++)
    {
        auto volume = MeshCalculator::CalculateCellVolume(cIdx, mMesh);

        mMesh.cells[cIdx].volume = volume;
    }
}

void Grid::CheckMesh()
{}

size_t Grid::GetNumCells() const
{
    return mMesh.cells.size();
}

size_t Grid::GetNumFaces() const
{
    return mMesh.faces.size();
}

size_t Grid::GetNumNodes() const
{
    return mMesh.nodes.size();
}

const Cell &Grid::GetCell(size_t cellIndex) const
{
    return mMesh.cells.at(cellIndex);
}

const Face &Grid::GetFace(size_t faceIndex) const
{
    return mMesh.faces.at(faceIndex);
}

const Node &Grid::GetNode(size_t nodeIndex) const
{
    return mMesh.nodes.at(nodeIndex);
}

}  // namespace OpenOasis::CommImp::Spatial