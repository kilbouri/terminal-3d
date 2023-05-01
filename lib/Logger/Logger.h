#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

// =================
// MACRO DEFINITIONS
// =================
#define LEVEL_DEBUG_I 0
#define LEVEL_INFO_I 1
#define LEVEL_WARNING_I 2
#define LEVEL_ERROR_I 3

// ================
// TYPE DEFINITIONS
// ================
typedef enum {
    Debug = LEVEL_DEBUG_I,
    Info = LEVEL_INFO_I,
    Warning = LEVEL_WARNING_I,
    Error = LEVEL_ERROR_I
} LogLevel;

typedef struct {
    FILE* outTarget;
    LogLevel logLevel;
} Logger;

// ==========
// OPERATIONS
// ==========
Logger GetLogger(FILE* logTarget, LogLevel minLevel);
Logger GetFileLogger(char* logTarget, LogLevel minLevel);
void CloseLogger(Logger logger);
void LogDebug(Logger logger, const char* __restrict__ __format, ...);
void LogInfo(Logger logger, const char* __restrict__ __format, ...);
void LogWarning(Logger logger, const char* __restrict__ __format, ...);
void LogError(Logger logger, const char* __restrict__ __format, ...);

#endif // LOGGER_H
