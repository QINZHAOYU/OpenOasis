/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IByteStateConverter.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  An optional complement to support state management.
 *
 ** ***********************************************************************************/
#pragma once
#include "IIdentifiable.h"
#include <vector>
#include <memory>


namespace OpenOasis
{
/// @brief An optional interface to save and transform the model state.
///
/// This interface is an optional complement to the `IManageState` interface.Both are
/// extensions to `ILinkableComponent`, meant to provide state management.
/// It defines methods for converting a state into a byte stream and reading in
/// a state from a byte stream.
///
/// This facilitates external modules,
/// e.g. a GUI or an operational control system, to save a model's state somewhere as
/// persistent state.
class IByteStateConverter
{
public:
    /// @brief Converts the state with the stateId into a byte stream.
    ///
    /// @param stateId Id of the state.
    /// @return The state identified by stateId as a byte stream.
    virtual std::stringstream
    ConvertToByteStream(const std::shared_ptr<IIdentifiable> &stateId) = 0;

    /// @brief Creates a state from a byte stream and return the identifier.
    ///
    /// @param byteStream State as a byte stream.
    /// @return `IIdentifiable` identifying the state.
    virtual std::shared_ptr<IIdentifiable>
    ConvertFromByteStream(const std::stringstream &byteStream) = 0;
};

}  // namespace OpenOasis
