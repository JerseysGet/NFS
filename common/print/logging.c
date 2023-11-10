#include "logging.h"

#include <stdarg.h>
#include <time.h>

char __LOG_BUFFER[LOG_BUFFER_SIZE];

#define UNUSED(x) \
    do {          \
        (void)x;  \
    } while (0)

#define MAX_LOG_FILE_NAME 64
#define LOG_FILE_FORMAT "%s%02d_%02d_%02d-%02d:%02d:%02d.log"


ErrorCode createLogFile(FILE** ret, char* prefix) {
    time_t timer = time(NULL);
    struct tm timeNow = *localtime(&timer);
    char logFileName[MAX_LOG_FILE_NAME];
    sprintf(logFileName, LOG_FILE_FORMAT, prefix, timeNow.tm_mday, timeNow.tm_mon + 1, timeNow.tm_year % 100, timeNow.tm_hour, timeNow.tm_min, timeNow.tm_sec);
#ifdef DEBUG
    printf("Logging to %s\n", logFileName);
#endif
    *ret = fopen(logFileName, "w");
    return SUCCESS;
}