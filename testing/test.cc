#include <gtest/gtest.h>

#include "hash_table.h"
#include "node.h"

#define ARR_LEN 10


TEST(GraphTest, NodeAddTest) {
	graph_node_t* people_arr[ARR_LEN] = { NULL };
	// graph_node_t* class_arr[10] = { NULL };

	// the people we like
	graph_node_t* maddie = add_node('S', "Maddie Goldman");
	graph_node_t* henry = add_node('S', "Henry Fisher");
	graph_node_t* matt = add_node('S', "Matt Murphy");
	graph_node_t* myles = add_node('S', "Myles Becker");
	graph_node_t* cameron = add_node('S', "Cameron Chen");
	graph_node_t* sooji = add_node('S', "Sooji Son");
	graph_node_t* abyaya = add_node('S', "Abyaya Lamsal");
	graph_node_t* joshua = add_node('S', "Joshua Ekirikubinza");
	graph_node_t* cara = add_node('S', "Cara Bresnahan");
	graph_node_t* tapiwa = add_node('S', "Tapiwa Zvidzwa");

	people_arr[0] = maddie;
	people_arr[1] = henry;
	people_arr[2] = matt;
	people_arr[3] = myles;
	people_arr[4] = cameron;
	people_arr[5] = sooji;
	people_arr[6] = abyaya;
	people_arr[7] = joshua;
	people_arr[8] = cara;
	people_arr[9] = tapiwa;

	graph_node_t* csc213 = add_node('C', "CSC213");
	graph_node_t* csc261 = add_node('C', "CSC261");
	graph_node_t* mus116 = add_node('C', "MUS116");
	graph_node_t* csc301 = add_node('C', "CSC301");


	for (int i = 0; i < ARR_LEN; ++i)		
		add_neighbor(people_arr[i], csc213);

	add_neighbor(maddie, csc261);
	add_neighbor(cameron, csc261);
	add_neighbor(abyaya, csc261);
	add_neighbor(cara, csc261);

	add_neighbor(henry, mus116);
	add_neighbor(myles, mus116);
	add_neighbor(tapiwa, mus116);

	// (last semester)
	add_neighbor(maddie, csc301);
	add_neighbor(myles, csc301);
	add_neighbor(abyaya, csc301);
	add_neighbor(cameron, csc301);
	add_neighbor(joshua, csc301);


	list_node_t* current_neighbor = csc213->neighbors; 
	for (int i = 0; i < ARR_LEN; ++i)	{
		ASSERT_EQ(current_neighbor->graph_node, people_arr[ARR_LEN - 1 - i]);
		current_neighbor = current_neighbor->next;
	}

	ASSERT_EQ(csc261->neighbors->graph_node, cara);
	ASSERT_EQ(csc261->neighbors->next->graph_node, abyaya);
	ASSERT_EQ(csc261->neighbors->next->next->graph_node, cameron);
	ASSERT_EQ(csc261->neighbors->next->next->next->graph_node, maddie);
	ASSERT_EQ(csc261->neighbors->next->next->next->next, (list_node_t*) NULL);

	ASSERT_EQ(mus116->neighbors->graph_node, tapiwa);
	ASSERT_EQ(mus116->neighbors->next->graph_node, myles);
	ASSERT_EQ(mus116->neighbors->next->next->graph_node, henry);

	ASSERT_EQ(csc301->neighbors->graph_node, joshua);
	ASSERT_EQ(csc301->neighbors->next->graph_node, cameron);
	ASSERT_EQ(csc301->neighbors->next->next->graph_node, abyaya);
	ASSERT_EQ(csc301->neighbors->next->next->next->graph_node, myles);
	ASSERT_EQ(csc301->neighbors->next->next->next->next->graph_node, maddie);


	ASSERT_EQ(maddie->neighbors->graph_node, csc301);
	ASSERT_EQ(maddie->neighbors->next->graph_node, csc261);
	ASSERT_EQ(maddie->neighbors->next->next->graph_node, csc213);

}


