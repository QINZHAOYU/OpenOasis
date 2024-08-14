// /**
// ***********************************************************************************
//  *    @File      :  IterationController.cpp
//  *    @Brief     :  IterationController controls iterations among linkable
//  components.
//  *
//  **
//  ***********************************************************************************/
// #include "IterationController.h"
// #include "Models/CommImp/Arguments.h"
// #include "Models/Utils/Exception.h"
// #include "Models/Utils/StringHelper.h"
// #include "Models/CommImp/Output.h"
// #include "Models/CommImp/Time.h"
// #include "Models/CommImp/TimeSet.h"


// namespace OpenOasis::CommImp::DevSupports
// {
// using namespace std;
// using namespace OpenOasis::Utils;

// //
// ------------------------------------------------------------------------------------

// IterationController::IterationController(const string &id) : LinkableComponent(id)
// {
//     mCaption     = "Iteration Controller";
//     mDescription = "IterationController controls iterations among linkable
//     components";

//     mRequiredArguments       = {"ID", "MaxIter", "Eps", "Relaxation"};
//     mArguments["ID"]         = make_shared<ArgumentString>("ID", mId);
//     mArguments["MaxIter"]    = make_shared<ArgumentInt>("MaxIter", mMaxIter);
//     mArguments["Eps"]        = make_shared<ArgumentDouble>("Eps", mEps);
//     mArguments["Relaxation"] = make_shared<ArgumentDouble>("Relaxation",
//     mRelaxation);

//     mTimeExtent  = nullptr;
//     mCurrentTime = nullptr;
// }

// void IterationController::InitializeArguments()
// {
//     for (const auto &kid : mRequiredArguments)
//     {
//         any value = mArguments[kid]->GetValue();
//         if (kid == "MaxIter")
//         {
//             mMaxIter = any_cast<int>(value);
//         }
//         else if (kid == "Eps")
//         {
//             mEps = any_cast<double>(value);
//         }
//         else if (kid == "Relaxation")
//         {
//             mRelaxation = any_cast<double>(value);
//         }
//     }
// }

// void IterationController::AddComponent(shared_ptr<ILinkableComponent> component)
// {
//     auto comp = dynamic_pointer_cast<LinkableComponent>(component);
//     if (!comp)
//     {
//         return;
//     }

//     comp->SetCascadingUpdateCallsDisabled(true);

//     mComponentSet[component->GetId()] = comp;
// }

// void IterationController::InitializeInputs()
// {
//     mInnerInputSet.clear();
//     mInputs.clear();

//     for (const auto &comPair : mComponentSet)
//     {
//         const auto &component = comPair.second;
//         for (const auto &input : component->GetInputs())
//         {
//             if (input->GetProviders().empty())
//                 continue;

//             // Check if the input is outer. Note that the input is outer if it is not
//             // provided by any of the components in the internal set.
//             const auto &providers = input->GetProviders();
//             bool        isOuter =
//                 all_of(begin(providers), end(providers), [&](const auto &provider) {
//                     string compId(provider.lock()->GetComponent().lock()->GetId());
//                     return mComponentSet.count(compId) == 0;
//                 });

//             if (isOuter)
//             {
//                 mInputs.push_back(input);
//             }
//             else
//             {
//                 mInnerInputSet.push_back(input);
//             }
//         }
//     }
// }

// void IterationController::InitializeOutputs()
// {
//     mInnerOutputSet.clear();
//     mOutputs.clear();

//     for (const auto &comPair : mComponentSet)
//     {
//         const auto &component = comPair.second;
//         for (const auto &output : component->GetOutputs())
//         {
//             if (output->GetConsumers().empty())
//                 continue;

//             // Check if the output is outer.
//             const auto &consumers = output->GetConsumers();
//             bool        isOuter =
//                 all_of(begin(consumers), end(consumers), [this](const auto &consumer)
//                 {
//                     string compId(consumer.lock()->GetComponent().lock()->GetId());
//                     return mComponentSet.count(compId) == 0;
//                 });

//             if (isOuter)
//             {
//                 mOutputs.push_back(output);
//             }
//             else
//             {
//                 mInnerOutputSet.push_back(output);
//             }
//         }
//     }
// }

// void IterationController::InitializeTime()
// {
//     mTimeExtent = make_shared<TimeSet>();

//     auto currents = vector<shared_ptr<ITime>>();
//     auto begins   = vector<shared_ptr<ITime>>();
//     auto ends     = vector<shared_ptr<ITime>>();

//     for (const auto &comp : mComponentSet)
//     {
//         currents.push_back(comp.second->GetNowTime());
//         begins.push_back(comp.second->GetStartTime());
//         ends.push_back(comp.second->GetEndTime());
//     }

//     // auto comp = [](shared_ptr<ITime> a, shared_ptr<ITime> b) {
//     //     return a->GetTimeStamp() < b->GetTimeStamp();
//     // };
//     sort(begin(currents), end(currents));
//     sort(begin(begins), end(begins));
//     sort(begin(ends), end(ends));

//     mCurrentTime = make_shared<Time>(currents.front()->GetTimeStamp());
//     mTimeExtent->AddTime(make_shared<Time>(begins.front()->GetTimeStamp()));
//     mTimeExtent->AddTime(make_shared<Time>(ends.back()->GetTimeStamp()));
// }

