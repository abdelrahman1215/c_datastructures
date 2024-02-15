#ifndef AVL_TREE
#define AVL_TREE

#ifdef __cplusplus
extern "C"{
#endif

//todo :
//delete function
//lookup nearest function
//get node_no function
//divide into .c and .h files
//write function deoxygen docs
//write makescript
//write test

#include "def.h"
#include "../libsrc/mem.c"
#include <stdlib.h>


typedef struct AVL_node {
    struct AVL_node *prev_node;
    struct AVL_node *left_node;
    struct AVL_node *right_node;

    u32 height;

    int *obj_ptr;
}AVL_node;

typedef struct AVL_tree {
    size_t obj_size;
    free_func *free_obj;

    AVL_node *root;
    u32 node_no;
}AVL_tree;

static i128 min(i128 num1 , i128 num2){
    if(num1 > num2){
        return num2;
    }

    return num1;
}

static i128 max(i128 num1 , i128 num2){
    if(num1 > num2){
        return num1;
    }

    return num2;
}

AVL_tree *new_AVL_tree(size_t obj_size , free_func free_obj){
    if(!obj_size){
        return NULL;
    }

    AVL_tree *ret = (AVL_tree *)calloc(1 , sizeof(AVL_tree));
    if(!ret){
        return NULL;
    }

    ret -> obj_size = obj_size;

    ret -> free_obj = free;
    if(free_obj){
        ret -> free_obj = free_obj;
    }

    return ret;
}

static void free_tree(AVL_node *node_ptr , free_func *free_obj){
    if(!node_ptr || !free_obj){
        return;
    }

    free_tree(node_ptr -> right_node , free_obj);
    free_tree(node_ptr -> left_node , free_obj);

    free_obj(node_ptr -> obj_ptr);
    free(node_ptr);
}

datastruct_err destroy_AVL_tree(AVL_tree *tree_ptr){
    if(!tree_ptr){
        return Invalid_Input;
    }

    free_tree(tree_ptr -> root , tree_ptr -> free_obj);
    free(tree_ptr);
    return Success;
}

static AVL_node *AVL_tree_create_node(void *obj_ptr , size_t obj_size){
    AVL_node *ret = (AVL_node *)calloc(1 , sizeof(AVL_node));
    if(!ret){
        return NULL;
    }

    ret -> obj_ptr = (int *)copy_object(obj_ptr , obj_size);
    if(!ret -> obj_ptr){
        free(ret);
        return NULL;
    }

    return ret;
}

static i64 get_AVL_tree_balance(AVL_node *subtree_ptr){
    i64 left_sub_tree_height = -1;
    if(subtree_ptr -> left_node != NULL){
        left_sub_tree_height = subtree_ptr -> left_node -> height;
    }

    i64 right_sub_tree_height = -1;
    if(subtree_ptr -> right_node != NULL){
        right_sub_tree_height = subtree_ptr -> right_node -> height;
    }

    return left_sub_tree_height - right_sub_tree_height;
}

i64 max_height(AVL_node *node1_ptr , AVL_node *node2_ptr){
    if(node1_ptr == NULL){
        if(node2_ptr == NULL){
            return -1;
        }
        return node2_ptr -> height;
    }

    if(node2_ptr == NULL){
        return node1_ptr -> height;
    }

    return max(node1_ptr -> height , node2_ptr -> height);
}

static void AVL_tree_right_rotation(AVL_node *subtree_ptr , AVL_tree *tree_ptr){
    if(!subtree_ptr){
        return;
    }

    AVL_node *left = subtree_ptr -> left_node;

    if(!left){
        return;
    }

    subtree_ptr -> left_node = left -> right_node;
    if(left -> right_node){
        left -> right_node -> prev_node = subtree_ptr;
    }
    AVL_node *subtree_prev = subtree_ptr -> prev_node;
    subtree_ptr -> prev_node = left;
    
    
    left -> prev_node = subtree_prev;
    left -> right_node = subtree_ptr;

    if(subtree_prev != NULL){
        if(subtree_prev -> right_node == subtree_ptr){
            subtree_prev -> right_node = left;
        }else{
            subtree_prev -> left_node = left;
        }
    }

    if(tree_ptr -> root == subtree_ptr){
        tree_ptr -> root = left;
    }

    subtree_ptr -> height = max_height(subtree_ptr -> left_node , subtree_ptr -> right_node) + 1;
    left -> height = max_height(left -> left_node , left -> right_node) + 1;
    if(subtree_prev != NULL){
        subtree_prev -> height = max_height(subtree_prev -> left_node , subtree_prev -> right_node);
    }
}

static void AVL_tree_left_rotation(AVL_node *subtree_ptr , AVL_tree *tree_ptr){
    if(!subtree_ptr){
        return;
    }

    AVL_node *right = subtree_ptr -> right_node;

    if(!right){
        return;
    }

    subtree_ptr -> right_node = right -> left_node;
    if(right -> left_node){
        right -> left_node -> prev_node = subtree_ptr;
    }
    AVL_node *subtree_prev = subtree_ptr -> prev_node;
    subtree_ptr -> prev_node = right;
    
    
    right -> prev_node = subtree_prev;
    right -> left_node = subtree_ptr;

    if(subtree_prev != NULL){
        if(subtree_prev -> left_node == subtree_ptr){
            subtree_prev -> left_node = right;
        }else{
            subtree_prev -> right_node = right;
        }
    }

    if(tree_ptr -> root == subtree_ptr){
        tree_ptr -> root = right;
    }

    subtree_ptr -> height = max_height(subtree_ptr -> left_node , subtree_ptr -> right_node) + 1;
    right -> height = max_height(right -> left_node , right -> right_node) + 1;
    if(subtree_prev != NULL){
        subtree_prev -> height = max_height(subtree_prev -> left_node , subtree_prev -> right_node);
    }
}

static void balance_subtree(AVL_node *subtree_ptr , AVL_tree *tree_ptr){
    if(!subtree_ptr){
        return;
    }

    i64 balance = get_AVL_tree_balance(subtree_ptr);
    if(balance >= -1 && balance <= 1){
        return;
    }

    if(balance < -1){//right heavy
        if(subtree_ptr -> right_node -> right_node == NULL){//the heaviness is caused by the right-left grandchild
            AVL_tree_right_rotation(subtree_ptr -> right_node , tree_ptr);
        }
        AVL_tree_left_rotation(subtree_ptr , tree_ptr);
    }else if(balance > 1){//left heavy
        if(subtree_ptr -> left_node -> left_node == NULL){//the heaviness is caused by the left-right grandchild
            AVL_tree_left_rotation(subtree_ptr -> left_node , tree_ptr);
        }
        AVL_tree_right_rotation(subtree_ptr , tree_ptr);
    }
}

static void balance_tree(AVL_tree *tree_ptr){
    if(!tree_ptr){
        return;
    }

    if(!tree_ptr -> root){
        return;
    }

    i64 balance = get_AVL_tree_balance(tree_ptr -> root);
    if(balance >= -1 && balance <= 1){
        return;
    }

    AVL_node *target;
    for(target = tree_ptr -> root ;target != NULL && (target -> left_node != NULL || target -> right_node != NULL);){
        balance = get_AVL_tree_balance(target);
        if(balance >= -1 && balance <= 1){
            target = target -> prev_node;
            break;
        }

        if(balance < -1){//right heavy
            target = target -> right_node;
        }else if(balance > 1){//left heavy
            target = target -> left_node;
        }
    }

    AVL_node *tmp;
    for(AVL_node *node_ptr = target ; node_ptr != NULL ; node_ptr = tmp){
        balance = get_AVL_tree_balance(node_ptr);
        tmp = node_ptr -> prev_node;
        if(balance >= -1 && balance <= 1){
            break;
        }

        balance_subtree(node_ptr , tree_ptr);
    }
}

datastruct_err AVL_tree_add_node(AVL_tree *tree_ptr , void *obj_ptr){
    if(!tree_ptr|| !obj_ptr){
        return Invalid_Input;
    }

    AVL_node *new_node = AVL_tree_create_node(obj_ptr , tree_ptr -> obj_size);
    if(new_node == NULL){
        return Allocation_err;
    }

    if(tree_ptr -> root == NULL){
        tree_ptr -> root = new_node;
        return Success;
    }

    AVL_node *target = tree_ptr -> root;
    int diff = 0;
    while(1){
        diff = memcmp(obj_ptr , target -> obj_ptr , tree_ptr -> obj_size);
        //if the difference is negtive then the added object is less than the object stored in the current node
        //if the difference is positive then the added object is more than the object stored in the current node
        if(diff > 0){
            if(target -> right_node == NULL){
                break;
            }
            target = target -> right_node;
        } else{
            if(target -> left_node == NULL){
                break;
            }
            target =  target -> left_node;
        }
    }

    diff = memcmp(obj_ptr , target -> obj_ptr , tree_ptr -> obj_size);
    if(diff > 0){
        target -> right_node = new_node;
    } else{
        target -> left_node = new_node;
    }

    new_node -> prev_node = target;

    if(new_node -> prev_node -> height == 0){
        i64 max = 0;
        for(AVL_node *node_ptr = new_node -> prev_node; node_ptr != NULL ; node_ptr = node_ptr -> prev_node){
            max = max_height(node_ptr -> left_node , node_ptr -> right_node);
            if(node_ptr -> height >= max + 1){
                break;
            }

            node_ptr -> height = max + 1;
        }
    }

    i64 balance = get_AVL_tree_balance(tree_ptr -> root);
    if(balance < -1 || balance > 1){
        balance_tree(tree_ptr);
    }

    return Success;
}

static void *AVL_tree_lookup_node_by_value(AVL_tree *tree_ptr , void *val_ptr){
    if(!tree_ptr || !val_ptr){
        return NULL;
    }

    i64 diff;
    for(AVL_node *node_ptr = tree_ptr -> root ; node_ptr != NULL ;){
        diff = memcmp(val_ptr , node_ptr -> obj_ptr , tree_ptr -> obj_size);

        //if the difference is zero return the node
        //if the difference is negtive then the added object is less than the object stored in the current node
        //if the difference is positive then the added object is more than the object stored in the current node
        if(diff == 0){
            return node_ptr;
        }

        if(diff > 0){
            if(node_ptr -> right_node == NULL){
                break;
            }
            node_ptr = node_ptr -> right_node;
        } else if(diff < 0){
            if(node_ptr -> left_node == NULL){
                break;
            }
            node_ptr =  node_ptr -> left_node;
        }
    }

    return NULL;
}

//datastruct_err AVL_tree_delete_node(AVL_tree *tree_ptr , void *val_ptr){
// wirte this function
//}

datastruct_err AVL_tree_lookup_value(AVL_tree *tree_ptr , void *val_ptr){
    if(!tree_ptr || !val_ptr){
        return Invalid_Input;
    }

    return AVL_tree_lookup_node_by_value(tree_ptr , val_ptr) != NULL ? Success : Not_Found;
}

#ifdef __cplusplus
}
#endif

#endif