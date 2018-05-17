#include <time.h>
#include <stdio.h>
#include <gtest/gtest.h>

#include "../io/io.h"

int get_next_name(FILE* fp, char name[MAX_STR], char* c) {
    // get type
    *c = fgetc(fp);
    if (*c == EOF || *c == '\n')
        return *c;

    // clear comma and space
    char nothing = fgetc(fp);
    nothing = fgetc(fp);

    int str_len = 0;
    while ((name[str_len] = fgetc(fp)) != ';') {
        if (name[str_len] == EOF) {
            printf("Bad input file format (found EOF in name)\n");
            exit(1);
        }
        str_len++;
    }
    name[str_len] = '\0';

    return 0;
}

// hack. copy the same function into this file
hash_table_t* read_from_file(const char *file_path) {
    hash_table_t* ht = (hash_table_t*) malloc(sizeof(hash_table_t));
    hash_table_initialize(ht);

    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        perror("Failed to open file for reading");
        exit(2);
    }

    // adding members
    while (true) {
        char type;
        char name[MAX_STR];
        int ret = get_next_name(fp, name, &type);
        if (ret == '\n')
            break;
        graph_node_t* new_node = graph_add(type, name);
        hash_table_add(ht, new_node);
        // get rid of the next space
        char nothing = fgetc(fp);
    }

    // adding relationships
    while (true) {
        // get name/type of node1
        char node1_type;
        char node1_name[MAX_STR];
        int ret = get_next_name(fp, node1_name, &node1_type);
        if (ret == EOF)
            break;
        graph_node_t* node1 = hash_table_search(ht, node1_type, node1_name);
        // get rid of the next space
        char nothing = fgetc(fp);

        while (true) {
            
            // get name/type of node2
            char node2_type;
            char node2_name[MAX_STR];
            int ret = get_next_name(fp, node2_name, &node2_type);
            if (ret == EOF || ret == '\n')
                break;
            graph_node_t* node2 = hash_table_search(ht, node2_type, node2_name);
            add_node_neighbor(node1, node2);
            // get rid of the next space
            nothing = fgetc(fp);
        }
    }

    return ht;
}

/*
 * test the time it takes to run each of the three bfs tests at 3 different
 * thread counts
 */
TEST(RuntimeTest, BFSTestSparse) {
  // create hash table and starting point for the hash table search
  hash_table_t* ht = read_from_file("../io/large_graph_sparse.txt");
  graph_node_t* start = hash_table_search(ht, 'S', "student 1");
  
  clock_t begin_1 = clock();
  bfs(start, 3, 1);
  clock_t end_1 = clock();
  double time_spent_1 = (double)(end_1 - begin_1) / CLOCKS_PER_SEC;
  std::cout << "Sparse, 1 thread:  " << time_spent_1 << std::endl;

  clock_t begin_2 = clock();
  bfs(start, 3, 4);
  clock_t end_2 = clock();
  double time_spent_2 = (double)(end_2 - begin_2) / CLOCKS_PER_SEC;
  std::cout << "Sparse, 4 thread:  " << time_spent_2 << std::endl;

  clock_t begin_3 = clock();
  bfs(start, 3, 8);
  clock_t end_3 = clock();
  double time_spent_3 = (double)(end_3 - begin_3) / CLOCKS_PER_SEC;
  std::cout << "Sparse, 8 thread:  " << time_spent_3 << std::endl;
}

/*
 * test the time it takes to run each of the three bfs tests at 3 different
 * thread counts
 */
TEST(RuntimeTest, BFSTestMid) {
  // create hash table and starting point for the hash table search
  hash_table_t* ht = read_from_file("../io/large_graph_mid.txt");
  graph_node_t* start = hash_table_search(ht, 'S', "student 1");
  
  clock_t begin_1 = clock();
  bfs(start, 3, 1);
  clock_t end_1 = clock();
  double time_spent_1 = (double)(end_1 - begin_1) / CLOCKS_PER_SEC;
  std::cout << "Mid, 1 thread:  " << time_spent_1 << std::endl;

  clock_t begin_2 = clock();
  bfs(start, 3, 4);
  clock_t end_2 = clock();
  double time_spent_2 = (double)(end_2 - begin_2) / CLOCKS_PER_SEC;
  std::cout << "Mid, 4 thread:  " << time_spent_2 << std::endl;

  clock_t begin_3 = clock();
  bfs(start, 3, 8);
  clock_t end_3 = clock();
  double time_spent_3 = (double)(end_3 - begin_3) / CLOCKS_PER_SEC;
  std::cout << "Mid, 8 thread:  " << time_spent_3 << std::endl;
}

/*
 * test the time it takes to run each of the three bfs tests at 3 different
 * thread counts
 */
TEST(RuntimeTest, BFSTestDense) {
  // create hash table and starting point for the hash table search
  hash_table_t* ht = read_from_file("../io/large_graph_dense.txt");
  graph_node_t* start = hash_table_search(ht, 'S', "student 1");
  
  clock_t begin_1 = clock();
  bfs(start, 3, 1);
  clock_t end_1 = clock();
  double time_spent_1 = (double)(end_1 - begin_1) / CLOCKS_PER_SEC;
  std::cout << "Dense, 1 thread:  " << time_spent_1 << std::endl;

  clock_t begin_2 = clock();
  bfs(start, 3, 4);
  clock_t end_2 = clock();
  double time_spent_2 = (double)(end_2 - begin_2) / CLOCKS_PER_SEC;
  std::cout << "Dense, 4 thread:  " << time_spent_2 << std::endl;

  clock_t begin_3 = clock();
  bfs(start, 3, 8);
  clock_t end_3 = clock();
  double time_spent_3 = (double)(end_3 - begin_3) / CLOCKS_PER_SEC;
  std::cout << "Dense, 8 thread:  " << time_spent_3 << std::endl;
}
