#ifndef __DELETE_H
#define __DELETE_H

#include "../../error/error.h"

#define MAX_PATH_SIZE 256

typedef struct DeleteRequest {
    char path[MAX_PATH_SIZE];
    char ToBeDeleted[MAX_PATH_SIZE];
} DeleteRequest;

#endif