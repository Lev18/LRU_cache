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
            ((token)->capacity) = (token)->capacity == 0 ? STR_INIT_CAP : ((token)->capacity *= 2); \
            (token)->string = (char*)realloc((token)->string, (token)->capacity * sizeof(char));  \
            assert ((token)->string != NULL && "Need more space");                                \
          }                                                                                       \
          (token)->string[(token)->size++] = elem; \
      } while(0);

#define append_token(token, elem_type, elem)                                                                       \
      do {                                                                                        \
          if ((token)->size ==(token)->capacity) {                                                \
            (token)->capacity = (token)->capacity == 0 ? STR_INIT_CAP : ((token)->capacity *= 2); \
            (token)->items = (elem_type*)realloc((token)->items, (token)->capacity * sizeof(elem_type));  \
            assert ((token)->items != NULL && "Need more space");                                \
          }                                                                                       \
          (token)->items[(token)->size++] = elem; \
      } while(0);
size_t cur_pos = 0;

typedef struct {
  int size;
  const char* str;
}String_view;

typedef struct {
    int size;
    int capacity;
    char* string;
}String_Builder;

typedef struct {
    int size;
    int capacity;
    String_view* items;
}String_Buffer;


typedef enum {
    LRUCREATE,
    PUT,
    GET
}Instructs;

void init_lru_cache(LRU_cache* cache, int cap) {
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

void chop_char(String_view* sv, const char* file_cont) {
  char ch = *(file_cont + cur_pos);
  if (isalnum(ch)) {
    ++sv->size;
  }
  ++cur_pos;
}

void remove_noise(const char* file_cont)  {
  char ch = *(file_cont + cur_pos);
  while (!isalnum(ch) && ch != '\0') {
    ch = *(file_cont + (++cur_pos));
  }
}

String_view* tokenize(const char* file_cont) {
  String_view* sv = (String_view*)malloc(sizeof(String_view));
  sv->size = 0;
  remove_noise(file_cont);
  sv->str = file_cont + cur_pos;
 // sv->str = *file_cont;
 
  int i = 0;
  char ch = *(file_cont + cur_pos);
  
  while (ch != '\0' && ch != ' ' && ch != '\n') {
    chop_char(sv, file_cont);
    ch = *(file_cont + cur_pos);
    ++i;
  }
  ++cur_pos;
  return sv;
}


int main(void) {
    // LRU_cache cache;
    FILE* file;
    file = fopen("./test1.td", "r");
    if (file != NULL){
        String_Buffer buffer = {
            .size = 0,
            .capacity = 0,
            .items = NULL
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
        append(&sb, '\0');

        while (sb.string[cur_pos] != '\0') {
            append_token(&buffer, String_view, *(tokenize((const char*)sb.string)));
        }

        int i = 0;
        while (i < buffer.size) {
            int j = 0;
            while (j < buffer.items[i].size) {
                printf("%c", *(buffer.items[i].str + j));
                ++j;
            }
            ++i;
            printf("\n");
        }

        fclose(file);
    }
    else {
        printf("Unable to open file");
        exit(1);
    }
    return 0;
}
