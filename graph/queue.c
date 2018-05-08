#include <stdlib.h>

#include "queue.h"

void init_node_queue(node_queue_t* queue) {
  queue->front = NULL;
  queue->back = NULL;
  queue->size = 0;
}

bool queue_empty(node_queue_t* queue) {
  return queue->size == 0;
}

void queue_push(node_queue_t* queue, graph_node_t* node) {
  queue_node_t *new_node = (queue_node_t*) malloc(sizeof(queue_node_t));
  new_node->g_node = node;
  new_node->next = NULL;
  if (queue_empty(queue)) {
    queue->front = new_node;
    queue->back = new_node;
  } else {
    queue->back->next = new_node;
    queue->back = new_node;
  }
  queue->size++;
}

graph_node_t* queue_pop(node_queue_t* queue) {
  if (queue_empty(queue)) return NULL;
  queue_node_t* ret_node = queue->front;
  queue->front = ret_node->next;
  queue->size--;
  return ret_node->g_node;
}
