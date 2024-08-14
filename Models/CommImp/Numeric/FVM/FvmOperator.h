/** ***********************************************************************************
 *    Copyright (C) 2024, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  FvmOperator.h
 *    @License   :  Apache-2.0
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/CommImp/Numeric/Operator.h"
#include "Models/Utils/RegisterFactory.h"
#include "Models/Utils/Logger.h"
#include "Models/Utils/StringHelper.h"
#include <algorithm>


namespace OpenOasis::CommImp::Numeric::FVM
{
using namespace OpenOasis::Utils;
using Spatial::Grid;


/// @brief FVM operator base class.
/// @note Fvm operator doesn't handle boundary faces.
class FvmOperator : public Operator
{
protected:
    std::vector<std::string>   mParametersRequired = {};
    std::vector<OperatorParam> mParams             = {};
    OperatorMode               mMode               = OperatorMode::UnknownMode;
    OperatorType               mType               = OperatorType::UnknownOp;
    std::string                mName               = "";
    std::string                mVariable           = "";

    std::shared_ptr<Grid>       mGrid;
    NumericField                mVarField;
    std::optional<NumericField> mFaceCoeField;
    std::optional<NumericValue> mFaceCoeValue;

public:
    FvmOperator()          = default;
    virtual ~FvmOperator() = default;

    const std::vector<std::string> &GetParametersRequired() const override
    {
        return mParametersRequired;
    }

    OperatorMode GetMode() const override
    {
        return mMode;
    }

    OperatorType GetType() const override
    {
        return mType;
    }

    std::string GetName() const override
    {
        return mName;
    }

    std::string GetVariable() const override
    {
        return mVariable;
    }

    void SetParameter(const OperatorParam &value) override
    {
        if (std::find(mParametersRequired.begin(), mParametersRequired.end(), value.key)
            != mParametersRequired.end())
        {
            mParams.push_back(value);
        }
    }

    void SetField(const NumericField &field) override
    {
        mVarField = field;
    }

    void SetCoefficient(const NumericField &coef) override
    {
        mFaceCoeField = coef;
    }

    void SetCoefficient(const NumericValue &coef) override
    {
        mFaceCoeValue = coef;
    }

    void SetGrid(const std::shared_ptr<Grid> &grid) override
    {
        mGrid = grid;
    }

    std::vector<std::string> Validate() const override
    {
        std::vector<std::string> errors;

        if (!mGrid)
        {
            errors.push_back(
                StringHelper::FormatSimple("Operator [{}] grid is not set.", mName));
        }

        if (mVarField.id.empty())
        {
            errors.push_back(StringHelper::FormatSimple(
                "Operator [{}] data field is not set.", mName));
        }

        if (!mFaceCoeField && !mFaceCoeValue)
        {
            errors.push_back(StringHelper::FormatSimple(
                "Operator [{}] coefficient is not set.", mName));
        }

        for (const auto &err : errors)
        {
            Logger::Error(err);
        }

        return errors;
    }

    std::optional<std::vector<LinearEqs>> GetLinearEqs() const override
    {
        return std::nullopt;
    }

    std::optional<NumericField> GetResult() const override
    {
        return std::nullopt;
    }

protected:
    inline real GetFaceCoefficient(size_t i)
    {
        if (mFaceCoeValue)
            return mFaceCoeValue.value().sValue.value();

        return mFaceCoeField.value().sField.value()(i);
    }
};


// Commomly used operators.

using CurlOperator      = FvmOperator;
using DivOperator       = FvmOperator;
using GradOperator      = FvmOperator;
using LaplacianOperator = FvmOperator;
using DdtOperator       = FvmOperator;
using D2dt2Operator     = FvmOperator;
using SuOperator        = FvmOperator;
using SpOperator        = FvmOperator;


// Register FVM operator factory.

class FvmOperatorRegister;
RegisterFactory(FvmOperator);


}  // namespace OpenOasis::CommImp::Numeric::FVM