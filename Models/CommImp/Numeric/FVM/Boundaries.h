/** ***********************************************************************************
 *    Copyright (C) 2024, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Boundaries.h
 *    @License   :  Apache-2.0
 *
 ** ***********************************************************************************/
#pragma once
#include "FvmBoundary.h"


namespace OpenOasis::CommImp::Numeric::FVM
{
/// @brief Boundary operator calculates gradientfor scalar variable in cell domain.
/// @details The Finite Volume Method in Computational Fluid Dynamics, chapter 8.3.
class Boundary01 : public FvmBoundary
{
private:
    ScalarFieldFp mFaceField;
    VectorFieldFp mCellGradient;

public:
    Boundary01(const std::string &variable);
    virtual ~Boundary01() = default;


    std::optional<NumericField> GetResult() const override;

    std::vector<std::string> Validate() const override;

    void Process() override;

private:
    void GenerateFaceField();
    void GenerateCellGradient();
};

}  // namespace OpenOasis::CommImp::Numeric::FVM