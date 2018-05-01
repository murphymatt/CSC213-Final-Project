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
#include "node.h"

 // This makes the header file work for both C and C++
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_ARR_LENGTH 256

typedef struct hash_node {
    graph_node_t* graph_node;
    struct hash_node* next;
} hash_node_t;

typedef struct hash_table {
  hash_node_t* table[MAX_ARR_LENGTH];
} hash_table_t;

/* Hash table essentials */
hash_table_t* initialize_hash_table();
void add(hash_table_t* hash, graph_node_t* graph_node);

void delete_hash_node(hash_table_t* hash, graph_node_t* graph_node);

unsigned long hash_function(const char* word);


// This makes the header file work for both C and C++
#ifdef __cplusplus
}
#endif


#endif
