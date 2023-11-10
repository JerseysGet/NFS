#ifndef __STORAGE_SERVER_H
#define __STORAGE_SERVER_H

#include "../common/error/error.h"
#include "../common/networking/nm_ss/ss_connect.h"

extern FILE* logFile;

typedef struct StorageServer {
    int aliveSockfd;       /* Passive socket used by NM to check if SS is alive */
    int aliveSockPort;     /* Port for aliveSockfd */
    int clientSockfd;      /* Passive socket for client to connect to SS */
    int clientSockPort;    /* Port for clientSockfd */
    int passiveSockfd;     /* Passive socket used by nm to communicate with Ss*/
    int passiveSockPort;   /* Port for PassiveSockfd */
    AccessiblePaths paths; /* List of paths accessible by SS */
    int nmSockfd;          /* Active socket for talking to NM */
} StorageServer;

ErrorCode initSS(StorageServer* ss);
ErrorCode inputPaths(StorageServer* ss);
void destroySS(StorageServer* ss);

#endif