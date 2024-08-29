#include <stdio.h>
#include <stdlib.h>

#include "list.h"

typedef struct listnode listnode_t;

    struct listnode {
        listnode_t *next;
        listnode_t *prev;
        void *data;
    };

    struct list {
        listnode_t *head;
        cmpfunc_t cmpfunc;
        int numitems;
    };

list_t *list_create(cmpfunc_t cmpfunc){
    list_t *list = malloc(sizeof(list_t));
        list->head = NULL;
        list->cmpfunc = cmpfunc;
        list->numitems = 0;
    return list;
}

void list_destroy(list_t *list){
    free(list);
}

int list_size(list_t *list){
    return list->numitems;
}

int list_addfirst(list_t *list, void *elem){
    listnode_t *node = (listnode_t*)malloc(sizeof(listnode_t));
        node->data = elem;  // Gives this node data to NULL, since this is the head of the list
        if(list->head == NULL) // If the list is empty
        {
            list->head = node; // New head of the list, is the added node
            node->prev = NULL; // New node previous will point to NULL, since this is the only node in the list
            node->next = NULL; // New node next will point to NULL, since this is the only node in the list
        }else{ // If the list is not empty
            node->prev = list->head->prev; // New node previous is equal to the current head of the list's previous
            list->head->prev = node; // Current head of the list's previous, will point to New node
            node->next = list->head; // New node's next, will point to current head of the list
            list->head = node; // New node is the head of the list
        }
    list->numitems++;   // Increase number of items
    return 1;
}

int list_addlast(list_t *list, void *elem){
    listnode_t *node = (listnode_t*)malloc(sizeof(listnode_t));
    listnode_t *last = list->head;
        node->data = elem;  // Gives this node data
        node->next = NULL;    // This node will be the last in the list, so the next will point to NULL

        if(list->head == NULL){ // If list is empty, add first
            node->prev = NULL;
            list->head = node;
        }else{  // If list is not empty, add last
            while(last->next != NULL){
                last = last->next;
            }
            last->next = node;  
            node->prev = last;
        }
    list->numitems++;   // Increase number of items
    return 1;
}

void *list_popfirst(list_t *list){
    if(list->head == NULL){ // If the list is empty
        printf("Can't pop empty list!\n");
    }else{
        listnode_t *OldHead;
        OldHead = list->head; // Save old head of the list for later use
        if(list->head->next == NULL){ // If the list only contain one element
            list->head = NULL;
        }else{
            list->head->next->prev = list->head->prev; // Second node in list, will get the prevoius-pointer for the current head
            list->head = list->head->next; // New head of the list is the second node of the list
        }
        free(OldHead); // Free old head of the list form memory
        list->numitems--; // Decrease number of items in the list by 1
    }
    return 0;
}

void *list_poplast(list_t *list){
    if(list->head == NULL){ // If the list is empty
        printf("Can't pop empty list!\n");
    }else{
        listnode_t *currentnode, *tmp;
        currentnode = list->head;
        if(list->head->next == NULL){ // If the list only contain one element
            list->head = NULL;
        }else{
            while(currentnode->next != NULL){ // Iterate through the list, until currentnode points to NULL
                tmp = currentnode; // tmp is second to last node
                currentnode = currentnode->next; // currentnode is last node
            }
            tmp->next = currentnode->next; // tmp will now point to NULL
        }
        free(currentnode); // Free current node form the memory
        list->numitems--; // Decrease number of items in the list by 1
    }
    return 0;
}

int list_contains(list_t *list, void *elem){
    listnode_t *node = (listnode_t*)malloc(sizeof(listnode_t));
    node = list->head;
    while(node->data != elem){ // Iterate through list until it finds the matching element
        node = node->next;
        if(node == NULL){ // If the node is equal to NULL, the list does not contain the matching element
            return 0;
        }
    }
    return 1;
}

void list_sort(list_t *list){
    listnode_t *node, *tmp = (listnode_t*)malloc(sizeof(listnode_t));
    node = list->head;
    /* Bubble sort */
    int i, j;
    for(i = 0; i < list->numitems; i++){
        for(j = 0; j < (list->numitems - i - 1); j++){
            if(list->cmpfunc(node->data, node->next->data) > 0){
                tmp = node->next->data;
                node->next->data = node->data;
                node->data = tmp;   
            }
            node = node->next;
        }
        node = list->head;
    }
}

/* struct for list iterator */
struct list_iter {
    listnode_t *node;
    list_t *list;
};

list_iter_t *list_createiter(list_t *list){
    list_iter_t *NewIterator = malloc(sizeof(list_iter_t)); //create space in memory
        NewIterator->list = list; //set the list for the iterator equal to the Original list, so they will have the same content
        NewIterator->node = list->head; //set the first node for the iterator equal to the head of the Original list
    return NewIterator;
}

void list_destroyiter(list_iter_t *iter){
    free(iter);
}

/* Returns 0 if the given list iterator has reached the end of the
 * list, or 1 otherwise. */
int list_hasnext(list_iter_t *iter){
    if(iter->node != NULL){
        return 1;
    }
    return 0;
}

/* Returns the next element in the sequence represented by the given
 * list iterator. */
void *list_next(list_iter_t *iter){
    void *data;
        data = iter->node->data;
        if(iter->node != NULL){
            iter->node = iter->node->next;
        }
    return data;
}
