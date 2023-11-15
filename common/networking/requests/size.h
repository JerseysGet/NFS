#ifndef __SIZE_H 
#define __SIZE_H

#include "../../error/error.h"

#define MAX_PATH_SIZE 256

typedef struct SizeRequest {
    char path[MAX_PATH_SIZE];
} SizeRequest;

#endif