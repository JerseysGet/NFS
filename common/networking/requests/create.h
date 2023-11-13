#ifndef __CREATE_H
#define __CREATE_H

#include <stdbool.h>

#define MAX_PATH_SIZE 256

typedef struct CreateRequest {
    char path[MAX_PATH_SIZE];
    char newObject[MAX_PATH_SIZE];
    bool isDirectory;
} CreateRequest;

#endif