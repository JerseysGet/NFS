#include "trie.h"

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define FILE_SEPARATOR "/"

typedef struct TrieNodeStruct {
    struct TrieNodeStruct* parent;
    struct TrieNodeStruct* next;
    struct TrieNodeStruct* prev;
    struct TrieNodeStruct* firstChild;
    char* token;
    SSInfo ssinfo;
} TrieNodeStruct;

typedef TrieNodeStruct* TrieNode;

// UTIL FUNCTIONS

TrieNode trie = NULL;
pthread_mutex_t lock;

TrieNode newNode(SSInfo ssinfo, char* token) {
    TrieNode ret = malloc(sizeof(TrieNodeStruct));
    assert(ret);
    ret->parent = NULL;
    ret->next = NULL;
    ret->prev = NULL;
    ret->firstChild = NULL;
    ret->token = strdup(token);
    ret->ssinfo = ssinfo;
    return ret;
};

void deleteNode(TrieNode node) {
    free(node->token);
    free(node);
}

void link(TrieNode a, TrieNode b) {
    if (a) a->next = b;
    if (b) b->prev = a;
}

void appendChild(TrieNode parent, TrieNode child) {
    assert(child);
    assert(parent);
    link(child, parent->firstChild);
    parent->firstChild = child;
    child->parent = parent;
}

// Removes itr from its links
void removeChild(TrieNode itr) {
    link(itr->prev, itr->next);
    deleteNode(itr);
}

// Returns NULL if none found
TrieNode findChild(TrieNode parent, char* token) {
    for (TrieNode itr = parent->firstChild; itr != NULL; itr = itr->next) {
        if (strcmp(itr->token, token)) continue;
        return itr;
    }

    return NULL;
}

TrieNode searchNode(char* path) {
    TrieNode parent = trie;
    char* cpy = strdup(path);
    char* token = strtok(cpy, FILE_SEPARATOR);
    while (token != NULL) {
        TrieNode child = findChild(parent, token);
        if (child == NULL) {
            parent = NULL;
            goto clean_and_return;
        }
        parent = child;
        token = strtok(NULL, FILE_SEPARATOR);
    }
clean_and_return:
    free(cpy);
    return parent;
}

// EXPOSED FUNCTIONS

void initTrie() {
    SSInfo ssinfo = INVALID_SSINFO;
    trie = newNode(ssinfo, "");
    pthread_mutex_init(&lock, NULL);
}

void lockTrie() {
    pthread_mutex_lock(&lock);
}

void unlockTrie() {
    pthread_mutex_unlock(&lock);
}

ErrorCode addToTrie(char* path, SSInfo ssinfo) {
    TrieNode find = searchNode(path);
    if (find != NULL) return 3;
    TrieNode parent = trie;
    char* cpy = strdup(path);
    char* token = strtok(cpy, FILE_SEPARATOR);
    while (token != NULL) {
        TrieNode child = findChild(parent, token);
        if (child == NULL) {
            child = newNode(ssinfo, token);
            appendChild(parent, child);
        }
        parent = child;
        token = strtok(NULL, FILE_SEPARATOR);
    }
    free(cpy);
    return SUCCESS;
}

ErrorCode search(char* path, SSInfo* ret) {
    TrieNode node = searchNode(path);
    if (node)
        *ret = node->ssinfo;
    else
        return 1;

    return SUCCESS;
}

void deleteNodeFromTrie(TrieNode node) {
    TrieNode parent = node->parent;
    if (parent->firstChild == node) {
        parent->firstChild = node->next;
    }

    removeChild(node);
}

ErrorCode deleteFromTrie(char* path) {
    TrieNode node = searchNode(path);
    if (node == NULL) {
        return 1;
    }
    if (node->firstChild != NULL) {
        return 2;
    }

    deleteNodeFromTrie(node);
    return SUCCESS;
}

void destroySSFromTrieHelper(TrieNode node, SSInfo ssinfo) {
    if (node == NULL) return;
    for (TrieNode itr = node->firstChild; itr != NULL;) {
        TrieNode next = itr->next;
        destroySSFromTrieHelper(itr, ssinfo);
        itr = next;
    }

    if (SSInfoEqual(&node->ssinfo, &ssinfo)) deleteNodeFromTrie(node);
}

ErrorCode deleteSSFromTrie(SSInfo ssinfo) {
    destroySSFromTrieHelper(trie, ssinfo);
    return SUCCESS;
}

void destroyTrieHelper(TrieNode node) {
    if (node == NULL) return;
    for (TrieNode itr = node->firstChild; itr != NULL;) {
        TrieNode next = itr->next;
        destroyTrieHelper(itr);
        itr = next;
    }

    deleteNode(node);
}

void destroyTrie() {
    pthread_mutex_destroy(&lock);
    destroyTrieHelper(trie);
}