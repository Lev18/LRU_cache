#include "hash_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/random.h>


int hash_func(int key, size_t size) {
    key *= 1128889;
    return key % size;
}

bool containsKey(Hash_map* map, int key) {
    int index = hash_func(key, map->capacity);
    Hash_node* curr = map->array[index];
    while (curr != NULL) {
        if (curr->key == key) return true;
        curr = curr->_nxt;
    }
    return false;
}

void init_map(Hash_map* map) {
    map->size = 0;
    map->capacity = 1;
    map->array = (Hash_node**)malloc(map->capacity * sizeof(Hash_node*));
    map->array[0] = NULL;
}


void rehash_map(Hash_map* map) {
    int old_cap = map->capacity;
    map->capacity = (map->capacity * 2 + 1);
    
    Hash_node** new_bucket = (Hash_node**)malloc(map->capacity * sizeof(Hash_node*));
    for (int i = 0; i < map->capacity; ++i) {
      new_bucket[i] = NULL;
    }
    
    for (int i = 0; i < old_cap; ++i) {
      Hash_node* curr = map->array[i];
      while (curr != NULL) {
        Hash_node* next = curr->_nxt;
        int new_ind = hash_func(map->array[i]->key, map->capacity);
        curr->_nxt = new_bucket[new_ind];
        new_bucket[new_ind] = curr;
        curr = next;
      }
    }
    free(map->array);
    map->array = new_bucket;
    
}

void put(Hash_map* map, Hash_node* element){
    assert(map->capacity > 0 && "Capacity should be greather from zero");
    if ((double)map->size / map->capacity >= LOAD_FACTOR) {
        rehash_map(map);
    }
    
    int index = hash_func(element->key, map->capacity);
    if (map->array[index] != NULL) {
      Hash_node* tmp = map->array[index];
      while(tmp->_nxt != NULL) {
        tmp = tmp->_nxt;
      }
      tmp->_nxt = element;
    }
    else {
      map->array[index] = element;
    }
    ++map->size;
}



int main() {
  Hash_map map;
  init_map(&map);
  Hash_node* node[200];//= (Hash_node**)malloc(sizeof(Hash_node*) * 6);
  
 unsigned int randomNum;

    // Get random numbers
    
  for (int i = 0; i < 3; ++i) {
    node[i] = (Hash_node*)malloc(sizeof(Hash_node));
   if (getrandom(&randomNum, sizeof(randomNum), 0) != sizeof(randomNum)) {
        perror("getrandom failed");
        return EXIT_FAILURE;
    }

    // Scale to a desired range 
    node[i]->key = 11;
    node[i]->iterator = NULL;
    node[i]->_nxt = NULL;
    put(&map, node[i]);
  }

     

  for (int i = 0; i < map.capacity; ++i) {
      Hash_node* curr = map.array[i];
      while (curr != NULL) {
          printf("%d->",curr->key);
          curr = curr->_nxt;
      }
      printf("%d\n", i);
    }
  printf("%d\n", containsKey(&map, 11));
  
  return 0;
}
