/* Author: Steffen Viken Valvaag <steffenv@cs.uit.no> */
#include "list.h"
#include "set.h"
#include "common.h"
#include "printing.h"

static void print_argument(char *root);
void **dubptr_argument(char *root);

/*
 * Case-insensitive comparison function for strings.
 */
static int compare_words(void *a, void *b)
{
    return strcasecmp(a, b);
}

/*
 * Returns the set of (unique) words found in the given file.
 */
static set_t *tokenize(char *filename)
{
	set_t *wordset = set_create(compare_words);
	list_t *wordlist = list_create(compare_words);
	list_iter_t *it;
	FILE *f;
	
	f = fopen(filename, "r");
	if (f == NULL) {
		perror("fopen");
		ERROR_PRINT("fopen() failed");
	}
	tokenize_file(f, wordlist);
	
	it = list_createiter(wordlist);
	while (list_hasnext(it)) {
		set_add(wordset, list_next(it));		
	}
	list_destroyiter(it);
	list_destroy(wordlist);
	return wordset;
}

/*
 * Prints a set of words.
 */
static void printwords(char *prefix, set_t *words)
{
	set_iter_t *it;
	
	it = set_createiter(words);
	INFO_PRINT("%s: ", prefix);
	while (set_hasnext(it)) {
		printf(" %s", (char *)set_next(it));
	}
	printf("\n");
	set_destroyiter(it);
}

/* 
 * PRINT A SET OF WORDS, TAKING A PATH AS ARGUMENT.
 */
static void print_argument(char *root)
{
	/* creating list with spam words */
	list_t *files = find_files(root); // list with FILE PATH
	list_iter_t *files_iter = list_createiter(files); // iterator for FILE PATH
	list_t *list = list_create(compare_words); // list we will add EVERY spam words to
	FILE *name; // will hold the FILE NAME
		/* iterate over FILE PATHS */
		while(list_hasnext(files_iter) == 1){
			name = fopen(list_next(files_iter), "r"); // open file and read it
			tokenize_file(name, list); // add every word to "spamlist"
			fclose(name); // close file
		}
	/* creating set with spam words */
	set_t *set = set_create(compare_words); // set we will add spam words to
	list_iter_t *list_iter = list_createiter(list); // iterator for list with every spam words
	char *word; // will hold words
		/* iterate through spam list, and add to spam set */	
		while(list_hasnext(list_iter) == 1){
			word = list_next(list_iter);
			set_add(set, word);
		}
	/* free lists from memory */
	list_destroy(files);
	list_destroy(list);
	list_destroyiter(files_iter);
	list_destroyiter(list_iter);

	printwords(root, set);
		set_destroy(set);
}

/* 
 * RETURN A DOUBLE POINTER WHICH CONTAINS A SET OF WORDS FOR EACH TEXT DOCUMENT INSIDE A FOLDER
 * TAKING A PATH AS ARGUMENT.
 */
void **dubptr_argument(char *root){
	/* creating double pointer */
	list_t *files = find_files(root); // list with FILE PATH
	list_iter_t *files_iter = list_createiter(files); // iterator for FILE PATH
	char *name; // will hold the FILE NAME
	set_t **set = malloc(list_size(files) * sizeof(set_t *)); // double pointer will contain all the different sets
	int i = 0;
		/* iterate over FILE PATHS */
		while(list_hasnext(files_iter) == 1){
			name = list_next(files_iter);
			set[i] = tokenize(name); // add new set to double pointer
			i++;
		}
	list_destroy(files);
	list_destroyiter(files_iter);

	return set;
}

/*
 * Main entry point.
 */
int main(int argc, char **argv)
{
	char *spamdir, *nonspamdir, *maildir;
	
	if (argc != 4) {
		DEBUG_PRINT("usage: %s <spamdir> <nonspamdir> <maildir>\n", argv[0]);
		return 1;
	}
	spamdir = argv[1];
	nonspamdir = argv[2];
	maildir = argv[3];

	/* quantity of each folder */
	int q_spam = 4;
	int q_nonspam = 4;
	int q_mail = 5;

	// spamfilter.exe data/spam data/nonspam data/mail

	/* prints a set of words */
	// print_argument(spamdir);
	// print_argument(nonspamdir);
	// print_argument(maildir);

	/* double pointer which contains every set form spam, non-spam and mail */
	void **spamset = dubptr_argument(spamdir);
	void **nonspamset = dubptr_argument(nonspamdir);
	void **mailset = dubptr_argument(maildir);

	/* spam mail intersection */
	set_t *spam_inter = spamset[0];
	for(int inter = 1; inter < q_spam; inter++){
		spam_inter = set_intersection(spam_inter, spamset[inter]);
	}

	/* non-spam union */
	set_t *nonspam_uni = nonspamset[0];
	for(int uni = 1; uni < q_spam; uni++){
		nonspam_uni = set_union(nonspam_uni, nonspamset[uni]);
	}

	/* difference between spam and non-spam mail*/
	set_t *spam_nonspam_diff = set_difference(spam_inter, nonspam_uni);
		char *diff_pre = "THIS IS THE DIFFERENCE BETWEEN SPAM AND NON-SPAM";
		printwords(diff_pre, spam_nonspam_diff);

	/* check intersection between each mail, and spam and non-spam difference */
	set_t *check;
	for(int i; i < q_mail; i++){
		check = set_intersection(mailset[i], spam_nonspam_diff);
		if(set_size(check) == 0){
			printf("MAIL IS NOT SPAM!\n");
		}else{
			printf("MAIL IS SPAM! %i WORD(S) MATHCED\n", set_size(check));
		}
	}

	/* freeing sets form memory */
	for(int spamfree = 0; spamfree < q_spam; spamfree++){
		set_destroy(spamset[spamfree]);
	}
	free(spamset);
		set_destroy(spam_inter);
	for(int nonspamfree = 0; nonspamfree < q_nonspam; nonspamfree++){
		set_destroy(nonspamset[nonspamfree]);
	}
	free(nonspamset);
		set_destroy(nonspam_uni);
		set_destroy(spam_nonspam_diff);
	for(int mailfree = 0; mailfree < q_mail; mailfree++){
		set_destroy(mailset[mailfree]);
	}
	free(mailset);
		set_destroy(check);
    return 0;
}
