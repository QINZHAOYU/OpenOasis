/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ExchangeItemHelper.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To provide methods to check connectability among exchange items.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IInput.h"
#include "Models/Inc/IOutput.h"
#include "Models/Inc/IValueSet.h"


namespace OpenOasis
{
namespace CommImp
{
namespace DevSupports
{
/// @brief This class offers support in check whether exchange items are connectable,
/// whether an additional consumer can be added to an (adapted)output etc.
class ExchangeItemHelper final
{
public:
    /// @brief Check if a consumer can be added to an output or adapted output.
    /// The value definition, element sets and the time set are checked.
    ///
    /// @param provider The output item.
    /// @param consumer The new consumer.
    ///
    /// @throw Throw an exception if not connectable.
    static void CheckProviderConsumerConnectable(
        const std::shared_ptr<IOutput> &provider,
        const std::shared_ptr<IInput>  &consumer);

    /// @brief Check if a consumer can be added to an output or adapted output item.
    /// The value definition, element sets and the time set are checked.
    ///
    /// @param provider The output item.
    /// @param consumer The new consumer.
    ///
    /// @return True if the consumer can be added indeed: False, for the others.
    static bool ProviderConsumerConnectable(
        const std::shared_ptr<IOutput> &provider,
        const std::shared_ptr<IInput>  &consumer);

    /// @brief Check if a provider and a consumer can be connected, as far as the
    /// time set types are concerned (has durations or not).
    ///
    /// @param provider The output item.
    /// @param consumer The new consumer.
    ///
    /// @return True if the consumer can be added indeed: False, for the others.
    static bool ProviderConsumerConnectableForTimeSet(
        const std::shared_ptr<IOutput> &provider,
        const std::shared_ptr<IInput>  &consumer);

    /// @brief Check if a provider and a consumer can be connected, as far as the
    /// element set is concerned (type and sizes).
    ///
    /// @param provider The output item.
    /// @param consumer The new consumer.
    ///
    /// @return True if the consumer can be added indeed: False, for the others.
    static bool ProviderConsumerConnectableForElementSet(
        const std::shared_ptr<IOutput> &provider,
        const std::shared_ptr<IInput>  &consumer);

    /// @brief Check if a new consumer is compatible with existing consumers.
    /// The value definition, element sets and the time set are checked.
    ///
    /// @param outputItem The output item.
    /// @param newConsumer The new consumer.
    ///
    /// @throw Throw an exception if not compatible.
    static void CheckConsumersCompatible(
        const std::shared_ptr<IOutput> &outputItem,
        const std::shared_ptr<IInput>  &newConsumer);

    /// @brief Check if a new consumer is compatible with existing consumers.
    /// The value definition, element sets and the time set are checked.
    ///
    /// @param outputItem The output item.
    /// @param newConsumer The new consumer.
    ///
    /// @return True if the consumer is compatible; false, for the others.
    static bool ConsumersCompatible(
        const std::shared_ptr<IOutput> &outputItem,
        const std::shared_ptr<IInput>  &newConsumer);

    /// @brief Check if a new consumer is compatible with existing consumers.
    /// as far as the time set types are concerned (has durations or not).
    ///
    /// @param outputItem The output item.
    /// @param newConsumer The new consumer.
    ///
    /// @return True if the consumer is compatible; false, for the others.
    static bool ConsumersCompatibleForTimeSet(
        const std::shared_ptr<IOutput> &outputItem,
        const std::shared_ptr<IInput>  &newConsumer);

    /// @brief Check if a new consumer is compatible with existing consumers.
    /// as far as the element set is concerned (type and sizes).
    ///
    /// @param outputItem The output item.
    /// @param newConsumer The new consumer.
    ///
    /// @return True if the consumer is compatible; false, for the others.
    static bool ConsumersCompatibleForElementSet(
        const std::shared_ptr<IOutput> &outputItem,
        const std::shared_ptr<IInput>  &newConsumer);

    /// @brief Check if the quantity(no quality) value definition of a provider fits
    /// accoring to what the consumer requires.
    ///
    /// @param provider The provider.
    /// @param consumer The consumer.
    ///
    /// @return True if the value defintion fit; false, for the others.
    static bool OutputAndInputValueDefinitionFit(
        const std::shared_ptr<IBaseExchangeItem> &provider,
        const std::shared_ptr<IBaseExchangeItem> &consumer);

    /// @brief Check if the element set and the time set of a provider fit accoring to
    /// what the consumer requires. This method can be used to check of the providing
    /// component yet needs to do another `Update()` step.
    ///
    /// @param provider The provider.
    /// @param consumer The consumer.
    ///
    /// @return True if both element set and the time set fit; false, for the others.
    static bool OutputAndInputFit(
        const std::shared_ptr<IBaseExchangeItem> &provider,
        const std::shared_ptr<IBaseExchangeItem> &consumer);

    /// @brief Check if the time set of a provider fits accoring to what the consumer
    /// requires. This method can be used to check of the providing component yet needs
    /// to do another `Update()` step.
    ///
    /// @param provider The provider.
    /// @param consumer The consumer.
    ///
    /// @return True if both element set and the time set fit; false, for the others.
    static bool OutputAndInputTimeSetsFit(
        const std::shared_ptr<IBaseExchangeItem> &provider,
        const std::shared_ptr<IBaseExchangeItem> &consumer);

    /// @brief Check if the element set of a provider fits accoring to what the consumer
    /// requires. This method can be used to check of the providing component yet needs
    /// to do another `Update()` step.
    ///
    /// @param provider The provider.
    /// @param consumer The consumer.
    ///
    /// @return True if both element set and the time set fit; false, for the others.
    static bool OutputAndInputElementSetsFit(
        const std::shared_ptr<IBaseExchangeItem> &provider,
        const std::shared_ptr<IBaseExchangeItem> &consumer);

    /// @brief Check if the content of a value set is consistent with the number of time
    /// steps and the number of elements defined by the exchange item.
    ///
    /// @param exchangeItem The exchange item specifying time and space.
    /// @param valueSet The value set to be checked.
    ///
    /// @return True if matching; false, for the others.
    static bool CheckValueSizes(
        const std::shared_ptr<IBaseExchangeItem> &exchangeItem,
        const std::shared_ptr<IValueSet>         &valueSet);

    /// @brief Get the earliest time from all consumers of specified output item.
    ///
    /// @param output The output item.
    ///
    /// @return The earliest time required by consumers.
    static std::shared_ptr<ITime>
    GetEarliestConsumerTime(const std::shared_ptr<IOutput> &outputs);

    /// @brief Get the latest time from all consumers of specified output item.
    ///
    /// @param output The output item.
    ///
    /// @return The latest time required by consumers.
    static std::shared_ptr<ITime>
    GetLatestConsumerTime(const std::shared_ptr<IOutput> &outputs);

private:
    static bool ConsumersCompatibleForTimeAndOrElementSet(
        const std::shared_ptr<IOutput> &outputItem,
        const std::shared_ptr<IInput> &consumer, bool doCheckTime, bool doCheckSpace);

    static bool ProviderConsumerConnectableForTimeAndOrElementSet(
        const std::shared_ptr<IOutput> &outputItem,
        const std::shared_ptr<IInput> &consumer, bool doCheckTime, bool doCheckSpace);
};

}  // namespace DevSupports
}  // namespace CommImp
}  // namespace OpenOasis