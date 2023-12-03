#ifdef __cplusplus
extern "C"{
#endif

#ifndef LINKED_LIST
#define LINKED_LIST

#include "def.h"
#include "../libsrc/mem.c"

typedef struct node {
    struct node *prev;

    void *obj_ptr;
    size_t obj_size;
    free_func free_obj;

    struct node *next;
} node;

typedef struct linked_list {
    u64 node_no;
    node *first_node;
    node *last_node;
} linked_list;

#endif

#ifdef __cplusplus
}
#endif