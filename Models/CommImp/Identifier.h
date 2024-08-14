/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Identifier.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Generic implementation of `IIdentifiable` interface.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IIdentifiable.h"


namespace OpenOasis::CommImp
{
/// @brief Generic implementation of `IIdentifiable` interface.
class Identifier : public IIdentifiable
{
private:
    std::string mCaption;
    std::string mDescription;
    std::string mId;

public:
    virtual ~Identifier()
    {}

    Identifier(const std::string &id);

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IDescribable`.
    //

    virtual std::string GetCaption() const override;

    virtual void SetCaption(const std::string &value) override;

    virtual std::string GetDescription() const override;

    virtual void SetDescription(const std::string &value) override;

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IIdentifiable`.
    //

    virtual std::string GetId() const override;
};
}  // namespace OpenOasis::CommImp