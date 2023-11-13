#include "client.h"

#include <stdio.h>

#include "../common/networking/networking.h"
#include "../common/print/logging.h"

ErrorCode initClient(Client* client) {
    if (initLogger("logs/storage_server/", false)) {
        eprintf("Could not create log file\n");
        return FAILURE;
    }

    if (startLogging()) {
        eprintf("Could not start logging\n");
        return FAILURE;
    }

    lprintf("Main : Creating Passive Socket for Client's Alive Socket");
    if (createPassiveSocket(&client->aliveSockfd, 0)) {
        return FAILURE;
    } else {
        lprintf("Main : Getting port for Client's Alive Socket");
        if (getPort(client->aliveSockfd, &client->aliveSockPort))
            return FAILURE;
    }
    lprintf("Main : Creating Passive Socket for Client's Passive Socket");
    if (createPassiveSocket(&client->passiveSockfd, 0)) {
        return FAILURE;
    } else {
        lprintf("Main : Getting port for Client's Passive Socket");
        if (getPort(client->passiveSockfd, &client->passiveSockPort))
            return FAILURE;
    }
    lprintf("Main : Creating Active Socket for Client's NM Socket");
    if (createActiveSocket(&client->nmSockfd))
        return FAILURE;

    return SUCCESS;
}

void destroyClient(Client* client) {
    lprintf("Main : Closing all sockfds in Client");
    endLogging();
    destroyLogger();
    closeSocket(client->aliveSockfd);
    closeSocket(client->passiveSockfd);
    closeSocket(client->nmSockfd);
}