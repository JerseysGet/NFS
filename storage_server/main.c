#include "storage_server.h"

int main() {
    StorageServer ss;
    SSInitRequest req;
    if (initSS(&ss)) {
        destroySS(&ss);
        return FAILURE;
    }
    req.paths = ss.paths;
    req.SSAlivePort = ss.aliveSockPort;
    req.SSClientPort = ss.clientSockPort;
    req.SSPassivePort = ss.passiveSockfd;
    connectToServer(ss.nmSockfd, SS_LISTEN_PORT);
    if (sendSSRequest(ss.nmSockfd, &req)) {
        destroySS(&ss);
        return FAILURE;
    }
    destroySS(&ss);
}