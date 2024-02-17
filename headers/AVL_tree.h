#ifndef AVL_TREE
#define AVL_TREE

#ifdef __cplusplus
extern "C"{
#endif

//todo :
//write function deoxygen docs
//write makescript
//write test
//write test_script

#include "def.h"

typedef struct AVL_node AVL_node;
typedef struct AVL_tree AVL_tree;

AVL_tree *new_AVL_tree(size_t obj_size , free_func free_obj);
datastruct_err destroy_AVL_tree(AVL_tree *tree_ptr);
datastruct_err AVL_tree_add_node(AVL_tree *tree_ptr , void *obj_ptr);
datastruct_err AVL_tree_delete_node(AVL_tree *tree_ptr , void *val_ptr);

void *AVL_tree_lookup_nearest_value(AVL_tree *tree_ptr , void *val_ptr);
datastruct_err AVL_tree_lookup_value(AVL_tree *tree_ptr , void *val_ptr);
u32 AVL_tree_get_node_no(AVL_tree *tree_ptr);

#ifdef __cplusplus
}
#endif

#endif