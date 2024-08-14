/** ***********************************************************************************
 *    @File      :  LinkLoader.cpp
 *    @Brief     :  To provide unified interfaces for loading coupling configurations.
 *
 ** ***********************************************************************************/
#include "LinkLoader.h"
#include "Utils/Logger.h"
#include "Utils/FilePathHelper.h"
#include "Utils/Exception.h"
#include "Utils/StringHelper.h"
#include "Utils/MapHelper.h"
#include <set>


namespace OpenOasis::CommImp::IO
{
using namespace std;
using namespace Utils;


void LinkLoader::Load()
{
    shared_ptr<spdlog::logger> logger = Logger::GetLogger();
    logger->info("Loading links from [{}].", mLinkFile);
    LoadComponents();
    LoadLinks();
    logger->info("Loading links completed.");
}

void LinkLoader::LoadComponents()
{
    auto compsJson = mLoader.GetJson(mLoader.GetJson(), "comps").value();
    auto compIds   = mLoader.GetKeys(compsJson);

    for (auto id : compIds)
    {
        auto confs = mLoader.GetMap(compsJson, id);

        if (confs.count("type") * confs.count("task") * confs.count("dll") < 1)
        {
            throw IllegalArgumentException(StringHelper::FormatSimple(
                "Component [{}] configuration is incomplete.", id));
        }

        ComponentInfo infos = {confs["type"], confs["task"], confs["dll"]};
        mComps.emplace(id, infos);
    }
}

vector<string> LinkLoader::GetComponentIds() const
{
    return MapHelper::GetKeys(mComps);
}

LinkLoader::ComponentInfo LinkLoader::GetComponentInfo(const string &id) const
{
    return mComps.at(id);
}

void LinkLoader::LoadLinks()
{
    auto linksJson = mLoader.GetJson(mLoader.GetJson(), "links").value();
    auto linkIds   = mLoader.GetKeys(linksJson);

    unordered_map<string, vector<string>>                linkGroups;
    unordered_map<string, string>                        linkModes;
    unordered_map<string, unordered_map<string, string>> linkConfs;
    for (auto id : linkIds)
    {
        auto linkJson = mLoader.GetJson(linksJson, id).value();

        // Get the mode and parameters of the current link.
        auto   mode   = mLoader.GetValue<string>(linkJson, "mode");
        string modeId = mode.has_value() ? mode.value() : "PULL";
        linkModes[id] = modeId;

        auto parms    = mLoader.GetMap(linkJson, "params");
        linkConfs[id] = parms;

        // Get data exchage pipelines of the current link.
        auto pipelinesJson = mLoader.GetJson(linkJson, "pipelines").value();
        int  pipelineNum   = mLoader.GetArraySize(pipelinesJson);
        for (int i = 0; i < pipelineNum; i++)
        {
            LoadPipeline(id, pipelinesJson, i, linkGroups);
        }
    }

    // Collect the coupling groups.
    CollectIteratorGroups(linkGroups, linkModes, linkConfs);
}

vector<LinkLoader::ElementInfo>
LinkLoader::GetComponentOutputs(const string &compId) const
{
    if (mOuts.count(compId) == 0)
    {
        return {};
    }

    return mOuts.at(compId);
}

unordered_map<string, vector<LinkLoader::ElementInfo>>
LinkLoader::GetOutputConsumers(const string &compId, const ElementInfo &output) const
{
    string outputId = GenerateUniqueElementId(compId, output);

    auto consumers = mOutputConsumers.at(outputId);
    unordered_map<string, vector<ElementInfo>> res;
    for (const auto &consumerId : consumers)
    {
        auto consumer = SplitUniqueElementId(consumerId);
        auto compId   = consumer[0];
        auto elemId   = consumer[1];
        auto elemType = consumer[2];
        auto state    = consumer[3];
        auto tOptId   = consumer[4];
        auto sOptId   = consumer[5];

        ElementInfo elemInfo = {elemId, elemType, state, tOptId, sOptId};
        if (res.count(compId) == 0)
        {
            res[compId] = {elemInfo};
        }
        else
        {
            res[compId].emplace_back(elemInfo);
        }
    }

    return res;
}

vector<LinkLoader::ElementInfo>
LinkLoader::GetComponentInputs(const string &compId) const
{
    if (mInps.count(compId) == 0)
    {
        return {};
    }

    return mInps.at(compId);
}

unordered_map<string, vector<LinkLoader::ElementInfo>>
LinkLoader::GetInputProviders(const string &compId, const ElementInfo &input) const
{
    string inputId   = GenerateUniqueElementId(compId, input);
    auto   providers = mInputProviders.at(inputId);

    unordered_map<string, vector<ElementInfo>> res;
    for (const auto &providerId : providers)
    {
        auto provider = SplitUniqueElementId(providerId);
        auto compId   = provider[0];
        auto elemId   = provider[1];
        auto elemType = provider[2];
        auto state    = provider[3];
        auto tOptId   = provider[4];
        auto sOptId   = provider[5];

        ElementInfo elemInfo = {elemId, elemType, state, tOptId, sOptId};
        if (res.count(compId) == 0)
        {
            res[compId] = {elemInfo};
        }
        else
        {
            res[compId].emplace_back(elemInfo);
        }
    }

    return res;
}

unordered_map<string, vector<string>> LinkLoader::GetIteratorGroups() const
{
    return mIterGroups;
}

multimap<string, string> LinkLoader::GetIteratorConfigs(const string &iterId) const
{
    if (mIterConfigs.count(iterId) == 0)
    {
        return {};
    }

    return mIterConfigs.at(iterId);
}

void LinkLoader::CollectIteratorGroups(
    const unordered_map<string, vector<string>>                &linkGroups,
    const unordered_map<string, string>                        &linkModes,
    const unordered_map<string, unordered_map<string, string>> &linkConfs)
{
    int maxGroupSize = linkModes.size();

    // Collect the loop coupling groups.
    vector<set<string>> groups(maxGroupSize), links(maxGroupSize);
    for (auto &pair : linkGroups)
    {
        string linkId = pair.first;
        if (StringHelper::ToUpper(linkModes.at(linkId)) != "LOOP")
        {
            continue;
        }

        // Check if the link has been added to a group.
        auto comps = pair.second;
        for (int i = 0; i < maxGroupSize; i++)
        {
            if (groups[i].count(comps.front()))
            {
                groups[i].insert(comps.back());
                links[i].insert(linkId);
                break;
            }

            if (groups[i].count(comps.back()))
            {
                groups[i].insert(comps.front());
                links[i].insert(linkId);
                break;
            }
        }
    }

    for (int i = 0; i < maxGroupSize; i++)
    {
        if (groups[i].empty())
        {
            continue;
        }

        string groupId       = StringHelper::FormatSimple("LOOP_GROUP_{}", i);
        mIterGroups[groupId] = vector<string>(groups[i].begin(), groups[i].end());
    }

    // Collect loop coupling groups parameters.
    for (int i = 0; i < maxGroupSize; i++)
    {
        if (groups[i].empty())
        {
            continue;
        }

        string groupId = StringHelper::FormatSimple("LOOP_GROUP_{}", i);
        multimap<string, string> params;
        for (const auto &linkId : links[i])
        {
            auto confs = linkConfs.at(linkId);
            for (auto &pair : confs)
            {
                params.insert(pair);
            }
        }

        mIterConfigs[groupId] = params;
    }
}

void LinkLoader::LoadPipeline(
    const string &linkId, const nlohmann::json &json, int index,
    unordered_map<string, vector<string>> &linkGroups)
{
    auto pipelineJson = mLoader.GetJson(json, index).value();

    // Get the source and target components, also the exchanged state.
    string srcCompId = mLoader.GetValue<string>(pipelineJson, "src_component").value();
    string srcState  = mLoader.GetValue<string>(pipelineJson, "src_state").value();
    string tarCompId = mLoader.GetValue<string>(pipelineJson, "tar_component").value();

    // Get the temporal and spatial operators.
    auto   tOperator = mLoader.GetValue<string>(pipelineJson, "temporal_operators");
    string tOptId    = tOperator.has_value() ? tOperator.value() : "";

    auto   sOperator = mLoader.GetValue<string>(pipelineJson, "spatial_operators");
    string sOptId    = sOperator.has_value() ? sOperator.value() : "";

    // Get the target element.
    auto   tarElement  = mLoader.GetMap(pipelineJson, "tar_element");
    string tarElemId   = tarElement["id"];
    string tarElemType = tarElement["type"];

    // Collect the inputs of the target component.
    ElementInfo tarElemInfo = {tarElemId, tarElemType, srcState, "", ""};
    mInps[tarCompId].emplace_back(tarElemInfo);

    // Get the source elements.
    auto elementsJson = mLoader.GetJson(pipelineJson, "src_elements").value();
    int  elementNum   = mLoader.GetArraySize(elementsJson);
    for (int j = 0; j < elementNum; j++)
    {
        auto   srcElements = mLoader.GetMap(elementsJson, j);
        string srcElemId   = srcElements["id"];
        string srcElemType = srcElements["type"];

        // Collect the outputs of the source component.
        ElementInfo srcElemInfo = {srcElemId, srcElemType, srcState, tOptId, sOptId};
        mOuts[srcCompId].emplace_back(srcElemInfo);

        // Collect the exchange item map.
        CollectExchangeItemMap(srcCompId, srcElemInfo, tarCompId, tarElemInfo);
    }

    // Collect the coupling groups.
    linkGroups[linkId] = {srcCompId, tarCompId};
}

void LinkLoader::CollectExchangeItemMap(
    const string &srcCompId, const ElementInfo &output, const string &tarCompId,
    const ElementInfo &input)
{
    string outputId = GenerateUniqueElementId(srcCompId, output);
    string inputId  = GenerateUniqueElementId(tarCompId, input);

    auto setter = [](const string                          &key,
                     const string                          &value,
                     unordered_map<string, vector<string>> &map) {
        if (map.count(key) == 0)
        {
            map[key] = {value};
        }
        else
        {
            map[key].emplace_back(value);
        }
    };

    setter(outputId, inputId, mOutputConsumers);
    setter(inputId, outputId, mInputProviders);
}

string LinkLoader::GenerateUniqueElementId(
    const string &compId, const ElementInfo &element) const
{
    return StringHelper::FormatSimple(
        "{}_{}_{}_{}_{}_{}",
        compId,
        element[0],
        element[1],
        element[2],
        element[3],
        element[4]);
}

vector<string> LinkLoader::SplitUniqueElementId(const string &uniqueId) const
{
    auto parts = StringHelper::Split(uniqueId, '_');
    if (parts.size() != 6)
    {
        throw IllegalArgumentException(
            StringHelper::FormatSimple("Invalid unique element id [{}].", uniqueId));
    }

    return parts;
}


}  // namespace OpenOasis::CommImp::IO