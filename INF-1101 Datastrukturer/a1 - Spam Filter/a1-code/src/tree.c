#include <stdlib.h>
#include <stdio.h>

#include "set.h"

void *set_preOrder(set_iter_t *iter);

/*
 * The type of sets.
 */
struct treenode;
typedef struct treenode treenode_t;
    struct treenode {
        void *data;
        treenode_t* left;
        treenode_t* right;
        treenode_t* parent;
    };

struct set {
    treenode_t *root;
    cmpfunc_t cmpfunc;
    int size;
};


/* Creates a new set using the given comparison function
 * to compare elements of the set. */
set_t *set_create(cmpfunc_t cmpfunc){
    set_t *set = malloc(sizeof(set_t));
    if(set == NULL){
        return NULL;
    }
    set->cmpfunc = cmpfunc;
    set->root = NULL;
    set->size = 0;
    return set;
}

/* Destroys the given set in Post-Order fashion. Subsequently accessing the set
 * will lead to undefined behavior. */
void set_destroy(set_t *set){
    treenode_t *iter = set->root;
    treenode_t *tmp = iter;
    /* will iterate until root is NULL */
    while(set->root != NULL){
        /* if possible, move to left, and jump to the start of the loop */
        if(iter->left != NULL){
            iter = iter->left;
            continue;
        }
        /* if possible, move to right, and jump to the start of the loop */
        if(iter->right != NULL){
            iter = iter->right;
            continue;
        }
        /* free leaf, and go to parent of leaf
         * set value of leaf to NULL, so we wont get there again */
        tmp = iter;
        iter = iter->parent;
        if(iter == NULL){
            free(tmp);
            set->root = NULL;
            continue;
        }
        if(iter->left == tmp){
            iter->left = NULL;
        }else{
            iter->right = NULL;
        }
        free(tmp);
    }
    free(set);
}

/* Returns the size (cardinality) of the given set. */
int set_size(set_t *set){
    return set->size;
}

/* Adds the given element to the given set. */
void set_add(set_t *set, void *elem){
    /* check if the given element is already in the set */
    if(set_contains(set, elem) == 1){
        return;
    }
    treenode_t *node = malloc(sizeof(treenode_t));
    treenode_t *iter = set->root;
    treenode_t *tmp;

    /* check if the tree is empty */
    if(iter == NULL){
        set->root = node;
        node->left = node->right = NULL;
        node->parent = NULL;
        node->data = elem;
        set->size++;
        return;
    }
    /* iterate untill it reachs the end */
    while (iter != NULL)
    {
        // tmp will be the leaf of the tree
        tmp = iter;
        if(set->cmpfunc(elem, iter->data) > 0){
            iter = iter->right;
        }else{
            iter = iter->left;
        }
    }
    /* check if the element will be on the left or right side of the leaf */
    if(set->cmpfunc(elem, tmp->data) > 0){
        tmp->right = node;
        node->left = node->right = NULL;
        node->parent = tmp;
        node->data = elem;
    }else{
        tmp->left = node;
        node->left = node->right = NULL;
        node->parent = tmp;
        node->data = elem;
    }
    set->size++;
}

/* Returns 1 if the given element is contained in
 * the given set, 0 otherwise. */
int set_contains(set_t *set, void *elem){
    treenode_t *iter = set->root;
    /* if the set is empty, return 0 */
    if(set->root == NULL){
        return 0;
    }
    /* will iterate by subtracting the given element and elements in the set,
     * and check if they will be 0 */
    while(set->cmpfunc(elem, iter->data) != 0)
    {
        /* if the given element is greater than the current element in the set 
         * if it is, move to the right, otherwise to the left */
        if(set->cmpfunc(elem, iter->data) > 0){
            iter = iter->right;
        }else{
            iter = iter->left;
        }
        /* if the iterator as reached the end of the tree, retrun 0 */
        if(iter == NULL){
            return 0;
        }
    }
    return 1;
}

/* Returns the union of the two given sets; the returned
 * set contains all elements that are contained in either
 * a or b. */
set_t *set_union(set_t *a, set_t *b){
    set_t *c = set_copy(a); // make a copy of set a
    set_iter_t *b_iter = set_createiter(b); // iterator for set b
    void *elem;

    while(set_hasnext(b_iter) == 1){
        elem = set_preOrder(b_iter);
        set_add(c, elem);
    }

    return c;
}

/* Returns the intersection of the two given sets; the
 * returned set contains all elements that are contained
 * in both a and b. */
