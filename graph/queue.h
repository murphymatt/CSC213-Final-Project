#ifndef QUEUE_H
#define QUEUE_H

// This makes the header file work for both C and C++
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "node.h"

typedef struct queue_node {
  graph_node_t* g_node;
  struct queue_node* next;
} queue_node_t;

typedef struct node_queue {
  queue_node_t* front;
  queue_node_t* back;
  int size;
} node_queue_t;

void init_node_queue(node_queue_t* queue);

bool queue_empty(node_queue_t* queue);

void queue_push(node_queue_t* queue, graph_node_t* node);

graph_node_t* queue_pop(node_queue_t* queue);

// This makes the header file work for both C and C++
#ifdef __cplusplus
}
#endif

#endif
