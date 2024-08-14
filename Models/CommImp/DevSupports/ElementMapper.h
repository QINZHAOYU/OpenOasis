/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ElementMapper.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To convert one ValueSet (inputValues) associated one ElementSet
 *    (fromElements) to a new ValueSet that corresponds to another ElementSet
 *    (toElements).
 *
 *    The conversion is a two step procedure where the first step(Initialize) is
 *    executed at initialisation time only, whereas the MapValues
 *    is executed during time stepping.
 *
 *    The Initialize method will create a conversion matrix with the same number of
 *    rows as the number of elements in the ElementSet associated to the accepting
 *    component (i.e. the toElements) and the same number of columns as the
 *    number of elements in the ElementSet associated to the providing component
 *    (i.e. the fromElements).
 *
 *    Mapping is possible for any zero-, one- and two-dimensional elemets.
 *    Zero dimensional elements will always be points,one-dimensional elements will
 *    allways be polylines and two-dimensional elements will allways be polygons.
 *
 *    The ElementMapper contains lots of methods for mapping between the different
 *    element types. As an example, polyline to polygon mapping may be done either
 *    as Weighted Mean or as Weighted Sum. Typically the method choice will
 *    depend on the quantity mapped.
 *
 *    Such that state variables such as water level will be mapped using
 *    Weighted Mean whereas flux variable such as seepage from river to groundwater
 *    will be mapped using Weighted Sum.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IIdentifiable.h"
#include "Models/Inc/IElementSet.h"
#include "Models/Inc/IValueSet.h"
#include "Models/CommImp/Numeric/Matrix.h"
#include "Models/CommImp/Spatial/Geom.h"
#include <optional>


namespace OpenOasis::CommImp::DevSupports
{
using namespace Spatial;
using namespace Numeric;


/// @brief Predined element mapping methods.
enum class ElementMapperMethod
{
    None,
    Nearest,
    Inverse,
    Mean,
    Sum,
    WeightedMean,
    WeightedSum,
    Distribute,
    Value
};


/// @brief ElementMapper class converts one ValueSet(inputValues) associated one
/// ElementSet(fromElements) to a new ValueSet that corresponds to another
/// ElementSet(toElements).
/// @note  Coupling to 3d elements is not currently supported.
class ElementMapper
{
private:
    std::optional<ElementMapperMethod>  mMethod;
    std::shared_ptr<DoubleSparseMatrix> mMappingMatrix;

    bool mUseSearchTree       = false;
    bool mIsInitialised       = false;
    int  mNumberOfFromColumns = 0;
    int  mNumberOfToRows      = 0;

public:
    virtual ~ElementMapper()
    {}

    ElementMapper();

    std::shared_ptr<DoubleSparseMatrix> GetMappingMatrix() const;

    void SetUseSearchTree(bool value);

    bool IsInitialized() const;

    bool GetUseSearchTree() const;

    ElementType GetTargetElementType();

    /// @brief Calculates for each set of timestep data.
    /// A resulting IValueSet through multiplication of an IValueSet(inputValues) vector
    /// with the mapping matrix.
    ///
    /// @param inputValues IValueSet of values to be mapped.
    /// @return A valueset found by mapping of the inputValues on to the toElementSet.
    std::shared_ptr<IValueSet> MapValues(const std::shared_ptr<IValueSet> &inputValues);

    /// @brief Initialises the ElementMapper.
    /// The initialisation includes setting the isInitialised flag and calls
    /// UpdateMappingMatrix for calculation of the mapping matrix.
    ///
    /// @param method String description of mapping method.
    /// @param fromElements The IElementSet to map from.
    /// @param toElements The IElementSet to map to.
    void Initialise(
        const std::shared_ptr<IIdentifiable> &method,
        const std::shared_ptr<IElementSet>   &fromElements,
        const std::shared_ptr<IElementSet>   &toElements);

    /// @brief Creates a result valueset of the size specified.
    /// @returns An IValueSet of the correct size.
    static std::shared_ptr<IValueSet>
    CreateResultValueSet(int numtimes, int numElements);

