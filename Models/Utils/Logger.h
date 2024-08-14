/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  Logger.h
 *    @License   :  Apache-2.0
 *
 ** ***********************************************************************************/
#pragma once
#include "ThirdPart/spdlog/spdlog.h"
#include "ThirdPart/spdlog/sinks/rotating_file_sink.h"


namespace OpenOasis
{
namespace Utils
{
/// @brief To get rotating logger with specified id.
class Logger
{
public:
    static std::shared_ptr<spdlog::logger>
    GetLogger(const std::string &loggerId = "OasisLog");

    static void
    SetLevel(const std::string &level, const std::string &loggerId = "OasisLog");

    static void
    Debug(const std::string &message, const std::string &loggerId = "OasisLog");

    static void
    Info(const std::string &message, const std::string &loggerId = "OasisLog");

    static void
    Warn(const std::string &message, const std::string &loggerId = "OasisLog");

    static void
    Error(const std::string &message, const std::string &loggerId = "OasisLog");


private:
    static int DEFAULT_LOGGER_SIZE;
    static int DEFAULT_FILE_NUM;

    static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> mLoggers;
};

}  // namespace Utils
}  // namespace OpenOasis
