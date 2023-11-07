#ifndef HASH
#define HASH

#ifdef __cplusplus
extern "C"{
#endif

#include "def.h"
#include "mem.c"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef u32(hash_func) (const char * key , u32 limit);

typedef struct entry{
    str key;
    size_t key_len;
    free_func *free_obj;
    void *obj_ptr;
    size_t obj_size;
    struct entry *next;
}entry;

typedef struct hashmap{
    size_t size;
    bool list_of_list;
    entry **entries;
    hash_func *hash_ptr;
}hashmap;

u32 hash1(const char *key , u32 limit){
    size_t len = strnlen_s(key , UINT32_MAX);

    char f = key[0] , s = key[0 + (len > 1)], bl = key[len - (1 + (len > 1))] , l = key[len - 1];
    u32 ret = f << 24 | s << 16 | bl << 8 | l;
    
    if(len < INT_MAX){
        ret ^= ~len;
    }else{
        ret ^= len;
    }

    if(limit > 0){
        ret %= limit;
    }
    return ret;
}

u32 hash2(const char *key , u32 limit){
    u32 h = 0x12345678;\

    for (size_t i = 0 ; key[i] ; i++) {
        h ^= key[i];
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    if(limit > 0){
        h %= limit;
    }
    return h;
}

u32 def_hash(const char *key , u32 limit){
    u32 ret;
    u32 h1 = hash1(key , 0);
    u32 h2 = hash2(key , 0);

    ret = (u16)h1 | (h2 >> 16) << 16;

    if(!limit){
        return ret;
    }
    
    return ret % limit;
}

static bool init_hashmap(hashmap *map_ptr , size_t size , hash_func *hash_ptr , bool list_of_list){
    if(size == 0){
        return false;
    }

    map_ptr -> size = size;
    map_ptr -> list_of_list = list_of_list;
    map_ptr -> entries = (entry **)calloc(size , sizeof(entry *));
    if(hash_ptr == NULL){
        map_ptr -> hash_ptr = def_hash;
    }
    return true;
}

hashmap *new_hashmap(size_t size , hash_func *hash_ptr , bool list_of_list){
    hashmap *ret = (hashmap *)calloc(1 , sizeof(hashmap));
    if(!init_hashmap(ret , size , hash_ptr , list_of_list)){
        free(ret);
        return NULL;
    }

    return ret;
}

void destroy_hashmap(hashmap *map_ptr){
    entry **entries = map_ptr -> entries;
    free_func *free_obj = NULL;

    for(size_t i = 0 ; i < map_ptr -> size ; i++){
        for(entry *j = entries[i] , *tmp ; j != NULL ; j = tmp){
            tmp = j -> next;
            free_obj = j -> free_obj;
            free_obj(j -> obj_ptr);
            free(j -> key);
            free(j);
        }
    }

    free(entries);

    map_ptr -> size = 0;
    map_ptr -> entries = NULL;
    map_ptr -> hash_ptr = NULL;
}

bool hashmap_add_element(const char *key , void *obj_ptr , size_t obj_size , free_func *free_func_ptr , hashmap *map_ptr){
    if(!key[0]){
        return false;
    }

    u32 index = map_ptr -> hash_ptr(key , map_ptr -> size);
    size_t key_len = strnlen_s(key , UINT32_MAX);

    entry **node_ptr = map_ptr -> entries + index;
    for(entry *curr_node = *node_ptr ; curr_node != NULL && map_ptr -> list_of_list == true ; node_ptr = &curr_node -> next , curr_node = curr_node -> next){
        if(curr_node -> key_len == key_len && curr_node -> key[0] == key[0]){
            if(!strcmp(curr_node -> key , key)){
                return false;
            }
        }
    }

    if(*node_ptr != NULL){
        return false;
    }
    
    *node_ptr = (entry *)calloc(1 , sizeof(entry));

    entry *target = *node_ptr;

    target -> key_len = strnlen_s(key , UINT32_MAX);
    target -> key = copy_obj((void *)key , target -> key_len + 1);/*(char *)calloc(target -> key_len + 1 , 1);
    strcpy_s(target -> key , target -> key_len + 1 , key);*/

    target -> obj_size = obj_size;
    target -> obj_ptr = copy_obj(obj_ptr , obj_size);/*calloc(1 , obj_size);
    memcpy_s(target -> obj_ptr , obj_size , obj_ptr , obj_size);*/

    target -> next = NULL;

    if(free_func_ptr == NULL){
        target -> free_obj = free;
    }else{
        target -> free_obj = free_func_ptr;
    }

    return true;
}

bool hashmap_delete_element(const char* key , hashmap *map_ptr){
    if(!key[0]){
        return false;
    }

    u32 index = map_ptr -> hash_ptr(key , map_ptr -> size);
    entry **entries = map_ptr -> entries;
    entry *prev = NULL;
    entry *node = NULL;
    size_t key_len = strnlen_s(key , UINT32_MAX);

    
    for(node = entries[index] ; node != NULL ; prev = node , node = node -> next){
        if(node -> key_len != key_len || node -> key[0] != key[0]){
            continue;
        }

        if(!strcmp(node -> key , key)){
            break;
        }
    }

    if(node == NULL){
        return false;
    }

    if(prev){
        prev -> next = node -> next;
    }else{
        entries[index] = node -> next;
    }

    entry *target = node;
    free_func *free_obj = target -> free_obj;
    free_obj(target -> obj_ptr);
    free(target -> key);
    free(target);

    return true;
}

entry *hashmap_lookup_entry(const char *key , hashmap *map_ptr){
    if(!key[0]){
        return NULL;
    }

    u32 index = map_ptr -> hash_ptr(key , map_ptr -> size);
    entry *node = NULL;
    size_t key_len = strnlen_s(key , UINT32_MAX);

    
    for(node = map_ptr -> entries[index] ; node != NULL ; node = node -> next){
        
        if(node -> key_len != key_len || node -> key[0] != key[0]){
            continue;
        }

        if(!strcmp(node -> key , key)){
            return node;
        }
    }
    
    return NULL;
}

void *hashmap_lookup_obj(const char *key , hashmap *map_ptr){
    entry *target_entry = hashmap_lookup_entry(key , map_ptr);

    if(target_entry == NULL){
        return NULL;
    }else{
        return target_entry -> obj_ptr;
    }
}

bool hashmap_edit_entry(const char *key , hashmap *map_ptr , void *new_val_ptr , size_t new_val_size , free_func new_free_func_ptr){
    if(!key || !map_ptr || !new_val_ptr || !new_val_size){
        return false;
    }

    if(!key[0]){
        return false;
    }

    entry *target = hashmap_lookup_entry(key , map_ptr);

    if(!target){
        return false;
    }

    free_func *free_obj = target -> free_obj;
    free_obj(target -> obj_ptr);
    target -> obj_ptr = copy_obj(new_val_ptr , new_val_size);

    target -> obj_size = new_val_size;

    if(new_free_func_ptr){
        target -> free_obj = new_free_func_ptr;
    }
    return true;
}

#ifdef __cplusplus
}
#endif

#endif