#include "io.h"

/*
 * out_file is a E+1 line file where E is edges in graph
 * first line:  Type, Value; Type2, Value2; etc
 * subsequent: Type, Value: Type2, Value2; Type3, Value3 
 *    where Value has an edge with Value2, and Value3
 */
void write_to_file(hash_table_t *ht, const char *file_path) {
    FILE *fp = fopen(file_path, "w");
    if (fp == NULL) {
        perror("Failed to open file for writing");
        exit(2);
    }

    hash_node_t* current;
    for (int i = 0; i < MAX_ARR_LENGTH; i++) {
        current = ht->table[i];
        while (current != NULL) {
            fprintf(fp, "%c, %s; ", current->graph_node->type, current->graph_node->val);
            current = current->next;
        }
    }

    set_flags(ht, 0);
    for (int i = 0; i < MAX_ARR_LENGTH; i++) {
        current = ht->table[i];
        while (current != NULL) {
            fprintf(fp, "\n");
            current->graph_node->flag = 1;
            fprintf(fp, "%c, %s: ", current->graph_node->type, current->graph_node->val);

            list_node_t* cur_neighbor = current->graph_node->neighbors;
            while (cur_neighbor != NULL) {
                if (cur_neighbor->graph_node->flag == 0) {
                    fprintf(fp, "%c, %s; ", cur_neighbor->graph_node->type, cur_neighbor->graph_node->val);
                }
                cur_neighbor = cur_neighbor->next;
            }
            current = current->next;
        }
    }
}

hash_table_t* read_from_file(const char *file_path) {
    hash_table_t* ht = (hash_table_t*) malloc(sizeof(hash_table_t));
    initialize_hash_table(ht);

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

        graph_node_t* new_node = add_node(type, name);
        add(ht, new_node);
        // get rid of the next space
        char c = fgetc(fp);
    }

    // adding relationships
    while (true) {
        // get name/type of node1
        char node1_type;
        char node1_name[MAX_STR];
        int ret = get_next_name(fp, node1_name, &node1_type);
        if (ret == EOF)
            break;
        graph_node_t* node1 = search_table(ht, node1_type, node1_name);
        while (true) {
            // get name/type of node2
            char node2_type;
            char node2_name[MAX_STR];
            int ret = get_next_name(fp, node2_name, &node2_type);
            if (ret == EOF || ret == '\n')
                break;
            graph_node_t* node2 = search_table(ht, node2_type, node2_name);
            add_neighbor(node1, node2);
        }
    }

    return ht;
}

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

int main() {
    // while (true) {
    //     print_prompt();
    // }
    // graph_node_t* maddie = add_node('S', "Maddie Goldman");
    // graph_node_t* henry = add_node('S', "Henry Fisher");
    // graph_node_t* matt = add_node('S', "Matt Murphy");
    // graph_node_t* csc213 = add_node('C', "CSC213");
    // graph_node_t* csc321 = add_node('C', "CSC321");

    // add_neighbor(maddie, csc213);
    // add_neighbor(henry, csc213);
    // add_neighbor(matt, csc213);

    // add_neighbor(matt, csc321);
    // add_neighbor(henry, csc321);

    // hash_table_t* ht = (hash_table_t*) malloc(sizeof(hash_table_t));
    // initialize_hash_table(ht);

    // add(ht, maddie);
    // add(ht, henry);
    // add(ht, matt);
    // add(ht, csc213);
    // add(ht, csc321);

    char* path = "/Users/henryfisher/grinnell/213/CSC213-Final-Project/io/file.txt";

    // write_to_file(ht, path);
    hash_table_t* ht = read_from_file(path);

    hash_node_t* current;
    for (int i = 0; i < MAX_ARR_LENGTH; i++) {
        current = ht->table[i];
        while (current != NULL) {
            printf("%c, %s\n", current->graph_node->type, current->graph_node->val);
            current = current->next;
        }
    }
    
}

void print_prompt() {
    printf("1......Create a new entry\n");
    printf("2......Delete an entry\n");
    printf("3......List all entries\n");
    // add more
    printf("Enter your choice: \n");
}


