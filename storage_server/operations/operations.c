#include "operations.h"

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
        if (closedir(d) == -1) {
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