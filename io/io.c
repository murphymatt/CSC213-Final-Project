#include "io.h"

#define THREADS 4

/*
 * out_file is a E+1 line file where E is edges in graph
 * first line:  Type, Value; Type2, Value2; etc
 * subsequent: Type, Value; Type2, Value2; Type3, Value3 
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
        current = ht->table[i]->hash_node;
        while (current != NULL) {
            fprintf(fp, "%c, %s; ", current->graph_node->type, current->graph_node->val);
            current = current->next;
        }
    }

    hash_table_set_flags(ht, 0);
    for (int i = 0; i < MAX_ARR_LENGTH; i++) {
        current = ht->table[i]->hash_node;
        while (current != NULL) {
            fprintf(fp, "\n");
            current->graph_node->flag = 1;
            fprintf(fp, "%c, %s; ", current->graph_node->type, current->graph_node->val);

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
    hash_table_initialize(ht);

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
        printf("%c, %s\n", type, name);
        graph_node_t* new_node = graph_add(type, name);
        hash_table_add(ht, new_node);
        // get rid of the next space
        char nothing = fgetc(fp);
    }

    // adding relationships
    while (true) {
        // get name/type of node1
        char node1_type;
        char node1_name[MAX_STR];
        int ret = get_next_name(fp, node1_name, &node1_type);
        if (ret == EOF)
            break;
        printf("%c, %s\n", node1_type, node1_name);
        graph_node_t* node1 = hash_table_search(ht, node1_type, node1_name);
        // get rid of the next space
        char nothing = fgetc(fp);

        while (true) {
            
            // get name/type of node2
            char node2_type;
            char node2_name[MAX_STR];
            int ret = get_next_name(fp, node2_name, &node2_type);
            if (ret == EOF || ret == '\n')
                break;
            printf("%c, %s\n", node2_type, node2_name);
            graph_node_t* node2 = hash_table_search(ht, node2_type, node2_name);
            add_node_neighbor(node1, node2);
            // get rid of the next space
            nothing = fgetc(fp);
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

    //read hash table contents from existing file
    hash_table_t* ht = read_from_file("file_name.txt"); 

    //user can add to the hash table
    get_user_input(ht);
    write_to_file(ht, "/output.txt");


    return 0;

    
}

void get_user_input(hash_table_t* ht) {
        
  char action;
  char node_type;
  char student_name[MAX_STR];
  char class_name[MAX_STR]; 
  graph_node_t* new_student;
  graph_node_t* new_class;
  graph_node_t* sad_node; 
  graph_node_t* search_node; 
  list_node_t* current;
  hash_table_t* intersection;
  char move; 
  int done = 0;
  int exit = 0; 

  while(!exit) {
    printf("Actions:\n"); 
    printf("S......Add a student\n");
    printf("C......Add a class\n"); 
    printf("D......Delete a student node\n"); 
    printf("E......Delete a class node\n"); 
    printf("K......List classes for a student\n"); 
    printf("L......List students in a class\n");
    printf("A......See classes in common between two students\n");
    printf("B......See students in common between two classes\n");
    printf("Q......quit\n"); 
    // add more

  
    printf("Enter your choice: ");
    action = getchar();
    printf("%d\n", action);

    switch(action) {
    case 'S':
      printf("Enter the student name: ");
      scanf("%s", student_name);

      new_student = graph_add('S', student_name);
      while(!done) {
        printf("Add classes (q when done): ");
        scanf("%s", class_name);
        if(strcmp(class_name, "q") == 0) {
          done = 1;
          move = getchar();
          continue;
        }//end if
        new_class = graph_add('C', class_name);
        add_node_neighbor(new_student, new_class); 
      }//end while
                  
      hash_table_add(ht, new_student);//add node to hash
      break; 
      //continue; 

    case 'C': 
      printf("Enter the class name:");
      scanf("%s", class_name);

      new_class = graph_add('C', class_name);
      while(!done) {
          printf("Add students (q when done):");
          scanf("%s", student_name);
          if(strcmp(student_name, "q") == 0) {
              done = 1;
              move=getchar();
              continue;
          }//end if
          new_student = graph_add('S', student_name);
          add_node_neighbor(new_class, new_student); 
      }//end while

      hash_table_add(ht, new_class);
      break; 

    case 'D':
      printf("Enter name to delete: ");
      scanf("%s", student_name);
      sad_node = hash_table_search(ht, 'S', student_name);

      //error check
      if(sad_node == NULL) {
        printf("%s doesn't exist in the graph\n", student_name);
        break;
      }
      

      graph_delete(sad_node); 
      move = getchar(); 
      break;
    
    case 'E': 
      printf("Enter class to delete:"); 
      scanf("%s", class_name); 
      sad_node = hash_table_search(ht, 'C', class_name); 

      //error check 
      if(sad_node == NULL) {
          printf("%s doesn't exist in the graph\n", class_name);
          break;
      }

      graph_delete(sad_node); 
      move = getchar(); 
      break;

    case 'K':
      printf("Enter student name: ");
      scanf("%s", student_name); 
      graph_node_t* student = hash_table_search(ht, 'S', student_name);
      if (student == NULL) {
        printf("That student doesn't exist\n");
        break;
      }
      current = student->neighbors;
      while(current != NULL) {
        printf("%s\n", current->graph_node->val);
        current = current->next;
      }
      break;

    case 'L':
      printf("Enter class name: ");
      scanf("%s", class_name); 
      graph_node_t* class = hash_table_search(ht, 'C', class_name);
      if (class == NULL) {
        printf("That class doesn't exist\n");
        break;
      }
      current = class->neighbors;
      while(current != NULL) {
        printf("%s\n", current->graph_node->val);
        current = current->next;
      }
      break;

    case 'A':
      printf("Enter first student name: ");
      char student1_str[MAX_STR];
      scanf("%s", student1_str);
      printf("Enter second student name: ");
      char student2_str[MAX_STR];
      scanf("%s", student2_str);

      graph_node_t* student1 = hash_table_search(ht, 'S', student1_str);
      if (student1 == NULL) {
        printf("That student doesn't exist\n");
        break;
      }

      graph_node_t* student2 = hash_table_search(ht, 'S', student2_str);
      if (student2 == NULL) {
        printf("That student doesn't exist\n");
        break;
      }

      intersection = 
        hash_table_intersection(bfs(student1, 1, THREADS), bfs(student2, 1, THREADS));
      current = get_nodes(intersection);
      while (current != NULL) {
        printf("%s\n", current->graph_node->val);
      }
      break;

    case 'B':
      printf("Enter first class name: ");
      char class1_str[MAX_STR];
      scanf("%s", class1_str);
      printf("Enter second class name: ");
      char class2_str[MAX_STR];
      scanf("%s", class2_str);

      graph_node_t* class1 = hash_table_search(ht, 'C', class1_str);
      if (class1 == NULL) {
        printf("That class doesn't exist\n");
        break;
      }

      graph_node_t* class2 = hash_table_search(ht, 'C', class2_str);
      if (class2 == NULL) {
        printf("That class doesn't exist\n");
        break;
      }

      intersection = 
        hash_table_intersection(bfs(class1, 1, THREADS), bfs(class2, 1, THREADS));
      current = get_nodes(intersection);
      while (current != NULL) {
        printf("%s\n", current->graph_node->val);
      }
      break;
      
    case 'Q':
      printf("Bye!\n");
      exit = 1;
      break;
      
    }//end switch

  }//end while
}


