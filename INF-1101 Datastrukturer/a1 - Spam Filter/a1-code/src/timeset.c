#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "set.h"
#include "common.h"
#include "printing.h"
#include "time.h"

int cmpfunc(void *a, void *b){
	if(a < b){
		return -1;
	}else if(a > b){
		return 1;
	}else{
		return 0;
	}
}


int main(/* int argc, char **argv */){

	/*
    char *add_dir, *contains_dir;
	
	if (argc != 3) {
		DEBUG_PRINT("usage: %s <add_dir> <contains_dir>\n", argv[0]);
		return 1;
	}
	add_dir = argv[1];
	contains_dir = argv[2];
	*/

    int numbers = 50;
	int i, j, *elem;
	set_t *testset = set_create(cmpfunc);
	unsigned long long start, end, total;
	srand(time(NULL));   // Initialization

		start = gettime();
	for(i = 0; i < numbers; i++){

		elem =  rand() % numbers;
		set_add(testset, elem);

	}
		end = gettime();
		total = (end - start);
   		printf("Total time taken by CPU: %llu\n", total  );


    return 0;
}