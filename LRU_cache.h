#ifndef LRU_CACHE
#define LRU_CACHE

#include "hash_table.h"
#include "list.h"

typedef struct {
    int capacity;
    List key_hits;
    Hash_map hash_table;
}LRU_cache;

void init_lru_cache(LRU_cache* cache, int capacity);
int get_element(LRU_cache * cache, int key);
void put_element(LRU_cache* cache, int key, int value);

#endif
