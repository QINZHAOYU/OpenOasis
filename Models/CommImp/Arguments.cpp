/** ***********************************************************************************
 *    @File      :  Arguments.cpp
 *    @Brief     :  None.
 *
 ** ***********************************************************************************/
#include "Arguments.h"


namespace OpenOasis::CommImp
{
using namespace std;


// class ArgumentString----------------------------------------------------------------

ArgumentString::ArgumentString(const string &id, const string &value) : TArgument(id)
{
    mCaption     = id;
    mDescription = "String Argument";

    mDefaultValue = "";
    mValue        = value;
}


// class ArgumentBool------------------------------------------------------------------

ArgumentBool::ArgumentBool(const string &id, bool value) : TArgument(id)
{
    mCaption     = id;
    mDescription = "Bool Argument";

    mDefaultValue = true;
    mValue        = value;
}


// class ArgumentInt-------------------------------------------------------------------

ArgumentInt::ArgumentInt(const string &id, int value) : TArgument(id)
{
    mCaption     = id;
    mDescription = "Integer Argument";

    mDefaultValue = 0;
    mValue        = value;
}


// class ArgumentDouble----------------------------------------------------------------

ArgumentDouble::ArgumentDouble(const string &id, double value) : TArgument(id)
{
    mCaption     = id;
    mDescription = "Double Argument";

    mDefaultValue = 0.0;
    mValue        = value;
}


// class ArgumentDateTime--------------------------------------------------------------

ArgumentDateTime::ArgumentDateTime(const string &id, const DateTime &value) :
    TArgument(id)
{
    mCaption     = id;
    mDescription = "DateTime Argument";

    mDefaultValue = DateTime::Zero();
    mValue        = value;
}


}  // namespace OpenOasis::CommImp