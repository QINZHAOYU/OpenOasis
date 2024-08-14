/** ***********************************************************************************
 *    Copyright (C) 2023, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  MeshLoader.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Loading mesh data from files.
 *    The default mesh loader predefined a fixed set of csv files for storing mesh data,
 *    including  :
 *
 *    nodes.csv, used for storing node coordinates data, formated as :
 *               "nId, x, y, z"
 *    faces.csv, used for storing face nodes index data, formated as :
 *               "fId, nId1, nId2"
 *    cells.csv, used for storing cell faces index data, formated as :
 *               "cId, fId1, fId2, fId3, ..."
 *
 *    And more   :
 *
 *    zones.csv, used for storing zone faces index data, formated as :
 *               "zId, fId1, fId2, fId3, ..."
 *    patches.csv, used for storing patch faces indexes, formated as :
 *               "pId, fId1, fId2, fId3, ..."
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/CommImp/Spatial/Coordinate.h"
#include <string>
#include <vector>
#include <unordered_map>


namespace OpenOasis::CommImp::IO
{
using Spatial::Coordinate;

/// @brief Default `Mesh` data loader.
/// @details In default mode, coordinates are defined on nodes.
class MeshLoader
{
private:
    std::string mMeshDir;

    std::unordered_map<std::string, std::vector<int>> mPatchFaces;
    std::unordered_map<std::string, std::vector<int>> mZoneFaces;

    std::unordered_map<int, Coordinate> mNodeCoords;
    std::unordered_map<int, Coordinate> mFaceCoords;
    std::unordered_map<int, Coordinate> mCellCoords;

    std::unordered_map<int, std::vector<int>> mFaceNodes;
    std::unordered_map<int, std::vector<int>> mCellFaces;

public:
    MeshLoader(const std::string &meshDir);
    ~MeshLoader()
    {}

    virtual void Load();

    virtual std::unordered_map<std::string, std::vector<int>> &GetPatches();
    virtual std::unordered_map<std::string, std::vector<int>> &GetZones();

    virtual std::unordered_map<int, Coordinate> &GetNodeCoordinates();
    virtual std::unordered_map<int, Coordinate> &GetFaceCoordinates();
    virtual std::unordered_map<int, Coordinate> &GetCellCoordinates();

    virtual std::unordered_map<int, std::vector<int>> &GetFaceNodes();
    virtual std::unordered_map<int, std::vector<int>> &GetCellFaces();

protected:
    void LoadNodes(const std::string &file = "nodes.csv");
    void LoadFaces(const std::string &file = "faces.csv");
    void LoadCells(const std::string &file = "cells.csv");
    void LoadZones(const std::string &file = "zones.csv");
    void LoadPatches(const std::string &file = "patches.csv");

    void CheckIds(const std::vector<std::string> &ids, const std::string &meta);

    void GenerateFaceCoordinates();
    void GenerateCellCoordinates();
};
}  // namespace OpenOasis::CommImp::IO
