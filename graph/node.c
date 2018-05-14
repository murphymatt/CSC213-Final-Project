#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"

////////////////////////////////////////////////////////////////////////////////
// graph functions
graph_node_t* graph_add(char type, const char* val) {
  graph_node_t* new_node = (graph_node_t*) malloc(sizeof(graph_node_t));
  char* malloc_val =  (char*) malloc(sizeof(char) * MAX_STR);
  if (new_node != NULL && malloc_val != NULL) {
    new_node->type = type;
    strcpy(malloc_val, val);
    new_node->val = malloc_val;
    new_node->neighbors = NULL;
    new_node->flag = 0;
    new_node->m = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    return new_node;
  } else {
    return NULL;
  } 
}

void add_node_neighbor(graph_node_t* node1, graph_node_t* node2) {
  pthread_mutex_lock(&node1->m);
  pthread_mutex_lock(&node2->m);
  list_node_t* node1_list = (list_node_t*) malloc(sizeof(list_node_t));
  list_node_t* node2_list = (list_node_t*) malloc(sizeof(list_node_t));

  if (node1_list != NULL && node2_list != NULL) {
    node1_list->graph_node = node1;
    // Add node1 to the beginning of node2's neighbor list
    node1_list->next = node2->neighbors;
    node2->neighbors = node1_list;

    node2_list->graph_node = node2;
    // Add node2 to the beginning of node1's neighbor list
    node2_list->next = node1->neighbors;
    node1->neighbors = node2_list;
  }
  pthread_mutex_unlock(&node1->m);
  pthread_mutex_unlock(&node2->m);
}

void graph_delete(graph_node_t* sad_node) {
  pthread_mutex_lock(&sad_node->m);
  if (sad_node == NULL) {
    pthread_mutex_unlock(&sad_node->m);
    return;
  }

  list_node_t* current = sad_node->neighbors;

  // Free all list nodes pointing to sad_node
  while (current != NULL) {
    graph_node_t* current_graph = current->graph_node;
    list_node_t* current_neighbor = current_graph->neighbors;
    list_node_t* previous = NULL;
    while (current_neighbor != NULL) {
      if (current_neighbor->graph_node == sad_node) {
        if (previous == NULL) {
          current_graph->neighbors = current_neighbor->next;
        } else {
          previous->next = current_neighbor->next;
        }
        free(current_neighbor);
        break;
      }
      previous = current_neighbor;
      current_neighbor = current_neighbor->next;
    }
    current = current->next;
  }

  // Free all list nodes created on sad_node's behalf
  current = sad_node->neighbors;
  while (current != NULL) {
    list_node_t* temp = current;
    current = current->next;
    free(temp);
  }

  // Free sad node :(
  pthread_mutex_unlock(&sad_node->m);
  free(sad_node);
}

////////////////////////////////////////////////////////////////////////////////
// list functions
list_node_t* list_node_append(list_node_t* list, graph_node_t* node) {
  list_node_t* new_node;
  if ((new_node = malloc(sizeof(list_node_t))) == NULL) {
    fprintf(stderr, "list_node_append: Error allocating space for list node\n");
    exit(EXIT_FAILURE);
  }
  new_node->graph_node = node;
  new_node->next = list;
  list = new_node;
  return list;
}

// O(n) linear search through list node
bool list_node_contains(list_node_t* list, char type, const char* val) {
  graph_node_t* search_node = add_node(type, val);
  list_node_t* cur = list;
  while (cur != NULL) {
    if (_compare_node(cur->graph_node, search_node)) {
      free(search_node);
      return true;
    }
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////
// helper function
bool _compare_node(graph_node_t* node1, graph_node_t* node2) {
  return node1->type == node2->type && !strcmp(node1->val, node2->val);
}

