/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Exception.hpp
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  To register custom exceptions based on `std::exception`.
 *
 ** ***********************************************************************************/
#pragma once
#include <exception>
#include <stdexcept>
#include <string>
#include <memory>


namespace OpenOasis
{
namespace Utils
{
#define _STRINGFY(arg) #arg
#define _REGISTER_EXCEPTION(exceName)                                                  \
    class exceName : public std::exception                                             \
    {                                                                                  \
    private:                                                                           \
        std::string msg;                                                               \
                                                                                       \
    public:                                                                            \
        explicit exceName(const std::string &message = "")                             \
        {                                                                              \
            msg = _STRINGFY([exceName]) + message;                                     \
        }                                                                              \
                                                                                       \
        explicit exceName(const std::shared_ptr<std::exception> &exp)                  \
        {                                                                              \
            msg = _STRINGFY([exceName]) + std::string(exp->what());                    \
        }                                                                              \
                                                                                       \
        const char *what() const noexcept                                              \
        {                                                                              \
            return msg.c_str();                                                        \
        }                                                                              \
    };


_REGISTER_EXCEPTION(NotSupportedException);
_REGISTER_EXCEPTION(ArgumentOutOfRangeException);
_REGISTER_EXCEPTION(IllegalArgumentException);
_REGISTER_EXCEPTION(NotImplementedException);
_REGISTER_EXCEPTION(InvalidOperationException);
_REGISTER_EXCEPTION(FileLoadException);
_REGISTER_EXCEPTION(InvalidDataException);
_REGISTER_EXCEPTION(IllegalStateException);

}  // namespace Utils
}  // namespace OpenOasis