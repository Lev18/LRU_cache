#ifndef LRU_CACHE
#define LRU_CACHE

#include "hash_table.h"
#include "list.h"

typedef struct {
    size_t capacity;
    List* key_hits;
    Hash_map* hash_table;
}LRU_cache;

void init_lru_cache(LRU_cache* cache, size_t capacity);
int get(int key);
void put(int key, int value);

#endif
