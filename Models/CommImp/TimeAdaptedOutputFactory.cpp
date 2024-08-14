/** ***********************************************************************************
 *    @File      :  TimeAdaptedOutputFactory.cpp
 *    @Brief     :  Factory to create instance of temporal `IAdaptedOutput`.
 *
 ** ***********************************************************************************/
#include "TimeAdaptedOutputFactory.h"
#include "Models/CommImp/DevSupports/TimeAdaptor.h"
#include "Models/Utils/VectorHelper.h"
#include "Models/Utils/Exception.h"


namespace OpenOasis::CommImp
{
using namespace DevSupports;
using namespace Utils;
using namespace std;


TimeAdaptedOutputFactory::TimeAdaptedOutputFactory(const string &id) : mId(id)
{}

string TimeAdaptedOutputFactory::GetCaption() const
{
    return mCaption;
}

void TimeAdaptedOutputFactory::SetCaption(const string &value)
{
    mCaption = value;
}

string TimeAdaptedOutputFactory::GetDescription() const
{
    return mDescription;
}

void TimeAdaptedOutputFactory::SetDescription(const string &value)
{
    mDescription = value;
}

string TimeAdaptedOutputFactory::GetId() const
{
    return mId;
}

vector<shared_ptr<IIdentifiable>>
TimeAdaptedOutputFactory::GetAvailableAdaptedOutputIds(
    const shared_ptr<IOutput> &adaptee, const shared_ptr<IInput> &target)
{
    if (!adaptee)
    {
        return {};
    }

    vector<shared_ptr<IIdentifiable>> ids;
    ids.emplace_back(make_shared<TimeAdaptor>(adaptee->GetId()));

    for (int i = 0; i < (int)ids.size(); i++)
    {
        const auto &item = dynamic_pointer_cast<IAdaptedOutput>(ids[i]);
        const auto &iter = find_if(
            begin(mAdaptorsCreatedSoFar),
            end(mAdaptorsCreatedSoFar),
            [&item](const auto &it) {
                return item->GetId() == it->GetId()
                       && item->GetAdaptee().lock()->GetId()
                              == it->GetAdaptee().lock()->GetId();
            });

        if (iter == mAdaptorsCreatedSoFar.end())
        {
            mAdaptorsCreatedSoFar.push_back(item);
        }
    }

    return ids;
}

shared_ptr<IAdaptedOutput> TimeAdaptedOutputFactory::CreateAdaptedOutput(
    const shared_ptr<IIdentifiable> &adaptorIdentifier,
    const shared_ptr<IOutput> &adaptee, const shared_ptr<IInput> &target)
{
    shared_ptr<IAdaptedOutput> adaptedOutput;
    for (const auto &adaptor : mAdaptorsCreatedSoFar)
    {
        if (adaptor->GetId() == adaptorIdentifier->GetId()
            && adaptor->GetAdaptee().lock()->GetId() == adaptee->GetId())
        {
            adaptedOutput = adaptor;
            break;
        }
    }

    if (adaptedOutput == nullptr)
    {
        throw IllegalArgumentException(
            "Unknown IAdaptedOutput - it does not originate from this factory");
    }

    // Connect the adaptor and the adaptee.
    if (!VectorHelper::IsContained(adaptee->GetAdaptedOutputs(), adaptedOutput))
    {
        adaptee->AddAdaptedOutput(adaptedOutput);
    }
    return adaptedOutput;
}

}  // namespace OpenOasis::CommImp
