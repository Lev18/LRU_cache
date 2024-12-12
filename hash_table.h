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
    int rehash_policy;
    Hash_node** array;
}Hash_map;

void init_map(Hash_map* map);
int hash_func(int key, size_t size);
void insert_hash_node(Hash_map* map, Hash_node* node);
void insert_list_node(Hash_map* map, List_Node* elem);

void rehash_map(Hash_map* map);
int containsKey(Hash_map* map, int key);
void erase_hash_node(Hash_map* map, int key);
List_Node* get_htable_value(Hash_map* map, int key);

#endif
