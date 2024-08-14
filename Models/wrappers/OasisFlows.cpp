// #include "OasisFlows.h"
// #include "Models/SystHeats/HeatConductionModel/HeatConductionModel.h"

// #ifndef PROJECT_VERSION
// #define PROJECT_VERSION "dev-0.0.0"
// #endif

// // using namespace OpenOasis::SystFluids::Hydrologics;
// using namespace OpenOasis::SystHeats;
// using namespace std;

// static vector<shared_ptr<OpenOasis::ILinkableComponent>> componenents;

// const char *GetOasisVersion()
// {
//     return PROJECT_VERSION;
// }

// void *GetHeatConductionComp(const char *id, const char *taskFile);


// static unordered_map<string, function<void *(const char *, const char *)>>
//     componentFactory = {{"HeatConductionComp", GetHeatConductionComp}};


// void *GetOasisComponent(const char *id, const char *type, const char *task)
// {
//     if (componentFactory.count(type) == 1)
//     {
//         return componentFactory[type](id, task);
//     }

//     return nullptr;
// }

// void *GetHeatConductionComp(const char *id, const char *taskFile)
// {
//     auto comp = make_shared<HeatConductionModel>(id, taskFile);
//     componenents.push_back(comp);

//     return comp.get();
// }