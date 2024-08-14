/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Describer.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Generic implementation of `IDescribable` interface.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IDescribable.h"


namespace OpenOasis::CommImp
{
/// @brief Generic implementation of `IDescribable` interface.
class Describer : public IDescribable
{
private:
    std::string mCaption     = "";
    std::string mDescription = "";

public:
    virtual ~Describer()
    {}

    Describer();
    Describer(const std::string &caption, const std::string &desc);

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IDescribable`.
    //

    virtual std::string GetCaption() const override;

    virtual void SetCaption(const std::string &value) override;

    virtual std::string GetDescription() const override;

    virtual void SetDescription(const std::string &value) override;
};
}  // namespace OpenOasis::CommImp