/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  TimeAdaptedOutputFactory.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Factory to create instance of temporal `IAdaptedOutput`.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IAdaptedOutputFactory.h"


namespace OpenOasis
{
namespace CommImp
{
/// @brief Time adapted output factory of `IAdaptedOutputFactory` implementation.
class TimeAdaptedOutputFactory : public IAdaptedOutputFactory
{
private:
    std::string mId;
    std::string mCaption;
    std::string mDescription;

    std::vector<std::shared_ptr<IAdaptedOutput>> mAdaptorsCreatedSoFar;

public:
    virtual ~TimeAdaptedOutputFactory()
    {}

    TimeAdaptedOutputFactory(const std::string &id);

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IDescribable`.
    //

    virtual void SetDescription(const std::string &value) override;

    virtual std::string GetDescription() const override;

    virtual std::string GetCaption() const override;

    virtual void SetCaption(const std::string &value) override;

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
};

}  // namespace CommImp
}  // namespace OpenOasis