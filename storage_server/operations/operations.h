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
#include<errno.h>
#include<sys/types.h>
#include "../../common/error/error.h"
#include "../../common/networking/requests/copy.h"
#include "../../common/networking/networking.h"

ErrorCode readFromFile(char* path, char* buffer, size_t bufferSize);
ErrorCode writeToFile(char* path, char* buffer, size_t bufferSize);
ErrorCode createFile(char* path,mode_t permissions);
ErrorCode createDirectory(char* path,mode_t permissions);
ErrorCode deleteFile(char* path);
ErrorCode deleteDirectory(char* path);

ErrorCode CopySend(char* dst);
ErrorCode CopyRecieve(char* src);

ErrorCode GetFileInfo(char* path,struct stat *info);
ErrorCode GetDirectoryInfo(char* path,struct stat *info);
ErrorCode IsDirectory(char* path);
ErrorCode GetDirectorySize(char* path,int* size);


ErrorCode ExecuteWrite(WriteRequest* Req);
ErrorCode ExecuteRead(ReadRequest* Req,int clientfd);
ErrorCode ExecuteMD(MDRequest* Req,int clientfd);
ErrorCode ExecuteRequest(RequestType reqType,void* request,int clientfd);



void CleanPacket(CopyPacket* packet);
ErrorCode send_INFOPKT(int sockfd,char* path,bool IsDir);

ErrorCode send_STOPPKT(int sockfd);

ErrorCode send_STARTPKT(int sockfd);

ErrorCode send_DATAPKT(int sockfd,char* data,int size);

ErrorCode ReceivePacket(int sockfd,CopyPacket* packet);

ErrorCode CopyDirectory_Send(char* src,char* dst,int sockfd);

ErrorCode SendfileData(char* filename,int sockfd);

ErrorCode ReceiveFileData(char* filename,int sockfd);
// inside indst path
ErrorCode CopyDirectory_Recieve(char* src,char* dst,int sockfd);
// ErrorCode getFileSize(char* path, int* size);
// ErrorCode getDirectorySize(char* path, int* size);

// ErrorCode getPermissions(char* path, int* permissions);


#endif