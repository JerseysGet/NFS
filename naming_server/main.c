#include "naming_server.h"

int main() {
    initNM();
    
    createSSListenerThread(&namingServer.ssListener);
    createSSAliveThread(&namingServer.ssAliveChecker);
    createClientListenerThread(&namingServer.clientListener);
    pthread_join(namingServer.ssListener, NULL);
    pthread_join(namingServer.ssAliveChecker, NULL);
    pthread_join(namingServer.clientListener, NULL);
    destroyNM();
}