#include "common/error_logger.h"

#include <fstream>

namespace common {

ErrorLogger::ErrorLogger() : m_logFileHandle(m_logFileName, std::ios::in | std::ios::out | std::ios::app) {
    m_logFileHandle << "New App Started: Beginning Error Logging\n";
}

ErrorLogger::~ErrorLogger() {
    m_logFileHandle.close();
}

void ErrorLogger::LogError(const std::string& message) {
    m_logFileHandle << message;
}

}  // namespace common