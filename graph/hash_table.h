#ifndef __hash_table_H__ 
#define __hash_table_H__ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "node.h"

#define MAX_ARR_LENGTH 256

 // This makes the header file work for both C and C++
#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

typedef struct hash_node {
  graph_node_t* graph_node;
  struct hash_node* next;
} hash_node_t;

typedef struct header_node {
  hash_node_t* hash_node;
  pthread_mutex_t m;
} header_node_t;

typedef struct hash_table {
  header_node_t** table;
} hash_table_t;

// initializes a hash table
void hash_table_initialize(hash_table_t* ht);

// adds graph node to hash table
void hash_table_add(hash_table_t* ht, graph_node_t* graph_node);

void hash_table_delete_entry(hash_table_t* ht, graph_node_t* graph_node);

graph_node_t* hash_table_search(hash_table_t* ht, char type, const char* val);

hash_table_t* bfs(graph_node_t* start, int dist, int num_threads);

void hash_table_set_flags(hash_table_t* ht, int n);

void _bfs_helper(hash_table_t* ret_table, graph_node_t* start, int dist);

unsigned long hash_function(const char* word);

// This makes the header file work for both C and C++
#ifdef __cplusplus
}
#endif


#endif
