#ifndef C_VECTOR
#define C_VECTOR

#ifdef __cplusplus
extern "C"{
#endif

#include "def.h"

#define CHUNK_SIZE 4 * KB
#define MAX_OBJECT_SIZE 16
typedef struct memory_chunk mem_chunk;
typedef struct c_vector c_vector;

/// @brief makes a new c_vector
/// @param obj_size the size of the objects that will be stored in the vector
/// @param free_obj_contents the function that will be used to free the object (enter NULL for stdlib free)
/// @return a pointer to the new c_vector
c_vector *new_c_vector(size_t obj_size , free_func free_obj_contents);

/// @brief destroys a c_vector
/// @param vec_ptr a pointer to the c_vector
void destroy_c_vector(c_vector *vec_ptr);

//--------------------------------

/// @brief add an element to a c_vector
/// @param vec_ptr a pointer to the c_vector
/// @param obj_ptr a pointer to the object that will be added (a copy will be added and no the object it self)
/// @return the status of the operation
bool c_vector_add_element(c_vector *vec_ptr , void *obj_ptr);

/// @brief remove an elment from a c_vector
/// @param vec_ptr a pointer to the c_vector
/// @param index the index of the element to be deleted
/// @return the status of the operation
bool c_vector_remove_element(c_vector *vec_ptr , u64 index);

/// @brief edit an element in a c_vector
/// @param vec_ptr a pointer to the c_vector
/// @param index the index of the element to be deleted
/// @param new_val_ptr the new value
/// @return the status of the operation
bool c_vector_edit_element(c_vector *vec_ptr , u64 index , void *new_val_ptr);\

//--------------------------------

/// @brief get (a copy of) an element of a c_vector (you will have to free that copy)
/// @param vec_ptr a pointer to the vector
/// @param index the index of the object
/// @return a pointer to the copy
void *c_vector_get_element(c_vector *vec_ptr , u64 index);

/// @brief get the number of elements stored in a c_vector
/// @param vec_ptr a pointer to the vector
/// @return the number of elements
u64 c_vector_get_elements_no(c_vector *vec_ptr);

/// @brief get the size of elements stored in a c_vector
/// @param vec_ptr a pointer to the c_vector
/// @return the size of the elments stored in the c_vector
size_t c_vector_get_obj_size(c_vector *vec_ptr);

#ifdef __cplusplus
}
#endif

#endif //C_VECTOR