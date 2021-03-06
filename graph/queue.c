#include <stdlib.h>

#include "queue.h"


void queue_node_init(queue_node_t* node, graph_node_t *gnode, int dist) {
  node->g_node = gnode;
  node->dist = dist;
  node->next = NULL;
}

void queue_init(queue_t* queue) {
  queue->front = NULL;
  queue->back = NULL;
  queue->size = 0;
}

bool queue_empty(queue_t* queue) {
  return queue->size == 0;
}

void queue_push(queue_t* queue, graph_node_t* node, int dist) {
  queue_node_t *new_node = (queue_node_t*) malloc(sizeof(queue_node_t));
  queue_node_init(new_node, node, dist);
  pthread_mutex_lock(&queue->m);
  if (queue_empty(queue)) {
    queue->front = new_node;
    queue->back = new_node;
  } else {
    queue->back->next = new_node;
    queue->back = new_node;
  }
  queue->size++;
  pthread_mutex_unlock(&queue->m);
}

queue_node_t* queue_pop(queue_t* queue) {
  pthread_mutex_lock(&queue->m);
  if (queue_empty(queue)) {
    pthread_mutex_unlock(&queue->m);
    return NULL;
  }
  queue_node_t* ret_node = queue->front;
  queue->front = ret_node->next;
  queue->size--;
  pthread_mutex_unlock(&queue->m);
  return ret_node;
}
