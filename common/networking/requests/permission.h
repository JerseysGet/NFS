#ifndef __PERMISSION_H 
#define __PERMISSION_H 

#include "../../error/error.h"

#define MAX_PATH_SIZE 256

typedef struct PermnRequest {
    char path[MAX_PATH_SIZE];
} PermRequest;

#endif