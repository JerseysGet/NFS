#include "client.h"

#include <stdio.h>

#include "../common/print/logging.h"
#include "../common/networking/networking.h"

FILE* logFile;

ErrorCode initClient(Client* client){
    if (createLogFile(&logFile, CLIENT_LOGS)) {
        eprintf("Could not create log file\n");
        return FAILURE;
    }
    LOG("Creating Passive Socket for Client's Alive Socket\n");
    if (createPassiveSocket(&client->aliveSockfd, 0)) {
        return FAILURE;
    } else {
        LOG("Getting port for Client's Alive Socket\n");
        if (getPort(client->aliveSockfd, &client->aliveSockPort))
            return FAILURE;
    }
    LOG("Creating Passive Socket for Client's Passive Socket\n");
    if (createPassiveSocket(&client->passiveSockfd, 0)) {
        return FAILURE;
    } else {
        LOG("Getting port for Client's Passive Socket\n");
        if (getPort(client->passiveSockfd, &client->passiveSockPort))
            return FAILURE;
    }
    LOG("Creating Active Socket for Client's NM Socket\n");
    if (createActiveSocket(&client->nmSockfd))
        return FAILURE;
    
    return SUCCESS;
}
void destroyClient(Client* client){
    LOG("Closing all sockfds in Client\n");
    closeSocket(client->aliveSockfd);
    closeSocket(client->passiveSockfd);
    closeSocket(client->nmSockfd);
    fclose(logFile);
}