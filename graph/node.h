#ifndef NODE_H
#define NODE_H

#include "list.h"

typedef struct node {
    int val;
    linked_list_t neighbors;
} node_t;

void node_init(node_t *n);

#endif
