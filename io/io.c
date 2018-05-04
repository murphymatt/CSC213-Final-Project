#include "io.h"

/*
 * out_file is a E+1 line file where E is edges in graph
 * first line:  Type, Value; Type2, Value2; etc
 * subsequent: Type, Value; Type2, Value2 where Value and Value2 have an edge
 */
void write_to_file(hash_table_t *ht, const char *file_path) {
    FILE *fp = fopen(file_path, "w");
    if (fp == NULL) {
        perror("Failed to open file");
        exit(2);
    }

    hash_node_t* current;
    for (int i = 0; i < MAX_ARR_LENGTH; i++) {
        current = ht->table[i];
        while (current != NULL) {
            printf("writing: %c, %s;\n", current->graph_node->type, current->graph_node->val);
            fprintf(fp, "%c, %s;", current->graph_node->type, current->graph_node->val);
            current = current->next;
        }
    }
}

// hash_table_t *read_from_file

int main() {
    graph_node_t* maddie = add_node('S', "Maddie Goldman");
    graph_node_t* henry = add_node('S', "Henry Fisher");
    graph_node_t* matt = add_node('S', "Matt Murphy");
    graph_node_t* csc213 = add_node('C', "CSC213");

    hash_table_t* ht = (hash_table_t*) malloc(sizeof(hash_table_t));
    initialize_hash_table(ht);

    add(ht, maddie);
    add(ht, henry);
    add(ht, matt);
    add(ht, csc213);

    char* path = "/Users/henryfisher/grinnell/213/CSC213-Final-Project/io/file.txt";

    write_to_file(ht, path);
}