#include "client.h"

int main() {
    Client client;
    ClientInitRequest req;
    if (initClient(&client)) {
        destroyClient(&client);
        return FAILURE;
    }
    req.clientAlivePort = client.aliveSockPort;
    req.clientPassivePort = client.passiveSockPort;
    connectToServer(client.nmSockfd, CLIENT_LISTEN_PORT);
    if (sendClientRequest(client.nmSockfd, &req)) {
        destroyClient(&client);
        return FAILURE;
    }
    destroyClient(&client);
}