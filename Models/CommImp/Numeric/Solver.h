/** ***********************************************************************************
 *    Copyright (C) 2023, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Solver.h
 *    @License   :  Apache-2.0
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/CommImp/Spatial/Grid.h"
#include "Config.h"
#include "Equation.h"
#include "Operator.h"
#include "Boundary.h"


namespace OpenOasis::CommImp::Numeric
{
using CommImp::Spatial::Grid;
using Utils::real;

using SolverParam = Configuration;


/// @brief Abstract solver class.
/// @details Each solver implementes an algorithm to complete the solution of the
/// equation. The solver is responsible for parsing the equation expression to be
/// solved, and discretizing the equation items in the computational domain,
/// combining them into a matrix, and solving it.
///
/// The solver is also responsible for initializing the relevant field quantities,
/// and providing specific discrete and stepping scheme.
///
/// The solver doesn't responsible for any IO operations, but provides a way to
/// access the solution field quantities.
class Solver
{
public:
    ///////////////////////////////////////////////////////////////////////////////////
    // Parameter setting and Configuration.
    //

    virtual const std::vector<std::string> &GetParametersRequired() const = 0;

    virtual void SetParameter(const SolverParam &param) = 0;

    virtual void SetGrid(const std::shared_ptr<Grid> &grid) = 0;

    virtual std::string GetName() = 0;

    ///////////////////////////////////////////////////////////////////////////////////
    // Boundary condition and initialization.
    //

    virtual void SetBoundaryCondition(
        size_t patchId, const std::string &varName, const std::string &bcType,
        const std::vector<double>            &bcTimeseries,
        const std::vector<BoundaryCondition> &bcValueset) = 0;

    virtual void SetInitialField(const std::shared_ptr<NumericField> &varField) = 0;

    virtual void SetInitialField(const NumericValue &var) = 0;

    virtual void SetCoefficient(const std::shared_ptr<NumericField> &coefField) = 0;

    virtual void SetCoefficient(const NumericValue &coef) = 0;

    ///////////////////////////////////////////////////////////////////////////////////
    // Equation parsing and step advancing.
    //

    virtual void AddEquation(const std::shared_ptr<Equation> &eq) = 0;

    virtual void AddOperator(const std::shared_ptr<Operator> &op) = 0;

    virtual void AddBoundary(const std::shared_ptr<Boundary> &bd) = 0;

    virtual std::vector<std::string> Activate() = 0;

    virtual void Advance() = 0;

    ///////////////////////////////////////////////////////////////////////////////////
    // Solution accessing.
    //

    virtual double GetElapsedTime() const = 0;

    virtual std::vector<std::string> GetVariables() const = 0;

    virtual std::optional<std::vector<std::shared_ptr<LinearEqs>>>
    GetLinearEqs() const = 0;

    virtual std::optional<std::shared_ptr<NumericField>>
    GetSolutions(const std::string &var) const = 0;
};

}  // namespace OpenOasis::CommImp::Numeric