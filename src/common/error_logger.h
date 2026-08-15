#ifndef COMMON_ERROR_LOGGER_H
#define COMMON_ERROR_LOGGER_H

#include <fstream>
#include <string>

namespace common {

class ErrorLogger {
public:
    ErrorLogger();
    ~ErrorLogger();

    ErrorLogger(const ErrorLogger&) = delete;
    ErrorLogger& operator=(const ErrorLogger&) = delete;

    ErrorLogger(ErrorLogger&&) = delete;
    ErrorLogger& operator=(ErrorLogger&&) = delete;

    void LogError(const std::string& message);

private:
    const std::string m_logFileName = "LogFile.txt";
    std::fstream m_logFileHandle;
};
}  // namespace common

#endif