#include "storage_server.h"

int main() {
    if (initSS(&ss)) {
        destroySS(&ss);
        return FAILURE;
    }


    if (connectToNM(&ss)) {
        destroySS(&ss);
        return FAILURE;
    }

    startAliveSocketThread(&ss.aliveThread);     
    startThreadForClient(&ss.cltThread);
    pthread_join(ss.cltThread.thread,NULL);
    destroySS(&ss);
}