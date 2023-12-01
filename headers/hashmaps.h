#ifndef HASH
#define HASH

#ifdef __cplusplus
extern "C"{
#endif

#include "def.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef u32(hash_func) (const char * key , u32 limit);

typedef struct entry entry;

typedef struct hashmap hashmap;

hashmap *new_hashmap(size_t size , hash_func *hash_ptr , bool list_of_list);
void destroy_hashmap(hashmap *map_ptr);

bool hashmap_add_element(const char *key , void *obj_ptr , size_t obj_size , free_func *free_func_ptr , hashmap *map_ptr);
bool hashmap_delete_element(const char* key , hashmap *map_ptr);

void *hashmap_lookup_obj(const char *key , hashmap *map_ptr);
bool hashmap_edit_entry(const char *key , hashmap *map_ptr , void *new_val_ptr , size_t new_val_size , free_func new_free_func_ptr);

#ifdef __cplusplus
}
#endif

#endif