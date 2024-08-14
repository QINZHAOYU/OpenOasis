/** ***********************************************************************************
 *    Copyright (C) 2024, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  GradOperators.h
 *    @License   :  Apache-2.0
 *
 ** ***********************************************************************************/
#pragma once
#include "FvmOperator.h"
#include <functional>
#include <memory>


namespace OpenOasis::CommImp::Numeric::FVM
{
/// @brief Grad01 operator for scalar field in cell domain.
/// @details The Finite Volume Method in Computational Fluid Dynamics, chapter 9.1. op2.
class Grad01 : public GradOperator
{
private:
    VectorFieldFp mFaceCorrVec;
    ScalarFieldFp mFaceField;
    VectorFieldFp mCellGradient;

public:
    Grad01();
    virtual ~Grad01() = default;

    std::optional<NumericField> GetResult() const override;

    std::vector<std::string> Validate() const override;

    void Process() override;

private:
    void Initialize();
    void GenerateFaceCorrVec();
    void GenerateFaceField();
    void CorrectFaceField();
    void UpdateCellGradient();
};


}  // namespace OpenOasis::CommImp::Numeric::FVM