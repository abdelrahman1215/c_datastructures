#ifndef C_VECTOR
#define C_VECTOR

#ifdef __cplusplus
extern "C"{
#endif

#include "def.h"

#define CHUNK_SIZE KB/2
#define MAX_OBJECT_SIZE 16
typedef struct memory_chunk mem_chunk;

typedef struct c_vector c_vector;

c_vector *new_c_vector(size_t obj_size , free_func free_obj_contents);

void free_c_vector_contents(c_vector *vec_ptr);

void destroy_c_vector(c_vector *vec_ptr);

bool c_vector_add_element(c_vector *vec_ptr , void *obj_ptr);

bool c_vector_remove_element(c_vector *vec_ptr , u64 index);

bool c_vector_edit_element(c_vector *vec_ptr , u64 index , void *new_val_ptr);

void *c_vector_get_element(c_vector *vec_ptr , u64 index);

#ifdef __cplusplus
}
#endif

#endif //C_VECTOR