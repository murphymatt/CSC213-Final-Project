#include "io.h"

/*
 * out_file is a E+1 line file where E is edges in graph
 * first line:  Type, Value; Type2, Value2; etc
 * subsequent: Type, Value; Type2, Value2 where Value and Value2 have an edge
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
            fprintf(fp, "%c, %s;", current->graph_node->type, current->graph_node->val);
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

    // continuously read new entries
    char type;
    while (true) {
        // get type
        type = fgetc(fp);
        if (type == EOF) {
            break;
        } else if (type != 'C' && type != 'S') {
            printf("Bad input file format (didn't find C or S)\n");
            exit(1);
        }

        // flush the comma and space
        char c = fgetc(fp);
        c = fgetc(fp);

        // get name
        char name[MAX_STR];
        int str_len = 0;
        while ((name[str_len] = fgetc(fp)) != ';') {
            if (name[str_len] == EOF) {
                printf("Bad input file format (found EOF in name)\n");
                exit(1);
            }
            str_len++;
        }
        name[str_len] = '\0';

        graph_node_t* new_node = add_node(type, name);
        add(ht, new_node);
    }
    return ht;
}

int main() {
    while (true) {
        print_prompt();
    }
    // graph_node_t* maddie = add_node('S', "Maddie Goldman");
    // graph_node_t* henry = add_node('S', "Henry Fisher");
    // graph_node_t* matt = add_node('S', "Matt Murphy");
    // graph_node_t* csc213 = add_node('C', "CSC213");

    // hash_table_t* ht = (hash_table_t*) malloc(sizeof(hash_table_t));
    // initialize_hash_table(ht);

    // add(ht, maddie);
    // add(ht, henry);
    // add(ht, matt);
    // add(ht, csc213);

    // char* path = "/Users/henryfisher/grinnell/213/CSC213-Final-Project/io/file.txt";

    // write_to_file(ht, path);
    // hash_table_t* ht = read_from_file(path);

    // hash_node_t* current;
    // for (int i = 0; i < MAX_ARR_LENGTH; i++) {
    //     current = ht->table[i];
    //     while (current != NULL) {
    //         printf("%c, %s\n", current->graph_node->type, current->graph_node->val);
    //         current = current->next;
    //     }
    // }
    
}

void print_prompt() {
    printf("1......Create a new entry\n");
    printf("2......Delete an entry\n");
    printf("3......List all entries\n");
    printf("Enter a number: \n");
}


