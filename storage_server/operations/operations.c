#include "operations.h"


void CleanPacket(CopyPacket* packet){
    memset(packet->ObjectInfo.ObjectName,'\0',MAX_PATH_SIZE);
    memset(packet->Data,'\0',MAX_DATA_LENGTH);
}

ErrorCode send_INFOPKT(int sockfd,char* path,bool IsDir) {
    CopyPacket packet;
    CleanPacket(&packet);
    packet.header = INFO;
    char* entity = strrchr(path,'/');
    if(entity != NULL){
        strcpy(packet.ObjectInfo.ObjectName,entity + 1);
    }
    else {
        strcpy(packet.ObjectInfo.ObjectName,path);
    }
    if(socketSend(sockfd,&packet,sizeof(CopyPacket))){
        return FAILURE;
    }
    return SUCCESS;
}
ErrorCode send_STOPPKT(int sockfd) {
    CopyPacket packet;
    cleanPacket();
    packet.header = STOP;
    if(socketSend(sockfd,&packet,sizeof(CopyPacket))){
        return FAILURE;
    }
    return SUCCESS;
}
ErrorCode send_STARTPKT(int sockfd) {
    CopyPacket packet;
    cleanPacket();
    packet.header = START;
    if(socketSend(sockfd,&packet,sizeof(CopyPacket))){
        return FAILURE;
    }
    return SUCCESS;
}
ErrorCode send_DATAPKT(int sockfd,char* data,int size) {
    CopyPacket packet;
    cleanPacket();
    packet.header = DATA;
    strcpy(packet.Data,data);
    if(socketSend(sockfd,&packet,sizeof(CopyPacket))){
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode ReceivePacket(int sockfd,CopyPacket* packet) {
    cleanPacket(packet);
    if(socketRecieve(sockfd,packet,sizeof(CopyPacket))){
        return FAILURE;
    }
    return SUCCESS;
}
ErrorCode CopyDirectory_Send(char* src,char* dst,int sockfd){
    DIR* Dptr;
    if ((Dptr = opendir(src)) == NULL) {
        eprintf("Could not open Source directory, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    if(send_INFOPKT(sockfd,src,true)){
        close(Dptr);
        return FAILURE;
    } 
    struct dirent* dir;
    while ((dir = readdir(Dptr)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue;
        }
        char entry_path[256];
        memset(entry_path, '\0', 256);
        snprintf(entry_path, sizeof(entry_path), "%s/%s", src, dir->d_name);
        if(IsDirectory(entry_path)){
            if (CopyDirectory_Send(entry_path,dst,sockfd))
                return FAILURE;
        }
        else{
            if(SendfileData(entry_path,sockfd)){
                close(Dptr);
                return FAILURE;
            }
        }
    }
    close(Dptr);
    if(send_STOPPKT(sockfd)){
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode SendfileData(char* filename,int sockfd){
    int fd;
    if(send_INFOPKT(sockfd,filename,false)){
        return FAILURE;
    }
    if ((fd = open(filename, O_RDONLY)) == -1) {
        eprintf("Could not open file, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    char buffer[MAX_DATA_LENGTH];
    memset(buffer,'\0',MAX_DATA_LENGTH);
    int chk;
    while((chk = read(fd,buffer,MAX_DATA_LENGTH)) > 0){
         send_DATAPKT(sockfd,buffer,MAX_DATA_LENGTH);
         memset(buffer,'\0',MAX_DATA_LENGTH);
    }
    close(fd);
    if(send_STOPPKT(sockfd)){
        return FAILURE;
    }
    if(chk == -1){
        eprintf("Could not read the file, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode ReceiveFileData(char* filename,int sockfd){
    if(createFile(filename,0644))
        return FAILURE;
    FILE* fptr;
    if ((fptr = fopen(filename, "w")) == NULL) {
        eprintf("Write:Could not open file, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    CopyPacket packet;
    ReceivePacket(sockfd,&packet);
    while(packet.header == DATA){
        if (fprintf(fptr, "%s", packet.Data) < 0) {
            eprintf("Write:Could not write into file, errno = %d, %s\n", errno, strerror(errno));
            fclose(fptr);
            return FAILURE;
        } 
    }
    fclose(fptr);
}
// inside indst path
ErrorCode CopyDirectory_Recieve(char* src,char* dst,int sockfd){
    DIR* Dptr;
    if ((Dptr = opendir(dst)) == NULL) {
        eprintf("Could not open Destination directory, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    CopyPacket Packet;
    if(ReceivePacket(sockfd,&Packet)){
        close(Dptr);
        return FAILURE;
    }
    while(Packet.header != STOP){  
       // if is dir()
            // create directory
            // recursive call
       // else
            // Receiv file info
        char entry_path[256];
        memset(entry_path, '\0', 256);
        snprintf(entry_path, sizeof(entry_path), "%s/%s", dst, Packet.ObjectInfo.ObjectName);
        if(Packet.ObjectInfo.IsDirectory){
            if(createDirectory(entry_path,0644))
                 return FAILURE;
            if(CopyDirectory_Recieve(src,entry_path,sockfd))
                 return FAILURE;
        } 
        else {
            if(ReceiveFileData(entry_path,sockfd))
                return FAILURE;
        }
        if(ReceivePacket(sockfd,&Packet)){
            close(Dptr);
            return FAILURE;
        }
    }

    close(Dptr);
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
ErrorCode createFile(char* path, mode_t perm) {
    int fd;
    if ((fd = open(path, O_CREAT, perm)) == -1) {
        eprintf("Could not create file, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    return SUCCESS;
}
ErrorCode createDirectory(char* path, mode_t perm) {
    if (mkdir(path, perm) == -1) {
        eprintf("Could not create Directory, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
}
ErrorCode deleteFile(char* path) {
    if (remove(path) == -1) {
        eprintf("Could not Delete file, errno = %d, %s\n", errno, strerror(errno));
        return FAILURE;
    }
    return SUCCESS;
}
ErrorCode deleteDirectory(char* path) {
    if (IsDir(path)) {
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
            if (deleteDirectory(entry_path))
                return FAILURE;
        }
        if (closedir(d) == -1) {    // ?? is Error handling really needed here
            eprintf("Could not close directry, errno = %d, %s\n", errno, strerror(errno));
            return FAILURE;
        }
        if (rmdir(path) == -1) {
            eprintf("Could not delete directory, errno = %d, %s\n", errno, strerror(errno));
            return FAILURE;
        }
    } else {
        if (remove(path) == -1) {
            eprintf("Could not Delete file, errno = %d, %s\n", errno, strerror(errno));
            return FAILURE;
        }
    }
    return SUCCESS;
}

// ErrorCode copyDirectory(??); TODO
// ErrorCode copyFile(??); TODO

ErrorCode GetFileInfo(char* path,struct stat *info) {
    if (stat(path, info) == -1) {
        eprintf("Could not read FileInfo, errno = %d, %s\n",errno,strerror(errno));
        return true;
    }
    return false;
}
ErrorCode GetDirectoryInfo(char* path,struct stat *info) {
    if (stat(path, info) == -1) {
        eprintf("Could not read DirectoryInfo, errno = %d, %s\n",errno,strerror(errno));
        return FAILURE;
    }
    int size = 0;
    if(GetDirectoryInfo(path,&size)){
        eprintf("Could not read Directory size, errno = %d, %s\n",errno,strerror(errno));
        return FAILURE;
    }
    info->st_size = size;
    return SUCCESS;
}
ErrorCode IsDirectory(char* path) {
    struct stat st;
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
        return SUCCESS;
    } 
    return FAILURE;
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
            if (GetDirectorySize(entry_path,size))
                return FAILURE;
        }
        if (closedir(d) == -1) {
            eprintf("Could not close directry, errno = %d, %s\n", errno, strerror(errno));
            return FAILURE;
        }
        /// should i add directory size also
    } else {
        struct stat st;
        if(stat(path, &st) != 0){
            eprintf("Could not read directory size, errno = %d, %s\n", errno, strerror(errno));
            return FAILURE;
        }
        *size += st.st_size;
    } 
    return SUCCESS;
}
// ErrorCode getFileSize(char* path, int* size);
// ErrorCode getDirectorySize(char* path, int* size);

// ErrorCode getPermissions(char* path, int* permissions);