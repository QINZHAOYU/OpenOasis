/** ***********************************************************************************
 *    Copyright (C) 2024, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Configs.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  General configuration structures.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Utils/CommConstants.h"
#include "Field.h"
#include <string>
#include <variant>
#include <optional>


namespace OpenOasis::CommImp::Numeric
{
using Utils::real;


struct Configuration
{
    std::string key;

    std::variant<std::string, real, int, bool> value;

    Configuration() = default;
    Configuration(const std::string &key, const std::string &value) :
        key(key), value(value)
    {}
    explicit Configuration(const std::string &key, real value) : key(key), value(value)
    {}
    explicit Configuration(const std::string &key, int value) : key(key), value(value)
    {}
    explicit Configuration(const std::string &key, bool value) : key(key), value(value)
    {}
};


struct NumericValue
{
    std::string id;

    std::optional<real>         sValue;
    std::optional<Vector<real>> vValue;
    std::optional<Tensor<real>> tValue;

    NumericValue() = default;
    NumericValue(const std::string &id, real &sValue) : id(id), sValue(sValue)
    {}
    NumericValue(const std::string &id, const Vector<real> &vValue) :
        id(id), vValue(vValue)
    {}
    NumericValue(const std::string &id, const Tensor<real> &tValue) :
        id(id), tValue(tValue)
    {}
};


struct NumericField
{
    std::string id;

    std::optional<ScalarFieldFp> sField;
    std::optional<VectorFieldFp> vField;
    std::optional<TensorFieldFp> tField;

    NumericField() = default;
    NumericField(const std::string &id, ScalarFieldFp &sField) : id(id), sField(sField)
    {}
    NumericField(const std::string &id, const VectorFieldFp &vField) :
        id(id), vField(vField)
    {}
    NumericField(const std::string &id, const TensorFieldFp &tField) :
        id(id), tField(tField)
    {}
};

}  // namespace OpenOasis::CommImp::Numeric