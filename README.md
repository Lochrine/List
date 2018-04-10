# List
List is a program that creates a linked list of integers based on the commands from a file.  The commands are either insert or delete.  The program will respond to the commands and update the list accordingly.  Finally, the program will output the number of nodes in the list and print the sorted list to the terminal.

Included are two folders: /list and /data. /list contains the relevant C file, list.c, and a makefile. /data contains several instances of data that can be used to display the program at work.

In the data folder, there are two types of files: test files, and output files.  After compiling, the program is passed a test file containing a set of instructions.  The data that the program outputs can be checked against the associated output file for correctness.  Test files are of the form "test.X", and output files are of the form "test.X.out", with X being the ID number of the associated test.

Example: "/list test.1" from the command line will produce output equivalent to the text file "test.1.out".
