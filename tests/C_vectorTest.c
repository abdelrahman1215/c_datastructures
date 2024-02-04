#include "../headers/c_vector.h"
#include <time.h>
#include <stdio.h>
#include <assert.h>

int main(){
    srand(time(NULL));

    c_vector *test = new_c_vector(sizeof(int) , NULL);
    assert(test != NULL);

    int nums[] = {1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10};

    printf("testing the adding function...");

    for(int i = 0 ; i < 10 ; i++){
        assert(c_vector_add_element(test , nums + i) == Success);
    }

    printf("success\n");

    printf("testing the getting function...");

    for(int i = 0 ; i < 10 ; i++){
        void *obj_ptr = c_vector_get_element(test , i);
        assert(obj_ptr != NULL);
        assert(*(int *)obj_ptr == nums[i]);
        free(obj_ptr);
    }

    printf("success\n");

    printf("testing the editing function...");

    u64 index = rand() % 10;
    int random = rand() % 1000;
    assert(c_vector_edit_element(test , index , &random) == Success);

    void *obj_ptr = c_vector_get_element(test , index);
    assert(obj_ptr != NULL);
    assert(*(int *)obj_ptr == random);
    free(obj_ptr);

    printf("success\n");

    c_vector_edit_element(test , index , nums + index);
    
    printf("testing the removing function...");

    assert(c_vector_remove_element(test , 0) == Success);
    obj_ptr = c_vector_get_element(test , 0);
    assert(obj_ptr != NULL);
    assert(*(int *)obj_ptr == nums[1]);
    free(obj_ptr);

    printf("success\n");

    destroy_c_vector(test);
    
    return 0;
}