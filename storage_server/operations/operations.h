#ifndef __OPERATIONS_H
#define __OPERATIONS_H

#include "../../common/error/error.h"

ErrorCode readFromFile(char* path, char* buffer, size_t bufferSize);
ErrorCode writeToFile(char* path, char* buffer, size_t bufferSize);
ErrorCode createFile(char* path);
ErrorCode createDirectory(char* path);
ErrorCode deleteFile(char* path);
ErrorCode deleteDirectory(char* path);

// ErrorCode copyDirectory(??); TODO
// ErrorCode copyFile(??); TODO

ErrorCode getFileSize(char* path, int* size);
ErrorCode getDirectorySize(char* path, int* size);

ErrorCode getPermissions(char* path, int* permissions);


#endif