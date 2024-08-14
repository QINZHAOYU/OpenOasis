// /**
// ***********************************************************************************
//  *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
//  *
//  *    @File      :  IterationController.h
//  *    @License   :  Apache-2.0
//  *
//  *    @Desc      :  IterationController controls iterations among linkable
//  components.
//  *
//  **
//  ***********************************************************************************/
// #pragma once
// #include "Models/CommImp/LinkableComponent.h"
// #include <unordered_map>
// #include <set>
// #include <map>


// namespace OpenOasis
// {
// namespace CommImp
// {
// namespace DevSupports
// {
// /// @brief IterationController controls iterations between linkable components.
// ///
// /// @note IterationController encapsulates a set of bidirectional iteratively coupled
// /// linkable components, takes over component updates and data exchange to
// /// achieve loop-driven mode.
// class IterationController : public LinkableComponent
// {
// public:
//     int    mMaxIter    = 2;      // Maximum number of iteration.
//     int    mIter       = 0;      // Current iteration step.
//     double mEps        = 1.e-6;  // Accuracy of iterative convergence.
//     double mRelaxation = 0.25;   // Relaxation coefficient.

//     std::map<std::string, std::shared_ptr<IIdentifiable>>     mStates;
//     std::map<std::string, std::shared_ptr<LinkableComponent>> mComponentSet;

//     std::vector<std::shared_ptr<IInput>>  mInnerInputSet;
//     std::vector<std::shared_ptr<IOutput>> mInnerOutputSet;

// public:
//     IterationController(const std::string &id);
//     virtual ~IterationController() = default;

//     void AddComponent(std::shared_ptr<ILinkableComponent> component);

// protected:
//     ///////////////////////////////////////////////////////////////////////////////////
//     // Override methods inherited from `LinkableComponent` for iteration.
//     //

//     void InitializeArguments() override;

//     void InitializeSpace() override
//     {}

//     void InitializeTime() override;

//     void InitializeInputs() override;

//     void InitializeOutputs() override;

//     std::vector<std::string> OnValidate() override;

//     void PrepareInputs() override
//     {}

//     void PrepareOutputs() override;

//     void
//     UpdateOutputs(const std::vector<std::shared_ptr<IOutput>> &requiredOutputs)
//     override
//     {}

//     void PerformTimestep(
//         const std::vector<std::shared_ptr<IOutput>> &requiredOutputs) override;

//     void ApplyInputData(const std::shared_ptr<IValueSet> &values) override
//     {}

//     bool IsIterationConverged() const override;

//     ///////////////////////////////////////////////////////////////////////////////////
//     // Additional methods used for iteration.
//     //

//     void CollectStates();
//     void ResetStates();
// };

// }  // namespace DevSupports
// }  // namespace CommImp
// }  // namespace OpenOasis