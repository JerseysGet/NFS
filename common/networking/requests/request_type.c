#include "request_type.h"

const char* const REQ_TYPE_TO_STRING[] = {
    [REQUEST_CREATE] = "REQUEST_CREATE",
    [REQUEST_DELETE] = "REQUEST_DELETE",
    [REQUEST_COPY] = "REQUEST_COPY",
    [REQUEST_READ] = "REQUEST_READ",
    [REQUEST_WRITE] = "REQUEST_WRITE",
    [REQUEST_SIZE] = "REQUEST_SIZE",
    [REQUEST_PERMISSION] = "REQUEST_PERMISSION",
    [REQUEST_LIST] = "REQUEST_LIST"
};

bool isPrivileged(RequestType type) {
    switch (type) {
        case REQUEST_DELETE:
        case REQUEST_CREATE:
        case REQUEST_COPY:
            return true;
        
        case REQUEST_READ:
        case REQUEST_WRITE:
        case REQUEST_SIZE:
        case REQUEST_PERMISSION:
        case REQUEST_LIST:
            return false;
    }

    return false;
}