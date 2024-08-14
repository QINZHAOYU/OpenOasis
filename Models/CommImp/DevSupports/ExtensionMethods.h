/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ExtensionMethods.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  Extensional methods for spatial and temporal operation.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/ILinkableComponent.h"
#include "Models/Inc/IQuality.h"
#include "Models/Inc/IQuantity.h"
#include "Models/Inc/ITime.h"
#include "Models/Inc/ITimeSet.h"
#include "Models/Inc/IElementSet.h"
#include "Models/Inc/IAdaptedOutput.h"
#include "Models/Inc/IValueSet.h"
#include "Models/Inc/IInput.h"
#include "Models/Inc/IOutput.h"
#include "Models/Inc/ILinkableComponent.h"
#include "Models/Utils/DateTime.h"
#include "Models/Utils/StringHelper.h"
#include "Models/Utils/CommConstants.h"
#include <unordered_map>


namespace OpenOasis::CommImp::DevSupports
{
using namespace Utils;

/// @brief Extensional static methods for spatial and temporal operation.
class ExtensionMethods final
{
public:
    ///////////////////////////////////////////////////////////////////////////////////
    // Methods related to linkable component.
    //

    /// @brief To initialize a linkable component with arguments.
    static void Initialize(
        const std::shared_ptr<ILinkableComponent>     &component,
        const std::vector<std::shared_ptr<IArgument>> &arguments);

    /// @brief Apply the arguments in the linkable component.
    ///
    /// If an argument in the arguments list comes with an id that does not exist in
    /// the arguments of the component, an exception is thrown.
    static void ApplyArguments(
        const std::shared_ptr<ILinkableComponent>     &component,
        const std::vector<std::shared_ptr<IArgument>> &arguments);

    /// @brief Get TimeExtent from a linkable component.
    /// If the component is not a time space version, this will throw an exception.
    static std::shared_ptr<ITimeSet>
    TimeExtent(const std::shared_ptr<ILinkableComponent> &component);

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods related to exchange items.
    //

    /// @brief Get the value definition as a quantity, throwing an exception if it is
    /// not a quantity.
    static std::shared_ptr<IQuantity>
    Quantity(const std::shared_ptr<IBaseExchangeItem> &item);

    /// @brief Get the value definition as a quality, throwing an exception if it is
    /// not a quality.
    static std::shared_ptr<IQuality>
    Quality(const std::shared_ptr<IBaseExchangeItem> &item);

    /// @brief Get the `IElementSet` from the baseitem. If not, exceptions are thrown.
    static std::shared_ptr<IElementSet>
    ElementSet(const std::shared_ptr<IBaseExchangeItem> &item);

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods related to time.
    //

    /// @brief Check if two time span or time stamp are the same.
    static bool TimeEquals(
        const std::shared_ptr<ITime> &time1, const std::shared_ptr<ITime> &time2);

    /// @brief Adds a number of days to the Time.
    static std::shared_ptr<ITime>
    AddDays(const std::shared_ptr<ITime> &time, double days);

    /// @brief Adds a number of seconds to the Time.
    static std::shared_ptr<ITime>
    AddSeconds(const std::shared_ptr<ITime> &time, double secs);

    /// @brief Return the start time of a time span or a time stamp.
    static std::shared_ptr<ITime> Start(const std::shared_ptr<ITime> &time);

    /// @brief Returns the end time of a time span or a time stamp.
    static std::shared_ptr<ITime> End(const std::shared_ptr<ITime> &time);

    /// @brief Returns the end time stamp of a time span or a time stamp.
    static double EndTimeStamp(const std::shared_ptr<ITime> &time);

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods related to time set.
    //

    /// @brief Set a single time stamp or span in the `ITimeSet`.
    ///
    /// @param timeSet Time set to update.
    /// @param timeStamp Time to use.
    static void SetSingleTime(
        const std::shared_ptr<ITimeSet> &timeSet, std::shared_ptr<ITime> timeStamp);

    /// @brief Set a single time stamp in the `ITimeSet`.
    ///
    /// @param timeSet Time set to update.
    /// @param timestamp Time to use.
    static void
    SetSingleTimeStamp(const std::shared_ptr<ITimeSet> &timeSet, double timestamp);

    /// @brief Set a single time stamp in the `ITimeSet`.
    ///
    /// @param timeSet Time set to update.
    /// @param dateTime Time to use.
    static void SetSingleTimeStamp(
        const std::shared_ptr<ITimeSet> &timeSet, const DateTime &dateTime);

