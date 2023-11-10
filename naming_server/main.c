#include "naming_server.h"

int main() {
    initNM();
    createSSListenerThread(&namingServer.ssListener);
    pthread_join(namingServer.ssListener, NULL);
    destroyNM();
}