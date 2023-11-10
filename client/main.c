#include "client.h"

int main() {
    Client client;
    if (initClient(&client)) {
        destroyClient(&client);
        return FAILURE;
    }

    destroyClient(&client);
}