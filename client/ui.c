#include <stdio.h>
#include <stdlib.h>
#include "../common/networking/requests/request_type.h"

int main() {
    // input option
    int option;
    RequestType type;

    printf("Select an option:\n");
    printf("1. Write to File\n");
    printf("2. Read a file\n");
    printf("3. Delete a file\n");
    printf("4. Delete a folder\n");
    printf("5. Create a file\n");
    printf("6. Create a folder\n");
    printf("7. List a folder\n");
    printf("8. Get size\n");
    printf("9. Get permissions\n");

    scanf("%d", &option);

    char path[512];
    char filename[256];

    switch (option) {
        case 1:
            type = REQUEST_WRITE;
            printf("Enter file path: ");
            scanf("%s", path);
            printf("Enter content to write: ");
            scanf("%s", filename);
            break;
        case 2:
            type = REQUEST_READ;
            printf("Enter file path: ");
            scanf("%s", path);
            break;
        case 3:
            type = REQUEST_DELETE;
            printf("Enter file path to delete: ");
            scanf("%s", path);
            break;
        case 4:
            type = REQUEST_DELETE;
            printf("Enter folder path to delete: ");
            scanf("%s", path);
            break;
        case 5:
            type = REQUEST_CREATE;
            printf("Enter file path: ");
            scanf("%s", path);
            printf("Enter file name: ");
            scanf("%s", filename);
            break;
        case 6:
            type = REQUEST_CREATE;
            printf("Enter folder path: ");
            scanf("%s", path);
            break;
        case 7:
            type = //??;
            printf("Enter folder path to list: ");
            scanf("%s", path);
            break;
        case 8:
            type = REQUEST_SIZE;
            printf("Enter file/folder path to get size: ");
            scanf("%s", path);
            break;
        case 9:
            type = REQUEST_PERMISSION;
            printf("Enter file/folder path to get permissions: ");
            scanf("%s", path);
            break;
        default:
            printf("Invalid option\n");
            return 1;
    }

    

    return 0;
}