/** ***********************************************************************************
 *    Copyright (C) 2024, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  FvmBoundary.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide base implementation of the Boundary for the FVM models.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/CommImp/Numeric/Boundary.h"
#include "Models/Utils/RegisterFactory.h"
#include "Models/Utils/Logger.h"
#include "Models/Utils/StringHelper.h"
#include "Models/Utils/MapHelper.h"


namespace OpenOasis::CommImp::Numeric::FVM
{
using namespace OpenOasis::Utils;

/// @brief FVM boundary class.
class FvmBoundary : public Boundary
{
protected:
    std::vector<std::string>   mParametersRequired = {};
    std::vector<OperatorParam> mParams             = {};
    BoundaryCondition          mDefaultBC          = {};
    OperatorMode               mMode               = OperatorMode::UnknownMode;
    OperatorType               mType               = OperatorType::UnknownOp;
    std::string                mName               = "";
    std::string                mVariable           = "";

    std::shared_ptr<Spatial::Grid>                mGrid;
    NumericField                                  mVarField;
    std::optional<NumericField>                   mFaceCoeField;
    std::optional<NumericValue>                   mFaceCoeValue;
    std::unordered_map<size_t, BoundaryCondition> mBoundaryConditions;

public:
    virtual ~FvmBoundary() = default;
    FvmBoundary(const std::string &variable) : mVariable(variable)
    {}

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

    void SetGrid(const std::shared_ptr<Spatial::Grid> &grid) override
    {
        mGrid = grid;
    }

    void SetDefaultBoundaryCondition(const BoundaryCondition &bc) override
    {
        mDefaultBC = bc;
    }

    const BoundaryCondition &GetDefaultBoundaryCondition() const override
    {
        return mDefaultBC;
    }

    void SetBoundaryCondition(
        std::vector<size_t> faceIndexes, const BoundaryCondition &bc) override
    {
        for (size_t faceIdx : faceIndexes)
        {
            if (mBoundaryConditions.count(faceIdx) != 0)
            {
                Logger::Warn(StringHelper::FormatSimple(
                    "Boundary condition for face index [{}] already set.\
                     Overwriting with new value or type.",
                    faceIdx));
            }

            mBoundaryConditions[faceIdx] = bc;
        }
    }

    const BoundaryCondition &GetBoundaryCondition(size_t faceIdx) const override
    {
        if (mBoundaryConditions.count(faceIdx) == 0)
        {
            return mDefaultBC;
        }

        return mBoundaryConditions.at(faceIdx);
    }

    std::vector<std::string> Validate() const override
    {
        std::vector<std::string> errors;

        if (!mGrid)
        {
            errors.push_back(
                StringHelper::FormatSimple("Boundary [{}] grid is not set.", mName));
        }

        if (mVarField.id.empty())
        {
            errors.push_back(StringHelper::FormatSimple(
                "Boundary [{}] data field is not set.", mName));
        }

        if (mVarField.id != mVariable)
        {
            errors.push_back(StringHelper::FormatSimple(
                "Boundary [{}] data field id [{}] does not match with variable [{}].",
                mName,
                mVarField.id,
                mVariable));
        }

        if (!mFaceCoeField && !mFaceCoeValue)
        {
            errors.push_back(StringHelper::FormatSimple(
                "Boundary [{}] coefficient is not set.", mName));
        }

        const auto &boundaryFaces = mGrid->GetBoundaryFaces();
        const auto &settedFaces   = MapHelper::GetKeys(mBoundaryConditions);
        if (boundaryFaces != settedFaces
            && mDefaultBC.type == BoundaryType::UnknownBound)
        {
            errors.push_back(StringHelper::FormatSimple(
                "Boundary [{}] has no default boundary condition set.", mName));
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


// Register FVM boundary factory.

class FvmBoundaryRegister;
RegisterFactory(FvmBoundary);


}  // namespace OpenOasis::CommImp::Numeric::FVM