TEST(GraphTest, NodeDeleteTest) {
		// the people we like
	graph_node_t* maddie = add_node('S', "Maddie Goldman");
	graph_node_t* henry = add_node('S', "Henry Fisher");
	graph_node_t* myles = add_node('S', "Myles Becker");
	graph_node_t* cameron = add_node('S', "Cameron Chen");
	graph_node_t* abyaya = add_node('S', "Abyaya Lamsal");

	//classes we have in common
	graph_node_t* csc213 = add_node('C', "CSC213");
	graph_node_t* csc261 = add_node('C', "CSC261");
	graph_node_t* mus116 = add_node('C', "MUS116");
	graph_node_t* csc301 = add_node('C', "CSC301");

	
	add_neighbor(henry, csc213);
	add_neighbor(myles, csc213);
	add_neighbor(cameron, csc213);
	add_neighbor(abyaya, csc213);
	add_neighbor(maddie, csc213);

	
	add_neighbor(cameron, csc261);
	add_neighbor(maddie, csc261);
	add_neighbor(abyaya, csc261);

	add_neighbor(henry, mus116);
	add_neighbor(myles, mus116);

	// (last semester)
	add_neighbor(maddie, csc301);
	add_neighbor(myles, csc301);
	add_neighbor(abyaya, csc301);
	add_neighbor(cameron, csc301);

	delete_node(maddie);

	list_node_t* current = csc213->neighbors;
	while (current != NULL) {
		ASSERT_NE(current->graph_node, maddie);
		current = current->next;
	}

	current = csc261->neighbors;
	while (current != NULL) {
		ASSERT_NE(current->graph_node, maddie);
		current = current->next;
	}

	delete_node(henry);
	delete_node(myles);

	ASSERT_EQ(mus116->neighbors, (list_node_t*) NULL);

	current = csc301->neighbors;
	while (current != NULL) {
		ASSERT_NE(current->graph_node, maddie);
		current = current->next;
	}

}

TEST(GraphTest, HashBasicAddTest) {
	hash_table_t* ht = (hash_table_t*) malloc(sizeof(hash_table_t));
	initialize_hash_table(ht);

	graph_node_t* maddie = add_node('S', "Maddie Goldman");
	graph_node_t* henry = add_node('S', "Henry Fisher");
	graph_node_t* matt = add_node('S', "Matt Murphy");

	add(ht, maddie);
	add(ht, henry);
	add(ht, matt);

	graph_node_t* new_maddie = search_table(ht, 'S', "Maddie Goldman");
	graph_node_t* new_henry = search_table(ht, 'S', "Henry Fisher");
	graph_node_t* new_matt = search_table(ht, 'S', "Matt Murphy");
	
	ASSERT_EQ(maddie, new_maddie);
	ASSERT_EQ(henry, new_henry);
	ASSERT_EQ(matt, new_matt);
}

