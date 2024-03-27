#include "copy_object.c"
#include <stdlib.h>
#include "../headers/dynamic_array.h"

struct memory_chunk{
    size_t actual_size;
    size_t used_size;
    void *mem_ptr;
};

struct dynamic_array {
    size_t chunk_size;

    u64 elements_no;

    size_t obj_actual_size;
    size_t obj_rounded_size;
    free_func *free_obj;

    mem_chunk **chunks_ptr;
    u64 chunk_no;
};

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

dynamic_array *new_dynamic_array(size_t obj_size , free_func free_obj){
    if(obj_size == 0 || obj_size > CHUNK_SIZE / 16){
        return NULL;
    }

    dynamic_array *ret = (dynamic_array *)calloc(1 , sizeof(dynamic_array));
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
    ret -> free_obj = free_obj;

    return ret;
}

void free_dynamic_array_contents(dynamic_array *vec_ptr){
    if(!vec_ptr){
        return;
    }

    if(!vec_ptr -> chunks_ptr){
        return;
    }

    for(size_t  i = 0 ; i < vec_ptr -> chunk_no ; i++){
        mem_chunk *curr_chunk = vec_ptr -> chunks_ptr[i];
        if(vec_ptr -> free_obj){
            for(char *j = (char *)curr_chunk -> mem_ptr ; j < (char *)curr_chunk -> mem_ptr + curr_chunk -> used_size ; j += vec_ptr -> obj_rounded_size){
                vec_ptr -> free_obj(copy_object(j , vec_ptr -> obj_actual_size));
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

datastruct_err destroy_dynamic_array(dynamic_array *vec_ptr){
    if(!vec_ptr){
        return Invalid_Input;
    }

    if(vec_ptr -> chunks_ptr){
        free_dynamic_array_contents(vec_ptr);
    }

    free(vec_ptr);
    return Success;
}

datastruct_err dynamic_array_add_chunk(dynamic_array *vec_ptr){
    if(!vec_ptr){
        return Invalid_Input;
    }

    mem_chunk *new_chunk = allocate_mem_chunk(vec_ptr -> chunk_size);
    if(!new_chunk){
        return Allocation_err;
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
        return Allocation_err;
    }

    vec_ptr -> chunks_ptr = tmp;

    vec_ptr -> chunks_ptr[vec_ptr -> chunk_no] = new_chunk;

    vec_ptr -> chunk_no++;

    return Success;
}

datastruct_err dynamic_array_add_element(dynamic_array *vec_ptr , void *obj_ptr){
    if(!obj_ptr || !vec_ptr){
        return Invalid_Input;
    }

    //allocate a new chunk if the last chunk is full
    if(vec_ptr -> chunks_ptr[vec_ptr -> chunk_no - 1] -> used_size == vec_ptr -> chunk_size){
        if(dynamic_array_add_chunk(vec_ptr) != Success){
            return Allocation_err;
        }
    }

    mem_chunk *target_chunk = vec_ptr -> chunks_ptr[vec_ptr -> chunk_no - 1];
    void *target_address = (char *)target_chunk -> mem_ptr + (target_chunk -> used_size);

    memcpy(target_address , obj_ptr , vec_ptr -> obj_actual_size);

    target_chunk -> used_size += vec_ptr -> obj_rounded_size;
    vec_ptr -> elements_no++;

    return Success;
}

void *dynamic_array_get_element_ptr(dynamic_array *vec_ptr , u64 index){
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

datastruct_err dynamic_array_remove_last_chunk(dynamic_array *vec_ptr){
    if(!vec_ptr){
        return Invalid_Input;
    }

    if(vec_ptr -> chunk_no <= 1){
        return Invalid_Input;
    }

    mem_chunk *target = vec_ptr -> chunks_ptr[vec_ptr -> chunk_no - 1];

    mem_chunk **tmp = (mem_chunk **)realloc(vec_ptr -> chunks_ptr , sizeof(mem_chunk *) * (--vec_ptr -> chunk_no));

    if(!tmp){
        return Allocation_err;
    }

    for(char *i = (char *)target -> mem_ptr ; vec_ptr -> free_obj && i < (char *)target -> mem_ptr + target -> used_size ; i += vec_ptr -> obj_rounded_size){
        vec_ptr -> free_obj(copy_object(i , vec_ptr -> obj_actual_size));
    }

    free(target -> mem_ptr);

    vec_ptr -> chunks_ptr = tmp;

    return Success;
}

datastruct_err dynamic_array_remove_element(dynamic_array *vec_ptr , u64 index){
    if(!vec_ptr){
        return Invalid_Input;
    }

    if(index >= vec_ptr -> elements_no){
        return Allocation_err;
    }

    u32 elements_per_chunk = (vec_ptr -> chunk_size / vec_ptr -> obj_rounded_size);
    u64 target_chunk_index = index / elements_per_chunk;
    mem_chunk *target_chunk = vec_ptr -> chunks_ptr[target_chunk_index];

    void *target = dynamic_array_get_element_ptr(vec_ptr , index);

    if(vec_ptr -> free_obj){
        void *copy = copy_object(target , vec_ptr -> obj_actual_size);
        if(!copy){
            return Allocation_err;
        }

        vec_ptr -> free_obj(copy);
    }


    u64 i;
    for(i = target_chunk_index ; i < vec_ptr -> chunk_no && target_chunk -> used_size > 0 ; i++ , target_chunk = vec_ptr -> chunks_ptr[i] , target = target_chunk -> mem_ptr){

        size_t size_to_copy = (size_t)(char *)((char *)target_chunk -> mem_ptr
        /*                                   */+ target_chunk -> used_size
        /*                                   */- (char *)target)
        /*                                   */- vec_ptr -> obj_rounded_size;

        if(size_to_copy > 0){
            memmove(target , (char *)target + vec_ptr -> obj_rounded_size , size_to_copy);
        }

        if(i == vec_ptr -> chunk_no - 1){
            break;
        }

        target = (char *)target_chunk -> mem_ptr + target_chunk -> used_size - vec_ptr -> obj_actual_size;
        memcpy(target , vec_ptr -> chunks_ptr[i + 1] -> mem_ptr , vec_ptr -> obj_rounded_size);
    }

    if(i > 0 && target_chunk -> used_size == 0){
        target_chunk = vec_ptr -> chunks_ptr[i - 1];
        if(vec_ptr -> chunk_no > 1 && i < vec_ptr -> chunk_no - 1){
            dynamic_array_remove_last_chunk(vec_ptr);
        }
    }

    target_chunk -> used_size -= vec_ptr -> obj_rounded_size;
    vec_ptr -> elements_no--;


    return Success;
}

datastruct_err dynamic_array_edit_element(dynamic_array *vec_ptr , u64 index , void *new_val_ptr){
    if(!vec_ptr || !new_val_ptr){
        return Invalid_Input;
    }

    if(index >= vec_ptr -> elements_no){
        return Invalid_Input;
    }

    void *target = dynamic_array_get_element_ptr(vec_ptr , index);

    if(!target){
        return Allocation_err;
    }

    if(vec_ptr -> free_obj){
        vec_ptr -> free_obj(copy_object(target , vec_ptr -> obj_actual_size));
    }

    memcpy(target , new_val_ptr , vec_ptr -> obj_actual_size);

    return Success;
}

void *dynamic_array_get_element(dynamic_array *vec_ptr , u64 index){
    void *target = dynamic_array_get_element_ptr(vec_ptr , index);

    if(!target){
        return NULL;
    }

    return copy_object(target , vec_ptr -> obj_actual_size);
}

u64 dynamic_array_get_elements_no(dynamic_array *vec_ptr){
    if(!vec_ptr){
        return 0;
    }

    return vec_ptr -> elements_no;
}

size_t dynamic_array_get_obj_size(dynamic_array *vec_ptr){
    if(!vec_ptr){
        return 0;
    }

    return vec_ptr -> obj_actual_size;
}