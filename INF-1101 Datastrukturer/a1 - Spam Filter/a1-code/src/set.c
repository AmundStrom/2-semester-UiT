#include <stdlib.h>
#include <stdio.h>

#include "set.h"
#include "list.h"

/* set contains a linked list and compare fucntion */
struct set {
    list_t *linkedlist;
    cmpfunc_t cmpfunc;
};

/* Creates a new set using the given comparison function
 * to compare elements of the set. */
set_t *set_create(cmpfunc_t cmpfunc){
    set_t *set = malloc(sizeof(set_t));
    set->linkedlist = list_create(cmpfunc); //we use a linked list to store elements
    set->cmpfunc = cmpfunc;
    return set;
}

/* Destroys the given set. Subsequently accessing the set
 * will lead to undefined behavior. */
void set_destroy(set_t *set){
    list_destroy(set->linkedlist); //first destory the linked list
    free(set); // then destroy the set
}

/* Returns the size (cardinality) of the given set. */
int set_size(set_t *set){
    return list_size(set->linkedlist);
}

/* first we check if the element already is in the list 
 * if it is not, add first then sort list */
void set_add(set_t *set, void *elem){
    if(list_contains(set->linkedlist, elem) == 1){
        return;
    }else{
        list_addfirst(set->linkedlist, elem);
        list_sort(set->linkedlist);
    }
}

/* return 1 if the given element is in the set
 * or return 0 if it is not in the set */
int set_contains(set_t *set, void *elem){
    if(list_contains(set->linkedlist, elem) == 1){
        return 1;
    }else{
        return 0;
    }
}

/* Returns the union of the two given sets; the returned
 * set contains all elements that are contained in either
 * a or b. */
set_t *set_union(set_t *a, set_t *b){
    set_t *c = set_copy(a); // create copy of set a to set c
    set_iter_t *b_iter = set_createiter(b); // iterator for set b
    void *elem;

    /* if set c does not contain an element from set b, add element to set c */
    while(set_hasnext(b_iter) == 1){
        elem = set_next(b_iter);
        set_add(c,elem);
    }

    return c;
}

/* Returns the intersection of the two given sets; the
 * returned set contains all elements that are contained
 * in both a and b. */
set_t *set_intersection(set_t *a, set_t *b){
    set_t *c = set_create(a->cmpfunc); // create new list
    list_iter_t *a_iter = list_createiter(a->linkedlist); // iterator for set a
    void *elem;

    /* checks if both set a and b contains the same element, add element to set c if they do */
    while(list_hasnext(a_iter) == 1){ 
        elem = list_next(a_iter); 
        if(list_contains(b->linkedlist, elem) == 1){ 
            set_add(c, elem);
        }
    }

    return c;
}

/* Returns the set difference of the two given sets; the
 * returned set contains all elements that are contained
 * in a and not in b. */
set_t *set_difference(set_t *a, set_t *b){
    /* same logic as above */
    set_t *c = set_create(a->cmpfunc);
    list_iter_t *a_iter = list_createiter(a->linkedlist);
    void *elem;

    /* checks if set b contains an element in set a, if it does not, add element to set c */
    while(list_hasnext(a_iter) == 1){
        elem = list_next(a_iter);
        if(list_contains(b->linkedlist, elem) == 0){
            set_add(c, elem);
        }
    }

    return c;
}

/* Returns a copy of the given set. */
set_t *set_copy(set_t *set){
    /* same logic as above */
    set_t *c = set_create(set->cmpfunc);
    list_iter_t *set_iter = list_createiter(set->linkedlist);
    void *elem;

    /* adds every element in the given set, to set c */
    while(list_hasnext(set_iter) == 1){
        elem = list_next(set_iter);
        set_add(c, elem);
    }

    return c;
}

/* struct only contains the linked list we will iterate through */
struct set_iter {
    list_iter_t *linkedlist;
};

/* create new iterator for the given set */
set_iter_t *set_createiter(set_t *set){
    set_iter_t *set_iter = malloc(sizeof(set_iter_t));
    set_iter->linkedlist = list_createiter(set->linkedlist);
    return set_iter;
}

/* destroys the given iterator */
void set_destroyiter(set_iter_t *iter){
    list_destroyiter(iter->linkedlist);
    free(iter);
}

/* retruns 0 if the given set iterator has reached the end of the set
 * otherwise returns 1 */
int set_hasnext(set_iter_t *iter){
    return list_hasnext(iter->linkedlist);
}

/* returns the current element in the given set iterator,
 * and moves to next element in the set */
void *set_next(set_iter_t *iter){
    return list_next(iter->linkedlist);
}