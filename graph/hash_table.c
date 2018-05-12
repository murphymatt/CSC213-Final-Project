#include "hash_table.h"
#include "queue.h"

void initialize_hash_table(hash_table_t* hash_table) {
  header_node_t** table = malloc(sizeof(hash_node_t) * MAX_ARR_LENGTH);
  if (table == NULL) {
    perror("Failed to malloc hash table");
    exit(2);
  }

  //malloc space for all headers
  for (int i = 0; i < MAX_ARR_LENGTH; i++) {
    header_node_t* new_header = (header_node_t*) malloc(sizeof(header_node_t));
    
    //error check 
    if (new_header == NULL) {
      perror("Failed to malloc header");
      exit(2);
    } //end if 

    //set fields of header 
    new_header->hash_node = NULL;
    new_header->m = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    table[i] = new_header;
  }//end for 
  hash_table->table = table; 
}


void add(hash_table_t* hash, graph_node_t* graph_node) {
  
  //malloc for new node to add 
  hash_node_t* new_node = (hash_node_t*) malloc(sizeof(hash_node_t));
  
  //error check, add node to graph 
  if (NULL != new_node) {
    new_node->graph_node = graph_node;
    new_node->m = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    //get hash value 
    unsigned long index = hash_function(graph_node->val);
    header_node_t* current = hash->table[index];

    //lock current header
    pthread_mutex_t m = current->m;
    pthread_mutex_lock(&m);

    new_node->next = current->hash_node;
    hash->table[index]->hash_node = new_node;

    //unlock current header
    pthread_mutex_unlock(&m);

  } else {
    perror("Failed to malloc"); 
    exit(2);
  } //end else 
}

void delete_hash_node(hash_table_t* hash, graph_node_t* graph_node) {
  
  //get hash value and associated header 
  unsigned long index = hash_function(graph_node->val);
  header_node_t* current_header = hash->table[index];
  
  //lock current header node 
  pthread_mutex_lock(&(current_header->m));
  hash_node_t* current = current_header->hash_node;

  hash_node_t* previous = NULL;
  while (NULL != current) {
    if (graph_node == current->graph_node) {
      /* First element? */
      if (NULL == previous) {
        current_header->hash_node = current->next;
      } else {
        previous->next = current->next;
      } //end else
      free(current);
      pthread_mutex_unlock(&(current_header->m));
      return;
    }//end if 
    previous = current;
    current = current->next;
  } //end while 
  
  pthread_mutex_unlock(&(current_header->m));//unlock header 
}


graph_node_t* search_table(hash_table_t* h_table, char type, const char* val) {
  
  //get hash value and header node
  unsigned long index = hash_function(val);
  header_node_t* header = h_table->table[index];

  //node to search for 
  graph_node_t* search_node = add_node(type, val);
  

  //lock current header 
  pthread_mutex_t m = header->m;
  pthread_mutex_lock(&m);
  hash_node_t* node = header->hash_node;

  //loop through until find search_node 
  while (node != NULL && !_compare_node(search_node, node->graph_node)) {
  	node = node->next;
  }//end while 
  
  free(search_node);
  pthread_mutex_unlock(&m);//unlock header 

  return node != NULL ? node->graph_node : NULL;

}

void set_flags(hash_table_t* ht, int n) {
  hash_node_t* current;
  for (int i = 0; i < MAX_ARR_LENGTH; i++) {
    current = ht->table[i]->hash_node;
    while (current != NULL) {
        current->graph_node->flag = n;
        current = current->next;
    }//end while
  }//end for 
}

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