    /// @brief Calculates for each set of timestep data.
    /// A resulting IValueSet through multiplication of an IValueSet(inputValues) vector
    /// with the mapping matrix.
    ///
    /// This version can be used if the output valueset is to be reused,e.g.
    /// (performance or for adding up)
    ///
    /// @param outputValues IValueset of mapped values, of the correct size.
    /// @param inputValues IValueSet of values to be mapped.
    void MapValues(
        const std::shared_ptr<IValueSet> &outputValues,
        const std::shared_ptr<IValueSet> &inputValues);

    /// @brief Extracts the (row, col) element from the MappingMatrix.
    ///
    /// @param row Zero based row index.
    /// @param col Zero based column index.
    /// @returns Element (row, column) from the mapping matrix.
    double GetValueFromMappingMatrix(int row, int col);

    /// @brief Sets individual (row,col) element in the MappingMatrix.
    ///
    /// @param value Element value to set.
    /// @param row Zero based row index.
    /// @param col Zero based column index.
    void SetValueInMappingMatrix(double value, int row, int col);

private:
    /// @brief Calculates the mapping matrix between fromElements and toElements.
    /// The mapping method is decided from the combination of methodIdentifier,
    /// fromElements.ElementType and toElements.ElementType.
    ///
    /// UpdateMappingMatrix method is called during initialisation,
    /// it must be called prior to MapValues().
    ///
    /// @param methodIdentifier String identification of mapping method.
    /// @param fromElements The IElementset to map from.
    /// @param toElements The IElementset to map to.
    void UpdateMappingMatrix(
        const std::shared_ptr<IIdentifiable> &methodIdentifier,
        const std::shared_ptr<IElementSet>   &fromElements,
        const std::shared_ptr<IElementSet>   &toElements);

    ///////////////////////////////////////////////////////////////////////////////////
    // Mapping methods.
    //

    void MapFromPointToPoint(
        const std::shared_ptr<IElementSet> &fromElements,
        const std::shared_ptr<IElementSet> &toElements);

    void MapFromPointToPolyline(
        const std::shared_ptr<IElementSet> &fromElements,
        const std::shared_ptr<IElementSet> &toElements);

    void MapFromPointToPolygon(
        const std::shared_ptr<IElementSet> &fromElements,
        const std::shared_ptr<IElementSet> &toElements);

    void MapFromPolylineToPoint(
        const std::shared_ptr<IElementSet> &fromElements,
        const std::shared_ptr<IElementSet> &toElements);

    void MapFromPolylineToPolygon(
        const std::shared_ptr<IElementSet> &fromElements,
        const std::shared_ptr<IElementSet> &toElements);

    void MapFromPolylineToPolyline(
        const std::shared_ptr<IElementSet> &fromElements,
        const std::shared_ptr<IElementSet> &toElements);

    void MapFromPolygonToPoint(
        const std::shared_ptr<IElementSet> &fromElements,
        const std::shared_ptr<IElementSet> &toElements);

    void MapFromPolygonToPolyline(
        const std::shared_ptr<IElementSet> &fromElements,
        const std::shared_ptr<IElementSet> &toElements);

    void MapFromPolygonToPolygon(
        const std::shared_ptr<IElementSet> &fromElements,
        const std::shared_ptr<IElementSet> &toElements);

public:
    ///////////////////////////////////////////////////////////////////////////////////
    // Static methods.
    //

    void ValidateIndicies(int row, int column);

    static GeomExtent GenerateExtent(const Point &point, real adjacent = 1.e-6);

    static GeomExtent GenerateExtent(const Polygon &polygon);

    static Point
    CreateXYPoint(const std::shared_ptr<IElementSet> &elementSet, int index);

    static Polygon
    CreateFromXYPolygon(const std::shared_ptr<IElementSet> &elementSet, int index);

    static Polygon
    CreateXYPolygon(const std::shared_ptr<IElementSet> &elementSet, int index);

    static Polyline
    CreateXYPolyline(const std::shared_ptr<IElementSet> &elementSet, int index);
};

}  // namespace OpenOasis::CommImp::DevSupports