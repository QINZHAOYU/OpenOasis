/** ***********************************************************************************
 *    Copyright (C) 2024, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  LinkLoader.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide unified interfaces for loading coupling configurations.
 *
 *    Coupling between components requires clarification of several issues, including:
 *        1. Which coupling mode is used, i.e., loop-mode or pull-mode?
 *        2. Which two components are coupled?
 *        3. Which component objects are coupled?
 *        4. Which object states are coupled?
 *        5. During which period of coupling?
 *        6. What coupling adaptation operations are used?
 *
 *    A component connection configuration needs to describe the above issues clearly,
 *    which is designed in the following format:
 *
 *    ```json
 *    {
 *        "comps": {
 *            "comp1": {
 *                "description": "{some description about comp1}",
 *                "type": "{component type of comp1}",
 *                "task": "{path_to_taskfile}/task.yaml",
 *                "dll": "{path_to_dllfile}/OasisFlows.dll",
 *                "link": false
 *            },
 *            "comp2": {
 *                "description": "{some description about comp2}",
 *                "type": "{component type of comp2}",
 *                "task": "{path_to_taskfile}/task.yaml",
 *                "dll": "{path_to_dllfile}/OasisFlows.dll",
 *                "link": false
 *            },
 *            ...
 *        },
 *        "links": {
 *            "link_comp1_to_comp2": {
 *                "description": "{some description about the coupling link}",
 *                "pipelines": [
 *                    {
 *                        "src_component": "{name of the source component}",
 *                        "src_state": "{state of the source element}",
 *                        "src_elements": [
 *                            {
 *                                "id": "c1_e1",
 *                                "type": "{type of the specified element}"
 *                            },
 *                            {
 *                                "id": "c1_e2",
 *                                "type": "{type of the specified element}"
 *                            },
 *                            ...
 *                        ],
 *                        "tar_component": "{name of the target component}",
 *                        "tar_element": {
 *                            "id": "c2_e1",
 *                            "type": "{type of the specified element}"
 *                        },
 *                        "temporal_operators": "accumulate",
 *                        "spatial_operators": "average"
 *                    }
 *                    ...
 *                ],
 *                "link_mode": "pull",
 *                "params":{
 *                    "max_iter_steps": "25",
 *                    "tolerance": "0.001",
 *                    ...
 *                }
 *            },
 *            ...
 *        }
 *    }
 *    ```
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Utils/JsonHandler.h"


namespace OpenOasis::CommImp::IO
{
using Utils::JsonLoader;

/// @brief Class forLoading components coupling configurations json file.
class LinkLoader
{
public:
    // Component information, contains:
    // - type
    // - task file path
    // - dll path
    using ComponentInfo = std::array<std::string, 3>;

    // Element(input/output) information, contains:
    // - id in component
    // - object type
    // - state
    // - temporal operator
    // - spatial operator
    using ElementInfo = std::array<std::string, 5>;

private:
    std::string mLinkFile;
    JsonLoader  mLoader;

    std::unordered_map<std::string, std::vector<std::string>> mOutputConsumers;
    std::unordered_map<std::string, std::vector<std::string>> mInputProviders;

    std::unordered_map<std::string, ComponentInfo>            mComps;
    std::unordered_map<std::string, std::vector<ElementInfo>> mInps;
    std::unordered_map<std::string, std::vector<ElementInfo>> mOuts;

    // Iterator groups, contains:
    // - iterator group id
    // - component ids
    std::unordered_map<std::string, std::vector<std::string>> mIterGroups;

    // Iterator configurations, contains:
    // - iterator group id
    // - iterator configurations
    std::unordered_map<std::string, std::multimap<std::string, std::string>>
        mIterConfigs;

public:
    LinkLoader(const std::string &json) : mLinkFile(json), mLoader(json)
    {}

    void Load();

    std::vector<std::string> GetComponentIds() const;

    ComponentInfo GetComponentInfo(const std::string &compId) const;

    std::vector<ElementInfo> GetComponentOutputs(const std::string &compId) const;

    std::unordered_map<std::string, std::vector<ElementInfo>>
    GetOutputConsumers(const std::string &compId, const ElementInfo &output) const;

    std::vector<ElementInfo> GetComponentInputs(const std::string &compId) const;

    std::unordered_map<std::string, std::vector<ElementInfo>>
    GetInputProviders(const std::string &compId, const ElementInfo &input) const;

    /// @brief Gets iterator groups consisted of components.
    /// @return The iterator groups, where each group contains a set of components.
    std::unordered_map<std::string, std::vector<std::string>> GetIteratorGroups() const;

    /// @brief Gets the iterator configurations of specified iterator group.
    std::multimap<std::string, std::string>
    GetIteratorConfigs(const std::string &iterId) const;

private:
    void LoadComponents();
    void LoadLinks();
    void LoadPipeline(
        const std::string &linkId, const nlohmann::json &json, int index,
        std::unordered_map<std::string, std::vector<std::string>> &linkGroups);

    void CollectExchangeItemMap(
        const std::string &srcCompId, const ElementInfo &output,
        const std::string &tarCompId, const ElementInfo &input);

    void CollectIteratorGroups(
        const std::unordered_map<std::string, std::vector<std::string>> &linkGroups,
        const std::unordered_map<std::string, std::string>              &linkModes,
        const std::unordered_map<
            std::string, std::unordered_map<std::string, std::string>> &linkConfs);

    std::string GenerateUniqueElementId(
        const std::string &compId, const ElementInfo &element) const;

    std::vector<std::string> SplitUniqueElementId(const std::string &uniqueId) const;
};

}  // namespace OpenOasis::CommImp::IO