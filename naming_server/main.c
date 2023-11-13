#include "naming_server.h"

int main() {
    if (initNM()) {
        
    }
    
    createSSListenerThread(&namingServer.ssListener);
    createSSAliveThread(&namingServer.ssAliveChecker);
    createClientListenerThread(&namingServer.clientListener);
    createClientAliveThread(&namingServer.clientAliveChecker);
    
    pthread_mutex_lock(&namingServer.cleanupLock);
    while (!namingServer.isCleaningup) {
        pthread_mutex_unlock(&namingServer.cleanupLock);
        pthread_mutex_lock(&namingServer.cleanupLock);
    }
    pthread_mutex_unlock(&namingServer.cleanupLock);
    // pthread_join(namingServer.ssListener, NULL);
    // pthread_join(namingServer.ssAliveChecker, NULL);
    // pthread_join(namingServer.clientListener, NULL);
    // pthread_join(namingServer.clientAliveChecker, NULL);
    destroyNM();
}