    /// @brief Set a single time stamp in the `ITimeSet`.
    ///
    /// @param timeSet Time set to update.
    /// @param dateTime Time to use.
    /// @param durationInDays Duration of span.
    static void SetSingleTimeSpan(
        const std::shared_ptr<ITimeSet> &timeSet, const DateTime &dateTime,
        double durationInDays);

    /// @brief Set a single time stamp in the `ITimeSet`.
    ///
    /// @param timeSet Time set to update.
    /// @param start Start time stamp.
    /// @param end End time stamp.
    static void SetSingleTimeSpan(
        const std::shared_ptr<ITimeSet> &timeSet, double start, double end);

    /// @brief Set a single time stamp in the `ITimeSet`.
    ///
    /// @param timeSet Time set to update.
    /// @param startTime Start Time.
    /// @param endTime End Time.
    static void SetSingleTimeSpan(
        const std::shared_ptr<ITimeSet> &timeSet,
        const std::shared_ptr<ITime> &startTime, const std::shared_ptr<ITime> &endTime);

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods related to value sets.
    //

    /// @brief Return the number of elements in the `IValueSet`.
    static int ElementCount(const std::shared_ptr<IValueSet> &values);

    /// @brief Return the number of times in the `IValueSet`.
    static int TimesCount(const std::shared_ptr<IValueSet> &values);

    /// @brief Returns an array of T's from the values, extracting data from the
    /// timeIndex - in principle a typed version
    /// of `IValueSet.GetElementValuesForTime()` returning an array.
    ///
    /// @param values Values to retrieve.
    /// @param timeIndex Index to retrieve data from.
    /// @return Array of T.
    template <typename T>
    static std::vector<T>
    GetElementValuesForTime(const std::shared_ptr<IValueSet> &values, int timeIndex)
    {
        const auto &elmtValues = values->GetElementValuesForTime(timeIndex);

        // Check if it's already an array.
        if (elmtValues.empty())
        {
            return {};
        }

        // Do a manual copy.
        std::vector<T> tArray(elmtValues.size());
        for (std::size_t i = 0; i < elmtValues.size(); i++)
        {
            tArray[i] = std::any_cast<T>(elmtValues[i]);
        }

        return tArray;
    }

    /// @brief Multiply all element values in the value set with the
    /// matching factors in the factors array.
    ///
    /// Assuming the value set contains doubles, and the number of elements in the
    /// value set matches the number of factors.
    ///
    /// @param sourceValueset Valueset to multiply.
    /// @param factors Factors to multiply element values with.
    /// @return A new value set where values have been multiplied.
    static std::shared_ptr<IValueSet> MultiplyElementValues(
        const std::shared_ptr<IValueSet> &sourceValueset,
        const std::vector<real>          &factors);


    ///////////////////////////////////////////////////////////////////////////////////
    // Methods related to list of arguments.
    //

    /// @brief Creates a dictionary of the argument array, indexing each argument on
    /// its Id.
    ///
    /// @param array Array input argument.
    /// @return A dictionary indexed by the argument Id's.
    static std::unordered_map<std::string, std::shared_ptr<IArgument>>
    Dictionary(const std::vector<std::shared_ptr<IArgument>> &array);

    /// @brief Apply the arguments in the newValues list to the arguments.
    ///
    /// It searches through all the arguments and sets the value for those with id's
    /// from the newValues.
    ///
    /// If an argument in the newValues list comes with an id that doesnt
    /// exist in the arguments, an exception is thrown.
    ///
    /// @param arguments Arguments to update.
    /// @param newValues New argument values to set.
    static void ApplyArguments(
        const std::vector<std::shared_ptr<IArgument>> &arguments,
        const std::vector<std::shared_ptr<IArgument>> &newValues);

    /// @brief Return the value of the argument with the specified id, assuming it
    /// has a value of type T.
    ///
    /// @param arguments Dictionary of arguments to search in.
    /// @param id Id of argument to search for.
    /// @return Value of argument.
    template <typename T>
    static T GetValue(
        const std::unordered_map<std::string, std::shared_ptr<IArgument>> &arguments,
        const std::string                                                 &id)
    {
        // IArgument *value;
        // std::unordered_map<std::string, IArgument *>::const_iterator
        //     arguments_iterator = arguments.find(id);
        // if (arguments_iterator == arguments.end())
        // {
        //     value = arguments_iterator->second;
        //     throw std::runtime_error(StringHelper::WStringToString(
        //         StringHelper::FormatSimple(L"Unknown argument id: '{0}'", id)));
        // }
        // else
        // {
        //     value = arguments_iterator->second;
        // }
        // if (value->GetValueType() != typeid(T))
        // {
        //     throw std::runtime_error(
        //         StringHelper::WStringToString(StringHelper::FormatSimple(
        //             L"Type mismatch for argument of id: '{0}'", id)));
        // }

        // return std::any_cast<T>(value->GetValue());

        return 0;
    }

