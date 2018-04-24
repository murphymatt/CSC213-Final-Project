/***********************************************************************
 * Name:        Henry Fisher
 * Description: Word map main file for word frequency project
 ***********************************************************************/

#include "hash_table.h"
#include <sys/time.h>

void printTime(struct timeval start, struct timeval end);
void fillHashTable(hashTable* hash, char filename[MAX_STR]);
wordNode* maximum(hashTable* hash);
wordNode* minimum(hashTable* hash);
int getFrequency(hashTable* hash, char word[MAX_STR]);
void insertWord(hashTable* hash, char word[MAX_STR]);
void deleteWord(hashTable* hash, char word[MAX_STR]);
int slotsTaken(hashTable* hash);

int main() {
	struct timeval overall_start, start, end;
	gettimeofday(&overall_start, NULL);
	wordNode* max;
	wordNode* min;
	wordNode* found;

	/* OXFORD FILE */
	char* oxfordPath = "OxfordMedical.txt";
	gettimeofday(&start, NULL);
	hashTable* oxfordHash = initializeHashTable();
	fillHashTable(oxfordHash, oxfordPath);
	gettimeofday(&end, NULL);

	printf("\n\n%s\n\n", oxfordPath);

	printf("Hash table created and filled\n");
	printTime(start, end);

	gettimeofday(&start, NULL);
	max = maximum(oxfordHash);
	gettimeofday(&end, NULL);
	printf("Max is '%s' with %d occurrences\n", max->word, max->frequency);
	printTime(start, end);

	gettimeofday(&start, NULL);
	min = minimum(oxfordHash);
	gettimeofday(&end, NULL);
	printf("Min is '%s' with %d occurrences\n", min->word, min->frequency);
	printTime(start, end);

	gettimeofday(&start, NULL);
	found = find(oxfordHash, "aardvark");
	gettimeofday(&end, NULL);
	printf("Found '%s' with %d occurrences\n", found->word, found->frequency);
	printTime(start, end);

	printf("Inserting anteater\n");
	gettimeofday(&start, NULL);
	insertWord(oxfordHash, "anteater");
	gettimeofday(&end, NULL);
	printf("anteater inserted\n");
	printTime(start, end);

	printf("Deleting the\n");
	gettimeofday(&start, NULL);
	deleteWord(oxfordHash, "the");
	gettimeofday(&end, NULL);
	printf("the deleted\n");
	printTime(start, end);

	/* WORDS 2 FILE */
	char* words2Path = "words2.txt";
	gettimeofday(&start, NULL);
	hashTable* words2Hash = initializeHashTable();
	fillHashTable(words2Hash, words2Path);
	gettimeofday(&end, NULL);

	printf("\n\n%s\n\n", words2Path);

	printf("Hash table created and filled\n");
	printTime(start, end);

	gettimeofday(&start, NULL);
	max = maximum(words2Hash);
	gettimeofday(&end, NULL);
	printf("Max is '%s' with %d occurrences\n", max->word, max->frequency);
	printTime(start, end);

	gettimeofday(&start, NULL);
	min = minimum(words2Hash);
	gettimeofday(&end, NULL);
	printf("Min is '%s' with %d occurrences\n", min->word, min->frequency);
	printTime(start, end);

	gettimeofday(&start, NULL);
	found = find(words2Hash, "likely");
	gettimeofday(&end, NULL);
	printf("Found '%s' with %d occurrences\n", found->word, found->frequency);
	printTime(start, end);

	printf("Inserting hyphenation\n");
	gettimeofday(&start, NULL);
	insertWord(words2Hash, "hyphenation");
	gettimeofday(&end, NULL);
	printf("hyphenation inserted\n");
	printTime(start, end);

	printf("Deleting brisk\n");
	gettimeofday(&start, NULL);
	deleteWord(words2Hash, "brisk");
	gettimeofday(&end, NULL);
	printf("brisk deleted\n");
	printTime(start, end);


	/* HOLMES FILE */
	char* holmesPath = "Holmes.txt";
	gettimeofday(&start, NULL);
	hashTable* holmesHash = initializeHashTable();
	fillHashTable(holmesHash, holmesPath);
	gettimeofday(&end, NULL);

	printf("\n\n%s\n\n", holmesPath);

	printf("Hash table created and filled\n");
	printTime(start, end);

	gettimeofday(&start, NULL);
	max = maximum(holmesHash);
	gettimeofday(&end, NULL);
	printf("Max is '%s' with %d occurrences\n", max->word, max->frequency);
	printTime(start, end);

	gettimeofday(&start, NULL);
	min = minimum(holmesHash);
	gettimeofday(&end, NULL);
	printf("Min is '%s' with %d occurrences\n", min->word, min->frequency);
	printTime(start, end);

	gettimeofday(&start, NULL);
	found = find(holmesHash, "holmes");
	gettimeofday(&end, NULL);
	printf("Found '%s' with %d occurrences\n", found->word, found->frequency);
	printTime(start, end);

	printf("Inserting wumpus\n");
	gettimeofday(&start, NULL);
	insertWord(holmesHash, "wumpus");
	gettimeofday(&end, NULL);
	printf("wumpus inserted\n");
	printTime(start, end);

	printf("Deleting king\n");
	gettimeofday(&start, NULL);
	deleteWord(holmesHash, "king");
	gettimeofday(&end, NULL);
	printf("king deleted\n");
	printTime(start, end);

	gettimeofday(&end, NULL);
	printf("TOTAL:\n");
	printTime(overall_start, end);
	return 0;
}

