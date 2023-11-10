#ifndef __LOGGING_H
#define __LOGGING_H

#include <stdio.h>

#include "../error/error.h"

/* Cannot log more than this at a time */
#define LOG_BUFFER_SIZE 4096
extern char __LOG_BUFFER[LOG_BUFFER_SIZE];

#ifdef DEBUG
#define LOG(...)                            \
    do {                                    \
        sprintf(__LOG_BUFFER, __VA_ARGS__); \
        printf("[LOG] %s", __LOG_BUFFER);         \
        fprintf(logFile, __LOG_BUFFER);     \
    } while (0)
#else
#define LOG(...)                       \
    do {                               \
        fprintf(logFile, __VA_ARGS__); \
    } while (0)
#endif

#define NM_LOGS "logs/naming_server/"
#define SS_LOGS "logs/storage_server/"
#define CLIENT_LOGS "logs/client/"

ErrorCode createLogFile(FILE** ret, char* prefix);

#endif