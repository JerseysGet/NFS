#include "alive_socket_thread.h"

#include "../print/logging.h"

void cleanAliveSocketThread(AliveSocketThread* thread) {
    close(thread->aliveSocket);
}   

void* aliveSocketThreadRoutine(void* arg) {
    AliveSocketThread* thread = (AliveSocketThread*) arg;
    
    while (!isCleaningUp()) {
        
    }
    
    lprintf("Alive Thread : Cleaning up");
    cleanAliveSocketThread(thread);
}