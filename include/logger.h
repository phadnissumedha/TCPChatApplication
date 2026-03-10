#ifndef LOGGER_H
#define LOGGER_H

#include <string>

void logMessage(std::string level, std::string message);

/* Logging macro */
#define LOG_MSG(level,message) logMessage(level,message)

#endif