#ifndef C_VECTOR
#define C_VECTOR

#ifdef __cplusplus
extern "C"{
#endif

#include "def.h"
#include "../libsrc/mem.c"
#include <stdlib.h>
#include <stdio.h>

#define CHUNK_SIZE 4096
#define MAX_OBJECT_SIZE 16
typedef struct memory_chunk{
    size_t actual_size;
    size_t used_size;
    void *mem_ptr;
}mem_chunk;

typedef struct c_vector {
    size_t chunk_size;

    u64 elements_no;

    size_t obj_actual_size;
    size_t obj_rounded_size;
    free_func *free_obj_contents;

    mem_chunk **chunks_ptr;
    u64 chunk_no;
}c_vector;

u64 round_to_next_power_of_two(u64 num){
    u16 highest_bit = 0;
    for(u128 i = num ; i > 1 ; i >>= 1 , highest_bit++){}

    u128 ret = 1 << highest_bit;

    if(ret == num){
        return ret;
    }

    return ret << 1;
}

mem_chunk *allocate_mem_chunk(size_t chunk_size){
    if(!chunk_size){
        return NULL;
    }

    mem_chunk *ret = (mem_chunk *)calloc(1 , sizeof(mem_chunk));
    if(!ret){
        return NULL;
    }

    ret -> actual_size = chunk_size;
    ret -> used_size = 0;
    ret -> mem_ptr = calloc(1 , chunk_size);
    if(!ret -> mem_ptr){
        free(ret);
        return NULL;
    }

    return ret;
}

c_vector *new_c_vector(size_t obj_size , free_func free_obj_contents){
    if(!obj_size || obj_size > MAX_OBJECT_SIZE){
        return NULL;
    }

    c_vector *ret = (c_vector *)calloc(1 , sizeof(c_vector));
    if(!ret){
        return NULL;
    }

    ret -> chunk_size = CHUNK_SIZE;
    ret -> chunks_ptr = (mem_chunk **)calloc(1 , sizeof(mem_chunk *));
    if(!ret -> chunks_ptr){
        free(ret);
        return NULL;
    }

    ret -> chunks_ptr[0] = allocate_mem_chunk(CHUNK_SIZE);
    if(!ret -> chunks_ptr[0]){
        free(ret -> chunks_ptr);
        free(ret);
        return NULL;
    }

    ret -> chunk_no = 1;
    ret -> obj_actual_size = obj_size;
    ret -> obj_rounded_size = round_to_next_power_of_two((u64)obj_size);
    ret -> free_obj_contents = free_obj_contents;

    return ret;
}

void free_c_vector_contents(c_vector *vec_ptr){
    if(!vec_ptr){
        return;
    }

    if(!vec_ptr -> chunks_ptr){
        return;
    }

    for(size_t  i = 0 ; i < vec_ptr -> chunk_no ; i++){
        mem_chunk *curr_chunk = vec_ptr -> chunks_ptr[i];
        if(vec_ptr -> free_obj_contents){
            for(char *j = (char *)curr_chunk -> mem_ptr ; j < (char *)curr_chunk -> mem_ptr + curr_chunk -> used_size ; j += vec_ptr -> obj_rounded_size){
                vec_ptr -> free_obj_contents(j);
            }
        }

        free(curr_chunk -> mem_ptr);
        free(curr_chunk);
    }

    free(vec_ptr -> chunks_ptr);
    vec_ptr -> chunks_ptr = NULL;
    vec_ptr -> chunk_no = 0;
    vec_ptr -> elements_no = 0;
}

void destroy_c_vector(c_vector *vec_ptr){
    if(!vec_ptr){
        return;
    }

    if(!vec_ptr -> chunks_ptr){
        return;
    }

    free_c_vector_contents(vec_ptr);
    free(vec_ptr);
}

bool c_vector_add_chunk(c_vector *vec_ptr){
    if(!vec_ptr){
        return false;
    }

    mem_chunk *new_chunk = allocate_mem_chunk(vec_ptr -> chunk_size);
    if(!new_chunk){
        return false;
    }

    mem_chunk **tmp;
    if(vec_ptr -> chunk_no > 0){
        tmp = (mem_chunk **)realloc(vec_ptr -> chunks_ptr , sizeof(mem_chunk *) * (vec_ptr -> chunk_no + 1));
    }else{
        tmp = (mem_chunk **)calloc(1 , sizeof(mem_chunk *));
    }

    if(!tmp){
        free(new_chunk -> mem_ptr);
        free(new_chunk);
        return false;
    }

    vec_ptr -> chunks_ptr = tmp;

    vec_ptr -> chunks_ptr[vec_ptr -> chunk_no] = new_chunk;

    vec_ptr -> chunk_no++;

    return true;
}

bool c_vector_add_element(c_vector *vec_ptr , void *obj_ptr){
    if(!obj_ptr || !vec_ptr){
        return false;
    }

    //allocate a new chunk if the last chunk is full
    if(vec_ptr -> chunks_ptr[vec_ptr -> chunk_no - 1] -> used_size == vec_ptr -> chunk_size){
        if(!c_vector_add_chunk(vec_ptr)){
            return false;
        }
    }

    mem_chunk *target_chunk = vec_ptr -> chunks_ptr[vec_ptr -> chunk_no - 1];
    void *target_address = (char *)target_chunk -> mem_ptr + (target_chunk -> used_size);

    memcpy_s(target_address , vec_ptr -> obj_actual_size , obj_ptr , vec_ptr -> obj_actual_size);

    target_chunk -> used_size += vec_ptr -> obj_rounded_size;
    vec_ptr -> elements_no++;

    return true;
}

static void *c_vector_get_element_ptr(c_vector *vec_ptr , u64 index){
    if(!vec_ptr){
        return NULL;
    }

    if(index >= vec_ptr -> elements_no){
        return NULL;
    }

    u32 elements_per_chunk = (vec_ptr -> chunk_size / vec_ptr -> obj_rounded_size);
    mem_chunk *target_chunk = vec_ptr -> chunks_ptr[index / elements_per_chunk];

    return (char *)target_chunk -> mem_ptr + ((index % elements_per_chunk) * vec_ptr -> obj_rounded_size);
}

bool c_vector_delete_element(c_vector *vec_ptr , u64 index){
    if(!vec_ptr){
        return false;
    }

    if(index >= vec_ptr -> elements_no){
        return false;
    }

    u32 elements_per_chunk = (vec_ptr -> chunk_size / vec_ptr -> obj_rounded_size);
    u64 target_chunk_index = index / elements_per_chunk;
    mem_chunk *target_chunk = vec_ptr -> chunks_ptr[target_chunk_index];

    void *target = c_vector_get_element_ptr(vec_ptr , index);

    if(vec_ptr -> free_obj_contents){
        vec_ptr -> free_obj_contents(target);
    }

    vec_ptr -> elements_no--;

    u64 i;
    for(i = target_chunk_index ; i < vec_ptr -> chunk_no ; i++ , target_chunk = vec_ptr -> chunks_ptr[i] , target = target_chunk -> mem_ptr){
        if(target_chunk -> used_size == 0){
            break;
        }

        size_t size_to_copy = (size_t)(char *)(  target_chunk -> mem_ptr
        /*                                   */+ target_chunk -> actual_size
        /*                                   */- target)
        /*                                   */- vec_ptr -> obj_rounded_size;

        if(size_to_copy > 0){
            memcpy_s(target , size_to_copy , (char *)target + vec_ptr -> obj_rounded_size , size_to_copy);
        }

        if(i == vec_ptr -> chunk_no - 1){
            break;
        }

        target = (char *)target_chunk -> mem_ptr + target_chunk -> used_size - vec_ptr -> obj_actual_size;
        memcpy_s(target , vec_ptr -> obj_rounded_size , vec_ptr -> chunks_ptr[i + 1] -> mem_ptr , vec_ptr -> obj_rounded_size);
    }

    if(i > 0 && target_chunk -> used_size == 0){
        target_chunk = vec_ptr -> chunks_ptr[i - 1];
    }

    target_chunk -> used_size -= vec_ptr -> obj_rounded_size;


    return true;
}

bool c_vector_edit_element(c_vector *vec_ptr , u64 index , void *new_val_ptr){
    if(!vec_ptr || !new_val_ptr){
        return false;
    }

    if(index >= vec_ptr -> elements_no){
        return false;
    }

    void *target = c_vector_get_element_ptr(vec_ptr , index);

    if(!target){
        return false;
    }

    memcpy_s(target , vec_ptr -> obj_actual_size , new_val_ptr , vec_ptr -> obj_actual_size);

    return true;
}

void *c_vector_get_element(c_vector *vec_ptr , u64 index){
    void *target = c_vector_get_element_ptr(vec_ptr , index);

    if(target == NULL){
        return NULL;
    }

    return copy_object(target , vec_ptr -> obj_actual_size);
}

#ifdef __cplusplus
}
#endif

#endif //C_VECTOR