    /// @brief Update the value of the argument with the specified id, assuming it
    /// has avalue of type T.
    ///
    /// @param arguments Dictionary of arguments to search in.
    /// @param id Id of argument to search for.
    /// @param newValue New value to set.
    /// @return Existing value of argument.
    template <typename T>
    static T UpdateValue(
        const std::unordered_map<std::string, std::shared_ptr<IArgument>> &arguments,
        const std::string &id, T newValue)
    {
        // IArgument *value;
        // std::unordered_map<std::string, IArgument *>::const_iterator
        //     arguments_iterator = arguments.find(id);
        // if (arguments_iterator == arguments.end())
        // {
        //     value = arguments_iterator->second;
        //     throw std::runtime_error(StringHelper::WStringToString(
        //         StringHelper::FormatSimple(L"Unknown argument id: '{0}'", id)));
        // }
        // else
        // {
        //     value = arguments_iterator->second;
        // }
        // if (value->GetValueType() != typeid(T))
        // {
        //     throw std::runtime_error(
        //         StringHelper::WStringToString(StringHelper::FormatSimple(
        //             L"Type mismatch for argument of id: '{0}'", id)));
        // }

        // T oldValue = static_cast<T>(value->GetValue());
        // // argument->SetValue(newValue);

        // return oldValue;

        return 0;
    }

    /// @brief Update the value of the argument with the specified id, assuming it
    /// has a value of type T.
    ///
    /// @param arguments List of arguments to search in.
    /// @param id Id of argument to search for.
    /// @param newValue New value to set.
    /// @return Existing value of argument.
    template <typename T>
    static T UpdateValue(
        const std::vector<std::shared_ptr<IArgument>> &arguments, const std::string &id,
        T newValue)
    {
        // for (auto argument : arguments)
        // {
        //     if (argument->GetId() == id)
        //     {
        //         if (argument->GetValueType() != typeid(T))
        //         {
        //             throw std::runtime_error(
        //                 StringHelper::WStringToString(StringHelper::FormatSimple(
        //                     L"Type mismatch for argument of id: '{0}'", id)));
        //         }
        //         T oldValue = static_cast<T>(argument->GetValue());
        //         argument->SetValue(newValue);
        //         return oldValue;
        //     }
        // }
        // throw std::runtime_error(StringHelper::WStringToString(
        //     StringHelper::FormatSimple(L"Unknown argument id: '{0}'", id)));

        return 0;
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // Methods used to searching for exchange items and adaptors, based on their id.
    //

    /// @brief Find the input item with the provided id.
    ///
    /// @param component Component to find input item in.
    /// @param id Id of input item to search for.
    /// @return Returns input item with id, or null if not found.
    static std::shared_ptr<IInput> FindInputItem(
        const std::shared_ptr<ILinkableComponent> &component, const std::string &id);

    /// @brief Find the output item with the provided id.
    ///
    /// @param component Component to find output item in.
    /// @param id Id of output item to search for.
    /// @return Returns output item with id, or null if not found.
    static std::shared_ptr<IOutput> FindOutputItem(
        const std::shared_ptr<ILinkableComponent> &component, const std::string &id);

    /// @brief Find the adaptor with the provided id.
    ///
    /// @param component Component to find adaptor in.
    /// @param adaptorId Id of adaptor to search for.
    /// @param output Output item to find adaptor for..
    /// @param input Input item to find adaptor for. Can be null.
    /// @return Returns adaptor with id, or null if not found.
    static std::shared_ptr<IAdaptedOutput> FindAdaptor(
        const std::shared_ptr<ILinkableComponent> &component,
        const std::string &adaptorId, const std::shared_ptr<IOutput> &output,
        const std::shared_ptr<IInput> &input);

    /// @brief Find the adaptor with the provided id.
    ///
    /// @param component Component to find adaptor in.
    /// @param factoryId Id of factory to search in, null to search all factories.
    /// @param adaptorId Id of adaptor to search for.
    /// @param output Output item to find adaptor for..
    /// @param input Input item to find adaptor for. Can be null.
    /// @return Returns adaptor with id, or null if not found.
    static std::shared_ptr<IAdaptedOutput> FindAdaptor(
        const std::shared_ptr<ILinkableComponent> &component,
        const std::string &factoryId, const std::string &adaptorId,
        const std::shared_ptr<IOutput> &output, const std::shared_ptr<IInput> &input);
};

}  // namespace OpenOasis::CommImp::DevSupports