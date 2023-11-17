#ifndef __COPY_H
#define __COPY_H

#include "../../error/error.h"

#define MAX_PATH_SIZE 256

typedef struct CopyRequest {
    char SrcPath[MAX_PATH_SIZE];
    char DestPath[MAX_PATH_SIZE];
    // more content to be defined;
} CopyRequest;


#endif
