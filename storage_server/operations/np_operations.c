#include "operations.h"

ErrorCode GetFileInfo(char* path, struct stat* info) {
    if (stat(path, info) == -1) {
        eprintf("Could not read FileInfo, errno = %d, %s\n", errno, strerror(errno));
        return true;
    }
    return false;
}

ErrorCode GetDirectoryInfo(char* path, struct stat* info) {
    if (stat(path, info) == -1) {
        eprintf("Could not read DirectoryInfo, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    int size = 0;
    if (GetDirectorySize(path, &size)) {
        eprintf("Could not read Directory size, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    info->st_size = size;
    return SUCCESS;
}
bool IsDirectory(char* path) {
    struct stat st;
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
        return true;
    }
    return false;
}
// requires file size intialize to zero
ErrorCode GetDirectorySize(char* path, int* size) {
    if (IsDirectory(path)) {
        DIR* d;
        if ((d = opendir(path)) == NULL) {
            eprintf("Could not open directory, errno = %d, %s\n", errno, strerror(errno));
            return FAILURE;
        }
        struct dirent* dir;
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                continue;
            }
            char entry_path[256];
            memset(entry_path, '\0', 256);
            snprintf(entry_path, sizeof(entry_path), "%s/%s", path, dir->d_name);
            if (GetDirectorySize(entry_path, size))
                return FAILURE;
        }
        if (closedir(d) == -1) {
            eprintf("Could not close directry, errno = %d, %s\n", errno, strerror(errno));
            return FAILURE;
        }
        /// should i add directory size also
    } else {
        struct stat st;
        if (stat(path, &st) != 0) {
            eprintf("Could not read directory size, errno = %d, %s\n", errno, strerror(errno));
            return FAILURE;
        }
        *size += st.st_size;
    }
    return SUCCESS;
}

ErrorCode readFromFile(char* path, char* buffer, size_t bufferSize) {
    int fd;
    if ((fd = open(path, O_RDONLY)) == -1) {
        eprintf("Read:Could not open file, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    if (read(fd, buffer, bufferSize) == -1) {
        eprintf("Read:Could not read file, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    if (close(fd) == -1) {
        eprintf("Read:Could not close file, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    return SUCCESS;
}
ErrorCode writeToFile(char* path, char* buffer, size_t bufferSize) {
    FILE* fptr;
    if ((fptr = fopen(path, "w")) == NULL) {
        eprintf("Write:Could not open file, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    if (fprintf(fptr, "%s", buffer) < 0) {
        eprintf("Write:Could not write into file, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    if (fclose(fptr) == EOF) {
        eprintf("Write:Could not close file, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    return SUCCESS;
}
ErrorCode ExecuteWrite(WriteRequest* Req) {
    if (writeToFile(Req->path, Req->Data, sizeof(Req->Data))) {
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode ExecuteRead(ReadRequest* Req,int clientfd) {
    printf("1.\n");
    char buffer[1024];
    memset(buffer,'\0',1024);
    int fd;

    if ((fd = open(Req->path, O_RDONLY)) == -1) {
        eprintf("Read:Could not open file, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }

    printf("2.\n");
    ReadPacket packet;
    while(read(fd, buffer, 1024) > 0){
        printf("3.\n");
        if(sendDataPacket(buffer,clientfd)){
            close(fd);
            return FAILURE;
        }
        memset(buffer,'\0',1024);
    }
    close(fd);
    printf("4.\n");
    if(send_STOP_PKT(clientfd)){
        return FAILURE;
    }
    printf("5.\n");
    return SUCCESS;
}

ErrorCode ExecuteList(ListRequest* Req, int clientfd) {
    return SUCCESS;
}

ErrorCode ExecuteMD(MDRequest* Req, int clientfd) {
    struct stat st;
    if (IsDirectory(Req->path)) {
        if (GetDirectoryInfo(Req->path, &st))
            return FAILURE;
    } else {
        if (GetFileInfo(Req->path, &st))
            return FAILURE;
    }

    if (socketSend(clientfd, &st, sizeof(struct stat))) {
        printf("c\n");
        eprintf("Could not Send metadata to Client");
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode ExecuteRequest(RequestType reqType, void* request, int clientfd) {
    switch (reqType) {
        case REQUEST_WRITE:
            return ExecuteWrite((WriteRequest*)request);
        case REQUEST_READ:
            return ExecuteRead((ReadRequest*)request, clientfd);
        case REQUEST_METADATA:
            return ExecuteMD((MDRequest*)request, clientfd);
        case REQUEST_LIST:
            return ExecuteList((ListRequest*)request, clientfd);
        default:
    }
    return FAILURE;
}