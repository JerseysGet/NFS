#ifndef __TRIE_H
#define __TRIE_H

#include <stdbool.h>

#include "../../common/error/error.h"
#include "../../common/networking/ack/ssinfo.h"

void initTrie();

void lockTrie();

void unlockTrie();

/*
    return 3 = path already exists
    Must be externally synchronized
*/
ErrorCode addToTrie(char* path, SSInfo ssinfo);

/*
    return 1 = path not found
    Must be externally synchronized
*/
ErrorCode search(char* path, SSInfo* ret);

/*
    return 1 = path not found
    return 2 = path isn't empty
    return 0 = success
    Must be externally synchronized
*/
ErrorCode deleteFromTrie(char* path);

/*
    return 0 = success
    Must be externally synchronized
*/
ErrorCode deleteSSFromTrie(SSInfo ssinfo);

/*

*/
char** getChildren(char* path, int* count);

/*
    Should have exactly one thread calling this, and no threads waiting on lock. Should not hold the lock while calling destroyTrie()
*/
void destroyTrie();

#endif