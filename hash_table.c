#include "hash_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/random.h>

void init_map(Hash_map* map) {
    map->size = 0;
    map->capacity = 1;
    map->array = (Hash_node**)malloc(map->capacity * sizeof(Hash_node*));
    map->array[0] = NULL;
}

int hash_func(int key, size_t size) {
    // key ^= (key >> 16);
    // key *= 50331653;
     // key ^= (key >> 8);
    return (key % size);
}

int containsKey(Hash_map* map, int key) {
    int index = hash_func(key, map->capacity);
    Hash_node* curr = map->array[index];
    while (curr != NULL) {
        if (curr->key == key) return index;
        curr = curr->_nxt;
    }
    return -1;
}

void rehash_map(Hash_map* map) {
    int old_cap = map->capacity;
    map->capacity = (map->capacity * 2 + 1);
    ++map->rehash_policy;
    
    Hash_node** new_bucket = (Hash_node**)malloc(map->capacity * sizeof(Hash_node*));
    for (int i = 0; i < map->capacity; ++i) {
      new_bucket[i] = NULL;
    }
    
    for (int i = 0; i < old_cap; ++i) {
      Hash_node* curr = map->array[i];
      while (curr != NULL) {
        Hash_node* next = curr->_nxt;
        int new_ind = hash_func(curr->key, map->capacity);
        curr->_nxt = new_bucket[new_ind];
        new_bucket[new_ind] = curr;
        curr = next;
      }
    }
    free(map->array);
    map->array = new_bucket;
    
}

void insert_list_node(Hash_map* map, List_Node* elem) {
    assert(map->capacity > 0 && "Capacity should be greather than zero");
    
    if (containsKey(map, elem->value.first) == -1) {
        if ((double)map->size / map->capacity >= LOAD_FACTOR) {
            rehash_map(map);
        }

        Hash_node* element = (Hash_node*)malloc(sizeof(Hash_node));
        element->key = elem->value.first;
        element->iterator = elem;
        int index = hash_func(element->key, map->capacity);
        if (map->array[index] != NULL) {
            Hash_node* tmp = map->array[index];
            while(tmp->_nxt != NULL) {
                tmp = tmp->_nxt;
            }
            tmp->_nxt = element;
        }

       map->array[index] = element;
       ++map->size;
    }
}
void insert_hash_node(Hash_map* map, Hash_node* element) {
    assert(map->capacity > 0 && "Capacity should be greather than zero");
    
    if (containsKey(map, element->key) == -1) {
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

       map->array[index] = element;
       ++map->size;
    }
}

void erase_hash_node(Hash_map* map, int key) {
    int rem_node_ind = containsKey(map, key);
    if (rem_node_ind != -1) {
        Hash_node* head = map->array[rem_node_ind];
        if (head->_nxt == NULL) {
            free(map->array[rem_node_ind]);
            map->array[rem_node_ind] = NULL;
        }
        else {
            Hash_node* tmp = head->_nxt;
            while (tmp->_nxt != NULL && tmp->key != key) {
                head = head->_nxt;
                tmp = tmp->_nxt;
            }
            head->_nxt = tmp->_nxt;
        }
    }
}

List_Node* get_htable_value(Hash_map* map, int key) {
    int index = containsKey(map, key);
    if (index >= 0) {
        return map->array[index]->iterator;
    }
    return NULL;
}

/*
int main() {
  Hash_map map;
  init_map(&map);
  Hash_node* node[300];//= (Hash_node**)malloc(sizeof(Hash_node*) * 6);

for (int i = 0; i < 71; ++i) {
    if (i % 7 == 0) {
        node[i] = (Hash_node*)malloc(sizeof(Hash_node));
        node[i]->key = i;
        node[i]->iterator = NULL;
        node[i]->_nxt = NULL;
        insert(&map, node[i]);
    }
}

  unsigned int randomNum;

    // Get random numbers

  for (int i = 0; i < 300; ++i) {
    node[i] = (Hash_node*)malloc(sizeof(Hash_node));
   if (getrandom(&randomNum, sizeof(randomNum), 0) != sizeof(randomNum)) {
        perror("getrandom failed");
        return EXIT_FAILURE;
    }

    // Scale to a desired range 
    node[i]->key = randomNum;
    node[i]->iterator = NULL;
    node[i]->_nxt = NULL;
    insert(&map, node[i]);
  }

  for (int i = 0; i < map.capacity; ++i) {
      Hash_node* curr = map.array[i];
      while (curr != NULL) {
          printf("%d->",curr->key);
          curr = curr->_nxt;
      }
      printf("%d\n", i);
    }
    
  printf("%d\n", containsKey(&map, 7));
  erase_hash_node(&map, 7);

  printf("%d\n", containsKey(&map, 7));
   return 0;
}
*/
