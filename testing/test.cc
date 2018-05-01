#include <gtest/gtest.h>

#include "../hash_table.h"
#include "../graph/node.h"


TEST(GraphTest, BasicNodeTest) {
	node_t* people_arr[10] = { NULL };
	// node_t* class_arr[10] = { NULL };

	node_t* maddie = add_node('S', "Maddie Goldman");
	node_t* henry = add_node('S', "Henry Fisher");
	node_t* matt = add_node('S', "Matt Murphy");
	people_arr[0] = maddie;
	people_arr[1] = henry;
	people_arr[2] = matt;

	node_t* csc213 = add_node('C', "CSC213");
	node_t* csc261 = add_node('C', "CSC261");
	node_t* mus116 = add_node('C', "MUS116");

	for (int i = 0; i < 10; ++i) {
		if (people_arr[i] == NULL)
			break;
		
		add_neighbor(people_arr[i], csc213);
	}

	add_neighbor(maddie, csc261);
	add_neighbor(henry, mus116);

	printf("%s\n", maddie->neighbors->graph_node->val);
	ASSERT_EQ(maddie->neighbors->graph_node, csc213);
	printf("%s\n", maddie->neighbors->next->graph_node->val);
	ASSERT_EQ(maddie->neighbors->next->graph_node, csc213);

	printf("%s\n", henry->neighbors->graph_node->val);
	ASSERT_EQ(henry->neighbors->graph_node, csc213);
	printf("%s\n", henry->neighbors->next->graph_node->val);
	ASSERT_EQ(henry->neighbors->next->graph_node, mus116);

	// Delete after
}