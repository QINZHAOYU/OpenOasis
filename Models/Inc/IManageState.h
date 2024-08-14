/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  IManageState.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide an optional interface to handling component state so it
 *                  can be saved, restored and cleared.
 *
 ** ***********************************************************************************/
#pragma once
#include "IIdentifiable.h"
#include <memory>


namespace OpenOasis
{
/// @brief It provides additional methods for handling component state so it can be
/// saved, restored and cleared.
///
/// An optional interface to be implemented by components in addition to the
/// `ILinkableComponent` interface. It can be left completely to the component to handle
/// persistence of state or it can also implement `IByteStateConverter` and provide
/// ways for state to be converted to and from an array of bytes.
class IManageState
{
public:
    /// @brief Stores the linkable component's current state to a snapshot.
    ///
    /// @return Identifier of the stored state.
    virtual std::shared_ptr<IIdentifiable> KeepCurrentState() = 0;

    /// @brief Restores the state identified by the stateId.
    ///
    /// @param stateId Identifier of the component state to be restored.
    ///
    /// @throw If the stateId identifier is not known by the
    /// linkable component, an exception should be thrown.
    virtual void RestoreState(const std::shared_ptr<IIdentifiable> &stateId) = 0;

    /// @brief Clears a state from the linkable component.
    ///
    /// @param stateId Identifier of the componentstate to be cleared.
    ///
    /// @throw If the stateId identifier is not known by the
    /// linkable component, an exception should be trown.
    virtual void ClearState(const std::shared_ptr<IIdentifiable> &stateId) = 0;
};

}  // namespace OpenOasis
