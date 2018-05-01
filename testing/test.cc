#include <gtest/gtest.h>

#include "../hash_table.h"
#include "../graph/node.h"

typedef struct node {
	
} node_t;

TEST(GraphTest, BasicNodeTest) {
	node_t* maddie = add_node('S', "Maddie Goldman");
	node_t* henry = add_node('S', "Henry Fisher");
	node_t* matt = add_node('S', "Matt Murphy");

	node_t* csc213 = add_node('C', "CSC213");
	node_t* csc261 = add_node('C', "CSC261");
	node_t* mus116 = add_node('C', "MUS116");

	add_neighbor(maddie, csc213);
	add_neighbor(maddie, csc261);

	add_neighbor(henry, csc213);


	add_neighbor(henry, csc213);
}