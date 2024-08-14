/** ***********************************************************************************
 *    @File      :  SpaceAdaptedOutputFactory.cpp
 *    @Brief     :  None.
 *
 ** ***********************************************************************************/
#include "SpaceAdaptedOutputFactory.h"
#include "Output.h"
#include "Input.h"
#include "Arguments.h"
#include "Identifier.h"
#include "Describer.h"
#include "Models/CommImp/DevSupports/SpaceMapAdaptor.h"
#include "Models/CommImp/DevSupports/SpaceLengthAdaptor.h"
#include "Models/CommImp/DevSupports/SpaceAreaAdaptor.h"
#include "Models/Utils/Exception.h"
#include "Models/Utils/VectorHelper.h"
#include "Models/Utils/StringHelper.h"


namespace OpenOasis::CommImp
{
using namespace DevSupports;
using namespace Utils;
using namespace std;


// class StaticConstructor-------------------------------------------------------------

vector<shared_ptr<SpaceAdaptedOutputFactory::SpatialMethod>>
    SpaceAdaptedOutputFactory::mAvailableMethods;

SpaceAdaptedOutputFactory::StaticConstructor
             SpaceAdaptedOutputFactory::mStaticConstructor      = StaticConstructor();
const string SpaceAdaptedOutputFactory::mElementMapperPrefix    = "ElementMapper";
const string SpaceAdaptedOutputFactory::mElementOperationPrefix = "ElementOperation";

SpaceAdaptedOutputFactory::StaticConstructor::StaticConstructor()
{
    mAvailableMethods.clear();

    shared_ptr<SpatialMethod> method;

    // Operation values:
    // Id        :   0- 99
    // Point     : 100-199
    // Polyline  : 200-299
    // Polygon   : 100-199
    // Polyhedron: 100-199

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Polyline;
    method->mToElementsShapeType   = ElementType::Polyline;
    method->mElementMapperMethod   = ElementMapperMethod::None;
    method->mDescription           = "Polyline operation, multiply by line length";
    method->mId                    = mElementOperationPrefix + "200";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Polygon;
    method->mToElementsShapeType   = ElementType::Polygon;
    method->mElementMapperMethod   = ElementMapperMethod::None;
    method->mDescription           = "Polygon operation, multiply by area";
    method->mId                    = mElementOperationPrefix + "300";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Point;
    method->mToElementsShapeType   = ElementType::Point;
    method->mElementMapperMethod   = ElementMapperMethod::Nearest;
    method->mDescription           = "Point-to-point Nearest";
    method->mId                    = mElementMapperPrefix + "100";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Point;
    method->mToElementsShapeType   = ElementType::Point;
    method->mElementMapperMethod   = ElementMapperMethod::Inverse;
    method->mDescription           = "Point-to-point Inverse";
    method->mId                    = mElementMapperPrefix + "101";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Point;
    method->mToElementsShapeType   = ElementType::Polyline;
    method->mElementMapperMethod   = ElementMapperMethod::Nearest;
    method->mDescription           = "Point-to-Polyline Nearest";
    method->mId                    = mElementMapperPrefix + "200";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Point;
    method->mToElementsShapeType   = ElementType::Polyline;
    method->mElementMapperMethod   = ElementMapperMethod::Inverse;
    method->mDescription           = "Point-to-Polyline Inverse";
    method->mId                    = mElementMapperPrefix + "201";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Point;
    method->mToElementsShapeType   = ElementType::Polygon;
    method->mElementMapperMethod   = ElementMapperMethod::Mean;
    method->mDescription           = "Point-to-polygon Mean";
    method->mId                    = mElementMapperPrefix + "300";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Point;
    method->mToElementsShapeType   = ElementType::Polygon;
    method->mElementMapperMethod   = ElementMapperMethod::Sum;
    method->mDescription           = "Point-to-polygon Sum";
    method->mId                    = mElementMapperPrefix + "301";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Polyline;
    method->mToElementsShapeType   = ElementType::Point;
    method->mElementMapperMethod   = ElementMapperMethod::Nearest;
    method->mDescription           = "Polyline-to-point Nearest";
    method->mId                    = mElementMapperPrefix + "400";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Polyline;
    method->mToElementsShapeType   = ElementType::Point;
    method->mElementMapperMethod   = ElementMapperMethod::Inverse;
    method->mDescription           = "Polyline-to-point Inverse";
    method->mId                    = mElementMapperPrefix + "401";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Polyline;
    method->mToElementsShapeType   = ElementType::Polygon;
    method->mElementMapperMethod   = ElementMapperMethod::WeightedMean;
    method->mDescription           = "Polyline-to-polygon Weighted Mean";
    method->mId                    = mElementMapperPrefix + "500";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Polyline;
    method->mToElementsShapeType   = ElementType::Polygon;
    method->mElementMapperMethod   = ElementMapperMethod::WeightedSum;
    method->mDescription           = "Polyline-to-polygon Weighted Sum";
    method->mId                    = mElementMapperPrefix + "501";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Polygon;
    method->mToElementsShapeType   = ElementType::Point;
    method->mElementMapperMethod   = ElementMapperMethod::Value;
    method->mDescription           = "Polygon-to-point Value";
    method->mId                    = mElementMapperPrefix + "600";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Polygon;
    method->mToElementsShapeType   = ElementType::Polyline;
    method->mElementMapperMethod   = ElementMapperMethod::WeightedMean;
    method->mDescription           = "Polygon-to-Polyline Weighted Mean";
    method->mId                    = mElementMapperPrefix + "700";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Polygon;
    method->mToElementsShapeType   = ElementType::Polyline;
    method->mElementMapperMethod   = ElementMapperMethod::WeightedSum;
    method->mDescription           = "Polygon-to-Polyline Weighted Sum";
    method->mId                    = mElementMapperPrefix + "701";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Polygon;
    method->mToElementsShapeType   = ElementType::Polygon;
    method->mElementMapperMethod   = ElementMapperMethod::WeightedMean;
    method->mDescription           = "Polygon-to-polygon Weighted Mean";
    method->mId                    = mElementMapperPrefix + "800";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Polygon;
    method->mToElementsShapeType   = ElementType::Polygon;
    method->mElementMapperMethod   = ElementMapperMethod::WeightedSum;
    method->mDescription           = "Polygon-to-polygon Weighted Sum";
    method->mId                    = mElementMapperPrefix + "801";
    mAvailableMethods.push_back(method);

    method                         = make_shared<SpatialMethod>();
    method->mFromElementsShapeType = ElementType::Polygon;
    method->mToElementsShapeType   = ElementType::Polygon;
    method->mElementMapperMethod   = ElementMapperMethod::Distribute;
    method->mDescription           = "Polygon-to-polygon Distribute";
    method->mId                    = mElementMapperPrefix + "802";
    mAvailableMethods.push_back(method);
}


// class SpaceAdaptedOutputFactory-----------------------------------------------------

SpaceAdaptedOutputFactory::SpaceAdaptedOutputFactory(const string &id)
{
    mId = id;
}

string SpaceAdaptedOutputFactory::GetCaption() const
{
    return mCaption;
}

void SpaceAdaptedOutputFactory::SetCaption(const string &value)
{
    mCaption = value;
}

string SpaceAdaptedOutputFactory::GetDescription() const
{
    return mDescription;
}

void SpaceAdaptedOutputFactory::SetDescription(const string &value)
{
    mDescription = value;
}

string SpaceAdaptedOutputFactory::GetId() const
{
    return mId;
}

vector<shared_ptr<IIdentifiable>>
SpaceAdaptedOutputFactory::GetAvailableAdaptedOutputIds(
    const shared_ptr<IOutput> &adaptee, const shared_ptr<IInput> &target)
{
    shared_ptr<Output> tsAdaptee = dynamic_pointer_cast<Output>(adaptee);

    // Only works with element set as spatial definition.
    if (!tsAdaptee || !tsAdaptee->GetElementSet())
    {
        return {};
    }

    vector<shared_ptr<IIdentifiable>> methods;
    GetAvailableOperationMethods(methods, tsAdaptee->GetElementSet()->GetElementType());

    // Check if the target is there and is a timespace input.
    shared_ptr<Input> tsTarget = dynamic_pointer_cast<Input>(target);
    if (!tsTarget || !tsTarget->GetElementSet())
    {
        return methods;
    }

    GetAvailableMappingMethods(
        methods,
        tsAdaptee->GetElementSet()->GetElementType(),
        tsTarget->GetElementSet()->GetElementType());

    return methods;
}

void SpaceAdaptedOutputFactory::GetAvailableOperationMethods(
    vector<shared_ptr<IIdentifiable>> &methods, ElementType sourceElementType)
{
    for (int i = 0; i < (int)mAvailableMethods.size(); i++)
    {
        const auto &availableMethod = mAvailableMethods[i];

        // Check if operation or mapping, only add operations here.
        if (availableMethod->mElementMapperMethod == ElementMapperMethod::None)
        {
            // In case of an operation, the target is not important.
            if (sourceElementType == mAvailableMethods[i]->mFromElementsShapeType)
            {
                auto tempVar = make_shared<Identifier>(mAvailableMethods[i]->mId);
                tempVar->SetDescription(mAvailableMethods[i]->mDescription);
                methods.push_back(tempVar);
            }
        }
    }
}

void SpaceAdaptedOutputFactory::GetAvailableMappingMethods(
    vector<shared_ptr<IIdentifiable>> &methods, ElementType sourceElementType,
    ElementType targetElementType)
{
    for (int i = 0; i < (int)mAvailableMethods.size(); i++)
    {
        auto availableMethod = mAvailableMethods[i];

        // Check if operation or mapping - only add mappings here
        if (availableMethod->mElementMapperMethod != ElementMapperMethod::None)
        {
            // In case of a mapping, the target is important
            if (sourceElementType == mAvailableMethods[i]->mFromElementsShapeType)
            {
                if (targetElementType == mAvailableMethods[i]->mToElementsShapeType)
                {
                    auto tempVar = make_shared<Identifier>(mAvailableMethods[i]->mId);
                    tempVar->SetDescription(mAvailableMethods[i]->mDescription);
                    methods.push_back(tempVar);
                }
            }
        }
    }
}

shared_ptr<IAdaptedOutput> SpaceAdaptedOutputFactory::CreateAdaptedOutput(
    const shared_ptr<IIdentifiable> &adaptedOutputId,
    const shared_ptr<IOutput> &adaptee, const shared_ptr<IInput> &target)
{
    shared_ptr<IElementSet> targetElmtSet = nullptr;
    auto                    tsTarget      = dynamic_pointer_cast<Input>(target);
    if (target && dynamic_pointer_cast<IElementSet>(tsTarget->GetSpatialDefinition()))
    {
        targetElmtSet = tsTarget->GetElementSet();
    }

    auto method = FindMethod(adaptedOutputId);

    shared_ptr<IAdaptedOutput> adaptedOutput;

    if (method->mElementMapperMethod != ElementMapperMethod::None)
    {
        if (!targetElmtSet)
        {
            throw invalid_argument(
                "Target not defined or spatial definition is not an element set. Can not create adaptor");
        }
        adaptedOutput =
            make_shared<SpaceMapAdaptor>(adaptedOutputId, adaptee, targetElmtSet);
    }
    else
    {
        if (mElementOperationPrefix + string("200") == adaptedOutputId->GetId())
        {
            adaptedOutput =
                make_shared<SpaceLengthAdaptor>(adaptedOutputId->GetId(), adaptee);
        }
        else if (mElementOperationPrefix + string("300") == adaptedOutputId->GetId())
        {
            adaptedOutput =
                make_shared<SpaceAreaAdaptor>(adaptedOutputId->GetId(), adaptee);
        }
    }

    if (!adaptedOutput)
    {
        throw invalid_argument("Adapted output id could not be found");
    }

    // Connect adaptor and adaptee.
    if (!VectorHelper::IsContained(adaptee->GetAdaptedOutputs(), adaptedOutput))
    {
        adaptee->AddAdaptedOutput(adaptedOutput);
    }

    return adaptedOutput;
}

shared_ptr<SpaceAdaptedOutputFactory::SpatialMethod>
SpaceAdaptedOutputFactory::FindMethod(shared_ptr<IIdentifiable> identifiable)
{
    for (auto availableMethod : mAvailableMethods)
    {
        if (availableMethod->mId == identifiable->GetId())
        {
            return availableMethod;
        }
    }
    throw invalid_argument(StringHelper::FormatSimple(
        "Invalid indentifier {}, identifier does not belong to this factory.",
        identifiable->GetId()));
}

vector<shared_ptr<IIdentifiable>> SpaceAdaptedOutputFactory::GetAvailableMethods(
    ElementType sourceElementType, ElementType targetElementType)
{
    vector<shared_ptr<IIdentifiable>> methods;
    GetAvailableOperationMethods(methods, sourceElementType);
    GetAvailableMappingMethods(methods, sourceElementType, targetElementType);
    return vector<shared_ptr<IIdentifiable>>(methods);
}

shared_ptr<IDescribable> SpaceAdaptedOutputFactory::GetAdaptedOutputDescription(
    shared_ptr<IIdentifiable> identifiable)
{
    auto spatialMethod = FindMethod(identifiable);
    return make_shared<Describer>(spatialMethod->mId, spatialMethod->mDescription);
}

bool SpaceAdaptedOutputFactory::HasId(shared_ptr<IIdentifiable> identifiable)
{
    for (auto availableMethod : mAvailableMethods)
    {
        if (availableMethod->mId == identifiable->GetId())
        {
            return true;
        }
    }
    return false;
}

ElementMapperMethod
SpaceAdaptedOutputFactory::GetMethod(const shared_ptr<IIdentifiable> &identifiable)
{
    return FindMethod(identifiable)->mElementMapperMethod;
}

ElementType
SpaceAdaptedOutputFactory::GetToElementType(shared_ptr<IIdentifiable> identifiable)
{
    return FindMethod(identifiable)->mToElementsShapeType;
}

vector<shared_ptr<IArgument>> SpaceAdaptedOutputFactory::GetAdaptedOutputArguments(
    shared_ptr<IIdentifiable> methodIdentifier)
{
    if (!(StringHelper::StartsWith(methodIdentifier->GetId(), mElementMapperPrefix)
          || StringHelper::StartsWith(
              methodIdentifier->GetId(), mElementOperationPrefix)))
    {
        throw runtime_error(StringHelper::FormatSimple(
            "Unknown method identifier: [{}] .", methodIdentifier));
    }

    for (int i = 0; i < (int)mAvailableMethods.size(); i++)
    {
        auto method = mAvailableMethods[i];

        if (method->mId == methodIdentifier->GetId())
        {
            if (method->mElementMapperMethod == ElementMapperMethod::None)
            {
                auto arguments = vector<shared_ptr<IArgument>>();

                auto tempVar = make_shared<ArgumentString>("Caption", method->mId);
                tempVar->SetDescription(
                    "Internal ElementOperation AdaptedOutput Caption");
                arguments.push_back(tempVar);

                auto tempVar2 =
                    make_shared<ArgumentString>("Description", method->mDescription);
                tempVar2->SetDescription("Operation description");
                arguments.push_back(tempVar2);

                auto tempVar3 = make_shared<ArgumentString>("Type", "SpatialOperation");
                tempVar3->SetDescription("Using an Element Operator");
                arguments.push_back(tempVar3);

                auto tempVar4 = make_shared<ArgumentString>(
                    "FromElementType",
                    StringHelper::ToString((int)method->mFromElementsShapeType));
                tempVar4->SetDescription("Valid From-Element Types");
                arguments.push_back(tempVar4);
                return arguments;
            }
            else
            {
                auto arguments = vector<shared_ptr<IArgument>>();

                auto tempVar = make_shared<ArgumentString>("Caption", method->mId);
                tempVar->SetDescription("Internal ElementMapper AdaptedOutput Caption");
                arguments.push_back(tempVar);

                auto tempVar2 =
                    make_shared<ArgumentString>("Description", method->mDescription);
                tempVar2->SetDescription("Mapping description");
                arguments.push_back(tempVar2);

                auto tempVar3 = make_shared<ArgumentString>("Type", "SpatialMapping");
                tempVar3->SetDescription("Using the ElementMapper");
                arguments.push_back(tempVar3);

                auto tempVar4 = make_shared<ArgumentString>(
                    "FromElementType",
                    StringHelper::ToString((int)method->mFromElementsShapeType));
                tempVar4->SetDescription("Valid From-Element Types");
                arguments.push_back(tempVar4);

                auto tempVar5 = make_shared<ArgumentString>(
                    "ToElementType",
                    StringHelper::ToString((int)method->mToElementsShapeType));
                tempVar5->SetDescription("Valid To-Element Types");
                arguments.push_back(tempVar5);
                return arguments;
            }
        }
    }
    throw runtime_error(StringHelper::FormatSimple(
        "Unknown methodID: [{}] .", methodIdentifier->GetId()));
}


}  // namespace OpenOasis::CommImp