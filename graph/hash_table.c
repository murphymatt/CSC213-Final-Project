/***********************************************************************
 * Name:        Henry Fisher
 * Description: Hash table implementation for word frequency project
 ***********************************************************************/

#include "hash_table.h"
#include "queue.h"

/* Hash table essentials */

/*
pre: none
post: hash is either a hash table, or NULL (if space couldn't be created)
*/
void initialize_hash_table(hash_table_t* hash_table) {
  hash_node_t** table = malloc(sizeof(hash_node_t) * MAX_ARR_LENGTH);
  if (table == NULL) {
    perror("Failed to malloc hash table");
    exit(2);
  }
  for (int i = 0; i < MAX_ARR_LENGTH; i++) {
    header_node_t* new_header = (header_node_t*) malloc(sizeof(header_node_t));
    if (new_header == NULL) {
      perror("Failed to malloc header");
      exit(2);
    }
    new_header->hash_node = NULL;
    new_header->m = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    table[i] = new_header;
  }
  hash_table->table = table; 
}

/* 
pre: hash is not NULL, word has null pointer
post: word will either be added with frequency of 1, or have had its
frequency incremented
*/
void add(hash_table_t* hash, graph_node_t* graph_node) {
  
  //malloc for new node to add 
  hash_node_t* new_node = (hash_node_t*) malloc(sizeof(hash_node_t));
  
  //error check, add node to graph 
  if (NULL != new_node) {
    new_node->graph_node = graph_node;
    new_node->m = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    unsigned long index = hash_function(graph_node->val);
    header_node_t* current = hash->table[index];

    //lock current header
    pthread_mutex_t m = current->m;
    pthread_mutex_lock(&m); 

    new_node->next = current->hash_node;
    hash->table[index]->hash_node = new_node;

  } else {
    perror("Failed to malloc"); 
    exit(2);
  } //end else 
 
  //unlock current header
  pthread_mutex_unlock(&m); 
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
  header_node_t* header = h_table->table[hash];

  pthread_mutex_t m = header->m;
  pthread_mutex_lock(&m);
  hash_node_t* node = header->hash_node;


  while (node != NULL && !_compare_node(search_node, node->graph_node)) {
  	node = node->next;
  }
  free(search_node);
  pthread_mutex_unlock(&m);

  return node != NULL ? node->graph_node : NULL;

}

void set_flags(hash_table_t* ht, int n) {
  hash_node_t* current;
  for (int i = 0; i < MAX_ARR_LENGTH; i++) {
    current = ht->table[i];
    while (current != NULL) {
        current->graph_node->flag = n;
        current = current->next;
    }
  }
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
    if (0 == word[i]) break;
    hash = ((hash << 5) + hash) + (word[i]);
  }
  hash = hash % MAX_ARR_LENGTH;
  return hash;
}

hash_table_t* bfs(graph_node_t* start, int dist, int num_threads) {
  hash_table_t* ret_table = malloc(sizeof(hash_table_t));
  initialize_hash_table(ret_table);
  if (dist == 0) {
    add(ret_table, start);
    return ret_table;
  }

  num_threads=4;
  pthread_t threads[num_threads];

  // _bfs_helper(ret_table, start, dist);

  // create shared node queue to store nodes accessed by threads
  queue_t *queue = (queue_t*) malloc(sizeof(queue_t));
  queue_init(queue);
  queue_push(queue, start, 0);

  // start search
  while (!queue_empty(queue)) {
    queue_node_t* subtree_queue_node = queue_pop(queue);

    // ensure that we are still within our original designated neighborhood
    int graph_dist = subtree_queue_node->dist;
    if (graph_dist > dist) continue;

    // take our root and iterate over neighbors
    graph_node_t* subtree_root = subtree_queue_node->g_node;
    list_node_t* node = subtree_root->neighbors;
    while (node != NULL) {
      // pull graph node and iterate
      graph_node_t *g_node = node->graph_node;
      node = node->next;
      // node is already contained in the list, skip this: O(1)
      if (search_table(ret_table, g_node->type, g_node->val) != NULL) continue;
      add(ret_table, g_node);
      queue_push(queue, g_node, graph_dist+1);
    }
  }
   
  return ret_table;
}

/*
 * pre: ret_table starts empty at beginning of search
 * post: ret_table contains all nodes in graph within distance from start
 */
void _bfs_helper(hash_table_t* ret_table, graph_node_t* start, int dist) {
  if (dist <= 0) return;

  list_node_t* node = start->neighbors;
  while (node != NULL) {
    graph_node_t* g_node = node->graph_node;
    // node is already contained in search list... skip this
    if (search_table(ret_table, g_node->type, g_node->val) != NULL) continue;
    add(ret_table, g_node);
    _bfs_helper(ret_table, g_node, dist-1);
    node = node->next;
  }
}
