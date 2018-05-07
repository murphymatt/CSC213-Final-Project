#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"

graph_node_t* add_node(char type, const char* val) {
  graph_node_t* new_node = (graph_node_t*) malloc(sizeof(graph_node_t));
  char* malloc_val = (char*) malloc(sizeof(char) * MAX_STR);

  if (new_node != NULL && malloc_val != NULL) {
    strcpy(malloc_val, val);
    new_node->type = type;
    new_node->val = malloc_val;
    new_node->neighbors = NULL;
    return new_node;
  } else {
    perror("Malloc failed");
    exit(2);
  }
}

bool compare_node(graph_node_t* node1, graph_node_t* node2) {
  return node1->type == node2->type && !strcmp(node1->val, node2->val);
}

void add_neighbor(graph_node_t* node1, graph_node_t* node2) {
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
}


void delete_node(graph_node_t* sad_node) {
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
  free(sad_node);
}
