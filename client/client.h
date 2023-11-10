#ifndef __CLIENT_H
#define __CLIENT_H
#include "../common/error/error.h"
#include "../common/networking/networking.h"

typedef struct Client {
    int passiveSockfd;   /* Temporary */
    int passiveSockPort; /* Temporary */
    int nmSockfd;        /* To communicate with naming server */
    int aliveSockfd;     /* Passive socket used by NM to check if client is alive */
    int aliveSockPort;   /* Port for aliveSockfd */
} Client;

ErrorCode initClient(Client* ss);
void destroyClient(Client* ss);

#endif