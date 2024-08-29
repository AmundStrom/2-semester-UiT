#include <string.h>
#include <ctype.h>

#include "index.h"
#include "map.h"
#include "printing.h"
#include "trie.h"

/*
 * Implement your index here.
 */ 
struct index
{
    trie_t *trie;
    map_t *hashmap;
    list_t *documents;
    int doc_quantity;
};

/*
 * Struct to hold a single search result.
 * Implement this to work with the search result functions.
 */
struct search_result
{
    list_t *position;
    list_iter_t *position_iter;
    list_iter_t *documents;
    int size;
};

/* Struct to hold information about documents added to index */
struct document
{
    char *name;
    char **content;
    int size;
};
typedef struct document document_t;

/* Create new struct to hold information about the document added to the index */
document_t *document_create(char *document_name, list_t *words)
{
    document_t *doc = (document_t *)malloc(sizeof(document_t));
    doc->name = document_name;
    doc->content = calloc(list_size(words), sizeof(char*));
    doc->size = 0;
    return doc;
}

/* Deallocates the given document */
void document_destroy(document_t *doc){
    free(doc->content);
    free(doc);
}


/*
 * Creates a new, empty index.
 */
index_t *index_create()
{
    index_t *index = (index_t *)malloc(sizeof(index_t));
    index->trie = trie_create();
    index->hashmap = map_create(compare_pointers, djb2);
    index->documents = list_create(compare_strings);
    index->doc_quantity = 0;
    return index;
}


/*
 * Destroys the given index. Subsequently accessing the index will
 * lead to undefined behavior.
 */
void index_destroy(index_t *idx)
{
    document_t *doc;
    list_iter_t *iter = list_createiter(idx->documents);
    while( (doc = list_next(iter)) != NULL ){
        document_destroy(doc);
    }
    list_destroyiter(iter);
    list_destroy(idx->documents);

    trie_destroy(idx->trie);
    map_destroy(idx->hashmap);
    free(idx);
}


/*
 * Adds all the words from the given document to the given index.
 */
void index_add_document(index_t *idx, char *document_name, list_t *words)
{
    list_iter_t *iter = list_createiter(words);
    document_t *doc = document_create(document_name, words);
    void *pos = 0;
    void *val = 0;
        val++;
    char *key;
    int size = list_size(words);

    for(int i = 0; i < size; i++)
    {
        key = list_next(iter);
        trie_insert(idx->trie, key, val);
        map_put(idx->hashmap, key, pos);
        doc->content[i] = key;
        pos++;
    }
    list_addlast(idx->documents, doc);
    doc->size = size;
    idx->doc_quantity++;

    list_destroyiter(iter);
}


/* 
 * Separate search hits by checking which document they appear in.
 * Function deallocate given list.
 */
list_t **separate_search_hits(index_t *idx, list_iter_t **table_list, char **query_ptr, int words)
{
    /* Create lists equal to the amount of documents */
    list_t **sep_list = malloc(idx->doc_quantity * sizeof(list_t *));
        for(int i = 0; i < idx->doc_quantity; i++){
            sep_list[i] = list_create(compare_pointers);
        }
    int doc_counter;
    map_item_t *pos;

    /* Iterating over amount of words */
    for(int i = 0; i < words; i++)
    {
        list_iter_t *documents = list_createiter(idx->documents);
        document_t *doc = list_next(documents);
        doc_counter = 0;

        /* Iterate through table for current word */
        while(list_hasnext(table_list[i]))
        {
            pos = list_next(table_list[i]);
            /* If position is out of range for current document, move to next doucment */
            while( doc->size < (intptr_t)pos->value )
            {
                if(list_hasnext(documents)){
                    doc = list_next(documents);
                    doc_counter++;
                }
            }
            /* Check if current word is in current document
             * If it's not, move to next document */
            while(compare_strings(doc->content[ (intptr_t)pos->value ], query_ptr[i]) != 0)
            {
                if(list_hasnext(documents)){
                    doc = list_next(documents);
                    doc_counter++;
                }
            }
            list_addlast(sep_list[doc_counter], pos->value);
        }
        list_destroyiter(documents);
        list_destroyiter(table_list[i]);
    }
    free(table_list);

    return sep_list;
}


/* 
 * Connect search hits from differnet documents to the same list,
 * adding position and length of words that match the query.
 * Add -1 to the list, when it moves to next the document.
 * Function deallocate given list.
 */
