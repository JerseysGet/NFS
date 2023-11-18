#include "storage_server.h"

#include <stdio.h>
#include <sys/socket.h>

#include "../common/networking/networking.h"
#include "../common/print/logging.h"

#define JOIN_IF_CREATED(T, RET)      \
    do {                             \
        if (T) pthread_join(T, RET); \
    } while (0)

StorageServer ss;

void cleanupSuccess();

ErrorCode initSS() {
    ss.isCleaningup = 0;
    ss.exitCode = 0;

    initEscapeHatch(cleanupSuccess);

    if (initLogger("logs/storage_server/", false)) {
        eprintf("Could not create log file\n");
        return FAILURE;
    }

    if (startLogging()) {
        eprintf("Could not start logging\n");
        goto destroy_logger;
    }

    if (initAliveSocketThread(&ss.aliveThread)) {
        goto destroy_logging;
    }

    if (initThreadForClient(&ss.cltThread)) {
        goto destroy_alive_socket_thread;
    }

    lprintf("Main : Creating Passive Socket for SS's Passive Socket");
    if (createPassiveSocket(&ss.passiveSockfd, 0)) {
        goto destroy_thread_for_client;
    }
    lprintf("Main : Getting port for SS's Passive Socket");

    if (getPort(ss.passiveSockfd, &ss.passiveSockPort)) {
        goto destroy_passive_socket;
    }

    inputPaths(ss);
    lprintf("Main : Creating Active Socket for SS's NM Socket");
    if (createActiveSocket(&ss.nmSockfd))
        goto destroy_passive_socket;

    return SUCCESS;

    // destroy_active_socket:
    //     close(ss.nmSockfd);

destroy_passive_socket:
    close(ss.passiveSockfd);

destroy_thread_for_client:

destroy_alive_socket_thread:

destroy_logging:
    endLogging();

destroy_logger:
    destroyLogger();

    return FAILURE;
}

void destroySS() {
    lprintf("Main : Closing all sockfds of SS");

    shutdown(ss.aliveThread.aliveSocket, SHUT_RDWR);
    JOIN_IF_CREATED(ss.aliveThread.thread, NULL);

    endLogging();
    destroyLogger();
    JOIN_IF_CREATED(getLoggingThread(), NULL);
    closeSocket(ss.nmSockfd);
    exit(ss.exitCode);
}

ErrorCode inputPaths() {
    printf("Enter the number of paths: ");
    scanf("%d", &ss.paths.count);
    for (int i = 0; i < ss.paths.count; i++) {
        printf("Enter path %d: ", i + 1);
        scanf("%s", ss.paths.pathList[i]);
    }
    return SUCCESS;
}

ErrorCode connectToNM() {
    SSInitRequest req;
    req.paths = ss.paths;
    req.SSAlivePort = ss.aliveThread.alivePort;
    req.SSPassivePort = ss.passiveSockPort;
    req.SSClientPort = ss.cltThread.clientSockPort;
    connectToServer(ss.nmSockfd, SS_LISTEN_PORT);
    if (sendSSRequest(ss.nmSockfd, &req)) {
        return FAILURE;
    }

    return SUCCESS;
}

void initiateCleanup(ErrorCode exitCode) {
    ss.exitCode = exitCode;
    ss.isCleaningup = 1;
}

bool isCleaningUp() {
    return false;
}

void cleanupSuccess() {
    ss.isCleaningup = 1;
    destroySS();
}