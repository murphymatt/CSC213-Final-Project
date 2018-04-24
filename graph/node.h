#ifndef NODE_H
#define NODE_H

#include "list.h"
#define MAX_STR 64

typedef struct graph_node {
    char type;
    char val[MAX_STR];
    list_node_t* neighbors;
} graph_node_t;

typedef struct list_node {
    graph_node_t* graph_node;
    struct list_node* next;
} list_node_t;


node_t* add_node(char type, char* val);

void add_neighbor(node_t* node1, node_t* node2);

void delete_node(node_t* sad_node);


#endif