void printTime(struct timeval start, struct timeval end) {
	int u_sec = ((end.tv_sec - start.tv_sec)*1000000L
           +end.tv_usec) - start.tv_usec;
	printf("Time in microseconds: %d microseconds ", u_sec);
	printf("(%f ms)\n\n", u_sec/1000.0);
}

/* 
pre: hash is not NULL
post: hash table will be filled with wordNodes corresponding to the words in
filename
*/
void fillHashTable(hashTable* hash, char filename[MAX_STR]) {
	FILE* fp = fopen(filename, "r");
	if (NULL == fp) {
		fprintf(stderr, "Can't open file: %s\n", filename);
		exit(1);
	}

	int notEOF = 1;
	while (notEOF) {
		/* Fill word with next word */
		char word[MAX_STR];
		int i = 0;
		while (!isspace(word[i] = tolower(fgetc(fp)))) {
			if (word[i] == EOF) {
				notEOF = 0;
				break;
			}
			i++;
		}
		word[i] = '\0';
		if (0 != i) {
			/* Add word to hash table */
			// printf("%s\n", word);
			insertWord(hash, word);
		}
		
	}
}

/* 
pre: hash is not NULL
post: we return frequency of word in hash
*/
int getFrequency(hashTable* hash, char word[MAX_STR]) {
	wordNode* node = find(hash, word);
	if (NULL != node) {
		return node->frequency;
	} else {
		return 0;
	}
}

/* 
pre: hash is not NULL, word has null pointer
post: word was added to hash
*/
void insertWord(hashTable* hash, char word[MAX_STR]) {
	add(hash, word);
}

/* 
pre: hash is not NULL, word has null pointer
post: word was removed from hash
*/
void deleteWord(hashTable* hash, char word[MAX_STR]) {
	delete(hash, word);
}

/* testing purposes only */
int slotsTaken(hashTable* hash) {
	int c = 0;
	int i;
	for (i = 0; i < MAX_ARR_LENGTH; i++) {
		if (NULL != hash->table[i])
			c++;
	}
	return c;
}

/* 
pre: hash is not NULL
post: we return word that occurs the most in hash
*/
wordNode* maximum(hashTable* hash) {
	wordNode* maxNode = NULL;
	int i;
	for (i = 0; i < MAX_ARR_LENGTH; i++) {
		wordNode* node = hash->table[i];

		while (NULL != node) {
			if (NULL == maxNode) {
				maxNode = node;
			} else {
				if (node->frequency > maxNode->frequency) {
					maxNode = node;
				}
			}
			node = node->next;
		}

	}
	return maxNode;
}

/* 
pre: hash is not NULL
post: we return word that occurs the least in hash
*/
wordNode* minimum(hashTable* hash) {
	wordNode* minNode = NULL;
	int i;
	for (i = 0; i < MAX_ARR_LENGTH; i++) {
		wordNode* node = hash->table[i];

		while (NULL != node) {
			if (NULL == minNode) {
				minNode = node;
			} else {
				if (node->frequency < minNode->frequency) {
					minNode = node;
				}
			}
			node = node->next;
		}
		if (NULL != minNode && 1 == minNode->frequency) {
			break;
		}
	}
	return minNode;
}

