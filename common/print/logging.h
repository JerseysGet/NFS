#ifndef __LOGGING_H
#define __LOGGING_H

#include <stdio.h>

#include "../error/error.h"

#define NM_LOGS "logs/naming_server/"
#define SS_LOGS "logs/storage_server/"
#define CLIENT_LOGS "logs/client/"

ErrorCode createLogFile(FILE* ret, char* prefix);
void lprintf(FILE* file, char* format, ...);

#endif