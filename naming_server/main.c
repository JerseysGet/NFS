#include "naming_server.h"

int main() {
    initNM();
    
    createSSListenerThread(&namingServer.ssListener);
    createSSAliveThread(&namingServer.ssAliveChecker);
    createClientListenerThread(&namingServer.clientListener);
    createClientAliveThread(&namingServer.clientAliveChecker);
    pthread_join(namingServer.ssListener, NULL);
    pthread_join(namingServer.ssAliveChecker, NULL);
    pthread_join(namingServer.clientListener, NULL);
    pthread_join(namingServer.clientAliveChecker, NULL);
    destroyNM();
}