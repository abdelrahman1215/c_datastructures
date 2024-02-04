#include "../headers/linked_list.h"
#include <stdio.h>
#include <assert.h>

int main(){
    linked_list *test = new_linked_list();

    int nums[] = {1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10};
    datastruct_err err;

    printf("testing the adding function...");
    for(int i = 0 ; i < 10 ; i++){
        err = linked_list_add_node(nums + i , sizeof(int) , NULL , test);
        assert(err == Success);
    }
    printf("success\n");

    node *node_ptr = linked_list_get_first_node(test);
    assert(node_ptr != NULL);

    printf("testing the getting function...");
    void *obj_ptr = NULL;
    for(int i = 0 ; node_ptr != NULL ; node_ptr = linked_list_get_next_node(node_ptr) , i++){
        obj_ptr = linked_list_get_obj_ptr(node_ptr);
        assert(*(int *)obj_ptr == nums[i]);
    }
    printf("success\n");

    printf("testing the deleting function...");
    err = linked_list_delete_node(0 , test);
    assert(err == Success);
    
    node_ptr = linked_list_get_first_node(test);
    assert(node_ptr != NULL);

    obj_ptr = linked_list_get_obj_ptr(node_ptr);
    assert(*(int *)obj_ptr == nums[1]);
    printf("succes\n");

    destroy_linked_list(test);
    
    return 0;
}