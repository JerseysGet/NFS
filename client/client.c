#include "client.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../common/networking/networking.h"

#define JOIN_IF_CREATED(T, RET)      \
    do {                             \
        if (T) pthread_join(T, RET); \
    } while (0)

Client client;

void signalSuccess();

ErrorCode initClient() {
    pthread_mutex_init(&client.cleanupLock, NULL);
    client.exitCode = SUCCESS;
    if (initLogger("logs/client/", false)) {
        eprintf("Could not create log file\n");
        goto destroy_cleanup_lock;
    }

    if (startLogging()) {
        eprintf("Could not start logging\n");
        goto destroy_logger;
    }

    initEscapeHatch(signalSuccess);

    lprintf("Main : Creating Passive Socket for Client's Alive Socket");
    // if (createPassiveSocket(&client.aliveSockfd, 0)) {
    //     goto destroy_logging;
    // }

    // lprintf("Main : Getting port for Client's Alive Socket");
    // if (getPort(client.aliveSockfd, &client.aliveSockPort)) {
    //     goto destroy_alivesockfd;
    // }
    lprintf("Main : Creating Alive Thread for Client's Alive Socket");
    if (initAliveSocketThread(&client.thread)) {
        goto destroy_logging;
    }

    lprintf("Main : Creating Passive Socket for Client's Passive Socket");
    if (createPassiveSocket(&client.passiveSockfd, 0)) {
        goto destroy_aliveSocketthread;
    }

    lprintf("Main : Getting port for Client's Passive Socket");
    if (getPort(client.passiveSockfd, &client.passiveSockPort)) {
        goto destroy_passivesockfd;
    }

    lprintf("Main : Creating Active Socket for Client's NM Socket");
    if (createActiveSocket(&client.nmSockfd)) {
        goto destroy_passivesockfd;
    }

    return SUCCESS;

    // destroy_nmsockfd:
    //     close(client.nmSockfd);

destroy_passivesockfd:
    close(client.passiveSockfd);

destroy_aliveSocketthread:
    close(client.thread.aliveSocket);

destroy_logging:
    destroyEscapeHatch();
    endLogging();

destroy_logger:
    destroyLogger();

destroy_cleanup_lock:
    pthread_mutex_destroy(&client.cleanupLock);

    return FAILURE;
}

bool isCleaningUp() {
    // printf("trying to LOCK\n");
    pthread_mutex_lock(&client.cleanupLock);
    // printf("GOT LOCK\n");
    bool ret = client.isCleaningup;
    // printf("GOT RET\n");
    pthread_mutex_unlock(&client.cleanupLock);
    // printf("UNLOCKED\n");
    return ret;
}

void initiateCleanup(ErrorCode exitCode) {
    // printf("trying to lock\n");
    pthread_mutex_lock(&client.cleanupLock);
    // printf("signal cleanup got lock\n");
    client.exitCode = exitCode;
    client.isCleaningup = true;
    pthread_mutex_unlock(&client.cleanupLock);
    // printf("signal cleanup unlocked\n");
}

void destroyClient() {
    lprintf("Main : Closing all sockfds in Client");

    shutdown(client.thread.aliveSocket, SHUT_RDWR);

    lprintf("Main : Alive_socket joined");
    closeSocket(client.passiveSockfd);
    closeSocket(client.nmSockfd);

    endLogging();
    destroyLogger();
    JOIN_IF_CREATED(getLoggingThread(), NULL);
    pthread_mutex_destroy(&client.cleanupLock);
    exit(client.exitCode);
}

ErrorCode connectToNM() {
    if (connectToServer(client.nmSockfd, CLIENT_LISTEN_PORT)) {
        client.exitCode = FAILURE;
        return FAILURE;
    }

    ClientInitRequest req;
    req.clientAlivePort = client.thread.alivePort;
    req.clientPassivePort = client.passiveSockPort;

    if (sendClientRequest(client.nmSockfd, &req)) {
        client.exitCode = FAILURE;
        return FAILURE;
    }

    return SUCCESS;
}

