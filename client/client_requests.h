#ifndef __CLIENT_REQUEST_H
#define __CLIENT_REQUEST_H

#include "client.h"
#include <sys/stat.h>

#include "client_requests.h"

void printFileInfo(struct stat *fileStat);
ErrorCode ReadResponseHandler(int sockfd);
ErrorCode MetaDataResponseHandler(int sockfd);
ErrorCode ListResponseHandler(int sockfd);
ErrorCode inputAndSendRequest();

#endif
