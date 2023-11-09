#include "storage_server.h"

#define MAX_PATH_LENGTH 256
struct SSInitRequest {
    // Define the structure members 
    int numPaths; // Replace 
    int pathPorts[MAX_NUM_PATHS]; // Replace 
};

int main() {
    
    int numPaths;
    printf("Enter the number of paths: ");
    scanf("%d", &numPaths);

    
    char paths[numPaths][MAX_PATH_LENGTH];
    for (int i = 0; i < numPaths; i++) {
        printf("Enter path %d: ", i + 1);
        scanf("%s", paths[i]);
    }

   
    struct SSInitRequest initRequest;
    initRequest.numPaths = numPaths;
    for (int i = 0; i < numPaths; i++) {
        
        initRequest.pathPorts[i] = rand() % 65536;
    }

  
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("Error creating socket");
        exit(1);
    }

    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SS_LISTEN_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the actual server IP address

    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        close(socket_fd);
        exit(1);
    }

    
    if (send(socket_fd, &initRequest, sizeof(initRequest), 0) == -1) {
        perror("Error sending data");
        close(socket_fd);
        exit(1);
    }

    
    close(socket_fd);
    return 0;
}