set_t *set_intersection(set_t *a, set_t *b){
    set_t *c = set_create(a->cmpfunc); // make a copy of set a
    set_iter_t *a_iter = set_createiter(a); // iterator for set b
    void *elem;

    while(set_hasnext(a_iter) == 1){
        elem = set_preOrder(a_iter);
        if(set_contains(b, elem) == 1){
            set_add(c, elem);
        }
    }

    return c;
}

/* Returns the set difference of the two given sets; the
 * returned set contains all elements that are contained
 * in a and not in b. */
set_t *set_difference(set_t *a, set_t *b){
    set_t *c = set_create(a->cmpfunc); // make a copy of set a
    set_iter_t *a_iter = set_createiter(a); // iterator for set b
    void *elem;

    while(set_hasnext(a_iter) == 1){
        elem = set_preOrder(a_iter);
        if(set_contains(b, elem) == 0){
            set_add(c, elem);
        }
    }

    return c;
}

/* Returns a copy of the given set. */
set_t *set_copy(set_t *set){
    /* same logic as above */
    set_t *c = set_create(set->cmpfunc);
    set_iter_t *set_iter = set_createiter(set);
    void *elem;

    /* adds every element in the given set, to set c */
    while(set_hasnext(set_iter) == 1){
        elem = set_preOrder(set_iter);
        set_add(c, elem);
    }

    return c;
}

/*
 * The type of set iterators.
 */
struct set_iter;
typedef struct set_iter set_iter_t;

struct set_iter {
    treenode_t *root;
    treenode_t *rightmost;
    treenode_t *prev;
};

/* Creates a new set iterator for iterating over the given set. */
set_iter_t *set_createiter(set_t *set){
    set_iter_t *set_iter = malloc(sizeof(set_iter_t));
    if (set_iter == NULL)
	    return NULL;
    
    set_iter->root = set->root; // we will iterate with this variable
    set_iter->prev = set->root; // this will be the previous node of the tree

    set_iter->rightmost = set->root; // this will be the rightmost node in the tree
    /* if root is equal to NULL, we dont want to iterate */
    if(set->root != NULL){
        while(set_iter->rightmost->right != NULL){
            set_iter->rightmost = set_iter->rightmost->right;
        }
    }

    return set_iter;
}

/* Destroys the given set iterator. */
void set_destroyiter(set_iter_t *iter){
    free(iter);
}

/* Returns 0 if the given set iterator has reached the end of the
 * set, or 1 otherwise. */
int set_hasnext(set_iter_t *iter){
    if(iter->root == NULL){
        return 0;
    }else{
        return 1;
    }
}

/* Returns the next element in In-Order fashion represented by the given
 * set iterator. */
void *set_next(set_iter_t *iter){

    void *elem;
    int done = 1;

    while(done){
        elem = iter->root->data;
        /* if possible, move to left, and jump to start of the loop */
        if(iter->root->left != NULL && (iter->root->left != iter->prev && iter->root->right != iter->prev)){
            iter->root = iter->root->left;
            continue;
        }
        /* if right child is equal to previous child,
         * update previous and move to parent, then jump to start */
        if(iter->root->right == iter->prev){
            iter->prev = iter->root;
            iter->root = iter->root->parent;
            continue;
        }
        /* if left child is equal to previous child,
         * or left chils is equal to NULL, then return element */
        if(iter->root->left == iter->prev || iter->root->left == NULL){
            /* if possible, move to right child, else, move to parent and update previous */
            if(iter->root->right != NULL){
                iter->root = iter->root->right;
            }else{
                /* this statment will be true for the last element to be returned */
                if(iter->root == iter->rightmost){
                    iter->root = NULL;
                    return elem;
                }
                iter->prev = iter->root;
                iter->root = iter->root->parent;
            }
            return elem;
        }
    }
}

/* Returns the next element in Pre-Order fashion represented by the given
 * set iterator. */
void *set_preOrder(set_iter_t *iter){
    void *elem = iter->root->data;
    /* if possible, move to left, and return the element before moving */
    if(iter->root->left != NULL){
        iter->root = iter->root->left;
        return elem;
    }
    /* if possible, move to right, and return the element before moving */
    if(iter->root->right != NULL){
        iter->root = iter->root->right;
        return elem;
    }

    int done = 1;
    /* will "climb out" of a sub-tree, and move to right when possible */
    while(done){
        // previous is the node iter was just now on
        iter->prev = iter->root;
        // move iter to parent
        iter->root = iter->root->parent;
        // this will be true for the last element to be returned
        if(iter->root == NULL){
            return elem;
        }
        /* if iter->right is NOT equal to previous, and if iter->right is NOT equal to NULL,
         * move to right node and return element */
        if(iter->root->right != iter->prev && iter->root->right != NULL){
            iter->root = iter->root->right;
            return elem;
        }
    }
}