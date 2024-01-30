#ifdef __cplusplus
extern "C"{
#endif

#ifndef LINKED_LIST
#define LINKED_LIST

#include "def.h"

typedef struct node node;

typedef struct linked_list linked_list;


/// @brief make a new linked list
/// @return a pointer to the newly made linked list
linked_list *new_linked_list();

/// @brief frees a linked list & its contents
/// @param list_ptr a pointer to the linked list
datastruct_err destroy_linked_list(linked_list *list_ptr);

//--------------------------------

/// @brief adds a node to a linked list
/// @param obj_ptr a pointer to the object that will be stored in the node (a copy of the object will be stored instead of the object it self)
/// @param obj_size the size of the object
/// @param free_obj the function the will be used to free it (enter NULL for the default stdlib free)
/// @param list_ptr a pointer to the linked list
/// @return the status of the operation (true on success and false on failure)
datastruct_err linked_list_add_node(void *obj_ptr , size_t obj_size , free_func *free_obj , linked_list *list_ptr);

/// @brief delete a node from a linked list
/// @param index the index of the node
/// @param list_ptr a pointer to the list
/// @return the status of the operation (true on success and false on failure)
datastruct_err linked_list_delete_node(u64 index , linked_list *list_ptr);

/// @brief get a node from a linked list using its index
/// @param index the index of the node
/// @param list_ptr a pointer to the list
/// @return the node
node *linked_list_get_node(u64 index , linked_list *list_ptr);

//--------------------------------

/// @brief get the first node in a linked list
/// @param list_ptr a pointer to the list
/// @return a pointer to the first node
node *linked_list_get_first_node(linked_list *list_ptr);

/// @brief get the last node in a linked list
/// @param list_ptr a pointer to the list
/// @return a pointer to the last node
node *linked_list_get_last_node(linked_list *list_ptr);

/// @brief get the number of nodes in a linked list
/// @param list_ptr a pointer to the list
/// @return the number of nodes
u64 linked_list_get_node_no(linked_list *list_ptr);

//--------------------------------

/// @brief get the node previous to a node
/// @param node_ptr a pointer to the node
/// @return a pointer to the previous node
node *linked_list_get_prev_node(node *node_ptr);

/// @brief get the node next to a node
/// @param node_ptr a pointer to the node
/// @return a pointer to the next node
node *linked_list_get_next_node(node *node_ptr);

/// @brief get the object stored in a node
/// @param node_ptr a pointer to the node
/// @return a pointer to the object
void *linked_list_get_obj_ptr(node *node_ptr);

/// @brief get the size of the object stored in a node
/// @param node_ptr a pointer to the node
/// @return the size of the stored object
size_t linked_list_get_obj_size(node *node_ptr);

#endif

#ifdef __cplusplus
}
#endif