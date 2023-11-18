#ifndef __STORAGE_SERVER_H
#define __STORAGE_SERVER_H

#include <stdio.h>

#include "../common/error/error.h"
#include "../common/networking/networking.h"
#include "../common/networking/nm_ss/ss_connect.h"
#include "./threads/thread_for_client.h"
#include "../common/print/logging.h"
#include "./threads/thread_for_client.h"

typedef struct StorageServer {
    int aliveSockfd;       /* Passive socket used by NM to check if SS is alive */
    int aliveSockPort;     /* Port for aliveSockfd */
    int passiveSockfd;     /* Passive socket used by nm to communicate with SS   */
    int passiveSockPort;   /* Port for PassiveSockfd */
    AccessiblePaths paths; /* List of paths accessible by SS */
    int nmSockfd;          /* Active socket for talking to NM */
    ThreadForClient cltThread;
} StorageServer;

ErrorCode initSS(StorageServer* ss);
ErrorCode inputPaths(StorageServer* ss);
ErrorCode connectToNM(StorageServer* ss);
void destroySS(StorageServer* ss);

#endif