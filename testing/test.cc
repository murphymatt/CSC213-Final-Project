#include <gtest/gtest.h>

#include "hash_table.h"
#include "node.h"

#define ARR_LEN 3


TEST(GraphTest, BasicNodeTest) {
	graph_node_t* people_arr[ARR_LEN] = { NULL };
	// graph_node_t* class_arr[10] = { NULL };

	graph_node_t* maddie = add_node('S', "Maddie Goldman");
	graph_node_t* henry = add_node('S', "Henry Fisher");
	graph_node_t* matt = add_node('S', "Matt Murphy");
	people_arr[0] = maddie;
	people_arr[1] = henry;
	people_arr[2] = matt;

	graph_node_t* csc213 = add_node('C', "CSC213");
	graph_node_t* csc261 = add_node('C', "CSC261");
	graph_node_t* mus116 = add_node('C', "MUS116");

	for (int i = 0; i < ARR_LEN; ++i)		
		add_neighbor(people_arr[i], csc213);

	add_neighbor(maddie, csc261);
	add_neighbor(henry, mus116);

	ASSERT_EQ(maddie->neighbors->graph_node, csc261);
	ASSERT_EQ(maddie->neighbors->next->graph_node, csc213);

	ASSERT_EQ(henry->neighbors->graph_node, mus116);
	ASSERT_EQ(henry->neighbors->next->graph_node, csc213);

	list_node_t* current_neighbor = csc213->neighbors; 
	for (int i = 0; i < ARR_LEN; ++i)	{
		ASSERT_EQ(current_neighbor->graph_node, people_arr[ARR_LEN - 1 - i]);
		current_neighbor = current_neighbor->next;
	}

	// delete_node(maddie);
	// ASSERT_EQ(csc213->neighbors->graph_node, matt);
	// ASSERT_EQ(csc213->neighbors->next->graph_node, henry);
	// ASSERT_EQ(csc213->neighbors->next->next, NULL);
	
}
