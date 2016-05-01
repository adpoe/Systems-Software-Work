/*
   LAB#4

   FILL IN AND TEST ALL THESE FUNCTIONS PROTOTYPED BELOW
   SEVERAL ARE ALREADY FILED IN such as insertAtFront() and printList(), fatal, etc.

RUBRIK:

x --> works :: worth 15% searchNode
x --> works, uses insert at front :: worth 20% insertAtTai  - MUST RE_USE insertAtFront
x --> works:  worth 10% removeAtFront
x --> works, uses removeAtFront:: worth 40% removeNode    - MUST RE_USE removeAtFront
x --> works, uses removeAtFront:: worth 15% freeList      - MUST RE_USE removeAtFront

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Node
{
  char  *word;
  struct Node  *next;
} Node;


Node *searchNode( Node * head, char * key );
void insertAtFront( Node **head, char * key );  // ALREADY WRITTEN FOR YOU
void insertAtTail( Node **head, char * key );
void removeAtFront( Node ** head );
void removeNode( Node **head, char * key );
void freeList( Node **head );
void printList( Node * head ); // ALREADY WRITTEN FOR YOU
void fatal( char * msg ); // ALREADY WRITTEN FOR YOU

#define BUFFER_CAP 20

int main()
{
  Node *head = NULL;

  while (1)
  {
		char option;
		printf("\nChoose 'H'ead Insert, 'T'ail insert, 'R'emove, 'S'earch, F'ree, 'Q'uit " );
		fflush( stdout );
		int result = scanf(" %c%*[^\n]", &option); getchar();  // MAGIC BULLET TO CORRECTLY READ A SINGLE CHAR FROM STDIN
		if (result <1) fatal("failure reading from stdin\n");

		if (option == 'H' )
		{
			char * word=malloc(BUFFER_CAP);  // DONT ENTER ANY LONG WORDS!
			printf("Enter a word to insertAtFront: " );
			fflush( stdout );
			char * result = fgets( word, BUFFER_CAP, stdin );
			if (result==NULL) fatal("failure reading from stdin\n");
			strtok(word,"\n"); // overwrites '\n' with  '\0'
			insertAtFront( &head, word ); /* shallow copy string into list   */
			printList( head );
		}
		if (option == 'T' )
		{
			char * word=malloc(BUFFER_CAP);  // DONT ENTER ANY LONG WORDS!
			printf("Enter a word to insertAtTail: " );
			fflush( stdout );
			char * result = fgets( word, BUFFER_CAP, stdin );
			if (result==NULL) fatal("failure reading from stdin\n");
			strtok(word,"\n"); // overwrites '\n' with  '\0'
			insertAtTail( &head, word ); /* shallow copy string into list   */
			printList( head );
		}
    		if (option == 'R' )
		{
			char * word=malloc(BUFFER_CAP);  // DONT ENTER ANY LONG WORDS!
			printf("Enter a word to remove: " );
			fflush( stdout );
			char * result = fgets( word, BUFFER_CAP, stdin );
			if (result==NULL) fatal("failure reading from stdin\n");
			strtok(word,"\n"); // overwrites '\n' with  '\0'
			removeNode( &head, word );
			printList( head );
			free( word ); // we were just using it for matching
		}
		if (option == 'S' )
		{
			char * word=malloc(BUFFER_CAP);  // DONT ENTER ANY LONG WORDS!
			printf("Enter a word to find: " );
			fflush( stdout );
			char * result = fgets( word, BUFFER_CAP, stdin );
			if (result==NULL) fatal("failure reading from stdin\n");
			strtok(word,"\n"); // overwrites '\n' with  '\0'
			if (searchNode( head, word ))
				fprintf(stderr, "%s FOUND\n",word );
			else
				fprintf(stderr, "%s NOT FOUND\n",word );
			printList( head );
			free( word ); // we were just using it for matching
		}
		if (option == 'F' ) // free the entire list (remember to set head to NULL)
		{
			freeList( &head );
			printList( head );
		}
		else if (option == 'Q' )
			exit( 0 );
	} // END WHILE

  	return 0;
}

//  # # # # # # # # # # # # # # # # # # # # # # #
//  # # # # # # # # # # # # # # # # # # # # # # #

// GIVEN AS IS - DO NOT MODIFY
void printList( Node * head )
{
	fprintf(stderr, "\nLIST: ");
	while (head)
	{
		fprintf(stderr, "%s ", head->word );
		head=head->next;
	}
	int result =  fprintf(stderr, "\n");
	if (result<1) fatal("failure writing to stdout\n");
}


// GIVEN AS IS - DO NOT MODIFY
void fatal( char * msg )
{
	printf("\nFatal Error: %s\n\n", msg );
	exit(0);
}

