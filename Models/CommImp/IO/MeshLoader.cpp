/** ***********************************************************************************
 *    @File      :  MeshLoader.cpp
 *    @Brief     :  Loading mesh data from files.
 *
 ** ***********************************************************************************/
#include "MeshLoader.h"
#include "Models/Utils/Exception.h"
#include "Models/Utils/FilePathHelper.h"
#include "Models/Utils/StringHelper.h"
#include "Models/Utils/CsvHandler.h"
#include <set>


namespace OpenOasis::CommImp::IO
{
using namespace std;
using namespace Utils;

MeshLoader::MeshLoader(const string &meshDir)
{
    if (!FilePathHelper::DirectoryExists(meshDir))
    {
        throw IllegalArgumentException(
            StringHelper::FormatSimple("Mesh directory [{}] does not exist.", meshDir));
    }

    mMeshDir = meshDir;
}

void MeshLoader::Load()
{
#pragma omp parallel sections
    {
#pragma omp section
        {
            LoadNodes();
        }
#pragma omp section
        {
            LoadFaces();
        }
#pragma omp section
        {
            LoadCells();
        }
#pragma omp section
        {
            LoadPatches();
        }
#pragma omp section
        {
            LoadZones();
        }
    }

    GenerateFaceCoordinates();
    GenerateCellCoordinates();
}

unordered_map<string, vector<int>> &MeshLoader::GetPatches()
{
    return mPatchFaces;
}

unordered_map<string, vector<int>> &MeshLoader::GetZones()
{
    return mZoneFaces;
}

unordered_map<int, Coordinate> &MeshLoader::GetNodeCoordinates()
{
    return mNodeCoords;
}

unordered_map<int, Coordinate> &MeshLoader::GetFaceCoordinates()
{
    return mFaceCoords;
}

unordered_map<int, Coordinate> &MeshLoader::GetCellCoordinates()
{
    return mCellCoords;
}

unordered_map<int, vector<int>> &MeshLoader::GetFaceNodes()
{
    return mFaceNodes;
}

unordered_map<int, vector<int>> &MeshLoader::GetCellFaces()
{
    return mCellFaces;
}

void MeshLoader::LoadNodes(const string &nodeFile)
{
    const auto &file = FilePathHelper::Combine(mMeshDir, nodeFile);
    if (!FilePathHelper::FileExists(file))
        return;

    CsvLoader loader(file, true, true);
    if (loader.GetColumnCount() < 3)
    {
        throw InvalidDataException("Invalid Node data, to few columns.");
    }

    auto ids = loader.GetRowLabels().value();
    CheckIds(ids, "Node");

    for (std::size_t i = 0; i < ids.size(); ++i)
    {
        auto coor = loader.GetRow<real>(i).value();

        mNodeCoords[i] = {coor[0], coor[1], coor[2]};
    }
}

void MeshLoader::LoadFaces(const string &faceFile)
{
    const auto &file = FilePathHelper::Combine(mMeshDir, faceFile);
    if (!FilePathHelper::FileExists(file))
        return;

    CsvLoader loader(file, true, true);
    if (loader.GetColumnCount() < 2)
    {
        throw InvalidDataException("Invalid Face data, to few columns.");
    }

    auto ids = loader.GetRowLabels().value();
    CheckIds(ids, "Face");

    for (std::size_t i = 0; i < ids.size(); ++i)
    {
        mFaceNodes[i] = loader.GetRow<int>(i).value();
    }
}

void MeshLoader::LoadCells(const string &cellFile)
{
    const auto &file = FilePathHelper::Combine(mMeshDir, cellFile);
    if (!FilePathHelper::FileExists(file))
        return;

    CsvLoader loader(file, true, true);
    if (loader.GetColumnCount() < 3)
    {
        throw InvalidDataException("Invalid Cell data, to few columns.");
    }

    auto ids = loader.GetRowLabels().value();
    CheckIds(ids, "Cell");

    for (std::size_t i = 0; i < ids.size(); ++i)
    {
        mCellFaces[i] = loader.GetRow<int>(i).value();
    }
}

void MeshLoader::LoadPatches(const string &patchFile)
{
    const auto &file = FilePathHelper::Combine(mMeshDir, patchFile);
    if (!FilePathHelper::FileExists(file))
        return;

    CsvLoader loader(file, false, true);
    if (loader.GetColumnCount() < 1)
    {
        throw InvalidDataException("Invalid Patch data, to few columns.");
    }

    auto ids = loader.GetRowLabels().value();

    for (const auto &id : ids)
    {
        mPatchFaces[id] = loader.GetRow<int>(id).value();
    }
}

void MeshLoader::LoadZones(const string &zoneFile)
{
    const auto &file = FilePathHelper::Combine(mMeshDir, zoneFile);
    if (!FilePathHelper::FileExists(file))
        return;

    CsvLoader loader(file, false, true);
    if (loader.GetColumnCount() < 3)
    {
        throw InvalidDataException("Invalid Zone data, to few columns.");
    }

    auto ids = loader.GetRowLabels().value();

    for (const auto &id : ids)
    {
        mZoneFaces[id] = loader.GetRow<int>(id).value();
    }
}

void MeshLoader::CheckIds(const vector<string> &ids, const string &meta)
{
    vector<int> ids_int(ids.size());
    transform(
        begin(ids), end(ids), begin(ids_int), [](const auto &id) { return stoi(id); });

    if (ids_int.front() != 0)
    {
        throw InvalidDataException(StringHelper::FormatSimple(
            "Invalid [{}] data, ids don't start from 0.", meta));
    }

    if (ids_int.back() != int(ids_int.size()) - 1)
    {
        throw InvalidDataException(StringHelper::FormatSimple(
            "Invalid [{}] data, non-incremental ids.", meta));
    }
}

void MeshLoader::GenerateFaceCoordinates()
{
    for (const auto &face : mFaceNodes)
    {
        int id = face.first;

        real x = 0, y = 0, z = 0;
        for (int nId : face.second)
        {
            x += mNodeCoords[nId].x;
            y += mNodeCoords[nId].y;
            z += mNodeCoords[nId].z;
        }

        int size = (int)face.second.size();
        x /= size;
        y /= size;
        z /= size;

        mFaceCoords[id] = {x, y, z};
    }
}

void MeshLoader::GenerateCellCoordinates()
{
    for (const auto &cell : mCellFaces)
    {
        int id = cell.first;

        real x = 0, y = 0, z = 0;
        for (int fId : cell.second)
        {
            x += mFaceCoords[fId].x;
            y += mFaceCoords[fId].y;
            z += mFaceCoords[fId].z;
        }

        int size = (int)cell.second.size();
        x /= size;
        y /= size;
        z /= size;

        mCellCoords[id] = {x, y, z};
    }
}

}  // namespace OpenOasis::CommImp::IO