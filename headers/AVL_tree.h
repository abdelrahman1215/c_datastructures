#ifndef AVL_TREE
#define AVL_TREE

#ifdef __cplusplus
extern "C"{
#endif

#include "def.h"

typedef struct AVL_node AVL_node;
typedef struct AVL_tree AVL_tree;

/// @brief makes a new AVL_tree
/// @param obj_size the size of the objects that will be stored in the avl tree
/// @param free_obj the function that will be used to free the obiect (enter NULL for the stdlib free)
/// @return a pointer to the new AVL_tree
AVL_tree *new_AVL_tree(size_t obj_size , free_func free_obj);

/// @brief destroy an AVL_tree
/// @param tree_ptr a pointer to the tree
/// @return the status of the operation
datastruct_err destroy_AVL_tree(AVL_tree *tree_ptr);

//--------------------------------

/// @brief adds a node to an AVL_tree
/// @param tree_ptr a pointer to the tree
/// @param obj_ptr a pointer to the object that will be stored in the node (a copy will be used)
/// @return the status of the operation
datastruct_err AVL_tree_add_node(AVL_tree *tree_ptr , void *obj_ptr);

/// @brief deletes a node from an AVL_tree
/// @param tree_ptr a pointer to the tree
/// @param val_ptr a pointer to a value equal to the node stored in the node you want to delete
/// @return the status of the operation
datastruct_err AVL_tree_delete_node(AVL_tree *tree_ptr , void *val_ptr);

//--------------------------------

/// @brief lookup the nearest value to a supplied value in a tree
/// @param tree_ptr a pointer to the tree
/// @param val_ptr a pointer to the value
/// @return the nearest value (if the value it self exists in the tree it will be returned)
void *AVL_tree_lookup_nearest_value(AVL_tree *tree_ptr , void *val_ptr);

/// @brief lookup a value in an AVL_tree
/// @param tree_ptr a pointer to the tree
/// @param val_ptr a pointer to the value
/// @return returns the status of the operation (if the value is found it returns Found)
datastruct_err AVL_tree_lookup_value(AVL_tree *tree_ptr , void *val_ptr);

//--------------------------------

/// @brief tells the number of nodes in am AVL_tree
/// @param tree_ptr a pointer to the tree
/// @return the number of nodes
u32 AVL_tree_get_node_no(AVL_tree *tree_ptr);

#ifdef __cplusplus
}
#endif

#endif