#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

#ifdef __cplusplus
extern "C"{
#endif

#include "def.h"

#define CHUNK_SIZE 4 * KB
typedef struct memory_chunk mem_chunk;
typedef struct dynamic_array dynamic_array;

/// @brief makes a new dynamic_array
/// @param obj_size the size of the objects that will be stored in the dynamic array
/// @param free_obj the function that will be used to free the object (enter NULL for stdlib free)
/// @return a pointer to the new dynamic_array
dynamic_array *new_dynamic_array(size_t obj_size , free_func free_obj);

/// @brief destroys a dynamic_array
/// @param vec_ptr a pointer to the dynamic_array
/// @return the status of the operation
datastruct_err destroy_dynamic_array(dynamic_array *vec_ptr);

//--------------------------------

/// @brief add an element to a dynamic_array
/// @param vec_ptr a pointer to the dynamic_array
/// @param obj_ptr a pointer to the object that will be added (a copy will be added and no the object it self)
/// @return the status of the operation
datastruct_err dynamic_array_add_element(dynamic_array *vec_ptr , void *obj_ptr);

/// @brief remove an elment from a dynamic_array
/// @param vec_ptr a pointer to the dynamic_array
/// @param index the index of the element to be deleted
/// @return the status of the operation
datastruct_err dynamic_array_remove_element(dynamic_array *vec_ptr , u64 index);

/// @brief edit an element in a dynamic_array
/// @param vec_ptr a pointer to the dynamic_array
/// @param index the index of the element to be deleted
/// @param new_val_ptr the new value
/// @return the status of the operation
datastruct_err dynamic_array_edit_element(dynamic_array *vec_ptr , u64 index , void *new_val_ptr);\

//--------------------------------

/// @brief get (a copy of) an element of a dynamic_array (you will have to free that copy)
/// @param vec_ptr a pointer to the vector
/// @param index the index of the object
/// @return a pointer to the copy
void *dynamic_array_get_element(dynamic_array *vec_ptr , u64 index);

/// @brief get the number of elements stored in a dynamic_array
/// @param vec_ptr a pointer to the vector
/// @return the number of elements
u64 dynamic_array_get_elements_no(dynamic_array *vec_ptr);

/// @brief get the size of elements stored in a dynamic_array
/// @param vec_ptr a pointer to the dynamic_array
/// @return the size of the elments stored in the dynamic_array
size_t dynamic_array_get_obj_size(dynamic_array *vec_ptr);

#ifdef __cplusplus
}
#endif

#endif //DYNAMIC_ARRAY