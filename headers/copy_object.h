#ifndef MEM_C
#define MEM_C

#ifdef __cplusplus
extern "C" {
#endif

#include "../headers/def.h"

/// @brief dynamically allocates a copy of an object
/// @param obj_ptr a pointer to the object
/// @param obj_size the size of the object
/// @return a pointer to the copy
extern void *copy_object(void *obj_ptr , size_t obj_size);

#ifdef __cplusplus
}
#endif

#endif