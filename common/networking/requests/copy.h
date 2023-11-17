#ifndef __COPY_H
#define __COPY_H

#include "../../error/error.h"

#define MAX_PATH_SIZE 256
#define MAX_COPY_HEADER 64
#define MAX_DATA_LENGTH  1024 // FOR ONE PACKET

typedef struct CopyRequest {
    char SrcPath[MAX_PATH_SIZE];
    char DestPath[MAX_PATH_SIZE];
    // more content to be defined;
} CopyRequest;

typedef enum Copyheader {
    START,
    STOP,
    INFO,
    DATA
}Copyheader;

typedef struct FileInfo {
    char ObjectName[MAX_PATH_SIZE];
    bool IsDirectory;
}FileInfo;

typedef struct CopyPacket {
    Copyheader header;
    FileInfo ObjectInfo;
    char Data[1024];
}CopyPacket;

#endif
