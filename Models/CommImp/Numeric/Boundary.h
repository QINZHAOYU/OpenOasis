/** ***********************************************************************************
 *    Copyright (C) 2023, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Boundary.h
 *    @License   :  Apache-2.0
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/CommImp/Spatial/Grid.h"
#include "Operator.h"


namespace OpenOasis::CommImp::Numeric
{
struct BoundaryCondition;


/// @brief Abstract boundary class.
/// @details Each `boundary` is bounded to a variable with various types of boundary
/// conditions, and covers the whole domain boundary.
class Boundary : public Operator
{
public:
    virtual void SetDefaultBoundaryCondition(const BoundaryCondition &bc) = 0;

    virtual const BoundaryCondition &GetDefaultBoundaryCondition() const = 0;

    virtual void SetBoundaryCondition(
        std::vector<size_t> faceIndexes, const BoundaryCondition &bc) = 0;

    virtual const BoundaryCondition &GetBoundaryCondition(size_t faceIdx) const = 0;
};


/// @brief Boundary type enum.
enum class BoundaryType
{
    ValueBound,
    FluxBound,
    MixedBound,
    WallBound,
    FreeBound,
    SlipBound,
    UnknownBound
};


/// @brief Boundary condition.
struct BoundaryCondition
{
    BoundaryType type = BoundaryType::UnknownBound;

    std::unordered_map<std::string, real> conds;

    BoundaryCondition() = default;
    BoundaryCondition(
        BoundaryType type, const std::unordered_map<std::string, real> &conds) :
        type(type)
    {
        switch (type)
        {
        case BoundaryType::UnknownBound: return;
        case BoundaryType::ValueBound: {
            const std::string key = "value";
            if (conds.find(key) == conds.end())
            {
                throw std::invalid_argument("ValueBound missing required key: " + key);
            }

            this->conds[key] = conds.at(key);
            break;
        }
        case BoundaryType::FluxBound: {
            const std::string key = "flux";
            if (conds.find(key) == conds.end())
            {
                throw std::invalid_argument("FluxBound missing required key: " + key);
            }

            this->conds[key] = conds.at(key);
            break;
        }
        case BoundaryType::WallBound:
        case BoundaryType::FreeBound:
        case BoundaryType::MixedBound:
        case BoundaryType::SlipBound:
        default: throw std::invalid_argument("Unsupported boundary type.");
        }
    }
};

}  // namespace OpenOasis::CommImp::Numeric