// vector<string> IterationController::OnValidate()
// {
//     vector<string> errors;

//     for (const auto &comPair : mComponentSet)
//     {
//         const auto &innerInputIter =
//             find_if(begin(mInnerInputSet), end(mInnerInputSet), [&](const auto
//             &input) {
//                 return input->GetComponent().lock()->GetId() == comPair.first;
//             });
//         if (innerInputIter == end(mInnerInputSet))
//         {
//             string msg = StringHelper::FormatSimple(
//                 "LinkableComponent [{}] in InterationController [{}] has no inner
//                 inputs.", comPair.first, mId);
//             errors.push_back(msg);
//         }

//         const auto &innerOutputIter = find_if(
//             begin(mInnerOutputSet), end(mInnerOutputSet), [&](const auto &output) {
//                 return output->GetComponent().lock()->GetId() == comPair.first;
//             });
//         if (innerOutputIter == end(mInnerOutputSet))
//         {
//             string msg = StringHelper::FormatSimple(
//                 "LinkableComponent [{}] in InterationController [{}] has no inner
//                 outputs.", comPair.first, mId);
//             errors.push_back(msg);
//         }
//     }

//     if (mMaxIter < 1)
//     {
//         string msg = StringHelper::FormatSimple(
//             "MaxIter for IterationController [{}] must be greater than 1.", mId);
//         errors.push_back(msg);
//     }
//     if (mEps < 0)
//     {
//         string msg = StringHelper::FormatSimple(
//             "Eps for IterationController [{}] must be greater than 0.", mId);
//         errors.push_back(msg);
//     }
//     if (mRelaxation < 0 || mRelaxation > 1)
//     {
//         string msg = StringHelper::FormatSimple(
//             "Relaxation for IterationController [{}] must be in [0, 1].", mId);
//         errors.push_back(msg);
//     }

//     return errors;
// }

// void IterationController::PrepareOutputs()
// {
//     // To rebuild the output-linkages between components. To change the ownership
//     // component saved in the output item, while retaining the ownership of the
//     original
//     // component to the output. This allows calls to the output item to be
//     intercepted
//     // and the original component is still responsible for updating the output.

//     auto iterator = dynamic_pointer_cast<IterationController>(shared_from_this());
//     for (auto &output : mOutputs)
//     {
//         auto item = dynamic_pointer_cast<Output>(output);
//         if (!item)
//         {
//             throw NotSupportedException(StringHelper::FormatSimple(
//                 "Unsupported output type in component [{}] in IterationController {},
//                 \ only `OpenOasis::CommImp::Output` is supported.",
//                 output->GetComponent().lock()->GetId(),
//                 mId));
//         }

//         // This wouldn't erase the ownership of the original component to the output
//         and
//         // the linkages between the input items with the output item.
//         item->SetComponent(iterator);
//     }
// }

// void IterationController::PerformTimestep(
//     const vector<shared_ptr<IOutput>> &requiredOutputs)
// {
//     // printf("\n --- iteration at time: %f :\n", mCurrentTime->GetTimeStamp());
//     mIter = 0;
//     while (true)
//     {
//         CollectStates();

//         try
//         {
//             mComponentSet.begin()->second->Update();
//         }
//         catch (...)
//         {}

//         if (IsIterationConverged())
//         {
//             for (auto &comPair : mComponentSet)
//             {
//                 comPair.second->UpdateInputTimesAndValues();
//                 comPair.second->SetStatus(
//                     comPair.second->GetNowTime()->GetTimeStamp()
//                             >= GetEndTime()->GetTimeStamp() ?
//                         LinkableComponentStatus::Done :
//                         LinkableComponentStatus::Updated);
//             }
//             break;
//         }

//         mIter++;
//         ResetStates();
//     }

//     auto currents = vector<shared_ptr<ITime>>();
//     for (const auto &comp : mComponentSet)
//     {
//         currents.push_back(comp.second->GetNowTime());
//     }

//     // auto comp = [](shared_ptr<ITime> a, shared_ptr<ITime> b) {
//     //     return a->GetTimeStamp() < b->GetTimeStamp();
//     // };
//     sort(begin(currents), end(currents));
//     mCurrentTime = make_shared<Time>(currents.front()->GetTimeStamp());
// }

// bool IterationController::IsIterationConverged() const
// {
//     bool isIterationDone = mIter > mMaxIter;

//     bool isAllCompsConverged = true;
//     for (const auto &comPair : mComponentSet)
//     {
//         // Leave the check of whether the iterations converge to each components.
//         if (!comPair.second->IsIterationConverged())
//         {
//             isAllCompsConverged = false;
//             break;
//         }
//     }

//     return (isIterationDone) ? true : isAllCompsConverged;
// }

// void IterationController::CollectStates()
// {
//     mStates.clear();
//     for (const auto &comPair : mComponentSet)
//     {
//         mStates[comPair.first] = comPair.second->KeepCurrentState();
//     }
// }

// void IterationController::ResetStates()
// {
//     for (const auto &comPair : mComponentSet)
//     {
//         comPair.second->RestoreState(mStates[comPair.first]);
//         comPair.second->ClearState(mStates[comPair.first]);
//     }
// }

// }  // namespace OpenOasis::CommImp::DevSupports