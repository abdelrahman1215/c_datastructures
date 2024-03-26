#include "../headers/AVL_tree.h"
#include "copy_object.c"
#include <stdlib.h>

struct AVL_node {
    struct AVL_node *prev_node;
    struct AVL_node *left_node;
    struct AVL_node *right_node;

    u32 height;

    void *obj_ptr;
};

struct AVL_tree {
    AVL_node *last_edited_node;

    size_t obj_size;
    free_func *free_obj;

    AVL_node *root;
    u32 node_no;
};

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

static void free_AVL_node(AVL_node *node_ptr , free_func *free_obj){
    if(!node_ptr || !node_ptr){
        return;
    }

    free_obj(node_ptr -> obj_ptr);
    free(node_ptr);
}

static void free_AVL_tree(AVL_node *node_ptr , free_func *free_obj){
    if(!node_ptr || !free_obj){
        return;
    }

    free_AVL_tree(node_ptr -> right_node , free_obj);
    free_AVL_tree(node_ptr -> left_node , free_obj);

    free_AVL_node(node_ptr , free_obj);
}

datastruct_err destroy_AVL_tree(AVL_tree *tree_ptr){
    if(!tree_ptr){
        return Invalid_Input;
    }

    free_AVL_tree(tree_ptr -> root , tree_ptr -> free_obj);
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
    if(!subtree_ptr){
        return 0;
    }

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

static i64 max_height(AVL_node *node1_ptr , AVL_node *node2_ptr){
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

    i64 height;
    for(AVL_node *node_ptr = subtree_ptr ; node_ptr != NULL ; node_ptr = node_ptr -> prev_node){
        height = max_height(node_ptr -> left_node , node_ptr -> right_node) + 1;
        node_ptr -> height = height;
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

    i64 height;
    for(AVL_node *node_ptr = subtree_ptr ; node_ptr != NULL ; node_ptr = node_ptr -> prev_node){
        height = max_height(node_ptr -> left_node , node_ptr -> right_node) + 1;
        node_ptr -> height = height;
    }
}

static void balance_subtree(AVL_node *subtree_ptr , AVL_tree *tree_ptr){
    if(!subtree_ptr){
        return;
    }

    i64 balance = get_AVL_tree_balance(subtree_ptr);
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

    for(AVL_node *node_ptr = tree_ptr -> last_edited_node , *tmp ; node_ptr != NULL ; node_ptr = tmp){
        tmp = node_ptr -> prev_node;
        balance_subtree(node_ptr , tree_ptr);
    }

    tree_ptr -> last_edited_node = NULL;
}

datastruct_err AVL_tree_add_node(AVL_tree *tree_ptr , void *obj_ptr){
    if(!tree_ptr|| !obj_ptr){
        return Invalid_Input;
    }

    AVL_node *new_node = AVL_tree_create_node(obj_ptr , tree_ptr -> obj_size);
    if(new_node == NULL){
        return Allocation_err;
    }

    tree_ptr -> last_edited_node = new_node;
    tree_ptr -> node_no++;
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

    balance_tree(tree_ptr);

    return Success;
}

void *AVL_tree_lookup_nearest_value(AVL_tree *tree_ptr , void *val_ptr){
    if(!tree_ptr || !val_ptr){
        return NULL;
    }

    i64 diff;
    AVL_node *node_ptr;
    for(node_ptr = tree_ptr -> root ; node_ptr != NULL ;){
        diff = memcmp(val_ptr , node_ptr -> obj_ptr , tree_ptr -> obj_size);

        //if the difference is zero return the node
        //if the difference is negtive then the added object is less than the object stored in the current node
        //if the difference is positive then the added object is more than the object stored in the current node
        if(diff == 0){
            break;
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

    return node_ptr != NULL ? node_ptr -> obj_ptr : NULL;
}

static AVL_node *AVL_tree_lookup_node_by_value(AVL_tree *tree_ptr , void *val_ptr){
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

datastruct_err AVL_tree_delete_node(AVL_tree *tree_ptr , void *val_ptr){
    if(!tree_ptr || !val_ptr){
        return Invalid_Input;
    }

    if(!tree_ptr -> root){
        return Invalid_Input;
    }

    AVL_node *target = AVL_tree_lookup_node_by_value(tree_ptr , val_ptr);
    if(!target){
        return Not_Found;
    }

    if(target == tree_ptr -> last_edited_node){
        tree_ptr -> last_edited_node = target -> prev_node;
    }

    tree_ptr -> node_no--;

    if(target -> left_node == NULL && target -> right_node == NULL){
        if(target -> prev_node != NULL){
            if(target -> prev_node -> left_node == target){
                target -> prev_node -> left_node = NULL;
            }else{
                target -> prev_node -> right_node = NULL;
            }

            target -> prev_node -> height = max_height(target -> prev_node -> left_node , target -> prev_node -> right_node);
        }else{//root
            tree_ptr -> root = NULL;
        }
        free_AVL_node(target , tree_ptr -> free_obj);
        return Success;
    }

    AVL_node *succesor = NULL;

    //finding the succesor node
    if(target -> left_node != NULL && target -> right_node != NULL){//if the node has both nodes used it will go left then to the end of right
        for(succesor = target -> left_node ; succesor -> right_node != NULL ; succesor = succesor -> right_node){}
        if(succesor == target -> left_node){//if the succesor is the left node then it has no right daughters
            target -> left_node = succesor -> left_node;
        }else{
            succesor -> prev_node -> right_node = succesor -> left_node;
        }
    }

    else if(target -> left_node != NULL){//if only the right node is used it is the succesor
        succesor = target -> left_node;
        target -> left_node = NULL;
    }

    else {//if only the left node is used it is the succesor
        succesor = target -> right_node;
        target -> right_node = NULL;
    }

    AVL_node *succesor_prev = succesor -> prev_node;
    succesor -> prev_node = NULL;

    target -> obj_ptr = succesor -> obj_ptr;

    tree_ptr -> last_edited_node = succesor_prev;

    balance_tree(tree_ptr);
    
    return Success;
}

datastruct_err AVL_tree_lookup_value(AVL_tree *tree_ptr , void *val_ptr){
    if(!tree_ptr || !val_ptr){
        return Invalid_Input;
    }

    return AVL_tree_lookup_node_by_value(tree_ptr , val_ptr) != NULL ? Found : Not_Found;
}

u32 AVL_tree_get_node_no(AVL_tree *tree_ptr){
    if(!tree_ptr){
        return 0;
    }

    return tree_ptr -> node_no;
}