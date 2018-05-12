#ifndef QUEUE_H
#define QUEUE_H

// This makes the header file work for both C and C++
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <pthread.h>

#include "node.h"

// queue used for bfs, contains distance from start metric
typedef struct queue_node {
  graph_node_t* g_node;
  int dist;
  struct queue_node* next;
} queue_node_t;

typedef struct queue {
  queue_node_t* front;
  queue_node_t* back;
  int size;
  pthread_mutex_t m;
} queue_t;

void queue_node_init(queue_node_t* node, graph_node_t* gnode, int dist);

void queue_init(queue_t* queue);

bool queue_empty(queue_t* queue);

void queue_push(queue_t* queue, graph_node_t* node, int dist);

queue_node_t* queue_pop(queue_t* queue);

// This makes the header file work for both C and C++
#ifdef __cplusplus
}
#endif

#endif
