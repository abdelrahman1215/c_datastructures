#include "copy_object.c"
#include <string.h>
#include <stdlib.h>
#include "../headers/hashmap.h"


struct entry{
    str key;
    size_t key_len;
    free_func *free_obj;
    void *obj_ptr;
    size_t obj_size;
    struct entry *next;
};

struct hashmap{
    size_t size;
    bool list_of_list;
    entry **entries;
    hash_func *hash_ptr;
};

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
    u32 h = 0x12345678;

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
    if(!size || !map_ptr){
        return false;
    }

    if(map_ptr -> entries){
        return false;
    }

    map_ptr -> size = size;
    map_ptr -> list_of_list = list_of_list;
    map_ptr -> entries = (entry **)calloc(size , sizeof(entry *));
    if(!map_ptr -> entries){
        return false;
    }

    if(!hash_ptr){
        map_ptr -> hash_ptr = def_hash;
    }
    return true;
}

hashmap *new_hashmap(size_t size , hash_func *hash_ptr , bool list_of_list){
    hashmap *ret = (hashmap *)calloc(1 , sizeof(hashmap));
    if(!ret){
        return NULL;
    }

    if(!init_hashmap(ret , size , hash_ptr , list_of_list)){
        free(ret);
        return NULL;
    }

    return ret;
}

void free_entry(entry *entry_ptr){
    if(!entry_ptr){
        return;
    }

    entry_ptr -> free_obj(entry_ptr -> obj_ptr);
    free(entry_ptr -> key);
    free(entry_ptr);
}

void free_hashmap_contents(hashmap *map_ptr){
    if(!map_ptr){
        return;
    }

    if(!map_ptr -> entries){
        return;
    }

    entry **entries = map_ptr -> entries;

    for(size_t i = 0 ; i < map_ptr -> size ; i++){
        for(entry *j = entries[i] , *tmp ; j != NULL ; j = tmp){
            tmp = j -> next;
            free_entry(j);
        }
    }

    free(entries);

    map_ptr -> size = 0;
    map_ptr -> entries = NULL;
}

datastruct_err destroy_hashmap(hashmap *map_ptr){
    if(!map_ptr){
        return Invalid_Input;
    }

    if(map_ptr -> entries){
        free_hashmap_contents(map_ptr);
    }

    free(map_ptr);
    return Success;
}

entry *create_entry(const char *key , free_func *free_func_ptr , void *obj_ptr , size_t obj_size){
    if(!key || !obj_ptr || !obj_size){
        return NULL;
    }

    if(*key == '\000'){
        return NULL;
    }

    entry *ret = (entry *)calloc(1 , sizeof(entry));
    if(!ret){
        return NULL;
    }

    ret -> key_len = strnlen_s(key , UINT32_MAX);
    ret -> key = copy_object((void *)key , ret -> key_len + 1);
    if(!ret -> key){
        free(ret);
        return NULL;
    }

    ret -> obj_size = obj_size;
    ret -> obj_ptr = copy_object(obj_ptr , obj_size);
    if(!ret -> obj_ptr){
        free(ret -> key);
        free(ret);
        return NULL;
    }

    ret -> next = NULL;

    if(free_func_ptr == NULL){
        ret -> free_obj = free;
    }else{
        ret -> free_obj = free_func_ptr;
    }

    return ret;
}

datastruct_err hashmap_add_entry(const char *key , void *obj_ptr , size_t obj_size , free_func *free_func_ptr , hashmap *map_ptr){
    if(!key[0] || !map_ptr || !obj_ptr || !obj_size){
        return Invalid_Input;
    }

    if(!map_ptr -> entries){
        return Invalid_Input;
    }

    u32 index = map_ptr -> hash_ptr(key , map_ptr -> size);
    size_t key_len = strnlen_s(key , UINT32_MAX);

    entry **node_ptr = map_ptr -> entries + index;
    if(*node_ptr && !map_ptr -> list_of_list){
        return No_Space;
    }
    
    for(entry *curr_node = *node_ptr ; curr_node != NULL && map_ptr -> list_of_list == true ; node_ptr = &curr_node -> next , curr_node = curr_node -> next){
        if(curr_node -> key_len != key_len || curr_node -> key[0] != key[0]){
            continue;
        }
        
        if(!strcmp(curr_node -> key , key)){
            return Already_Exists;
        }
    }

    
    *node_ptr = create_entry(key , free_func_ptr , obj_ptr , obj_size);
    if(*node_ptr == NULL){
        return Allocation_err;
    }

    return Success;
}

datastruct_err hashmap_delete_entry(const char* key , hashmap *map_ptr){
    if(!key[0] || !map_ptr){
        return Invalid_Input;
    }

    if(!map_ptr -> entries){
        return Invalid_Input;
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
        return Not_Found;
    }

    if(prev){
        prev -> next = node -> next;
    }else{
        entries[index] = node -> next;
    }

    entry *target = node;
    free_entry(target);

    return Success;
}

entry *hashmap_lookup_entry(const char *key , hashmap *map_ptr){
    if(!key[0] || !map_ptr){
        return NULL;
    }

    if(!map_ptr -> entries){
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

void *hashmap_get_obj_ptr(entry *entry_ptr){
    if(!entry_ptr){
        return NULL;
    }

    return entry_ptr -> obj_ptr;
}

size_t hashmap_get_obj_size(entry *entry_ptr){
    if(!entry_ptr){
        return 0;
    }
    
    return entry_ptr -> obj_size;
}