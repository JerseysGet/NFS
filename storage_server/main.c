#include "storage_server.h"

int main() {
    StorageServer ss;
    if (initSS(&ss)) {
        destroySS(&ss);
        return FAILURE;
    }
    
    if (connectToNM(&ss)) {
        destroySS(&ss);
        return FAILURE;
    }

    destroySS(&ss);
}