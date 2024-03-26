#include "../headers/AVL_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main(){
    AVL_tree *test = new_AVL_tree(sizeof(int) , NULL);

    printf("testing the adding function...");

    datastruct_err error;
    for(int i = 1 ; i < 256 ; i++){
        error = AVL_tree_add_node(test , &i);
        assert(error == Success);
    }

    printf("success\n");

    printf("testing the lookup function...");

    for(int i = 1 ; i < 256 ; i++){
        error = AVL_tree_lookup_value(test , &i);
        assert(error == Found);
    }

    error = AVL_tree_lookup_value(test , "\000\000\001\000");
    assert(error == Not_Found);

    printf("success\n");

    printf("testing the delete function...");

    int random = rand();
    error = AVL_tree_delete_node(test , &random);
    assert(error == Success);

    error = AVL_tree_lookup_value(test , &random);
    assert(error == Not_Found);

    printf("success\n");

    destroy_AVL_tree(test);

    return 0;
}