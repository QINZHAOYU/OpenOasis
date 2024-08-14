/** ***********************************************************************************
 *    @File      :  GradOperators.cpp
 *    @Brief     :  Gradient operators for FVM.
 *
 ** ***********************************************************************************/
#include "GradOperators.h"
#include "Models/Utils/Exception.h"


namespace OpenOasis::CommImp::Numeric::FVM
{
using namespace std;
using namespace Utils;


// ------------------------------------------------------------------------------------

static const string GRAD01 = "FvcGrad01";


REGISTER_CLS(FvmOperator, Grad01, GRAD01)


// ------------------------------------------------------------------------------------

Grad01::Grad01()
{
    mMode = OperatorMode::Explicit;
    mType = OperatorType::GradOp;
    mName = GRAD01;
}

vector<string> Grad01::Validate() const
{
    vector<string> errors = FvmOperator::Validate();

    if (!mVarField.sField)
    {
        string msg = "FvcGrad01: only process scalar field which not specified.";
        errors.push_back(msg);
        Logger::Error(msg);
    }

    return errors;
}

optional<NumericField> Grad01::GetResult() const
{
    return NumericField(mVarField.id, mCellGradient);
}

void Grad01::Initialize()
{
    size_t nFaces = mGrid->GetNumFaces();
    mFaceField.Resize(nFaces);
    mFaceCorrVec.Resize(nFaces);

    size_t nCells = mGrid->GetNumCells();
    mCellGradient.Resize(nCells);
}

void Grad01::GenerateFaceCorrVec()
{
    size_t nFaces = mGrid->GetNumFaces();

#pragma omp parallel for
    for (size_t i = 0; i < nFaces; i++)
    {
        const auto &face      = mGrid->GetFace(i);
        const auto &faceCells = face.cellIndexes;
        if (faceCells.size() == 2)
        {
            const auto lCellCenter = mGrid->GetCell(faceCells[0]).centroid;
            const auto rCellCenter = mGrid->GetCell(faceCells[1]).centroid;

            Spatial::Coordinate midPoint;
            midPoint.x = (lCellCenter.x + rCellCenter.x) / 2;
            midPoint.y = (lCellCenter.y + rCellCenter.y) / 2;
            midPoint.z = (lCellCenter.z + rCellCenter.z) / 2;

            real x = face.centroid.x - midPoint.x;
            real y = face.centroid.y - midPoint.y;
            real z = face.centroid.z - midPoint.z;

            mFaceCorrVec(i) = {x, y, z};
        }
    }
}

void Grad01::Process()
{
    Initialize();
    GenerateFaceCorrVec();
    GenerateFaceField();
    UpdateCellGradient();

    for (int i = 0; i < 2; i++)
    {
        CorrectFaceField();
        UpdateCellGradient();
    }
}

void Grad01::GenerateFaceField()
{
    size_t      nFaces = mGrid->GetNumFaces();
    const auto &cField = mVarField.sField.value();

#pragma omp parallel for
    for (size_t i = 0; i < nFaces; i++)
    {
        const auto &face      = mGrid->GetFace(i);
        const auto &faceCells = face.cellIndexes;
        if (faceCells.size() == 2)
        {
            real lVal = cField(faceCells[0]);
            real rVal = cField(faceCells[1]);

            mFaceField(i) = (lVal + rVal) / 2;
        }
        else
        {
            mFaceField(i) = 0.0;
        }
    }
}

void Grad01::UpdateCellGradient()
{
    size_t nCells = mGrid->GetNumCells();

#pragma omp parallel for
    for (size_t i = 0; i < nCells; i++)
    {
        const auto &cell = mGrid->GetCell(i);

        Vector<real> temp = 0;
        for (size_t fIdx : cell.faceIndexes)
        {
            const auto &face = mGrid->GetFace(fIdx);
            real       &fVal = mFaceField(fIdx);

            temp += face.normal * face.area * fVal;
        }

        temp *= (1. / cell.volume);
        mCellGradient(i) = temp;
    }
}

void Grad01::CorrectFaceField()
{
#pragma omp parallel for
    for (int i = 0; i < mGrid->GetNumFaces(); i++)
    {
        const auto &faceCells = mGrid->GetFace(i).cellIndexes;
        if (faceCells.size() == 1)
            continue;

        const auto &lGrad       = mCellGradient(faceCells[0]);
        const auto &rGrad       = mCellGradient(faceCells[1]);
        const auto &faceCorrVec = mFaceCorrVec(i);

        real val = mFaceField(i) + (lGrad + rGrad) * 0.5 * faceCorrVec;

        mFaceField(i) = val;
    }
}


}  // namespace OpenOasis::CommImp::Numeric::FVM