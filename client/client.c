#include "client.h"

#include <stdio.h>
#include <stdlib.h>

#include "../common/networking/networking.h"
#include "../common/print/logging.h"
ErrorCode initClient(Client* client) {
    client->exitCode = SUCCESS;
    if (initLogger("logs/client/", false)) {
        eprintf("Could not create log file\n");
        return FAILURE;
    }

    if (startLogging()) {
        eprintf("Could not start logging\n");
        goto destroy_logger;
    }

    lprintf("Main : Creating Passive Socket for Client's Alive Socket");
    if (createPassiveSocket(&client->aliveSockfd, 0)) {
        goto destroy_logging;
    }

    lprintf("Main : Getting port for Client's Alive Socket");
    if (getPort(client->aliveSockfd, &client->aliveSockPort)) {
        goto destroy_alivesockfd;
    }

    lprintf("Main : Creating Passive Socket for Client's Passive Socket");
    if (createPassiveSocket(&client->passiveSockfd, 0)) {
        goto destroy_alivesockfd;
    }

    lprintf("Main : Getting port for Client's Passive Socket");
    if (getPort(client->passiveSockfd, &client->passiveSockPort)) {
        goto destroy_passivesockfd;
    }

    lprintf("Main : Creating Active Socket for Client's NM Socket");
    if (createActiveSocket(&client->nmSockfd)) {
        goto destroy_passivesockfd;
    }

    return SUCCESS;

// destroy_nmsockfd:
//     close(client->nmSockfd);

destroy_passivesockfd:
    close(client->passiveSockfd);

destroy_alivesockfd:
    close(client->aliveSockfd);

destroy_logging:
    endLogging();

destroy_logger:
    destroyLogger();

    return FAILURE;
}

void destroyClient(Client* client) {
    lprintf("Main : Closing all sockfds in Client");
    closeSocket(client->aliveSockfd);
    closeSocket(client->passiveSockfd);
    closeSocket(client->nmSockfd);
    endLogging();
    destroyLogger();
    exit(client->exitCode);
}

ErrorCode connectToNM(Client* client) {
    if (connectToServer(client->nmSockfd, CLIENT_LISTEN_PORT)) {
        client->exitCode = FAILURE;
        return FAILURE;    
    }
    
    ClientInitRequest req;
    req.clientAlivePort = client->aliveSockPort;
    req.clientPassivePort = client->passiveSockPort;

    if (sendClientRequest(client->nmSockfd, &req)) {
        client->exitCode = FAILURE;
        return FAILURE;
    }

    return SUCCESS;
}