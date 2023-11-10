#include "storage_server.h"

int main() {
    StorageServer ss;
    SSInitRequest req;
    initSS(&ss);
    req.paths = ss.paths;
    req.SSAlivePort = ss.aliveSockPort;
    req.SSClientPort = ss.clientSockPort;
    req.SSPassivePort = ss.passiveSockfd;
    if (sendSSRequest(ss.nmSockfd, &req)) {
        return FAILURE;
    }
    destroySS(&ss);
}