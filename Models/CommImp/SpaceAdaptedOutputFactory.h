/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  SpaceAdaptedOutputFactory.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Factory to create instance of spatial `IAdaptedOutput`.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IAdaptedOutputFactory.h"
#include "Models/Inc/IElementSet.h"
#include "Models/CommImp/DevSupports/ElementMapper.h"


namespace OpenOasis
{
namespace CommImp
{
using namespace DevSupports;

/// @brief Space adapted output factory of `IAdaptedOutputFactory` implementation.
class SpaceAdaptedOutputFactory : public IAdaptedOutputFactory
{
private:
    std::string mCaption;
    std::string mDescription;
    std::string mId;

public:
    virtual ~SpaceAdaptedOutputFactory()
    {}

    SpaceAdaptedOutputFactory(const std::string &id);

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IDescribable`.
    //

    virtual void SetCaption(const std::string &value) override;

    virtual std::string GetCaption() const override;

    virtual void SetDescription(const std::string &value) override;

    virtual std::string GetDescription() const override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IIdentifiable`.
    //

    virtual std::string GetId() const override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IAdaptedOutputFactory`.
    //

    virtual std::vector<std::shared_ptr<IIdentifiable>> GetAvailableAdaptedOutputIds(
        const std::shared_ptr<IOutput> &adaptee,
        const std::shared_ptr<IInput>  &target) override;

    virtual std::shared_ptr<IAdaptedOutput> CreateAdaptedOutput(
        const std::shared_ptr<IIdentifiable> &adaptedOutputId,
        const std::shared_ptr<IOutput>       &adaptee,
        const std::shared_ptr<IInput>        &target) override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Static methods.
    //

    static std::vector<std::shared_ptr<IIdentifiable>> GetAvailableMethods(
        std::shared_ptr<IOutput> adaptee, std::shared_ptr<IInput> target);

    static std::shared_ptr<IAdaptedOutput> CreateAdaptedOutputMethod(
        std::shared_ptr<IIdentifiable> adaptedOutputId,
        std::shared_ptr<IOutput> adaptee, std::shared_ptr<IInput> target);

    static std::shared_ptr<IAdaptedOutput> CreateAdaptedOutputMethod(
        std::shared_ptr<IIdentifiable> adaptedOutputId,
        std::shared_ptr<IOutput> adaptee, std::shared_ptr<IElementSet> taregtElmtSet);

    /// @brief Gives a list of id's + descriptions (strings) for available methods
    /// given the combination of fromElementType and toElementType.
    ///
    /// @param sourceElementType Element type of elements in the fromElementSet.
    /// @param targetElementType Element type of elements in the toElementSet.
    ///
    /// @return ArrayList of method descriptions.
    static std::vector<std::shared_ptr<IIdentifiable>>
    GetAvailableMethods(ElementType sourceElementType, ElementType targetElementType);

    /// @brief Gives a list of descriptions (strings) for available mapping methods
    /// given the combination of fromElementType and toElementType
    ///
    /// @param sourceElementType Element type of elements in the fromElementSet.
    /// @param targetElementType Element type of elements in the toElementSet.
    /// @param[out] methods ArrayList of method descriptions.
    static void GetAvailableMappingMethods(
        std::vector<std::shared_ptr<IIdentifiable>> &methods,
        ElementType sourceElementType, ElementType targetElementType);

    /// @brief Gives a list of descriptions (strings) for available operation methods
    /// given the source ElementType.
    ///
    /// @param sourceElementType Element type of elements in the source ElementSet.
    /// @param[out] methods ArrayList of method descriptions.
    static void GetAvailableOperationMethods(
        std::vector<std::shared_ptr<IIdentifiable>> &methods,
        ElementType                                  sourceElementType);

    static std::shared_ptr<IDescribable>
    GetAdaptedOutputDescription(std::shared_ptr<IIdentifiable> identifiable);

    /// @brief Checks if the provided id was created by/can be used by this factory.
    ///
    /// @param identifiable Id to check.
    ///
    /// @return True of identifier can be used with this factory.
    static bool HasId(std::shared_ptr<IIdentifiable> identifiable);

    static ElementMapperMethod
    GetMethod(const std::shared_ptr<IIdentifiable> &identifiable);

    static ElementType GetToElementType(std::shared_ptr<IIdentifiable> identifiable);

    /// @brief This method will return an ArrayList of `IAdaptedOutput` that the
    /// ElementMapper provides when mapping from the ElementType specified
    /// in the method argument.
    static std::vector<std::shared_ptr<IArgument>>
    GetAdaptedOutputArguments(std::shared_ptr<IIdentifiable> methodIdentifier);

private:
    ///////////////////////////////////////////////////////////////////////////////////
    // Static spatial adapted methods constructor.
    //

    /// @brief Static spatial adapted methods constructor.
    class StaticConstructor
    {
    public:
        StaticConstructor();
    };

    static SpaceAdaptedOutputFactory::StaticConstructor mStaticConstructor;

    static const std::string mElementMapperPrefix;
    static const std::string mElementOperationPrefix;

private:
    ///////////////////////////////////////////////////////////////////////////////////
    // Spatial adapted method.
    //

    /// @brief Spatial adapted method.
    class SpatialMethod
    {
    public:
        std::string mDescription;
        ElementType mFromElementsShapeType;
        std::string mId;
        ElementType mToElementsShapeType;

        ElementMapperMethod mElementMapperMethod = ElementMapperMethod::None;
    };

    static std::vector<std::shared_ptr<SpatialMethod>> mAvailableMethods;

    static std::shared_ptr<SpatialMethod>
    FindMethod(std::shared_ptr<IIdentifiable> identifiable);
};

}  // namespace CommImp
}  // namespace OpenOasis