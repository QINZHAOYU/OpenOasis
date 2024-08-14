/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  LinkableComponentStatus.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To enumerate the possible statuses that a linkable component can be.
 *
 ** ***********************************************************************************/
#pragma once


namespace OpenOasis
{
/// @brief Class enumerates the possible statuses that a linkable component can be in.
enum class LinkableComponentStatus
{
    /// @brief The linkable component instance has just been `Created`.
    ///
    /// This status must and will be followed by `Initializing`.
    Created,

    /// @brief The linkable component is initializing itself.
    ///
    /// This status will end in a status change to `Initialized` or `Failed`.
    Initializing,

    /// @brief The linkable component has succesfully initialized itself.
    ///
    /// The connections between its inputs/outputs and those of other components can be
    /// established.
    Initialized,

    /// @brief This status will end in a status change to `Valid` or `Invalid`.
    ///
    /// After links between the component's inputs/outputs and those of other components
    /// have been established, the component is validating whether its required
    /// input will be available when it updates itself, whether indeed it will be
    /// able to provide the required output during this update.
    Validating,

    /// @brief The component is in a valid state.
    ///
    /// When updating itself its required input will be available, and it will be able
    /// to provide the required output.
    Valid,

    /// @brief The component wants to update itself, but isnot yet able to perform the
    /// actual computation, because it is still waiting for input data
    /// from other components.
    WaitingForData,

    /// @brief The component is in an invalid state.
    ///
    /// When updating itself not all required input will be available, and/or it will
    /// not be able to provide the required output. After the user has modified
    /// the connections between the component's inputs/outputs and those of other
    /// components, the `Validating` state can be entered again.
    Invalid,

    /// @brief The component is preparing itself for the first `GetValues()` call.
    ///
    /// This state will end in a status change to `Updated` or `Failed`.
    Preparing,

    /// @brief The component is updating itself. It receives all required input data
    /// from other components, and is now performing the actual computation.
    ///
    /// This state will end in a status change to `Updated`, `Done` or `Failed`.
    Updating,

    /// @brief The component has succesfully updated itself.
    Updated,

    /// @brief The last update process that component performed was the final one.
    ///
    /// A next call to the `Update()` method will leave the component's internal state
    /// unchanged.
    Done,

    /// @brief The `ILinkableComponent` was requested to perform the actions to be
    /// performed before it will either be disposed or re-intialized again.
    ///
    /// Typical actions would be writing the final result files, close all open files,
    /// free memory, etc. When all required actions have been performed,
    /// the status switches to `Created` when re-initialization is possible.
    /// The status switches to `Finished` when component is to be disposed.
    Finishing,

    /// @brief The `ILinkableComponent` has successfully performed its finalization
    /// actions.
    ///
    /// Re-initialization of the component instance isn't possible and shouldn't be
    /// attempted. Instead the instance should be disposed, e.g.
    /// through the garbage collection mechanism.
    Finished,

    /// @brief The component was requested to perform the actions to be performed before
    /// it will either be disposed or re-initialized again.
    ///
    /// The linkable component has failed initialize itself, failed to prepare
    /// itself for computation, or failed to complete its update process.
    ///
    /// Typical actions would be writing the final result files, close all open files,
    /// free memory, etc. When all required actions have been performed,
    /// the status switches back to `Created` if the component supports being
    /// re-initialized. If it can't be re-initialized, it can be released from memory.
    Failed
};

}  // namespace OpenOasis
