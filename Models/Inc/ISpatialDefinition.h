/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ISpatialDefinition.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide a general spatial construct that all other
 *                  spatial constructions extend from.
 *
 *    Data in component is often related to spatial coordinates, either georeferenced or
 *    not. Although most models encapsulate data with a static spatial definition,
 *    some advanced models might contain dynamic spatial definitions (e.g. waves, moving
 *    grids). A `Version` number has been introduced to enable tracking of spatial
 *    changes over time. If the version changes, the spatial definition might need to be
 *    queried again during the computation process.
 *
 ** ***********************************************************************************/
#pragma once
#include "IDescribable.h"


namespace OpenOasis
{
/// @brief General spatial construct that all other spatial constructions extend from.
class ISpatialDefinition : public IDescribable
{
public:
    /// @brief Specifies the OGC Well-Known Text(WKT) representation of
    /// spatial reference system to be used in association with the coordinates in
    /// the `ISpatialDefinition`.
    virtual std::string GetSpatialReferenceSystem() const = 0;

    /// @brief Gets number of data elements in the spatial axis.
    virtual int GetElementCount() const = 0;

    /// @brief Gets the version number for the spatial axis.
    ///
    /// The version must be incremented if anything inside the spatial axis is changed,
    /// or if an entirely new spatial axis is provided.
    virtual int GetVersion() const = 0;
};

}  // namespace OpenOasis
