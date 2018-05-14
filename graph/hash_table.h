#ifndef __hash_table_H__ 
#define __hash_table_H__ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#include "node.h"
#include "queue.h"

#define MAX_ARR_LENGTH 256

 // This makes the header file work for both C and C++
#ifdef __cplusplus
extern "C" {
#endif

typedef struct hash_node {
  graph_node_t* graph_node;
  struct hash_node* next;
} hash_node_t;

typedef struct header_node {
  hash_node_t* hash_node;
  pthread_mutex_t m;
} header_node_t;

// we want to maintain a list of the non-empty hash bucket indices
typedef struct index_node {
  int ind;
  struct index_node *next;
} index_node_t;

typedef struct hash_table {
  header_node_t** table;
  index_node_t* index_list;
} hash_table_t;

typedef struct bfs_pthread_args {
  queue_t* node_queue;
  hash_table_t* ret_table;
  int dist;
} bfs_pthread_args_t;

// initializes a hash table with allocated memory
void hash_table_initialize(hash_table_t* ht);

// adds graph node to hash table
void hash_table_add(hash_table_t* ht, graph_node_t* graph_node);
void hash_table_delete_entry(hash_table_t* ht, graph_node_t* graph_node);
// hash_table_search returns a single node, bfs, returns a node neighborhood
graph_node_t* hash_table_search(hash_table_t* ht, char type, const char* val);
hash_table_t* bfs(graph_node_t* start, int dist, int num_threads);
hash_table_t* hash_table_intersection(hash_table_t* ht1, hash_table_t* ht2);

// list node functions
list_node_t* get_nodes(hash_table_t *ht);
list_node_t* node_intersection(list_node_t* lst1, list_node_t* lst2);

// thread functions
void* bfs_pthread_fn(void* args);

// helper functions
void hash_table_set_flags(hash_table_t* ht, int n);
void set_flags(hash_table_t* ht, int n);
unsigned long hash_function(const char* word);
void index_list_append(hash_table_t* ht, int ind);

// This makes the header file work for both C and C++
#ifdef __cplusplus
}
#endif

#endif
