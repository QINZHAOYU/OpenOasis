/** ***********************************************************************************
 *    @File      :  Identifier.cpp
 *    @Brief     :  Generic implementation of `IIdentifiable` interface.
 *
 ** ***********************************************************************************/
#include "Identifier.h"


namespace OpenOasis::CommImp
{
using namespace std;

Identifier::Identifier(const string &id) : mId(id)
{}

string Identifier::GetId() const
{
    return mId;
}

string Identifier::GetCaption() const
{
    return mCaption;
}

void Identifier::SetCaption(const string &caption)
{
    mCaption = caption;
}

string Identifier::GetDescription() const
{
    return mDescription;
}

void Identifier::SetDescription(const string &discription)
{
    mDescription = discription;
}
}  // namespace OpenOasis::CommImp