#include "storage_server.h"

int main() {
    StorageServer ss;
    if (initSS(&ss)) {
        destroySS(&ss);
        return FAILURE;
    }
    ThreadForClient client;
    initThreadForClient(&client);

    if (connectToNM(&ss)) {
        destroySS(&ss);
        return FAILURE;
    }
     
    startThreadForClient(&client);
    pthread_join(client.thread,NULL);
    destroySS(&ss);
}