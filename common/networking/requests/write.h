#ifndef __WRITE_H
#define __WRITE_H 

#include "../../error/error.h"

#define MAX_PATH_SIZE 256
#define MAX_DATA_SIZE 4096

typedef struct WriteRequest {
    char path[MAX_PATH_SIZE];
    char Data[MAX_DATA_SIZE];
} WriteRequest;

#endif