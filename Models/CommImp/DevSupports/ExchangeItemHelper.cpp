/** ***********************************************************************************
 *    @File      :  ExchangeItemHelper.cpp
 *    @Brief     :  To provide methods to check connectability among exchange items.
 *
 ** ***********************************************************************************/
#include "ExchangeItemHelper.h"
#include "ExtensionMethods.h"
#include "Models/Inc/IQuantity.h"
#include "Models/Inc/IQuality.h"
#include "Models/Utils/Exception.h"
#include "ThirdPart/MagicEnum/WrapMagicEnum.hpp"


namespace OpenOasis::CommImp::DevSupports
{
using namespace Utils;
using namespace std;


void ExchangeItemHelper::CheckProviderConsumerConnectable(
    const shared_ptr<IOutput> &provider, const shared_ptr<IInput> &consumer)
{
    if (!ProviderConsumerConnectable(provider, consumer))
    {
        throw InvalidOperationException(
            "consumer(" + consumer->GetCaption() + ")  and provider("
            + provider->GetCaption()
            + ") are not connectable, put an adapted output in between");
    }
}

bool ExchangeItemHelper::ProviderConsumerConnectable(
    const shared_ptr<IOutput> &provider, const shared_ptr<IInput> &consumer)
{
    if (!OutputAndInputValueDefinitionFit(provider, consumer))
    {
        return false;
    }

    return ProviderConsumerConnectableForTimeAndOrElementSet(
        provider, consumer, true, true);
}

bool ExchangeItemHelper::ProviderConsumerConnectableForTimeSet(
    const shared_ptr<IOutput> &provider, const shared_ptr<IInput> &consumer)
{
    return ProviderConsumerConnectableForTimeAndOrElementSet(
        provider, consumer, true, false);
}

bool ExchangeItemHelper::ProviderConsumerConnectableForElementSet(
    const shared_ptr<IOutput> &provider, const shared_ptr<IInput> &consumer)
{
    return ProviderConsumerConnectableForTimeAndOrElementSet(
        provider, consumer, false, true);
}

bool ExchangeItemHelper::ProviderConsumerConnectableForTimeAndOrElementSet(
    const shared_ptr<IOutput> &outputItem, const shared_ptr<IInput> &consumer,
    bool doCheckTime, bool doCheckSpace)
{
    return true;
}

void ExchangeItemHelper::CheckConsumersCompatible(
    const shared_ptr<IOutput> &outputItem, const shared_ptr<IInput> &newConsumer)
{
    if (!ConsumersCompatible(outputItem, newConsumer))
    {
        throw InvalidOperationException(
            "consumer(" + newConsumer->GetCaption() + ") can not be added to ("
            + outputItem->GetCaption()
            + "), because it is incompatible with existing consumers");
    }
}

bool ExchangeItemHelper::ConsumersCompatible(
    const shared_ptr<IOutput> &outputItem, const shared_ptr<IInput> &newConsumer)
{
    if (!OutputAndInputValueDefinitionFit(outputItem, newConsumer))
    {
        return false;
    }

    return ConsumersCompatibleForTimeAndOrElementSet(
        outputItem, newConsumer, true, true);
}

bool ExchangeItemHelper::ConsumersCompatibleForTimeSet(
    const shared_ptr<IOutput> &outputItem, const shared_ptr<IInput> &newConsumer)
{
    return ConsumersCompatibleForTimeAndOrElementSet(
        outputItem, newConsumer, true, false);
}

bool ExchangeItemHelper::ConsumersCompatibleForElementSet(
    const shared_ptr<IOutput> &outputItem, const shared_ptr<IInput> &newConsumer)
{
    return ConsumersCompatibleForTimeAndOrElementSet(
        outputItem, newConsumer, false, true);
}

bool ExchangeItemHelper::ConsumersCompatibleForTimeAndOrElementSet(
    const shared_ptr<IOutput> &outputItem, const shared_ptr<IInput> &consumer,
    bool doCheckTime, bool doCheckSpace)
{
    return true;
}

bool ExchangeItemHelper::OutputAndInputValueDefinitionFit(
    const shared_ptr<IBaseExchangeItem> &provider,
    const shared_ptr<IBaseExchangeItem> &consumer)
{
    auto pValueDef = provider->GetValueDefinition();
    auto cValueDef = consumer->GetValueDefinition();
    if (!pValueDef || !cValueDef)
    {
        return false;
    }

    auto pQuantity = dynamic_pointer_cast<IQuantity>(pValueDef);
    auto cQuantity = dynamic_pointer_cast<IQuantity>(cValueDef);
    if (!pQuantity || !cQuantity)
    {
        return false;
    }

    auto pUnit = pQuantity->GetUnit();
    auto cUnit = cQuantity->GetUnit();
    if (!pUnit || !cUnit)
    {
        return false;
    }

    auto pDimension = pUnit->GetDimension();
    auto cDimension = cUnit->GetDimension();
    if (!pDimension || !cDimension)
    {
        return false;
    }

    for (auto dimBase : magic_enum::enum_values<DimensionBase>())
    {
        if (pDimension->GetPower(dimBase) != cDimension->GetPower(dimBase))
        {
            return false;
        }
    }

    return true;
}

bool ExchangeItemHelper::OutputAndInputFit(
    const shared_ptr<IBaseExchangeItem> &provider,
    const shared_ptr<IBaseExchangeItem> &consumer)
{
    // if (!OutputAndInputTimeSetsFit(provider, consumer))
    // {
    //     return false;
    // }

    return OutputAndInputElementSetsFit(provider, consumer);
}

bool ExchangeItemHelper::OutputAndInputTimeSetsFit(
    const shared_ptr<IBaseExchangeItem> &provider,
    const shared_ptr<IBaseExchangeItem> &consumer)
{
    if (!provider || !consumer)
    {
        return false;
    }

    bool timeFits = true;

    auto sourceTimeSet = provider->GetTimeSet();
    auto targetTimeSet = consumer->GetTimeSet();

    if (!sourceTimeSet || sourceTimeSet->GetTimes().empty())
    {
        if (targetTimeSet)
        {
            // Source fits target if target requires only one time step.
            timeFits = targetTimeSet->GetTimes().size() == 1;
        }
    }
    else
    {
        if (!targetTimeSet || targetTimeSet->GetTimes().empty())
        {
            // Source fits target if source has values for only one time step available.
            timeFits = sourceTimeSet->GetTimes().size() == 1;
        }
        else
        {
            // Both source and target have timeset specification.
            if (dynamic_pointer_cast<IAdaptedOutput>(provider))
            {
                // For now: check if the target's last required time is included in the
                // source's time horizon.

                auto sourceTimeHorizon = sourceTimeSet->GetTimeHorizon();
                if (!sourceTimeHorizon)
                {
                    return false;
                }
                double startOfSourceTimeHorizon =
                    ExtensionMethods::Start(sourceTimeHorizon)->GetTimeStamp();
                double endOfSourceTimeHorizon =
                    ExtensionMethods::End(sourceTimeHorizon)->GetTimeStamp();

                auto   targetTimeHorizon = targetTimeSet->GetTimeHorizon();
                double startOfTargetTimeHorizon =
                    ExtensionMethods::Start(targetTimeHorizon)->GetTimeStamp();
                double endOfTargetTimeHorizon =
                    ExtensionMethods::End(targetTimeHorizon)->GetTimeStamp();

                if ((startOfTargetTimeHorizon <= startOfSourceTimeHorizon)
                    && (endOfTargetTimeHorizon <= endOfSourceTimeHorizon))
                {
                    timeFits = true;
                }
                else
                {
                    timeFits = false;
                }
            }
            else
            {
                timeFits = false;

                // regular (output) exchange item, check if all times fit.
                auto sourceTimes = sourceTimeSet->GetTimes();
                auto targetTimes = targetTimeSet->GetTimes();

                if (sourceTimes.size() == targetTimes.size())
                {
                    timeFits = true;

                    for (std::size_t idx = 0; idx < sourceTimes.size(); ++idx)
                    {
                        if (!ExtensionMethods::TimeEquals(
                                sourceTimes[idx], targetTimes[idx]))
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }

    return timeFits;
}

bool ExchangeItemHelper::OutputAndInputElementSetsFit(
    const shared_ptr<IBaseExchangeItem> &provider,
    const shared_ptr<IBaseExchangeItem> &consumer)
{
    return true;
}

bool ExchangeItemHelper::CheckValueSizes(
    const shared_ptr<IBaseExchangeItem> &exchangeItem,
    const shared_ptr<IValueSet>         &valueSet)
{
    return true;
}

shared_ptr<ITime>
ExchangeItemHelper::GetEarliestConsumerTime(const shared_ptr<IOutput> &output)
{
    shared_ptr<ITime> earliestRequiredTime = nullptr;

    for (auto &input : output->GetConsumers())
    {
        const auto &elem = input.lock();
        if (!elem)
        {
            continue;
        }

        const auto &times = elem->GetTimeSet();
        if (!times)
        {
            continue;
        }

        const auto &time = times->GetTimeHorizon();
        if (!earliestRequiredTime)
        {
            earliestRequiredTime = time;
        }
        else if (earliestRequiredTime->GetTimeStamp() < time->GetTimeStamp())
        {
            earliestRequiredTime = time;
        }
    }

    return earliestRequiredTime;
}

shared_ptr<ITime>
ExchangeItemHelper::GetLatestConsumerTime(const shared_ptr<IOutput> &output)
{
    shared_ptr<ITime> latestRequiredTime = nullptr;

    for (auto &input : output->GetConsumers())
    {
        const auto &elem = input.lock();
        if (!elem)
        {
            continue;
        }

        const auto &times = elem->GetTimeSet();
        if (!times)
        {
            continue;
        }

        const auto &time = ExtensionMethods::End(times->GetTimeHorizon());
        if (!latestRequiredTime)
        {
            latestRequiredTime = time;
        }
        else if (latestRequiredTime->GetTimeStamp() < time->GetTimeStamp())
        {
            latestRequiredTime = time;
        }
    }

    return latestRequiredTime;
}


}  // namespace OpenOasis::CommImp::DevSupports