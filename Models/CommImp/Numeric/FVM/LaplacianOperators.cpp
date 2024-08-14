/** ***********************************************************************************
 *    @File      :  LaplacianOperators.cpp
 *    @Brief     :  Laplacian Operators.
 *
 ** ***********************************************************************************/
#include "LaplacianOperators.h"
#include "Models/Utils/Exception.h"


namespace OpenOasis::CommImp::Numeric::FVM
{
using namespace std;
using namespace Utils;


// ------------------------------------------------------------------------------------

static const string LAPLACIAN01 = "FvmLaplacian01";


REGISTER_CLS(FvmOperator, Laplacian01, LAPLACIAN01)


// ------------------------------------------------------------------------------------

Laplacian01::Laplacian01()
{
    mMode = OperatorMode::Implicit;
    mType = OperatorType::LaplacianOp;
    mName = LAPLACIAN01;
}

vector<string> Laplacian01::Validate() const
{
    vector<string> errors = FvmOperator::Validate();

    if (!mVarField.sField)
    {
        string msg = "FvcLaplacian01: only process scalar field which not specified.";
        errors.push_back(msg);
        Logger::Error(msg);
    }

    return errors;
}

optional<vector<LinearEqs>> Laplacian01::GetLinearEqs() const
{
    return vector<LinearEqs>{mEquations};
}

void Laplacian01::Process()
{
    size_t nCells = mGrid->GetNumCells();

    Matrix<real> A(nCells, nCells);
    vector<real> b(nCells);

#pragma omp parallel for
    for (size_t i = 0; i < nCells; i++) {}

    mEquations = LinearEqs(A, b);
}


}  // namespace OpenOasis::CommImp::Numeric::FVM