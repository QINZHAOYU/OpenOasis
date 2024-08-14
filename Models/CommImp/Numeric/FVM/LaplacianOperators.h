/** ***********************************************************************************
 *    Copyright (C) 2023, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  LaplacianOperators.h
 *    @License   :  Apache-2.0
 *
 ** ***********************************************************************************/
#pragma once
#include "FvmOperator.h"
#include <functional>
#include <memory>


namespace OpenOasis::CommImp::Numeric::FVM
{
/// @brief Laplacian01 operator for scalar field in cell domain.
class Laplacian01 : public LaplacianOperator
{
private:
    LinearEqs mEquations;

public:
    Laplacian01();
    virtual ~Laplacian01() = default;


    std::optional<std::vector<LinearEqs>> GetLinearEqs() const override;

    std::vector<std::string> Validate() const override;

    void Process() override;

private:
};


}  // namespace OpenOasis::CommImp::Numeric::FVM