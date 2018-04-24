/***********************************************************************
 * Name:        Henry Fisher
 * Description: Hash table header for word frequency project
 ***********************************************************************/

#ifndef __hash_table_H__ 
#define __hash_table_H__ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ARR_LENGTH 256


typedef struct hash_table {
  hash_node_t* table[MAX_ARR_LENGTH];
} hash_table;

typedef struct hash_node {
    graph_node_t* graph_node;
    struct hash_node* next;
} hash_node_t;

/* Hash table essentials */
hash_table* initialize_hash_table();
void add(hash_table* hash, char word[MAX_STR]);
void delete(hash_table* hash, char word[MAX_STR]);
wordNode* find(hash_table* hash, char word[MAX_STR]);
unsigned long hash_function(char word[MAX_STR]);

#endif