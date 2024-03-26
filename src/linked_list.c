#include "copy_object.c"
#include <stdlib.h>
#include "../headers/linked_list.h"

struct node {
    struct node *prev;

    void *obj_ptr;
    size_t obj_size;
    free_func *free_obj;

    struct node *next;
};

struct linked_list {
    u64 node_no;
    node *first_node;
    node *last_node;
};

linked_list *new_linked_list(){
    return (linked_list *) calloc(1 , sizeof(linked_list));
}

void linked_list_free_node(node *node_ptr){
    if(!node_ptr){
        return;
    }

    free(node_ptr -> obj_ptr);
    free(node_ptr);
}

void free_linked_list_contents(linked_list *list_ptr){
    if(!list_ptr){
        return;
    }

    for(node *curr_node = list_ptr -> first_node , *tmp ; curr_node ; curr_node = tmp){
        tmp = curr_node -> next;
        linked_list_free_node(curr_node);
    }

    list_ptr -> first_node = NULL;
    list_ptr -> last_node = NULL;
    list_ptr -> node_no = 0;
}

datastruct_err destroy_linked_list(linked_list *list_ptr){
    if(!list_ptr){
        return  Invalid_Input;
    }

    if(list_ptr -> first_node){
        free_linked_list_contents(list_ptr);
    }

    free(list_ptr);
    return Success;
}

node *linked_list_create_node(void *obj_ptr , size_t obj_size , free_func *free_obj){
    if(!obj_ptr || !obj_size){
        return NULL;
    }

    node *ret = (node *) calloc(1 , sizeof(node));
    if(!ret){
        return NULL;
    }

    ret -> obj_ptr = copy_object(obj_ptr , obj_size);
    if(!ret -> obj_ptr){
        free(ret);
        return NULL;
    }

    ret -> obj_size = obj_size;

    if(free_obj){
        ret -> free_obj = free_obj;
    }else{
        ret -> free_obj = free;
    }

    return ret;
}

datastruct_err linked_list_add_node(void *obj_ptr , size_t obj_size , free_func *free_obj , linked_list *list_ptr){
    if(!obj_ptr || !list_ptr || !obj_size){
        return Invalid_Input;
    }

    node *new_node = linked_list_create_node(obj_ptr , obj_size , free_obj);
    if(!new_node){
        return Allocation_err;
    }

    if(!list_ptr -> first_node){
        list_ptr -> first_node = list_ptr -> last_node = new_node;
    }else{
        node *target = list_ptr -> last_node;
        list_ptr -> last_node = new_node;
        new_node -> prev = target;
        target -> next = new_node;
    }

    list_ptr -> node_no++;

    return Success;
}

node *linked_list_get_node(u64 index , linked_list *list_ptr){
    if(!list_ptr){
        return NULL;
    }

    if(index >= list_ptr -> node_no){
        return NULL;
    }

    u64 i = 0;
    node *ret = NULL;

    if(index <= list_ptr -> node_no / 2){
        for(ret = list_ptr -> first_node ; ret != NULL && i < index ; ret = ret -> next , i++){}
    }else{
        for(ret = list_ptr -> last_node  , i = list_ptr -> node_no - 1; ret && i > index ; ret = ret -> prev , i--){}
    }

    return ret;
}

datastruct_err linked_list_delete_node(u64 index , linked_list *list_ptr){
    if(!list_ptr){
        return Invalid_Input;
    }

    if(index >= list_ptr -> node_no){
        return Invalid_Input;
    }

    node *target = linked_list_get_node(index , list_ptr);

    if(index == 0){
        list_ptr -> first_node = list_ptr -> first_node -> next;
        if(list_ptr -> node_no == 1){
            list_ptr -> last_node = NULL;
        }else{
            target -> next -> prev = target -> prev;
        }
    }else{ 
        if(index == list_ptr -> node_no - 1){
            list_ptr -> last_node = list_ptr -> last_node -> prev;
        }else{
            target -> next -> prev = target -> prev;
        }
        target -> prev -> next = target -> next;
    }

    linked_list_free_node(target);

    list_ptr -> node_no--;

    return Success;
}

node *linked_list_get_first_node(linked_list *list_ptr){
    if(!list_ptr){
        return NULL;
    }

    return list_ptr -> first_node;
}

node *linked_list_get_last_node(linked_list *list_ptr){
    if(!list_ptr){
        return NULL;
    }

    return list_ptr -> last_node;
}

u64 linked_list_get_node_no(linked_list *list_ptr){
    if(!list_ptr){
        return 0;
    }

    return list_ptr -> node_no;
}

node *linked_list_get_prev_node(node *node_ptr){
    if(!node_ptr){
        return NULL;
    }

    return node_ptr -> prev;
}

node *linked_list_get_next_node(node *node_ptr){
    if(!node_ptr){
        return NULL;
    }

    return node_ptr -> next;
}

void *linked_list_get_obj_ptr(node *node_ptr){
    if(!node_ptr){
        return NULL;
    }

    return node_ptr -> obj_ptr;
}

size_t linked_list_get_obj_size(node *node_ptr){
    if(!node_ptr){
        return 0;
    }

    return node_ptr -> obj_size;
}