TEST(GraphTest, BFSTest) {
 	graph_node_t* people_arr[ARR_LEN] = { NULL };
	// graph_node_t* class_arr[10] = { NULL };

	// the people we like
	graph_node_t* maddie = add_node('S', "Maddie Goldman");
	graph_node_t* henry = add_node('S', "Henry Fisher");
	graph_node_t* matt = add_node('S', "Matt Murphy");
	graph_node_t* myles = add_node('S', "Myles Becker");
	graph_node_t* cameron = add_node('S', "Cameron Chen");
	graph_node_t* sooji = add_node('S', "Sooji Son");
	graph_node_t* abyaya = add_node('S', "Abyaya Lamsal");
	graph_node_t* joshua = add_node('S', "Joshua Ekirikubinza");
	graph_node_t* cara = add_node('S', "Cara Bresnahan");
	graph_node_t* tapiwa = add_node('S', "Tapiwa Zvidzwa");

	people_arr[0] = maddie;
	people_arr[1] = henry;
	people_arr[2] = matt;
	people_arr[3] = myles;
	people_arr[4] = cameron;
	people_arr[5] = sooji;
	people_arr[6] = abyaya;
	people_arr[7] = joshua;
	people_arr[8] = cara;
	people_arr[9] = tapiwa;

	graph_node_t* csc213 = add_node('C', "CSC213");
	graph_node_t* csc261 = add_node('C', "CSC261");
	graph_node_t* mus116 = add_node('C', "MUS116");
	graph_node_t* csc301 = add_node('C', "CSC301");


	for (int i = 0; i < ARR_LEN; ++i)		
		add_neighbor(people_arr[i], csc213);

	add_neighbor(maddie, csc261);
	add_neighbor(cameron, csc261);
	add_neighbor(abyaya, csc261);
	add_neighbor(cara, csc261);

	add_neighbor(henry, mus116);
	add_neighbor(myles, mus116);
	add_neighbor(tapiwa, mus116);

	// (last semester)
	add_neighbor(maddie, csc301);
	add_neighbor(myles, csc301);
	add_neighbor(abyaya, csc301);
	add_neighbor(cameron, csc301);
	add_neighbor(joshua, csc301);


	list_node_t* current_neighbor = csc213->neighbors; 

    // add each of the student and class nodes to the hash table
    hash_table_t ht;
    initialize_hash_table(&ht);

    for (int i = 0; i < ARR_LEN; ++i)
        add(&ht, people_arr[i]);
    add(&ht, csc213);
    add(&ht, csc261);
    add(&ht, mus116);
    add(&ht, csc301);

    hash_table_t *bfs_table = bfs(csc261, 1);
    // ensure resulting neighbors are within returned hash table
    graph_node_t* new_maddie  = search_table(bfs_table, 'S', "Maddie Goldman");
    graph_node_t* new_cameron = search_table(bfs_table, 'S', "Cameron Chen");
    graph_node_t* new_abyaya  = search_table(bfs_table, 'S', "Abyaya Lamsal");
    graph_node_t* new_cara    = search_table(bfs_table, 'S', "Cara Bresnahan");
    ASSERT_EQ(new_maddie,  maddie);
    ASSERT_EQ(new_cameron, cameron);
    ASSERT_EQ(new_abyaya,  abyaya);
    ASSERT_EQ(new_cara,    cara);
    
    // ensure no other nodes are within the returned hash table
    graph_node_t* no_matt = search_table(bfs_table, 'S', "Matt Murphy");
    ASSERT_TRUE(no_matt == NULL);
}

TEST(GraphTest, HashAddCollisionTest) {
	hash_table_t* ht = (hash_table_t*) malloc(sizeof(hash_table_t));
	initialize_hash_table(ht);

	graph_node_t* maddie = add_node('S', "Maddie Goldman");
	graph_node_t* maddie2 = add_node('C', "Maddie Goldman");

	ASSERT_NE(maddie, maddie2);

	add(ht, maddie);
	add(ht, maddie2);

	graph_node_t* new_maddie = search_table(ht, 'S', "Maddie Goldman");
	graph_node_t* new_maddie2 = search_table(ht, 'C', "Maddie Goldman");
	
	ASSERT_EQ(maddie, new_maddie);
	ASSERT_EQ(maddie2, new_maddie2);
}

TEST(GraphTest, HashDeleteTest) {
	hash_table_t* ht = (hash_table_t*) malloc(sizeof(hash_table_t));
	initialize_hash_table(ht);

	graph_node_t* maddie = add_node('S', "Maddie Goldman");
	graph_node_t* maddie2 = add_node('C', "Maddie Goldman");

	add(ht, maddie);
	add(ht, maddie2);

	graph_node_t* new_maddie = search_table(ht, 'S', "Maddie Goldman");
	graph_node_t* new_maddie2 = search_table(ht, 'C', "Maddie Goldman");

	ASSERT_EQ(maddie, new_maddie);
	ASSERT_EQ(maddie2, new_maddie2);

	delete_hash_node(ht, maddie);
	graph_node_t* new_new_maddie2 = search_table(ht, 'C', "Maddie Goldman");
	
	ASSERT_EQ(maddie2, new_new_maddie2);
}
