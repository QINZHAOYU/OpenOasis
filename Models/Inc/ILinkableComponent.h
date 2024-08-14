/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ILinkableComponent.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide key interface for module connection and data transfer.
 *
 *    1. An OpenOasis compliant component must implement the `ILinkableComponent`
 *       interface according to specifications provided from OpenOasis docs;
 *    2. An OpenOasis compliant component can also comply to one ore more extensions,
 *       by implementing the `ILinkableComponent` interface and the extension
 *       interfaces, according to the specifications;
 *
 ** ***********************************************************************************/
#pragma once
#include "IAdaptedOutputFactory.h"
#include "LinkableComponentStatus.h"
#include "LinkableComponentStatusChangeEventArgs.h"


namespace OpenOasis
{
/// @brief The key interface class for linkable modules connection and data transfer.
class ILinkableComponent : public IIdentifiable
{
public:
    using ListenFunc =
        std::function<void(std::shared_ptr<LinkableComponentStatusChangeEventArgs>)>;

public:
    /// @brief Gets arguments needed to let the component do its work.
    ///
    /// Arguments validations can be done either when they occur,
    /// or when the `Initialize()` method is called.
    virtual std::vector<std::shared_ptr<IArgument>> GetArguments() const = 0;

    /// @brief Gets status of the linkable component.
    ///
    /// The first status that a component sets is `Created`, as soon
    /// after it has been created.
    virtual LinkableComponentStatus GetStatus() const = 0;

    /// @brief Gets the input items for recieving values.
    /// @throw The method must be accessible after `Initialize()` method has
    /// been called and until the `Validate()` method has been invoked.
    /// Otherwise,
    /// `LinkableComponent` can't handle this, an exception must be thrown.
    virtual std::vector<std::shared_ptr<IInput>> GetInputs() const = 0;

    /// @brief Gets the output items for producing results.
    /// @throw The method must be accessible after `Initialize()` method has
    /// been invoked and until `Validate()` method has been invoked.
    /// Otherwise,
    /// `LinkableComponent` cant handle this, an exception must be thrown.
    virtual std::vector<std::shared_ptr<IOutput>> GetOutputs() const = 0;

    /// @brief Gets the list of `IAdaptedOutputFactory`, each allowing to create
    /// `IAdaptedOutput` item for making output fit to inputs
    /// in case they do not already do so.
    virtual std::vector<std::shared_ptr<IAdaptedOutputFactory>>
    GetAdaptedOutputFactories() const = 0;

    /// @brief Initializes the component.
    ///
    /// The `Initialize()` will and must be invoked before any other methods
    /// in the `ILinkableComponent` interface is invoked
    /// or accessed, except for the `GetArguments`.
    ///
    /// Immediatly after the method is been invoked, it changes the linkable
    /// component's status to `Initializing`. If component initializes
    /// succesfully, the status is changed to `Initialized`.
    ///
    /// The method will typically populate the component based on values
    /// specified in its arguments. Settings can be used to read input
    /// files, allocate memory, and organize input and output items.
    ///
    /// @throw When the method is executed and an error occurs, the status
    /// will change to `Failed`, and an exception will be thrown.
    /// @throw It's only required that `Initialize()` can be invoked once.
    /// If the `Initialize()` method is invoked more than once,
    /// `ILinkableComponent` can't handle this, thrown an exception.
    virtual void Initialize() = 0;

    /// @brief Validates the populated instance of the component.
    ///
    /// The method will must be invoked after various provider/consumer relations
    /// between this component's exchange items and the exchange items of
    /// other components present in the composition.
    ///
    /// Immediatly after this method is invoked, it changes the component's
    /// status to `Validating`. When the method has finished, the status
    /// of the component has changed to either `Valid` or `Invalid`.
    ///
    /// If there are messages while components status is `Valid`, the messages
    /// are purely informative. If there're messages while components
    /// status is `Invalid`, at least one of the messages
    /// indicates a fatal error.
    ///
    /// @throw The method must be accessible after the `Initialize()`
    /// has been called and until the `Finish()` method has been invoked.
    /// Otherwise, the linkable component can not handle this,
    /// an exception must be thrown.
    virtual std::vector<std::string> Validate() = 0;

    /// @brief Prepares the component for calls to the `Update()`.
    ///
    /// Before `Prepare()` is called, the component are not required to honor any
    /// type of action that retrieves values from the component.
    /// After `Prepare()` is called, the component must be ready for
    /// providing values.
    /// Immediatly after the method is invoked, it changes the component's
    /// status to `Preparing`. When the method has finished, the status
    /// of the component has changed to either `Updated` or `Failed`.
    ///
    /// @throw The method must be accessible after the `Initialize()` method
    /// has been called and until the `Finish()` method has been invoked.
    /// Otherwise, the linkable component can't handle it,
    /// an exception must be thrown.
    /// @throw It's required the `Prepare()` method can be invoked once.
    /// Otherwise, the linkable component can't handle this,
    /// an exception must be thrown.
    virtual void Prepare() = 0;

    /// @brief Updates the linkable component itself, thus reaching its next state.
    ///
    /// Immediately after `Update()` is invoked, it changes the component's
    /// status to `Updating`. If the method is performed succesfully,
    /// the component sets its status to `Updated`, unless
    /// after this update action is at the end of its computation,
    /// in which case it will set its status to `Done`.
    ///
    /// According to the 'pull-driven' approach, linkable components can be
    /// connected in a chain, where invoking `Update()` method
    /// on the last component in the chain trigger
    /// the entire stack of data exchange.
    ///
    /// The type of actions a component takes during the `Update()` method
    /// depends on the type of component.
    /// A numerical model that progresses in time will typically compute a time
    /// step. A database would typically look at the consumers of its
    /// output items, and perform one or more queries to be able
    /// to provide the values that the consumers required.
    ///
    /// @throw If during the `Update()` method problem arises,
    /// the component sets its state to `Failed` and throws an exception.
    virtual void Update() = 0;

    /// @brief Finishes the component computation, and then restart it if needed.
    ///
    /// This method is and must be invoked as the last of any methods in the
    /// `ILinkableComponent` interfaces.
    ///
    /// Immediatly after this method is invoked, it changes the component's
    /// status to `Finishing`. Once the finishing is completed,
    /// the component changes status to `Finished` if it can't be restarted;
    /// `Created`, otherwise.
    ///
    /// @throw This method must be accessible after the `Prepare()`
    /// method has been invoked. Otherwise,the component can't handled this,
    /// an exception must be thrown.
    virtual void Finish() = 0;

    virtual void RemoveListener(const ListenFunc &func) = 0;
    virtual void AddListener(const ListenFunc &func)    = 0;
};

}  // namespace OpenOasis
