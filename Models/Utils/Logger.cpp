/** ***********************************************************************************
 *    @File      :  Logger.cpp
 *    @Brief     :  Provide logger.
 *
 ** ***********************************************************************************/
#include "Logger.h"
#include <unordered_map>
#include <mutex>


namespace OpenOasis::Utils
{
using namespace std;


int Logger::DEFAULT_LOGGER_SIZE = 1024 * 1024 * 5;
int Logger::DEFAULT_FILE_NUM    = 9;

unordered_map<string, shared_ptr<spdlog::logger>> Logger::mLoggers = {
    {"OasisLog",
     spdlog::rotating_logger_mt(
         "OasisLog", "OasisLog", DEFAULT_LOGGER_SIZE, DEFAULT_FILE_NUM)}};

static mutex mtx;

shared_ptr<spdlog::logger> Logger::GetLogger(const string &loggerId)
{
    unique_lock<mutex> guard(mtx);

    if (mLoggers.find(loggerId) != mLoggers.end())
    {
        mLoggers[loggerId]->set_level(spdlog::level::warn);
        return mLoggers[loggerId];
    }
    else
    {
        auto logger = spdlog::rotating_logger_mt(
            loggerId, loggerId, DEFAULT_LOGGER_SIZE, DEFAULT_FILE_NUM);
        logger->set_level(spdlog::level::warn);

        mLoggers.insert(make_pair(loggerId, logger));
        return logger;
    }
}

void Logger::SetLevel(const string &level, const string &loggerId)
{
    GetLogger(loggerId)->set_level(spdlog::level::from_str(level));
}

void Logger::Debug(const string &message, const string &loggerId)
{
    GetLogger(loggerId)->debug(message);
}

void Logger::Info(const string &message, const string &loggerId)
{
    GetLogger(loggerId)->info(message);
}

void Logger::Warn(const string &message, const string &loggerId)
{
    GetLogger(loggerId)->warn(message);
}

void Logger::Error(const string &message, const string &loggerId)
{
    GetLogger(loggerId)->error(message);
}

}  // namespace OpenOasis::Utils