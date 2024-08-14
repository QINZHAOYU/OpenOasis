/** ***********************************************************************************
 *    @File      :  boundaries.cpp
 *    @Brief     :  None
 *
 ** ***********************************************************************************/
#include "boundaries.h"
#include "Models/Utils/Exception.h"


namespace OpenOasis::CommImp::Numeric::FVM
{
using namespace std;
using namespace Utils;


// ------------------------------------------------------------------------------------

static const string BOUNDARY01 = "FvcBoundary01";


REGISTER_CLS(FvmBoundary, Boundary01, BOUNDARY01)


// ------------------------------------------------------------------------------------

Boundary01::Boundary01(const string &variable) : FvmBoundary(variable)
{
    mMode = OperatorMode::Explicit;
    mType = OperatorType::GradOp;
    mName = BOUNDARY01;
}

vector<string> Boundary01::Validate() const
{
    vector<string> errors = FvmBoundary::Validate();

    if (!mVarField.sField)
    {
        string msg = "FvcBoundary01: only process scalar field which not specified.";
        errors.push_back(msg);
        Logger::Error(msg);
    }

    return errors;
}

optional<NumericField> Boundary01::GetResult() const
{
    return NumericField(mVarField.id, mCellGradient);
}

void Boundary01::Process()
{
    size_t nFaces = mGrid->GetNumFaces();
    mFaceField.Resize(nFaces);

    size_t nCells = mGrid->GetNumCells();
    mCellGradient.Resize(nCells);

    GenerateFaceField();
    GenerateCellGradient();
}

void Boundary01::GenerateFaceField()
{
    const auto &boundaryFaces = mGrid->GetBoundaryFaces();
    const auto &cField        = mVarField.sField.value();

#pragma omp parallel for
    for (auto faceIdx : boundaryFaces)
    {
        const auto &face = mGrid->GetFace(faceIdx);
        const auto &bc   = GetBoundaryCondition(faceIdx);

        if (bc.type == BoundaryType::ValueBound)
        {
            mFaceField(faceIdx) = bc.conds.at("value");
        }
        else if (bc.type == BoundaryType::FluxBound)
        {
            size_t cIndex = face.cellIndexes.at(0);

            real dist = mGrid->GetCellToFaceDist(cIndex, faceIdx);
            real gb   = face.area / dist;

            real coe  = GetFaceCoefficient(faceIdx);
            real cVal = cField(cIndex);
            real fVal = cVal - bc.conds.at("flux") / (coe * gb);

            mFaceField(faceIdx) = fVal;
        }
        else
        {
            throw NotImplementedException(StringHelper::FormatSimple(
                "FvcBoundary01: face [{}] has unsupported boundary type [{}].",
                faceIdx,
                bc.type));
        }
    }
}

void Boundary01::GenerateCellGradient()
{
    const auto &boundaryFaces = mGrid->GetBoundaryFaces();

#pragma omp parallel for
    for (auto faceIdx : boundaryFaces)
    {
        const auto &face = mGrid->GetFace(faceIdx);
        size_t      cIdx = face.cellIndexes.at(0);
        const auto &cell = mGrid->GetCell(cIdx);
        real        fVal = mFaceField(faceIdx);

        Vector<real> temp = face.normal * face.area * fVal;
        temp *= (1. / cell.volume);

        mCellGradient(cIdx) += temp;
    }
}


}  // namespace OpenOasis::CommImp::Numeric::FVM