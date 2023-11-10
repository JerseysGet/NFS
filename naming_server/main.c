#include "naming_server.h"

int main() {
    initNM();
    createSSListenerThread(&namingServer.ssListener);
    createSSAliveThread(&namingServer.ssAliveChecker);
    pthread_join(namingServer.ssListener, NULL);
    pthread_join(namingServer.ssAliveChecker, NULL);
    destroyNM();
}