#ifndef NODE_H
#define NODE_H

#define MAX_STR 64

// This makes the header file work for both C and C++
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <pthread.h>

typedef struct list_node {
  struct graph_node* graph_node;
  struct list_node* next;
} list_node_t;

typedef struct graph_node {
  char type; // S or C
  const char* val;
  list_node_t* neighbors;
  int flag; // generally useful to include :)
  pthread_mutex_t m;
} graph_node_t;

graph_node_t* add_node(char type, const char* val);
void add_node_neighbor(graph_node_t* node1, graph_node_t* node2);
void graph_delete(graph_node_t* sad_node);

// helper functions
bool _compare_node(graph_node_t* node1, graph_node_t* node2);

// list functions
list_node_t* list_node_append(list_node_t* list, graph_node_t* node);

// This makes the header file work for both C and C++
#ifdef __cplusplus
}
#endif

#endif
