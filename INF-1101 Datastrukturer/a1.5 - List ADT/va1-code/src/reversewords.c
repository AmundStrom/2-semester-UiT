#include "list.h"
#include "common.h"

#define TRUE 1

int main(int argc, char **argv)
{
    list_t *list = list_create((cmpfunc_t) strcmp); // create a new list
    list_t *list_rev = list_create((cmpfunc_t) strcmp); // create a new list for reversed order

    /* arguments written in the command line is added to **argv
     * if you write "sortwords data\hamlet.txt" the second argument will be data\hamlet.txt
     * therefore we open the second argument (argv[1]) since this is a file, and we will only read it ("r") */
    FILE *fd = fopen(argv[1], "r");
    tokenize_file(fd, list); // add text to list

    list_sort(list);// sort list
    list_iter_t *iter = list_createiter(list); // create iterator for given list

    while(list_hasnext(iter) == TRUE){ // while list_hasnext return 1, loop
        /* list_next funtion returns the current element and then moves to the next element
         * we add this element first to a new list, we will then get the reversed order form the sorted list */
        list_addfirst(list_rev, list_next(iter));
    }

    list_iter_t *iter_rev = list_createiter(list_rev);
    while(list_hasnext(iter_rev) == TRUE){ // while list_hasnext return 1, loop
        printf("%s ", list_next(iter_rev)); // print list content
    }
    return 0;
}
