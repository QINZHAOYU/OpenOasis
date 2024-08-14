/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Arguments.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide argument for component initialization.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IArgument.h"
#include "Models/Utils/DateTime.h"
#include <memory>


namespace OpenOasis
{
namespace CommImp
{
using Utils::DateTime;

/// @brief Argument is a class that contains (key,value) pairs.
template <typename T>
class TArgument : public IArgument
{
protected:
    std::string    mId;
    std::string    mDescription;
    std::string    mCaption;
    std::vector<T> mPossibleValues;
    T              mDefaultValue;
    T              mValue;
    bool           mIsReadOnly;
    bool           mIsOptional;

public:
    virtual ~TArgument()
    {}

    TArgument(const std::string &id)
    {
        mId = id;
    }

    TArgument(const std::shared_ptr<IArgument> &arg)
    {
        mId           = arg->GetId();
        mCaption      = arg->GetCaption();
        mDescription  = arg->GetDescription();
        mIsOptional   = arg->IsOptional();
        mIsReadOnly   = arg->IsReadOnly();
        mValue        = arg->GetValue();
        mDefaultValue = arg->GetDefaultValue();
    }

public:
    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IDescribable`.
    //

    std::string GetCaption() const override
    {
        return mCaption;
    }

    void SetCaption(const std::string &value) override
    {
        mCaption = value;
    }

    std::string GetDescription() const override
    {
        return mDescription;
    }

    void SetDescription(const std::string &value) override
    {
        mDescription = value;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IIdentifiable`.
    //

    std::string GetId() const override
    {
        return mId;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Implement methods inherited from `IArgument`.
    //

    const std::type_info &GetValueType() const override
    {
        return typeid(T);
    }

    bool IsReadOnly() const override
    {
        return mIsReadOnly;
    }

    bool IsOptional() const override
    {
        return mIsOptional;
    }

    std::any GetValue() const override
    {
        return mValue;
    }

    void SetValue(std::any value) override
    {
        mValue = std::any_cast<T>(value);
    }

    std::any GetDefaultValue() const override
    {
        return mDefaultValue;
    }

    std::vector<std::any> GetPossibleValues() const override
    {
        std::vector<std::any> values;
        for (const auto &val : mPossibleValues)
        {
            values.emplace_back(val);
        }

        return values;
    }
};


/// @brief String type arguments.
class ArgumentString : public TArgument<std::string>
{
public:
    ArgumentString(const std::string &id, const std::string &value);
};


/// @brief Bool type arguments.
class ArgumentBool : public TArgument<bool>
{
public:
    ArgumentBool(const std::string &id, bool value);
};


/// @brief Integer type arguments.
class ArgumentInt : public TArgument<int>
{
public:
    ArgumentInt(const std::string &id, int value);
};


/// @brief Double type arguments.
class ArgumentDouble : public TArgument<double>
{
public:
    ArgumentDouble(const std::string &id, double value);
};


/// @brief Datetime type arguments.
class ArgumentDateTime : public TArgument<DateTime>
{
public:
    ArgumentDateTime(const std::string &id, const DateTime &value);
};


}  // namespace CommImp
}  // namespace OpenOasis