list_t *connect_search_hits(index_t *idx, list_t **sep_list, char **query_ptr, int words)
{
    list_t* con_list = list_create(compare_pointers);
    list_iter_t *documents = list_createiter(idx->documents);
    document_t *doc = list_next(documents);
    void *buffer[words];
    void *diff = 0;
    void *null = 0;
        null--;
    for(int i = 0; i < idx->doc_quantity; i++)
    {
        memset(buffer, -1, words*sizeof(buffer[0]));
        list_sort(sep_list[i]); // Sort list by rising order
        list_iter_t *iter = list_createiter(sep_list[i]);

        /* Iterate over words that are in the same document */
        while(list_hasnext(iter))
        {
            diff = 0;
            /* Shuffle words to the left */
            for(int j = 0; j < (words-1); j++)
            {
                buffer[j] = buffer[j+1];
            }
            buffer[words-1] = list_next(iter); // Add next word to the end of buffer
            
            /* If buffer is filled, we can start checking if current buffer match the query */
            if( (intptr_t)buffer[0] != -1 )
            {
                diff += (intptr_t)buffer[words-1];  // Calculate difference between
                diff -= (intptr_t)buffer[0];        // end and start of buffer
                diff++;                             // to check if words appear in sequence
                if( (intptr_t)diff < (words * 2) )
                {
                    /* Iterate through buffer and compare if buffer is the same as query
                    * If it's not, go to end of while-loop */
                    for(int j = 0; j < words; j++)
                    {
                        if(compare_strings(doc->content[ (intptr_t)buffer[j] ], query_ptr[j]) != 0){
                            goto end;
                        }
                    }
                    list_addlast(con_list, buffer[0]);
                    list_addlast(con_list, diff);
                    memset(buffer, -1, words*sizeof(buffer[0]));
                }
            }
         end:
        }
        doc = list_next(documents);
        list_addlast(con_list, null);
        list_destroyiter(iter);
        list_destroy(sep_list[i]);
    }
    free(sep_list);
    list_destroyiter(documents);

    return con_list;
}

/* 
 * Return a list containing the positions and length of search hits that match the query.
 * List contain -1 to tell when to switch documents.
 */
list_t *position(index_t *idx, char *query)
{
    /* Convert query into char array, and count how many words are in query */
    int words = 1;
    char input[strlen(query)];
    for(int i = 0; query[i] != '\0'; i++)
    {
        input[i] = query[i];
        if ((int)isalpha((int)query[i]) == 0){
            words++;
        }
    }
    input[strlen(query)] = '\0';

    list_iter_t **table_list = malloc(words * sizeof(list_iter_t *)); // Will hold table for each word
    char **query_ptr = calloc(words, sizeof(char*)); // Will hold each word
    char *token;

    /* Get table for each word, and split query into double pointer */
    token = strtok(input, " ");
    for(int i = 0; i < words; i++)
    {
        table_list[i] = map_get_table(idx->hashmap, token);
            /* If map_get_table returns NULL, than the index does not contain the quary */
            if(table_list[i] == NULL)
            {
                for(int j = 0; j < i; j++)
                {
                    list_destroyiter(table_list[j]);
                }
                goto error;
            }
        query_ptr[i] = token;
        token = strtok(NULL, " ");
    }

    list_t **sep_list = separate_search_hits(idx, table_list, query_ptr, words);

    list_t *con_list = connect_search_hits(idx, sep_list, query_ptr, words);
    
    free(query_ptr);

    return con_list;
 error:
    free(query_ptr);
    list_t *empty = list_create(compare_pointers);
    return empty;
}


/*
 * Finds a query in the documents in the index.
 * The result is returned as a search_result_t which is later used to iterate the results.
 */
search_result_t *index_find(index_t *idx, char *query)
{
    search_result_t *result = (search_result_t *)malloc(sizeof(search_result_t));
    result->documents = list_createiter(idx->documents);
    result->position = position(idx, query); // we only need this for deallocating the list.
    result->position_iter = list_createiter(result->position);
    result->size = 0;

    /* Check which document the first query appears in */
    if(list_hasnext(result->position_iter)){
        int check;
        while( (check = (intptr_t)list_checknext(result->position_iter)) == -1 ){
            list_next(result->documents);
            list_next(result->position_iter);
        }
    }

    return result;
}


/*
 * Autocomplete searches the given trie for a word containing input.
 * The input string is NULL terminated and contains size letters (excluding null termination).
 * The output string MUST be null terminated.
 */
char *autocomplete(index_t *idx, char *input)
{
    char *complete = trie_find(idx->trie, input);
    if(complete != NULL)
    {
        complete[strlen(complete)] = '\0';
    }
    return complete;
}


/* 
 * Return the content of the current document.
 * Subsequent calls to this function should return the next document.
 * This function should only be called once for each document.
 * This function should return NULL if there are no more documents.
 */
char **result_get_content(search_result_t *res)
{
    if(res->documents == NULL){
        return NULL;
    }

    if(list_hasnext(res->documents)){
        document_t *doc = list_next(res->documents);
        res->size = doc->size;
        return doc->content;
    }

    list_destroyiter(res->documents);
    res->documents = NULL;
    return NULL;
}


/*
 * Get the length of the current document.
 * Subsequent calls should return the length of the same document.
 */
int result_get_content_length(search_result_t *res)
{
    return res->size;
}


/*
 * Get the next result from the current query.
 * The result should be returned as an int, which is the index in the document content.
 * Should return NULL at the end of the search results.
 */
search_hit_t *result_next(search_result_t *res)
{
    if(res->position == NULL){
        return NULL;
    }

    if(list_hasnext(res->position_iter))
    {
        search_hit_t *hit = (search_hit_t *)malloc(sizeof(search_hit_t));
        int pos = (intptr_t)list_next(res->position_iter);
            if( pos == -1){
                return NULL;
            }
        hit->location = pos;
        hit->len = (intptr_t)list_next(res->position_iter);
        return hit;
    }

    list_destroyiter(res->position_iter);
    list_destroy(res->position);
    res->position = NULL;
    return NULL;
}
