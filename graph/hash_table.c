/***********************************************************************
 * Name:        Henry Fisher
 * Description: Hash table implementation for word frequency project
 ***********************************************************************/

#include "hash_table.h"

#define MAX_ARR_LENGTH 256

/* Hash table essentials */

/*
pre: none
post: hash is either a hash table, or NULL (if space couldn't be created)
*/
hash_table_t* initialize_hash_table() {
	hash_table_t* hash = malloc(sizeof(hash_table_t));
	if (NULL != hash) {
    hash_node_t* table[MAX_ARR_LENGTH] = { NULL };
		hash->table = table; 
		return hash;
	} else {
		return NULL;
	}
}

/* 
pre: hash is not NULL, word has null pointer
post: word will either be added with frequency of 1, or have had its
frequency incremented
*/
void add(hash_table_t* hash, graph_node_t* graph_node) {
	hash_node_t* new_node = (hash_node_t*) malloc(sizeof(hash_node_t));
	if (NULL != new_node) {
		new_node->graph_node = graph_node;

		unsigned long index = hash_function(graph_node->val);
		hash_node_t* current = hash->table[index];

		new_node->next = current;
		hash->table[index] = new_node;

	} else {
		perror("Failed to malloc");
		exit(2);
	}
}

/*
pre: hash is initialized
post: if word was in hash, it has been removed. Otherwise, nothing changes
*/
void delete_hash_node(hash_table_t* hash, graph_node_t* graph_node) {
	unsigned long index = hash_function(graph_node->val);
	hash_node_t* current = hash->table[index];
	
	hash_node_t* previous = NULL;
	while (NULL != current) {
		if (graph_node == current->graph_node) {
			/* First element? */
			if (NULL == previous) {
				hash->table[index] = current->next;
			} else {
				previous->next = current->next;
			}
			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}
}

/*
 * pre: h_table is initialized, val is not null
 * post: returns node if node exists, else returns NULL
 */
graph_node_t* search_table(hash_table_t* h_table, char type, const char* val) {
  unsigned long hash = hash_function(val);
  graph_node_t* search_node = add_node(type, val);
  hash_node_t* node = h_table->table[hash];
  while (node != NULL) {
  	if (!compare_node(search_node, node->graph_node))
  		node = node->next;
  }

  return node != NULL ? node->graph_node : NULL;
}

/*
pre: none
post: returns int specific to word
citation: http://www.cse.yorku.ca/~oz/hash.html
*/
unsigned long hash_function(const char* word) {
	unsigned long hash = 5381;
  
  int i;
  for (i = 0; i < MAX_STR; i++) {
  	if (0 == word[i])
  		break;
    hash = ((hash << 5) + hash) + (word[i]);
	}
	hash = hash % MAX_ARR_LENGTH;
  return hash;
}

