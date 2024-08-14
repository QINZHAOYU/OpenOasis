/** ***********************************************************************************
 *    Copyright (C) 2022~2024, All OpenOasis Contributors. Join us on this journey!
 *
 *    @File      :  Launcher.cpp
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  OpenOasis component launcher.
 *
 ** ***********************************************************************************/
// #include "Models/CommImp/DevSupports/IterationController.h"
#include "Models/CommImp/LinkableComponent.h"
#include "Models/CommImp/IO/LinkLoader.h"
#include "Models/Utils/Logger.h"
#include "Models/wrappers/OasisFlows.h"
#include "Models/Utils/LibraryLoader.h"
#include "Models/Utils/StringHelper.h"
#include "ThirdPart/Args/args.hxx"
#include <iostream>
#include <thread>
#include <iomanip>


using namespace OpenOasis;
using namespace CommImp;
using namespace Utils;
using namespace std;


int main(int argc, const char *argv[])
{
    // Setup command line arguments parser.
    args::ArgumentParser parser("OpenOasis component launcher");
    args::HelpFlag       help(parser, "", "OpenOasis help menu", {'h', "help"});

    // Add positional arguments.
    args::Positional<std::string> configFile(
        parser, "conf", "Path to the link configuration json file");

    // Add optional arguments.
    args::ValueFlag<string> logLevel(
        parser, "", "Log level (debug, info, warn, err)", {"log"});

    // Parse command line arguments.
    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (const args::Error &e)
    {
        cerr << e.what() << endl;
        cerr << parser;
        return 1;
    }

    // Set log level.
    string logLevelStr = "info";
    if (logLevel)
    {
        logLevelStr = logLevel.Get();

        spdlog::set_level(spdlog::level::from_str(logLevelStr));

        auto logger = Logger::GetLogger();
        logger->set_level(spdlog::level::from_str(logLevelStr));
    }

    // Set config file path.
    string configFilePath;
    if (configFile)
    {
        configFilePath = configFile.Get();
    }
    else
    {
        return 0;
    }
    spdlog::info("Config file path: {}", configFilePath);

    // Init link configuration.
    IO::LinkLoader linkLoader(configFilePath);
    linkLoader.Load();
    spdlog::info("Link configuration loaded.");

    // Init library loader.
    LibraryLoader libLoader;

    // Load components.
    unordered_map<string, ILinkableComponent *> components;

    auto compIds = linkLoader.GetComponentIds();
    for (auto compId : compIds)
    {
        auto compInfo = linkLoader.GetComponentInfo(compId);
        auto type     = compInfo[0];
        auto taskFile = compInfo[1];
        auto dllPath  = compInfo[2];

        if (!libLoader.Load(dllPath))
        {
            spdlog::error("Failed to load dll/so from {}", dllPath);
            return 1;
        }
        else
        {
            auto ver = libLoader.RunFunction<const char *()>("GetOasisVersion");
            spdlog::info(
                "Dll/so for component {} loaded from {} (version: {})",
                compId,
                dllPath,
                ver);
        }

        auto rawComp =
            libLoader.RunFunction<void *(const char *, const char *, const char *)>(
                "GetOasisComponent", compId.c_str(), type.c_str(), taskFile.c_str());

        if (rawComp == nullptr)
        {
            spdlog::error("Failed to create component {}", compId);
            return 1;
        }

        auto comp = static_cast<ILinkableComponent *>(rawComp);
        if (comp == nullptr)
        {
            spdlog::error("Failed to cast component {} to ILinkableComponent", compId);
            return 1;
        }

        components[compId] = comp;
        spdlog::info("Component {} loaded from {}", compId, dllPath);
    }

    // Perpare components.
    unordered_map<string, vector<string>> compInputs, compOutputs;
    for (auto comp : components)
    {
        auto compId = comp.first;

        // Get component args.
        auto compPtr = comp.second;
        auto args    = compPtr->GetArguments();

        // Prepare inputs.
        auto inputs     = *(find_if(begin(args), end(args), [](const auto &it) {
            return it->GetId() == "INPUTTERS";
        }));
        auto inputInfos = linkLoader.GetComponentInputs(compId);

        vector<array<string, 3>> inputFlags;
        for (auto inputInfo : inputInfos)
        {
            auto id    = inputInfo[0];
            auto type  = inputInfo[1];
            auto state = inputInfo[2];

            inputFlags.emplace_back(array<string, 3>{type, id, state});
            compInputs[compId].push_back(
                StringHelper::FormatSimple("{}_{}_{}", type, id, state));
        }

        // inputs->SetValue(inputFlags);
        spdlog::info("Component {} inputs setted.", compId);

        // Prepare outputs.
        auto outputs     = *(find_if(begin(args), end(args), [](const auto &it) {
            return it->GetId() == "OUTPUTTERS";
        }));
        auto outputInfos = linkLoader.GetComponentOutputs(compId);

        vector<array<string, 3>> outputFlags;
        for (auto outputInfo : outputInfos)
        {
            auto id    = outputInfo[0];
            auto type  = outputInfo[1];
            auto state = outputInfo[2];

            outputFlags.emplace_back(array<string, 3>{type, id, state});
            compOutputs[compId].push_back(
                StringHelper::FormatSimple("{}_{}_{}", type, id, state));
        }

        // outputs->SetValue(outputFlags);
        spdlog::info("Component {} outputs setted.", compId);

        // init component.
        compPtr->Initialize();
        compPtr->Validate();
        spdlog::info("Component {} initialized.", compId);
    }

    // // Link components (uncomplete).
    // for (auto comp : components)
    // {
    //     auto srcCompId  = comp.first;
    //     auto inputInfos = linkLoader.GetComponentInputs(srcCompId);
    //     for (auto inputInfo : inputInfos)
    //     {
    //         auto consumers = linkLoader.GetOutputConsumers(srcCompId, inputInfo);
    //         for (auto consumer : consumers)
    //         {
    //             auto dstCompId = consumer.first;
    //         }
    //     }
    // }

    // Run components separately.
    for (auto comp : components)
    {
        auto compPtr = comp.second;
        compPtr->Prepare();
        spdlog::info("Component {} prepared.", comp.first);

        auto compId = comp.first;
        int  steps  = 0;
        while (compPtr->GetStatus() != LinkableComponentStatus::Done
               && compPtr->GetStatus() != LinkableComponentStatus::Failed)
        {
            compPtr->Update();
            steps++;
        }
        spdlog::info("Component {} updated for {} steps.", compId, steps);

        compPtr->Finish();
        spdlog::info("Component {} finished.", compId);
    }

    spdlog::info("All components finished.");

    return 0;
}
