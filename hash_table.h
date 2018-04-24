/***********************************************************************
 * Name:        Henry Fisher
 * Description: Hash table header for word frequency project
 ***********************************************************************/

#ifndef __HASHTABLE_H__ 
#define __HASHTABLE_H__ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR 100
/* 3000 leads to a ~75% fill rate for the hash table */
#define MAX_ARR_LENGTH 3000

typedef struct wordNode {
  char word[MAX_STR];
  int frequency;
  struct wordNode* next;
} wordNode;

typedef struct hashTable {
  wordNode* table[MAX_ARR_LENGTH];
} hashTable;

/* Hash table essentials */
hashTable* initializeHashTable();
void add(hashTable* hash, char word[MAX_STR]);
void delete(hashTable* hash, char word[MAX_STR]);
wordNode* find(hashTable* hash, char word[MAX_STR]);
unsigned long hashFunction(char word[MAX_STR]);

#endif