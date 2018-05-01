#ifndef NODE_H
#define NODE_H

#define MAX_STR 64

struct graph_node;

typedef struct list_node {
    struct graph_node* graph_node;
    struct list_node* next;
} list_node_t;

typedef struct graph_node {
    char type;
    char val[MAX_STR];
    list_node_t* neighbors;
} graph_node_t;


graph_node_t* add_node(char type, char* val);

void add_neighbor(graph_node_t* node1, graph_node_t* node2);

void delete_node(graph_node_t* sad_node);


#endif
