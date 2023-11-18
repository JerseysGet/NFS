#ifndef __OPERATIONS_H
#define __OPERATIONS_H

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../common/error/error.h"
#include "../../common/networking/networking.h"
#include "../../common/networking/requests/copy.h"

ErrorCode readFromFile(char* path, char* buffer, size_t bufferSize);
ErrorCode writeToFile(char* path, char* buffer, size_t bufferSize);
ErrorCode createFile(char* path, mode_t permissions);
ErrorCode createDirectory(char* path, mode_t permissions);
ErrorCode deleteFile(char* path);
ErrorCode deleteDirectory(char* path);

ErrorCode CopySend(char* dst);
ErrorCode CopyRecieve(char* src);

ErrorCode GetFileInfo(char* path, struct stat* info);
ErrorCode GetDirectoryInfo(char* path, struct stat* info);
bool IsDirectory(char* path);
ErrorCode GetDirectorySize(char* path, int* size);

ErrorCode ExecuteWrite(WriteRequest* Req);
ErrorCode ExecuteRead(ReadRequest* Req, int clientfd);
ErrorCode ExecuteList(ListRequest* Req, int clientfd);
ErrorCode ExecuteMD(MDRequest* Req, int clientfd);
ErrorCode ExecuteRequest(RequestType reqType, void* request, int clientfd);

void CleanPacket(CopyPacket* packet);
ErrorCode send_INFOPKT(int sockfd, char* path, bool IsDir);

ErrorCode send_STOPPKT(int sockfd);

ErrorCode send_STARTPKT(int sockfd);

ErrorCode send_DATAPKT(int sockfd, char* data, int size);

ErrorCode ReceivePacket(int sockfd, CopyPacket* packet);

ErrorCode CopyDirectory_Send(char* src, char* dst, int sockfd);

ErrorCode SendfileData(char* filename, int sockfd);

ErrorCode ReceiveFileData(char* filename, int sockfd);
// inside indst path
ErrorCode CopyDirectory_Recieve(char* src, char* dst, int sockfd);

#endif