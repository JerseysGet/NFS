#include <stdio.h>
#include <stdlib.h>
#include "../common/networking/requests/request_type.h"
#include "../common/networking/networking.h"
int main() {
    // input option

    /*
        given a socketfd to a server socket
        create a thread that accepts clients, then disconnects those clients
    */

    int port = 5555;
    int sockfd;
    createPassiveSocket(&sockfd, port);
    // given socket = sockfd
    
}