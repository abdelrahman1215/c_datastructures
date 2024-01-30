#ifndef HASH_MAP
#define HASH_MAP

#ifdef __cplusplus
extern "C"{
#endif

#include "def.h"
#include <stdbool.h>

typedef u32(hash_func) (const char * key , u32 limit);

typedef struct entry entry;

typedef struct hashmap hashmap;

/// @brief makes a new hashmap
/// @param size the size of the hashmap
/// @param hash_ptr a pointer to the hash function to be used (enter NULL to use the default hash)
/// @param list_of_list set to fasle if you want a constant size hashmap
/// @return returns a pointer to the hashmap
hashmap *new_hashmap(size_t size , hash_func *hash_ptr , bool list_of_list);

/// @brief frees a hashmap & its contents
/// @param map_ptr a pointer to the hashmap
datastruct_err destroy_hashmap(hashmap *map_ptr);

//--------------------------------

/// @brief adds an element to a hashmap
/// @param key the key that will be used to lookup the entry
/// @param obj_ptr a pointer to the object that will be stored in the entry (a copy of the object will be stored instead of the object it self)
/// @param obj_size the size of the object
/// @param free_func_ptr a pointer to the function that will be used to free the object (set to NULL for the default stdlib free)
/// @param map_ptr a pointer to the map
/// @return returns the status of the operation (true in the case of succes & false in the case of failure)
datastruct_err hashmap_add_entry(const char *key , void *obj_ptr , size_t obj_size , free_func *free_func_ptr , hashmap *map_ptr);

/// @brief delete an entry from a hashmap
/// @param key the key of the entry
/// @param map_ptr a pointer to the hashmap
/// @return returns the status of the operation (true in the case of succes & false in the case of failure)
datastruct_err hashmap_delete_entry(const char* key , hashmap *map_ptr);

//--------------------------------

/// @brief lookup an entry from a hashmap
/// @param key the key of the entry
/// @param map_ptr a pointer to the hashmap
/// @return a pointer to the entry (you can use the api functions to get the objects from it)
entry *hashmap_lookup_entry(const char *key , hashmap *map_ptr); 

//--------------------------------

/// @brief get the object stored in an entry
/// @param entry_ptr a pointer to the entry
/// @return a pointer to the obj
void *hashmap_get_obj_ptr(entry *entry_ptr);

/// @brief get the size of the object stored in an entry
/// @param entry_ptr a pointer to the entry
/// @return the size of the object
size_t hashmap_get_obj_size(entry *entry_ptr);

#ifdef __cplusplus
}
#endif

#endif