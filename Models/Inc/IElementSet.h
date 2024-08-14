/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IElementSet.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide an ordered list of elements for data exchanging.
 *
 *    Data exchange between components is nearly always related to one or more elements
 *    in a space, either geo-referenced or not. An elementset can be a list of 2D or 3D
 *    spatial elements or as a special case, a list of ID based (non spatial) elements.
 *    Possible element types are defined in `ElementType`.
 *
 *    For 3D elements (i.e. polyhedron) the shape can be queried by face. When the
 *    elementset is geo-referenced, coordinates (X, Y, Z) can be obtained for each
 *    node of an element.
 *
 *    A geo-referenced elementset needs to have a valid "SpatialReferenceSystem"
 *    property set in a `ISpatialDefinition`. This is a string that specifies the OGC
 *    WKT representation of a spatial reference. An empty string indicates that there
 *    in no spatial reference, which is only valid if the `ElementType` is `IdBased`.
 *
 *    While an `IElementSet` can be used to query the geometric description of a
 *    model schematization, it does not necessarily provide all topological knowledge
 *    on inter-element connections.
 *
 ** ***********************************************************************************/
#pragma once
#include "ISpatialDefinition.h"
#include <vector>


namespace OpenOasis
{
/// @brief Shape Type of elements in an `IElementSet`.
enum class ElementType
{
    IdBased,    // Identifiable based
    Point,      // Points
    Polyline,   // Lines / Polylines
    Polygon,    // Polygons
    Polyhedron  // Polyhedrons
};


/// @brief An list of elements having a common type.
class IElementSet : public ISpatialDefinition
{
public:
    /// @brief Gets `ElementType` of the elementset.
    virtual ElementType GetElementType() const = 0;

    /// @brief Index of element with id 'elementId' in the element set.
    ///
    /// @param elementId Identification string of the element required.
    /// @return Index of the element with the specified id; return -1 if not found.
    virtual int GetElementIndex(const std::string &elementId) = 0;

    /// @brief Gets Id of the index element in the element set.
    ///
    /// @param index The element index for which the element `Id` is requested.
    ///
    /// @return The id of the element with the specified index.
    /// If the `ElementType` of the elementset is not `IdBased`,
    /// an empty string may be returned.
    ///
    /// @throw If the index outside the range, throw an exception.
    virtual std::string GetElementId(int index) = 0;

    /// @brief Gets the number of faces in a element.
    ///
    /// @param elementIndex Index for the element.
    /// @return Number of faces.
    /// @throw If the index outside the range, throw an exception.
    virtual int GetFaceCount(int elementIndex) = 0;

    /// @brief Gets number of nodes for the element specified by the index.
    ///
    /// @param elementIndex The element index for the element.
    ///
    /// @return Number of nodes in element defined by the elementIndex.
    ///
    /// @throw If the index outside the range, throw an exception.
    /// @throw If the method is invoked with type `IdBased`,
    /// throw an exception.
    virtual int GetNodeCount(int elementIndex) = 0;

    /// @brief Gets an array with the node indices for a face.
    ///
    /// @param elementIndex Element index.
    /// @param faceIndex Face index.
    /// @return The node indices for this face.
    virtual std::vector<int> GetFaceNodeIndices(int elementIndex, int faceIndex) = 0;

    /// @brief X coordinate for the node with nodeIndex of the element with
    /// elementIndex.
    ///
    /// @param elementIndex Element index.
    /// @param nodeIndex Node index in the element with index elementIndex.
    ///
    /// @return X coordinate for the node.
    virtual double GetNodeXCoordinate(int elementIndex, int nodeIndex) = 0;

    /// @brief Y coordinate for the node with nodeIndex of the element with
    /// elementIndex.
    ///
    /// @param elementIndex  Element index.
    /// @param nodeIndex  Node index in the element with index elementIndex.
    ///
    /// @return Y coordinate for the node.
    virtual double GetNodeYCoordinate(int elementIndex, int nodeIndex) = 0;

    /// @brief Z coordinate for the node with nodeIndex of the element with
    /// elementIndex.
    ///
    /// @param elementIndex Element index.
    /// @param nodeIndex Node index in the element with index elementIndex.
    ///
    /// @return Z coordinate for the node.
    virtual double GetNodeZCoordinate(int elementIndex, int nodeIndex) = 0;
};

}  // namespace OpenOasis
