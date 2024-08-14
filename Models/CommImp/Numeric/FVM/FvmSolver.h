/** ***********************************************************************************
 *    Copyright (C) 2023, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  FvmSolver.h
 *    @License   :  Apache-2.0
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/CommImp/Numeric/Solver.h"
#include "Models/Utils/RegisterFactory.h"


namespace OpenOasis::CommImp::Numeric::FVM
{
using namespace OpenOasis::Utils;


/// @brief FVM solver base class.
class FvmSolver : public Solver
{};


// Register FVM solver factory.

class FvmSolverRegister;
RegisterFactory(FvmSolver);

}  // namespace OpenOasis::CommImp::Numeric::FVM