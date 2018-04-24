/***********************************************************************
 * Name:        Henry Fisher
 * Description: Driver file for traversal project
 ***********************************************************************/


 #include "tree.h"

 int main() {
 	tree* t = initializeTree();
 	treeAdd(t, 1);
 	treeAdd(t, 2);
 	treeAdd(t, 3);
 	breadthFirstTraversal(t);
 }