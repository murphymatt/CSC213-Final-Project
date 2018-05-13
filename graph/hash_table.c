#include "hash_table.h"
#include "queue.h"

/* Hash table essentials */

/*
pre: none
post: hash is either a hash table, or NULL (if space couldn't be created)
*/
void hash_table_initialize(hash_table_t* ht) {
  header_node_t** table = malloc(sizeof(hash_node_t) * MAX_ARR_LENGTH);
  if (table == NULL) {
    perror("Failed to malloc hash table");
    exit(2);
  }

  // malloc space for all headers
  for (int i = 0; i < MAX_ARR_LENGTH; i++) {
    header_node_t* new_header = (header_node_t*) malloc(sizeof(header_node_t));

    // error check 
    if (new_header == NULL) {
      perror("Failed to malloc header");
      exit(2);
    } // end if 

    // set fields of header 
    new_header->hash_node = NULL;
    new_header->m = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    table[i] = new_header;
  }
  ht->table = table; 
  ht->index_list = NULL;
}

/* 
pre: hash is not NULL, word has null pointer
post: word will either be added with frequency of 1, or have had its
frequency incremented
*/
void hash_table_add(hash_table_t* ht, graph_node_t* graph_node) {  
  //malloc for new node to add 
  hash_node_t* new_node = (hash_node_t*) malloc(sizeof(hash_node_t));

  //error check, add node to graph 
  if (NULL != new_node) {
    new_node->graph_node = graph_node;
    new_node->m = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    //get hash value and associated header 
    unsigned long index = hash_function(graph_node->val);
    header_node_t* current = ht->table[index];

    // if this bucket is empty, add its index into the index_table
    if (current->hash_node == NULL)
      index_list_append(ht, index);

    //lock current header
    pthread_mutex_t m = current->m;
    pthread_mutex_lock(&m);

    new_node->next = current->hash_node;
    ht->table[index]->hash_node = new_node;

    // unlock current header
    pthread_mutex_unlock(&m);

  } else {
    perror("Failed to malloc"); 
    exit(2);
  } // end else 
}

/*
pre: hash is initialized
post: if word was in hash, it has been removed. Otherwise, nothing changes
*/
void hash_table_delete_entry(hash_table_t* ht, graph_node_t* graph_node) {

  // get hash value and associated header 
  unsigned long index = hash_function(graph_node->val);
  header_node_t* current_header = ht->table[index];
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
    }// end if 
    previous = current;
    current = current->next;
  } // end while 

  pthread_mutex_unlock(&(current_header->m));//unlock header 
}

/*
 * pre: h_table is initialized, val is not null
 * post: returns node if node exists, else returns NULL
 */
graph_node_t* hash_table_search(hash_table_t* ht, char type, const char* val) {
  // get hash value and header node
  unsigned long index = hash_function(val);
  header_node_t* header = ht->table[index];

  // node to search for 
  graph_node_t* search_node = add_node(type, val);

  // lock current header 
  pthread_mutex_t m = header->m;
  pthread_mutex_lock(&m);
  hash_node_t* node = header->hash_node;

  // loop through until find search_node 
  while (node != NULL && !_compare_node(search_node, node->graph_node)) {
    node = node->next;
  }// end while 

  free(search_node);
  pthread_mutex_unlock(&m);  // unlock header 

  return node != NULL ? node->graph_node : NULL;
}

void hash_table_set_flags(hash_table_t* ht, int n) {
  hash_node_t* current;
  for (int i = 0; i < MAX_ARR_LENGTH; i++) {
    current = ht->table[i]->hash_node;
    while (current != NULL) {
      current->graph_node->flag = n;
      current = current->next;
    } // end while
  } // end for 
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
  hash_table_initialize(ret_table);
  hash_table_add(ret_table, start);
  if (dist < 1)
    return ret_table;

  // create shared node queue to store nodes accessed by threads
  queue_t *queue = (queue_t*) malloc(sizeof(queue_t));
  queue_init(queue);
  queue_push(queue, start, 0);

  list_node_t* node = start->neighbors;
  while (node != NULL) {
    graph_node_t *g_node = node->graph_node;
    node = node->next;
    if (hash_table_search(ret_table, g_node->type, g_node->val) != NULL)
      continue;
    hash_table_add(ret_table, g_node);
    queue_push(queue, g_node, 1);
  }

  pthread_t threads[num_threads];
  bfs_pthread_args_t thread_args[num_threads];

  // each thread shares pointers to the same queue and ret table
  for (int i = 0; i < num_threads; ++i) {
    thread_args[i].node_queue = queue;
    thread_args[i].ret_table = ret_table;
    thread_args[i].dist = dist;
  }

  for (int i = 0; i < num_threads; ++i)
    pthread_create(&threads[i], NULL, bfs_pthread_fn, &thread_args[i]);

  for (int i = 0; i < num_threads; ++i)
    pthread_join(threads[i], NULL);

  return ret_table;
}

void* bfs_pthread_fn(void* args) {
  bfs_pthread_args_t* bfs_args = (bfs_pthread_args_t*) args;
  queue_t *nq = bfs_args->node_queue;
  hash_table_t* ret_table = bfs_args->ret_table;
  int dist = bfs_args->dist;

  while (!queue_empty(nq)) {
    queue_node_t* subtree_queue_node = queue_pop(nq);

    // ensure that we are still within our original designated neighborhood
    int graph_dist = subtree_queue_node->dist;
    if (graph_dist >= dist) continue;

    // take our root and iterate over neighbors
    graph_node_t* subtree_root = subtree_queue_node->g_node;
    list_node_t* node = subtree_root->neighbors;
    while (node != NULL) {
      // pull graph node and iterate
      graph_node_t *g_node = node->graph_node;
      node = node->next;
      // node is already contained in the list, skip this: O(1)
      if (hash_table_search(ret_table, g_node->type, g_node->val) != NULL)
        continue;

      hash_table_add(ret_table, g_node);
      queue_push(nq, g_node, graph_dist+1);
    }
  }

  return NULL;
}

list_node_t* get_nodes(hash_table_t *ht) {
  list_node_t* ret = NULL;
  index_node_t* ind_node = ht->index_list;
  while (ind_node != NULL) {
    int ind = ind_node->ind;
    hash_node_t* h_node = ht->table[ind]->hash_node;
    while (h_node != NULL) {
      ret = list_node_append(ret, h_node->graph_node);
      h_node = h_node->next;
    }
    ind_node = ind_node->next;
  }
  return ret;
}

list_node_t* node_intersection(list_node_t* lst1, list_node_t *lst2) {
  list_node_t* ret = NULL;
  list_node_t* cur1 = lst1;
  while (cur1 != NULL) {
    graph_node_t *cur1_gnode = cur1->graph_node;
    list_node_t* cur2 = lst2;
    while (cur2 != NULL) {
      // we have found cur1 node in the other graph, add it to the ret list
      if (_compare_node(cur1_gnode, cur2->graph_node)) {
        ret = list_node_append(ret, cur1_gnode);
        break;
      }
      cur2 = cur2->next;
    }
    cur1 = cur1->next;
  }
  return ret;
}

// assumes that the ind is not already confirmed to not be in ht
void index_list_append(hash_table_t *ht, int ind) {
  // create new node
  index_node_t *new_node;
  if ((new_node = malloc(sizeof(index_node_t))) == NULL) {
    fprintf(stderr, "Error allocating space for new_node\n");
    exit(EXIT_FAILURE);
  }
  new_node->ind = ind;
  new_node->next = ht->index_list;
  ht->index_list = new_node;
}
