#ifndef __NETWORKING_H
#define __NETWORKING_H

#include <stddef.h>

#include "../error/error.h"

extern const char* GLOBAL_IP;
extern const int CLIENT_LISTEN_PORT;
extern const int SS_LISTEN_PORT;

ErrorCode createPassiveSocket(int* sockfd, int port);
ErrorCode createActiveSocket(int* sockfd);

ErrorCode connectToServer(int clientSockfd, int serverPort);

/*
    Does not print error message in case of FAILURE
*/
ErrorCode acceptClient(int serverSockfd, int* clientSockfd);

/*
    Does not print error message in case of FAILURE
*/
ErrorCode socketSend(int sockfd, void* dataPtr, size_t bytes);

/*
    Does not print error message in case of FAILURE
*/
ErrorCode socketRecieve(int sockfd, void* dataPtr, size_t bytes);

/*
    Does not print error message in case of FAILURE
*/
ErrorCode closeSocket(int sockfd);
#endif