#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "hash_table.h"


void write_to_file(hash_table_t *h_table, const char *file_path);

hash_table_t* read_from_file(const char *file_path);

int get_next_name(FILE* fp, char name[MAX_STR], char* c);

/*get user input for nodes to add and delete*/ 
void get_user_input();

#endif
