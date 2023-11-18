#ifndef __STORAGE_SERVER_H
#define __STORAGE_SERVER_H

#include <signal.h>
#include <stdio.h>

#include "../common/error/error.h"
#include "../common/networking/networking.h"
#include "../common/networking/nm_ss/ss_connect.h"
#include "../common/signals/cleanup_signal.h"
#include "../common/threads/alive_socket_thread.h"
#include "./threads/thread_for_client.h"
#include "../common/print/logging.h"
#include "./threads/thread_for_client.h"

typedef struct StorageServer {
    int passiveSockfd;     /* Passive socket used by nm to communicate with SS   */
    int passiveSockPort;   /* Port for PassiveSockfd */
    AccessiblePaths paths; /* List of paths accessible by SS */
    int nmSockfd;          /* Active socket for talking to NM */
    ThreadForClient cltThread;
    AliveSocketThread aliveThread;
    /* Cleanup stuff */
    sig_atomic_t isCleaningup;
    ErrorCode exitCode;
} StorageServer;

extern StorageServer ss;

ErrorCode initSS();
ErrorCode inputPaths();
ErrorCode connectToNM();
void destroySS();

#endif