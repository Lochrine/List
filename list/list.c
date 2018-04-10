/************************************************
Michael McLaughlin
Comp Arch
Professor Menendez
Project 1.4 - List
List is a program that reads in a bunch of commands from a file, and creates a linked list
based on those commands.  The commands are insert and delete, and the program creates and
updates the linked list as necessary.
************************************************/

//TODO: write function to clear list after printing

#include <stdio.h>
#include <stdlib.h>

typedef struct linkNode
{
	int item;
	struct linkNode *next;
} Node;

void printList(Node*);
int searchList(Node*, int);
int sumList(Node*);
void insert(Node*, int);
int delete(Node**, int);
int pop(Node**);
void sortList(Node*);
void change(Node*, Node*);
void clearList(Node**);

int main(int argc, char **argv)
{
	FILE *filePoint;
	char nextChar;
	//Open the file.  Return error if file does not exist
	filePoint = fopen(argv[1], "r");
	if (filePoint == NULL)
	{
		printf("error\n");
		return 0;
	}
	//Check and return a zero for an empty list if file is empty
	nextChar = fgetc(filePoint);
	if (nextChar == EOF)
	{
		printf("0\n\n");
		return 0;
	}
	//Count number of lines in file to use later
	int lineCount = 0;
	while (nextChar != EOF)
	{
		if (nextChar == '\n') {lineCount++;}
		nextChar = fgetc(filePoint);
	}
	rewind(filePoint);
	//Sets up space for an array of strings and saves instructions from file
	char instructions[lineCount + 1][15];
	int i = 0;
	while (fgets(instructions[i], 15, filePoint) != NULL)
	{
		i++;
	}
	//Declaring variables to get this party started
	char *startPtr;
	char *endPtr;
	long number;
	int deleteIndex;
	int checkForI = 0;
	Node *head = malloc(sizeof(Node));
	
	//Checks for first instruction, and iterates until an 'insert' is found
	if (instructions[checkForI][0] == 'd')
	{
		while (instructions[checkForI][0] == 'd')
		{
			checkForI++;
		}
	}
	if (instructions[checkForI][0] == 'i')
	{
		startPtr = instructions[checkForI];	//Pulls instruction, then
		startPtr += 2;				//moves pointer to int in the instruction
		number = strtol(startPtr, &endPtr, 10);	//Converts item to useable number
		head->item = (int)number;		//Sets first item in list
	}
	//Iterates through remaining instructions, and inserts or deletes from the list as
	//necessary
	int j;
	for (j = 1; j < lineCount; j++)
	{
		if (instructions[j][0] == 'i')
		{
			startPtr = instructions[j];
			startPtr += 2;
			number = strtol(startPtr, &endPtr, 10);
			insert(head, (int)number);
		}
		if (instructions[j][0] == 'd')
		{
			startPtr = instructions[j];
			startPtr += 2;
			number = strtol(startPtr, &endPtr, 10);
			deleteIndex = searchList(head, (int)number);	//Search for index 										//before deleting
			if (deleteIndex >= 0) {delete(&head, deleteIndex);}
		}
	}
	//Sort list, then print the total number of nodes followed by the sorted list
	sortList(head);
	printf("%d\n", sumList(head));
	printList(head);
	Node **headPointer = &head;
	clearList(headPointer);
	fclose(filePoint);
	return 0;
}

//Function printList() accepts a pointer of type Node, and iterates through the linked list
//while printing each value.  Type void - does not return an argument.
void printList(Node *first)
{
	if (first == NULL) {printf("0\n\n"); return;}
	Node *thisNode = first;
	while (thisNode != NULL)
	{
		printf("%d ", thisNode->item);
		thisNode = thisNode->next;
	}
	printf("\n");
}

//Function searchList() accepts two arguments: a pointer of type node, and an integer value
//that is to be searched for.  Iterates through the list to find the index of the desired node,
//then returns that index as an integer, or -1 if not found.
int searchList(Node *first, int target)
{
	int found = -1;
	int i = 0;
	Node *thisNode = first;
	while (thisNode != NULL)
	{
		if (thisNode->item == target) {found = i;}
		thisNode = thisNode->next;
		i++;
	}
	return found;
}