// GIVEN AS IS - DO NOT MODIFY
void insertAtFront( Node **head, char * key )
{
	Node *new =  malloc( sizeof(Node) );
	if (!new)  fatal("Malloc of new Node failed");
	new->word = key;
	new->next = *head;
	*head = new;
}

void insertAtTail( Node **head, char * word )
{
    // First, insert at front of the list
    /////////////////////////////////
    /////// INSERT AT FRONT /////////
    ////////////////////////////////
    insertAtFront(head, word);
    ////////////////////////////////

    // Get a ref to the head, call it currentNode
    Node * currentNode = *head; 
   
    // Get a ref to head node as we traverse list
    Node * headReference = *head;

    // Traverse to end of list
    // But first make sure we won't get a null pointer
    if (currentNode->next != NULL) 
    {
        while (currentNode->next != NULL)
        {
            currentNode = currentNode->next;
        } // end-while

        // Once we get to end of list, with a valid headNode
        // insert it at end of the list, and re-route references
        *head = headReference->next; 
        headReference->next = NULL;
        currentNode->next = headReference;
    } //end-if
}

void removeAtFront( Node ** head )
{
    // Get a reference to the head node that was passed in
    Node *headReference = *head; 
    
    // First check that our reference isn't NULL
    // Then clean up references, and remove the head
    if (NULL != head)
    {
        // Make our headNode the "next" in the list
        *head = headReference->next; 

        // free the old head Node
        free(headReference);
    } //end-if
}


void removeNode( Node ** head, char * key )
{
    int isWord;
    int match = 0;
    char *currentNodeData;
    // Get a reference to the head, and make it our currentNode,
    // so we can start traversing the list from front
    Node *currentNode = *head;
    
    // also need a ref to previous node so we can keep order
    Node *prevNode = *head;

    // Get a second reference to the head, to make sure we don't lose it! 
    Node *backupHeadRef = *head;
    
    // check if the node we want to remove IS the head node
    if (backupHeadRef == NULL)
    {
        return;
    }
    // else, check the data value of our node, and see if we have a match
    currentNodeData = currentNode->word;
    isWord = strcmp(currentNodeData, key); 

    // don't want to get a null pointer exception so make sure nothing 
    // we are going to touch is NULL
    if (isWord != 0 && NULL != currentNode  && NULL != currentNode->next)
    {
        
        // traverse until we find a node that has the data value we're looking for
        while (currentNode != NULL)
        {
            // check for a match at each node
            currentNodeData = currentNode->word;
            isWord = strcmp(currentNodeData, key);
            
            // if a match IS found, shift references so we can remove the value
            if (isWord == match)
            {
                prevNode->next = currentNode->next;
                currentNode->next = backupHeadRef;
                *head = currentNode;
                removeAtFront(head);
                // then, break out of while loop
                break;
            } // end-if
           prevNode = currentNode;
           currentNode = currentNode->next; 
        } // end-while
    } //end-if
    
    // Otherwise, check for a match
    else 
    {
      // If match is found, remove it 
      currentNodeData = currentNode->word;  
      isWord = strcmp(currentNodeData, key); 
      if ( isWord == match )
      {
          //////////////////////////
          //// RM AT FRONT /////////
          //////////////////////////
            removeAtFront( head );
          //////////////////////////

      } // end-if
    } // end-else
}

Node * searchNode ( Node * head, char * key )
{
    // setup references we'll need to search
    char * nodeData;
    int matchFound;
    int validMatch = 0;
    Node * returnValue = NULL;

	// Get a reference to our head node
    Node *headRef = head;
    // First, make sure we don't have a null head ref
    if (NULL != headRef)
    {
        // Then, iterate through the list until we get a match for the word we want
        while(NULL != headRef)
        {
            // Check the values of each node as we iterate, to see if we have a match
            nodeData = headRef->word;
            matchFound = strcmp(nodeData, key); 
            // If we find a match, it is our return value and we break
            if (matchFound == validMatch)
            {
                returnValue = headRef;
                break;
            } // end-if
            
            // Else, keep iterating
            headRef = headRef->next;
             
        } // end-while

    } // end-if

    return returnValue;
}

void freeList(  Node ** head )
{
	// setup references we'll need 
    // first, a reference to the list itself
    Node *listRef = *head;

    // then, get a ref to the currentNode, so we can free each node as we move through list
    // Node *lastNode = *head;
    Node *currentNode = *head;
    
    // make sure we don't have a null list
    if (NULL != listRef)
    {
        // if not, iterate through the list until we reach a null, freeing all the nodes along the way
        while (NULL != currentNode)
        {
            ///////////////////////////
           ///// REMOVE AT FRONT //////
           /////////////////////////// 
           removeAtFront(head);
           ///////////////////////////
           

           // lastNode = currentNode;
           // currentNode = currentNode->next;
           // free(lastNode);
           currentNode = currentNode->next;
        } // end-while

    } //end-if

    // once we're done, free the listNode reference, too
     *head = NULL;
}
