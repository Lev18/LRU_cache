#include "LRU_cache.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


#define LINE_SIZE 20
#define STR_INIT_CAP 2 

#define append(token, elem)                                                                       \
      do {                                                                                        \
          if ((token)->size ==(token)->capacity) {                                                \
            (token)->capacity = (token)->capacity == 0 ? STR_INIT_CAP : ((token)->capacity *= 2); \
            (token)->string = (char*)realloc((token)->string, (token)->capacity * sizeof(char));  \
            assert ((token)->string != NULL && "Need more space");                                \
          }                                                                                       \
          (token)->string[(token)->size++] = elem; \
      } while(0);

#define append_token(token, elem_type, elem)                                                                       \
      do {                                                                                        \
          if ((token)->size ==(token)->capacity) {                                                \
            (token)->capacity = (token)->capacity == 0 ? STR_INIT_CAP : ((token)->capacity *= 2); \
            (token)->item = (elem_type*)realloc((token)->item, (token)->capacity * sizeof(elem_type));  \
            assert ((token)->item != NULL && "Need more space");                                \
          }                                                                                       \
          (token)->item[(token)->size++] = elem; \
      } while(0);


typedef struct {
    int size;
    int capacity;
    char* string;
}String_Builder;

typedef struct {
    int size;
    int capacity;
    String_Builder** item;
}String_Buffer;


typedef enum {
    LRUCREATE,
    PUT,
    GET
}Instructs;

void init_lru_cache(LRU_cache* cache, size_t cap) {
    cache->capacity = cap;
    init_list(&cache->key_hits);
    init_map(&cache->hash_table);
}

void put_element(LRU_cache* cache, int key, int value) {
    if (containsKey(&cache->hash_table, key) != -1) {
        erase_elem(&cache->key_hits, get_htable_value(&cache->hash_table, key));
    }
    push_front(&cache->key_hits, &(pair){
        .first = key,
        .second = value
    });
    insert_list_node(&cache->hash_table, cache->key_hits.head);
    if (cache->key_hits.size > cache->capacity) {
        erase_hash_node(&cache->hash_table, cache->key_hits.tail->value.first);
        pop_back(&cache->key_hits);
    }
}

int get_element(LRU_cache* cache, int key) {
    if (containsKey(&cache->hash_table, key) == -1) return -1;
    List_Node* it = get_htable_value(&cache->hash_table, key);
    move_front(&cache->key_hits, it);    
    return it->value.second;
}

Instructs uint_instr(const char* inst) {
    if (strcmp(inst, "LRUCache") == 0) {
        return LRUCREATE;
    }
    else if (strcmp(inst, "put") == 0) {
        return PUT;
    }
    else {
        return GET;
    }
}

void init_string(String_Builder* sb) {
    sb->size = 0;
    sb->capacity = 0;
    sb->string = NULL;
}

void tokenize(String_Buffer* buffer, String_Builder* sb) {
    size_t i = 0;
    String_Builder* sb = (String_Builder*)malloc(sizeof(String_Buffer));
    init_string(sb);
       while (i < LINE_SIZE) {
        if (isalnum(line[i]) > 0) {
            append(sb, line[i]);
        }
        else if ( line[i] == '['  || line[i] == ','  || line[i] == ']') {
            append_token(buffer, String_Builder*, sb);
            sb = (String_Builder*)malloc(sizeof(String_Buffer));
            init_string(sb);
        }
        ++i;
    }
}

int main(void) {
    LRU_cache cache;
    FILE* file;
    char line[LINE_SIZE];

    file = fopen("./test1.td", "r");
    if (file != NULL){
        String_Buffer buffer = {
            .size = 0,
            .capacity = 0,
            .item = NULL
        };

        String_Builder sb = {
            .size = 0,
            .capacity = 0,
            .string = NULL
        };
        char ch;
        while ((ch = fgetc( file)) != EOF) {
            append(&sb, ch);
        }

        int i = 0;
        while (i < sb.size) {
                printf("%c\n", sb.string[i]);
            ++i;
        }
        printf("%d\n", sb.size);

        fclose(file);
    }
    else {
        printf("Unable to open file");
        exit(1);
    }
    return 0;
}
