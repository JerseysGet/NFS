#ifndef __SSINFO_H
#define __SSINFO_H

typedef struct SSInfo {
    int ssClientPort;
} SSInfo;

#define INVALID_SSINFO (SSInfo) {-1};

#endif