/***********************************************************************
 * Name:        Henry Fisher
 * Description: Hash table implementation for word frequency project
 ***********************************************************************/

#include "hash_table.h"

/* Hash table essentials */

/*
pre: none
post: hash is either a hash table, or NULL (if space couldn't be created)
*/
hashTable* initializeHashTable() {
	hashTable* hash = malloc(sizeof(hashTable));
	if (NULL != hash) {
		int i;
		for (i = 0; i < MAX_ARR_LENGTH; i++) {
			hash->table[i] = NULL;
		}
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
void add(hashTable* hash, char word[MAX_STR]) {
	wordNode* node = find(hash, word);
	/* If we've already added the word, just incremement frequency */
	if (NULL != node) {
		(node->frequency)++;
	} else {
		/* Otherwise, make a new node */
		wordNode* newNode = malloc(sizeof(wordNode));
		if (NULL != newNode) {
			strcpy(newNode->word, word);
			newNode->frequency = 1;
			newNode->next = NULL;

			unsigned long index = hashFunction(word);
			wordNode* cur = hash->table[index];
			/* Collsion handling */
			if (NULL != cur) {
				while (NULL != cur->next) {
					cur = cur->next;
				}
				cur->next = newNode;
			} else {
				hash->table[index] = newNode;
			}
		} else {
			printf("Failed to make space for new node!\n");
		}
	}
}

/*
pre: hash is initialized
post: if word was in hash, it has been removed. Otherwise, nothing changes
*/
void delete(hashTable* hash, char word[MAX_STR]) {
	unsigned long index = hashFunction(word);
	wordNode* cur = hash->table[index];
	
	wordNode* prev = NULL;
	while (NULL != cur) {
		if (strcmp(cur->word, word) == 0) {
			/* First element? */
			if (NULL == prev) {
				hash->table[index] = cur->next;
			} else {
				prev->next = cur->next;
			}
			free(cur);
			return;
		}
		prev = cur;
		cur = cur->next;
	}
}

/*
pre: hash is initialized
post: return wordNode containing word, or NULL if it doesn't exist
*/
wordNode* find(hashTable* hash, char word[MAX_STR]) {
	unsigned long index = hashFunction(word);
	wordNode* cur = hash->table[index];

	while (NULL != cur) {
		if (strcmp(cur->word, word) == 0) {
			break;
		}
		cur = cur->next;
	}
	return cur;
}

/*
pre: none
post: returns int specific to word
citation: http://www.cse.yorku.ca/~oz/hash.html
*/
unsigned long hashFunction(char word[MAX_STR]) {
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

