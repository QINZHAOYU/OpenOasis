/** ***********************************************************************************
 *    Copyright (C) 2023, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Operator.h
 *    @License   :  Apache-2.0
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/CommImp/Spatial/Grid.h"
#include "Matrix.h"
#include "Config.h"
#include <tuple>
#include <optional>


namespace OpenOasis::CommImp::Numeric
{
using Utils::real;
using Spatial::Grid;


/// @brief Linear equations consisting of coefficient matrix and source term vector.
/// @details The right-hand side of the linear equation is the source term,
/// which could be a scalar or a vector field.
using LinearEqs = std::tuple<Matrix<real>, std::vector<real>>;

using OperatorParam = Configuration;


/// @brief Type of operator.
enum class OperatorType
{
    GradOp,
    DivOp,
    LaplacianOp,
    CurlOp,
    DdtOp,
    D2dt2Op,
    SuOp,
    SpOp,
    UnknownOp,
};


/// @brief Mode of operator, neither explicit nor implicit.
enum class OperatorMode
{
    Explicit,
    Implicit,
    UnknownMode,
};

/// @brief Abstract operator class.
/// @details Each numerical operator can discretize specific equation terms, or say
/// carry out specific operations on field quantities, and return corresponding
/// coefficient matrix and source term vector.
class Operator
{
public:
    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for operator setting.
    //

    virtual const std::vector<std::string> &GetParametersRequired() const = 0;

    virtual void SetParameter(const OperatorParam &param) = 0;

    virtual void SetGrid(const std::shared_ptr<Grid> &grid) = 0;

    virtual void SetCoefficient(const std::shared_ptr<NumericField> &coef) = 0;

    virtual void SetCoefficient(const NumericValue &coef) = 0;

    virtual void SetField(const std::shared_ptr<NumericField> &field) = 0;

    virtual OperatorMode GetMode() const = 0;

    virtual OperatorType GetType() const = 0;

    /// @brief Get the variable that the operator operates on.
    virtual std::string GetVariable() const = 0;

    /// @brief Get the name of the operator.
    /// @return "Fvc-" prefix for explicit operator, means "Finite Volume Calculus",
    /// used for updating field. "Fvm-" prefix for implicit operator,
    /// means "Finite Volume Method", used for discretizing equation.
    virtual std::string GetName() const = 0;


    ///////////////////////////////////////////////////////////////////////////////////
    // Methods for discretizing.
    //

    /// @brief Validate the operator parameters and datas.
    virtual std::vector<std::string> Validate() const = 0;

    /// @brief Execute the implict or explicit operator.
    virtual void Process() = 0;

    /// @brief Get the coefficient matrix and source term after implicit operator.
    /// @note For Vector variable, discretization is done on each demension separately.
    /// If that, the demesions order must be "x, y, z".
    virtual std::optional<std::vector<std::shared_ptr<LinearEqs>>>
    GetLinearEqs() const = 0;

    /// @brief Get the updated or newly generated field after explicit operator.
    virtual std::optional<std::shared_ptr<NumericField>> GetResult() const = 0;
};

}  // namespace OpenOasis::CommImp::Numeric
