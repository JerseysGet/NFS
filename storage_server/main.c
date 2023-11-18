#include "storage_server.h"

int main() {
    StorageServer ss;
    if (initSS(&ss)) {
        destroySS(&ss);
        return FAILURE;
    }


    initThreadForClient(&ss.cltThread);

    if (connectToNM(&ss)) {
        destroySS(&ss);
        return FAILURE;
    }
     
    startThreadForClient(&ss.cltThread);
    pthread_join(ss.cltThread.thread,NULL);
    destroySS(&ss);
}