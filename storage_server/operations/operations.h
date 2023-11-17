#ifndef __OPERATIONS_H
#define __OPERATIONS_H

#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include <errno.h>
#include<sys/types.h>
#include "../../common/error/error.h"

ErrorCode readFromFile(char* path, char* buffer, size_t bufferSize);
ErrorCode writeToFile(char* path, char* buffer, size_t bufferSize);
ErrorCode createFile(char* path,mode_t permissions);
ErrorCode createDirectory(char* path,mode_t permissions);
ErrorCode deleteFile(char* path);
ErrorCode deleteDirectory(char* path);

// ErrorCode copyDirectory(??); TODO
// ErrorCode copyFile(??); TODO

ErrorCode GetFileInfo(char* path,struct stat *info);
ErrorCode GetDirectoryInfo(char* path,struct stat *info);
ErrorCode IsDirectory(char* path);
ErrorCode GetDirectorySize(char* path,int* size);

// ErrorCode getFileSize(char* path, int* size);
// ErrorCode getDirectorySize(char* path, int* size);

// ErrorCode getPermissions(char* path, int* permissions);


#endif