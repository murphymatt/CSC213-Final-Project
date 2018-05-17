In order to interact with the system, make and then run ./main from the io directory. A prompt will appear in standard output with a list of possible actions. From here, the instructions should be clear, but see the example below for a reference on what typical input should look like. When adding or deleting a node, the user must specify the node type (student or class), and the value associated with that node. 

Following an interaction with the user input prompt, all of the data in the graph is written to a file. Therefore if a user wants to view all of the contents of the databse, they may open this file. The file contents are displayed as: 

File Format: 
first line:  Type, Value; Type2, Value2; etc
subsequent: Type, Value; Type2, Value2; Type3, Value3 where Value has an edge with Value2, and Value3


Example Start

From within the io directory: 
> ./main

The following action menu will appear in standard output: 

Actions:
S......Add a student
C......Add a class
A......Add a student to a class
D......Delete a student node
E......Delete a class node
K......List classes for a student
L......List students in a class
F......See classes in common between two students
G......See students in common between two classes
Q......quit
Enter your choice:

The user can specify a command by typing the appropriate letter, the system is case sensitive. For example: 

Enter your choice: F
Enter first student name: Maddie Goldman
Enter second student name: Henry Fisher
CSC213

As seen in the file, "file.txt," CSC213 is the only class that Maddie and Henry have in common. If a user were to 
add or delete a student/class, "file.txt" would be updated when the program exits. 

Example End



