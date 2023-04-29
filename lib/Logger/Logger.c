#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#include "Logger.h"

#include <stdio.h>
#include <stdlib.h>

// ==========
// OPERATIONS
// ==========
Logger GetLogger(char* logTarget, LogLevel minLevel) {
    FILE* target = fopen(logTarget, "w");
    if (target == NULL) {
        errno = 1;
        perror("Failed to open log file");
        exit(1);
    }

    return (Logger) {
        .outTarget = target,
        .logLevel = minLevel,
    };
}

void CloseLogger(Logger logger) {
    fclose(logger.outTarget);
}

void LogDebug(Logger logger, const char* __restrict__ __format, ...) {
    if (logger.logLevel < LEVEL_DEBUG_I || logger.outTarget == NULL) {
        return;
    }

    fprintf(logger.outTarget, "[INF] ");

    va_list varargs;
    va_start(varargs, __format);
    vfprintf(logger.outTarget, __format, varargs);
    va_end(varargs);

    fprintf(logger.outTarget, "\n");
}

void LogInfo(Logger logger, const char* __restrict__ __format, ...) {
    if (logger.logLevel < LEVEL_INFO_I || logger.outTarget == NULL) {
        return;
    }

    fprintf(logger.outTarget, "[INF] ");

    va_list varargs;
    va_start(varargs, __format);
    vfprintf(logger.outTarget, __format, varargs);
    va_end(varargs);

    fprintf(logger.outTarget, "\n");
}

void LogWarning(Logger logger, const char* __restrict__ __format, ...) {
    if (logger.logLevel < LEVEL_WARNING_I || logger.outTarget == NULL) {
        return;
    }

    fprintf(logger.outTarget, "[WRN] ");

    va_list varargs;
    va_start(varargs, __format);
    vfprintf(logger.outTarget, __format, varargs);
    va_end(varargs);

    fprintf(logger.outTarget, "\n");
}

void LogError(Logger logger, const char* __restrict__ __format, ...) {
    if (logger.logLevel < LEVEL_ERROR_I || logger.outTarget == NULL) {
        return;
    }

    fprintf(logger.outTarget, "[ERR] ");

    va_list varargs;
    va_start(varargs, __format);
    vfprintf(logger.outTarget, __format, varargs);
    va_end(varargs);

    fprintf(logger.outTarget, "\n");
}