void* inputRequest(RequestType* requestType) {
    int option;
    printf("Select an option:\n");
    printf("1. Write to File\n");
    printf("2. Read a file\n");
    printf("3. Delete a file\n");
    printf("4. Delete a folder\n");
    printf("5. Create a file\n");
    printf("6. Create a folder\n");
    printf("7. List a folder\n");
    printf("8. Get size\n");
    printf("9. Get permissions\n");
    scanf("%d", &option);

    void* ret = NULL;
    switch (option) {
        case 1:
            *requestType = REQUEST_WRITE;
            ret = calloc(1, sizeof(WriteRequest));
            printf("Enter file path: ");
            scanf("%s", ((WriteRequest*)ret)->path);
            printf("Enter content to write: ");
            scanf("%s", ((WriteRequest*)ret)->Data);
            break;
        case 2:
            *requestType = REQUEST_READ;
            ret = calloc(1, sizeof(ReadRequest));
            printf("Enter file path: ");
            scanf("%s", ((ReadRequest*)ret)->path);
            break;
        case 3:
            *requestType = REQUEST_DELETE;
            ret = calloc(1, sizeof(DeleteRequest));
            printf("Enter file path to delete: ");
            scanf("%s", ((DeleteRequest*)ret)->path);
            break;
        case 4:
            *requestType = REQUEST_DELETE;
            ret = calloc(1, sizeof(DeleteRequest));
            printf("Enter folder path to delete: ");
            scanf("%s", ((DeleteRequest*)ret)->path);
            break;
        case 5:
            *requestType = REQUEST_CREATE;
            ret = calloc(1, sizeof(CreateRequest));
            printf("Enter file path: ");
            scanf("%s", ((CreateRequest*)ret)->path);
            printf("Enter file name: ");
            scanf("%s", ((CreateRequest*)ret)->newObject);
            ((CreateRequest*)ret)->isDirectory = false;
            break;
        case 6:
            *requestType = REQUEST_CREATE;
            ret = calloc(1, sizeof(CreateRequest));
            printf("Enter folder path: ");
            scanf("%s", ((CreateRequest*)ret)->path);
            ((CreateRequest*)ret)->isDirectory = true;
            break;
        case 7:
            // TODO !!
            *requestType = REQUEST_LIST;
            // ret = calloc(1, sizeof());
            // printf("Enter folder path to list: ");
            // scanf("%s", path);
            break;
        case 8:
            *requestType = REQUEST_SIZE;
            ret = calloc(1, sizeof(SizeRequest));
            printf("Enter file/folder path to get size: ");
            scanf("%s", ((SizeRequest*)ret)->path);
            break;
        case 9:
            *requestType = REQUEST_PERMISSION;
            ret = calloc(1, sizeof(PermRequest));
            printf("Enter file/folder path to get permissions: ");
            scanf("%s", ((SizeRequest*)ret)->path);
            break;
        default:
            printf("Invalid option\n");
            return NULL;
    }

    return ret;
}

void destroyRequest(void* request) {
    free(request);
}

ErrorCode inputAndSendRequest() {
    ErrorCode ret = SUCCESS;
    RequestType type;
    void* request = NULL;
    while (request == NULL) request = inputRequest(&type);
    lprintf("Main : sending RequestType..");
    if ((ret = sendRequestType(&type, client.nmSockfd))) {
        eprintf("Could not send request type\n");
        goto destroy_request;
    }

    bool recievedAck;
    RequestTypeAck requestTypeAck;
    recieveRequestTypeAck(&requestTypeAck, client.nmSockfd, TIMEOUT_MILLIS, &recievedAck);

    if (!recievedAck) {
        eprintf("RequestTypeAck timed out\n");
        ret = FAILURE;
        goto destroy_request;
    }
    lprintf("Main : received RequestType ack");
    if ((ret = sendRequest(type, request, client.nmSockfd))) {
        eprintf("Could not send request\n");
        goto destroy_request;
    }
    lprintf("Main : request sent");
destroy_request:
    destroyRequest(request);
    return ret;
}

void signalSuccess() {
    // printf("signalSuccess : ");
    initiateCleanup(SUCCESS);
    usleep(1000000);
    JOIN_IF_CREATED(client.thread.thread, NULL);
    destroyClient();
}