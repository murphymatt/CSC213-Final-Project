#include <stdlib.h>

#include "node.h"

node_t* add_node(char type, char* val) {
    node_t* new_node = (node_t*) malloc(sizeof(node_t));
    if (new_node != NULL) {
        new_node->type = type;
        new_node->val = val;
        new_node->neighbors = NULL;
        return new_node;
    } else {
        return NULL;
    }
}

void add_neighbor(node_t* node1, node_t* node2) {
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


void delete_node(node_t* sad_node) {
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
    list_node_t* current = sad_node->neighbors;
    while (current != NULL) {
        list_node_t* temp = current;
        current = curret->next;
        free(temp);
    }

    // Free sad node :(
    free(sad_node);
}










