#include "LRU_cache.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <dirent.h>

#define LINE_SIZE 20
#define STR_INIT_CAP 2 
#define ALL_INST 1000
#define VALUES_DIGITS 5
#define TEST_COUNT 25

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
int instr_id = 0;
char* test_files[TEST_COUNT];

typedef enum {
    LRUCREATE,
    PUT,
    GET
}Instr_type;

typedef struct {
    int key;
    int value;
    Instr_type type;
}Instructs;

Instructs inst_array[ALL_INST];

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

void uint_instr(String_Buffer* inst) {
    assert((inst->size * 10 / 25 ) < ALL_INST && "Insufficient space for all instructions");

    int it = 0;
    while (it < inst->size) {
        if (strncmp(inst->items[it].str, "LRUCache", inst->items[it].size) == 0) {
            inst_array[instr_id].type = LRUCREATE;
            inst_array[instr_id].key = atoi(inst->items[++it].str);
            inst_array[instr_id].value = -1; // atoi(inst->items[++it].str);
        }
        else if (strncmp(inst->items[it].str, "put",inst->items[it].size) == 0) {
            inst_array[instr_id].type = PUT;
            inst_array[instr_id].key = atoi(inst->items[++it].str);
            inst_array[instr_id].value = atoi(inst->items[++it].str);
        }
        else {
            inst_array[instr_id].type = GET;
            inst_array[instr_id].key = atoi(inst->items[++it].str);
            inst_array[instr_id].value = -1; // atoi(inst->items[++it].str);

        }
        ++it;
        ++instr_id;
    }
}

void init_str_buff(String_Buffer* sb) {
    sb->size = 0;
    sb->capacity = 0;
    sb->items = NULL;
}

void init_str(String_Builder* sb) {
    sb->size = 0;
    sb->capacity = 0;
    sb->string = NULL;
}

String_view* int_to_str(int val) {
      String_view* sv = (String_view*)malloc(sizeof(String_view));
      sv->size = 0;
      if (val < 0) {
        sv->str = "-1";
      }

      else {
        char buffer[VALUES_DIGITS];
        int i = 0;
        while (i < VALUES_DIGITS) {
            buffer[i] ='\0';
            ++i;
        } 
        
        int j = 0;
        if (val == 0) {
            buffer[0] = '0';
            j++;
        }

        String_Builder* sb = (String_Builder*)malloc(sizeof(String_Builder));
        init_str(sb);

        while (val) {
            buffer[j] = (val % 10) + 48;
            val /= 10;
            ++j;
        }
        --j;

        while (j >= 0) {
            append(sb, buffer[j]);
            --j;
        }
        append(sb, '\0');
        sv->str = sb->string;
        sv->size = sb->size;
      }
    return sv;
}

String_Buffer* cache_engine(String_Buffer* sb ) {
    String_Buffer* answer = (String_Buffer*)malloc(sizeof(String_Buffer));
    init_str_buff(answer);
    
    assert((inst_array[0].type == LRUCREATE) && "ALL tests should have 'LRUCache' instruction" );

    LRU_cache cache;
    int id = 0;

    int cont_key = 0;
    String_view temp = {.size = 4, .str = "null"};
    while (id < instr_id) {
        switch (inst_array[id].type) {
            case LRUCREATE:
                init_lru_cache(&cache, inst_array[id].key);
                append_token(answer, String_view, temp);
                break;       
            case PUT:
                put_element(&cache, inst_array[id].key,inst_array[id].value);
                append_token(answer, String_view, temp);
                break;
            case GET:
                cont_key = get_element(&cache, inst_array[id].key);
                append_token(answer, String_view, *int_to_str(cont_key));
                break;

        }
        //append_token(answer, , elem);
        ++id;
    }
    return answer;
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

int compare(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

int read_tests(const char* dir_path) {
   struct dirent *de;

    DIR* dr = opendir(dir_path);
    
    if (dr == NULL) {
        fprintf(stderr, "Unable to open directory");
        return 1;
    }

    int file_count = 0;
    while ((de = readdir(dr)) != NULL) {
        if (strstr(de->d_name,".td") != NULL) {
            test_files[file_count] = strdup(de->d_name);
            if (test_files[file_count] == NULL) {
                perror("Strdup failed");
                closedir(dr);
                return 1;
            }
            ++file_count;
        }
    }
    qsort(test_files, file_count, sizeof(char*), compare);

    closedir(dr);
    return file_count;
}

int main(void) {
    // LRU_cache cache;
    int ind = 0;
    int list_of_file = read_tests("./Tests");
    while(ind < list_of_file) {
            char file_path[256];
            snprintf(file_path, sizeof(file_path), "./Tests/%s", test_files[ind]);
            FILE* file = fopen(file_path, "r");

            if (file != NULL) {
                 String_Buffer buffer = {
                    .size = 0,
                    .capacity = 0,
                    .items = NULL
                } ;

                String_Builder sb = {
                     .size = 0,
                     .capacity = 0,
                     .string = NULL
                } ;

               char ch;
               while ((ch = fgetc( file)) != EOF) {
                   append(&sb, ch);
               }
              
               append(&sb, '\0');
               while (sb.string[cur_pos] != '\0') {
                   append_token(&buffer, String_view, *(tokenize((const char*)sb.string)));
               }
               uint_instr(&buffer);
               
               char f_p[256];
               snprintf(f_p,sizeof(f_p), "./Outputs/test%d.out", ind + 1); 
               FILE* file_w = fopen(f_p, "w");

               if (file_w == NULL) {
                   printf("Unable to open %s file for output\n", f_p);
                   exit(1);
               }

               String_Buffer* result = cache_engine(&buffer);
               int i = 0;
               while (i < result->size) {
                    fprintf(file_w, "%s\n", result->items[i].str);
                    ++i;
                }
                
               for (int i = 0; i < instr_id; ++i) {
                    inst_array[i].key = 0;
                    inst_array[i].value = 0;
                    inst_array[i].type = 0;
                }
                cur_pos = 0;
                instr_id = 0;

                printf("Runing test%d\n", ind + 1);
                fclose(file_w);
                fclose(file);
             }
            else {
                printf("Unable to open %s file\n", test_files[list_of_file]);
                exit(1);
             }

           

            free(test_files[ind]);
            ++ind;
    }
       return 0;
}
