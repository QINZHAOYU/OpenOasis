/** ***********************************************************************************
 *    @File      :  Describer.cpp
 *    @Brief     :  Generic implementation of `IDescribable` interface.
 *
 ** ***********************************************************************************/
#include "Describer.h"


namespace OpenOasis::CommImp
{
using namespace std;

Describer::Describer()
{}

Describer::Describer(const string &caption, const string &desc) :
    mCaption(caption), mDescription(desc)
{}

string Describer::GetCaption() const
{
    return mCaption;
}

void Describer::SetCaption(const string &caption)
{
    mCaption = caption;
}

string Describer::GetDescription() const
{
    return mDescription;
}

void Describer::SetDescription(const string &discription)
{
    mDescription = discription;
}
}  // namespace OpenOasis::CommImp