//Function sumList() accepts a pointer of type Node, and adds up the number of elements in
//the linked list.  It iterates through the list while keeping a count, then returns the count
//as an integer argument.
int sumList(Node *first)
{
	int i = 0;
	Node *thisNode = first;
	if (thisNode == NULL) {return i;}
	while (thisNode != NULL)
	{
		thisNode = thisNode->next;
		i++;
	}
	return i;
}

//Function insert() accepts two arguments: a pointer of type node, and an integer value.  This
//function creates a new node, and appends that node onto the end of the linked list.  The node
//contains the integer that is passed when the function is called.  Void - does not return an
//argument.  If the list contains the integer value already, then insert does nothing.
void insert(Node *first, int item)
{
	if (searchList(first, item) >= 0) {return;} //Check for value
	Node *thisNode = first;
	while (thisNode->next != NULL)
	{
		thisNode = thisNode->next;
	}

	thisNode->next = malloc(sizeof(Node));
	thisNode->next->item = item;
	thisNode->next->next = NULL;
}

//Function delete() takes in the address of a pointer of type Node, and an integer.  The function
//searches the linked list using the integer passed as the desired index, deletes the node, and
//updates the connections accordingly.  Returns an integer: -1 if nothing is deleted, and
//the value of the deleted item if the deletion was successful.
int delete(Node **first, int index)
{
	int i;
	int itemReturn = -1;
	Node *thisNode = *first;
	Node *temporary = NULL;	//Temporary node to hold spot during deletion

	if (index == 0)
	{
		return pop(first); //If item is at the front, simple removal
	}

	//If item is not at the front, iterate to the node directly before the desired item,
	//so that we can adjust links properly
	for (i = 0; i < index - 1; i++)
	{
		if (thisNode->next == NULL)
		{
			return -1;
		}
		thisNode = thisNode->next;
	}
	temporary = thisNode->next; //Point to node being removed
	itemReturn = temporary->item; //Pull value out of that node
	thisNode->next = temporary->next; //Move 'next' pointer from the previous node to the
					  //node located directly after the deleted node
	free(temporary); //See ya later alligator

	return itemReturn;
}

//Function pop() accepts the address of a pointer of type Node as an argument.  Pop is a simpler
//form of delete() that can be used when the element is at the front of the list.  Returns the
//value deleted if successful, or a -1 otherwise.
int pop(Node **first)
{
	int itemReturn = -1;
	Node *nextNode = NULL;

	if (*first == NULL)
	{
		return -1;
	}

	nextNode = (*first)->next;
	itemReturn = (*first)->item;
	free(*first);
	*first = nextNode;

	return itemReturn;
}

//Function sortList() accepts a pointer of type Node as its argument.  sortList uses a bubble
//sort to iterate through the list and switch values as necessary.  Uses the change() function
//as an assist.  Sorting is ordered least to greatest.  Void - does not return anything.
void sortList(Node *first)
{
	int changed;
	Node *leftNodePtr = first;
	Node *rightNodePtr = NULL; //Sets up pointer for the right end of the list (NULL
				   //at beginning of search)
	if (leftNodePtr == NULL) {return;}
	do
	{
		changed = 0;
		leftNodePtr = first; //Sets up pointer to beginning of list - left end
		while (leftNodePtr->next != rightNodePtr)
		{
			//Left pointer traverses list towards the right, comparing values along
			//the way.  Updates counter 'changed' if value switch has taken place
			if (leftNodePtr->item > leftNodePtr->next->item)
			{
				//Call change() function to switch the two values
				change(leftNodePtr, leftNodePtr->next);
				changed = 1;
			}
			leftNodePtr = leftNodePtr->next;
		}
		rightNodePtr = leftNodePtr; //Moves the right-hand pointer up
	}
	while (changed); //Repeats process until list is traversed with no changes made
}

//Function change() accepts two different pointers of type Node as its arguments.  Simply
//switches out the two values located at the nodes that are passed.  Void - does
//not return anything.
void change(Node *x, Node *y)
{
    int item = x->item;
    x->item = y->item;
    y->item = item;
}

//Function clearList() iterates through the entire linked list, freeing up any memory that
//was allocated during the creation.  Void - returns nothing.
void clearList(Node **first)
{
	Node *thisNode = *first;
	Node *nextNode;
	while (thisNode != NULL)
	{
		nextNode = thisNode->next;
		free(thisNode);
		thisNode = nextNode;
	}
	*first = NULL;
}
