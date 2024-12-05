#ifndef HASH_TAB
#define HASH_TAB
#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#define LOAD_FACTOR 0.65


typedef struct Hash_node {
    int key;
    struct Hash_node* _nxt;
    List_Node* iterator;    
}Hash_node;

typedef struct {
    size_t size;
    int capacity;
    Hash_node** array;
}Hash_map;


int hash_func(int key, size_t size);
void put(Hash_map* map, Hash_node* node);
void rehash_map(Hash_map* map);
bool containsKey(Hash_map*map, int key);

#endif
