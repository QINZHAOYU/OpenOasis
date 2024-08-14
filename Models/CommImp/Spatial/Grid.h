/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Grid.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Grid used for numerical calculation.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/CommImp/Numeric/Vector.h"
#include "Models/Utils/EventHandler.h"
#include "Mesh.h"
#include <string>


namespace OpenOasis::CommImp::Spatial
{
using namespace Utils;
using namespace Numeric;


/// @brief Grid encapsulate the mesh data for various numerical calculations.
/// @note The Grid garuntees the mesh indexes is valid and continuous.
class Grid : public std::enable_shared_from_this<Grid>
{
public:
    using ListenFunc = std::function<void(const std::shared_ptr<Grid> &)>;

protected:
    /// A `mVersion` event is fired when version of this grid changes.
    EventHandler<void, const std::shared_ptr<Grid> &> mVersionListeners;

    int  mVersion = 0;
    Mesh mMesh;

public:
    virtual ~Grid() = default;
    Grid(const Mesh &mesh);
    Grid(Mesh &&mesh);
    Grid(
        const std::unordered_map<size_t, Coordinate>               &nodeCoords,
        const std::unordered_map<size_t, Coordinate>               &faceCoords,
        const std::unordered_map<size_t, Coordinate>               &cellCoords,
        const std::unordered_map<size_t, std::vector<size_t>>      &faceNodes,
        const std::unordered_map<size_t, std::vector<size_t>>      &cellFaces,
        const std::unordered_map<std::string, std::vector<size_t>> &patchFaces = {},
        const std::unordered_map<std::string, std::vector<size_t>> &zoneCells  = {},
        int                                                         version    = 0);


    ///////////////////////////////////////////////////////////////////////////////////
    // Methods used for mesh activation and manipulations.
    //

    /// @brief Extract topology and geometry data.
    virtual void Activate();

    /// @brief Refine the mesh cell of a given index @p cellIndex for adaptive mesh.
    virtual void RefineCell(size_t cellIndex);

    /// @brief Relax the mesh cell of a given index @p cellIndex .
    virtual void RelaxCell(size_t cellIndex);

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods used for mesh element access and query.
    //

    std::vector<size_t> GetPatchFaces(const std::string &patchId) const;
    std::vector<size_t> GetZoneCells(const std::string &zoneId) const;

    std::vector<size_t> GetBoundaryCells() const;
    std::vector<size_t> GetBoundaryFaces() const;

    int GetVersion() const;

    size_t GetNumCells() const;
    size_t GetNumFaces() const;
    size_t GetNumNodes() const;

    const Cell &GetCell(size_t cellIndex) const;
    const Face &GetFace(size_t faceIndex) const;
    const Node &GetNode(size_t nodeIndex) const;
    const Mesh &GetMesh() const;

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods used for mesh topological analysis.
    //

    /// @brief  Get the distance between centers of a pair of cell and face.
    real GetCellToFaceDist(size_t cellIndex, size_t faceIndex) const;

    /// @brief Get the distance between centers of two cells.
    real GetCellToCellDist(size_t cIndex1, size_t cIndex2) const;

    /// @brief Get the cell vector.
    Vector<real> GetCellToCellVec(size_t cIndex1, size_t cIndex2) const;

    ///////////////////////////////////////////////////////////////////////////////////
    // Utility methods.
    //

    void AppendListener(const ListenFunc &func);
    void RemoveListener(const ListenFunc &func);

protected:
    void SetVerionTo(int version);

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods used for activating mesh data.
    //

    virtual void CollectCellsSharedNode();
    virtual void CollectFacesSharedNode();
    virtual void CollectCellsSharedFace();
    virtual void CollectCellNeighbors();
    virtual void CollectFaceCellSides();
    virtual void CollectPatchFaces();
    virtual void CollectZoneCells();
    virtual void CollectBoundaryCells();
    virtual void CollectBoundaryFaces();

    virtual void SortNodes();
    virtual void CheckMesh();
    virtual void CheckPatch();
    virtual void CheckZone();

    virtual void CalculateFaceNormal();
    virtual void CalculateFaceArea();
    virtual void CalculateFacePerimeter();
    virtual void CalculateCellSurface();
    virtual void CalculateCellVolume();

    virtual void CalculateCellToCellDist();
    virtual void CalculateCellToFaceDist();
};

}  // namespace OpenOasis::CommImp::Spatial