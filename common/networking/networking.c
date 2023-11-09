#include "networking.h"

#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

const char* GLOBAL_IP = "127.0.0.1";
int CLIENT_LISTEN_PORT = 9459;
int SS_LISTEN_PORT = 11764;

#define MAX_CONNECTION_IN_QUEUE 10

ErrorCode createPassiveSocket(int* sockfd, int port) {
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd == -1) {
        eprintf("Could not create passive socket, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(GLOBAL_IP);

    if (bind(*sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        eprintf("Could not bind to port %d, errno = %d, %s\n", port, errno, strerror(errno));
        return FAILURE;
    }

    if (listen(*sockfd, MAX_CONNECTION_IN_QUEUE) == -1) {
        eprintf("Could not listen() on port %d, errno = %d, %s\n", port, errno, strerror(errno));
        return FAILURE;
    }

    return SUCCESS;
}

ErrorCode createActiveSocket(int* sockfd) {
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd == -1) {
        eprintf("Could not create active socket, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }

    return SUCCESS;
}

ErrorCode connectToServer(int clientSockfd, int serverPort) {
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = serverPort;
    serverAddr.sin_addr.s_addr = inet_addr(GLOBAL_IP);

    if (connect(clientSockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        eprintf("Could not connect to port %d, errno = %d, %s\n", serverPort, errno, strerror(errno));
        return FAILURE;
    }

    return SUCCESS;
}

ErrorCode acceptClient(int serverSockfd, int* clientSockfd) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    clientAddrLen = sizeof(clientAddr);
    *clientSockfd = accept(serverSockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (*clientSockfd == -1) {
        return FAILURE;
    }

    return SUCCESS;
}

ErrorCode socketSend(int sockfd, void* dataPtr, size_t bytes) {
    if (send(sockfd, dataPtr, bytes, 0) == -1) {
        return FAILURE;
    }

    return SUCCESS;
}

ErrorCode socketRecieve(int sockfd, void* dataPtr, size_t bytes) {
    if (recv(sockfd, dataPtr, bytes, 0) == -1) {
        return FAILURE;
    }

    return SUCCESS;
}