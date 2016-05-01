#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdll.h"

////////// NOT SURE ABOUT THIS //////
// #include "string-Josephus.c"
////////////////////////////////////
/* just a convenience */
void fatal( char * msg )
{
    printf("%s\n",msg);
    exit(EXIT_FAILURE);
}


/* ----------------------------------------------------------------------------
	initList:

*/
void initList(CDLL *list, int (*compare)(void*, void*), void (*print)(void*, int),
              void (*freeData)(void *))
{
    /* Y O U R   C O D E    H E R E */

    // 1) set the head pointer in the CDLL struct to NULL
    list->head = NULL;
    // 2) assign each of the incoming function pointers into their respective fields in the CDLL struct
    list->compare = *compare;
    list->print = *print;
    list->freeData = *freeData;

}


/* ----------------------------------------------------------------------------
*/
void insertAtTail(CDLL *list, void *data)
{
    /* Y O U R   C O D E    H E R E */
    //CDLL_Node oldTail = *list->head->prev;
    //CDLL_Node *newNode = malloc(sizeOf(CDLL_Node));


    // CHECK FOR DUPES
    // If there is a result from searchList() which does NOT equal NULL, we have a dupe
    if(searchList(*list, data)) {
        // print duplicate error message
        printf("Duplicate content rejected.");
        // free CDLL struct
        free(list);
        // free data object
        free(data);
        // leave function now, if we have a dupe
        return;
    }
    CDLL *myList = list;
    void *myData = data;
    // IF List is empty, initialize it with the references we need
    if (myList->head == NULL) {
        /// initialize node
        CDLL_NODE * newNode = malloc(sizeof(CDLL_NODE));
        newNode->data = myData;
        myList->head = newNode;
        myList->head->prev = newNode;
        myList->head->next = newNode;
        // list->head->data = &data;
        // myList->head->data = myData;
        return;
    }

    // INSERT NODE
    // Get reference to tail node
    CDLL_NODE *oldTail = myList->head->prev;
    // Get a reference to head node
    CDLL_NODE *headNode = myList->head;
    // Get a reference to a NEW node and fill it with data value
    CDLL_NODE *newNode = malloc( sizeof(CDLL_NODE) );
    if (!newNode) fatal("Malloc of CDLL_NODE failed");
    newNode->data = myData;
    // Set proper next and prev for NEW node
    newNode->next = headNode;
    newNode->prev = oldTail;
    // Insert NEW node into last link in the LL
    oldTail->next = newNode;
    headNode->prev = newNode;

}



/* ----------------------------------------------------------------------------
	deleteNode:

	You have  passed in the pointer to the node to be deleted.
	No need to iterate or search. Of course a check for a NULL pointer passed in
	would not hurt.
	Delete the deadNode then return the pointer to it's successor (if CW) or
	if you are going CCW then return pointer to the deadNode's predecessor.
	If deadnode was the last node then return NULL since there is no succ or pred.
*/
CDLL_NODE * deleteNode(CDLL *list, CDLL_NODE *deadNode, int direction )
{
    /* Y O U R   C O D E    H E R E */
    // Check for NULL pointers, and error out,tif we have one
    if (list == NULL || deadNode == NULL) {
        fatal("Null pointer passed in to deleteNode");
    }


    // GET A REFERENCE TO THE HEAD NODE, AND THE LIST
    CDLL *myList = list;
    CDLL_NODE *headRef = list->head;

    // WHAT IS THE PREV OF OUR HEAD NODE, BEFORE WE DO ANYTHING?
    //printf("\nHeadNode's previous:  %s", headRef->prev->prev->data);

    // DELETE THE DEAD NODE
    // First, get a reference to its NEXT
    CDLL_NODE *nextNode = deadNode->next;
    //printf("\n--->deadNode->next: %s", nextNode->data);

    // Then, get a reference to its PREV
    CDLL_NODE *prevNode = deadNode->prev;
    //printf("\n--->deadNode->prev: %s", prevNode->data);

    // MAKE SURE DEAD NODE WAS NOT LAST NODE
    if(nextNode == deadNode) return NULL;


    // Re-assign references
    nextNode->prev = prevNode;
    prevNode->next = nextNode;

    // Cut out deadNode
    deadNode->next = NULL;
    deadNode->prev = NULL;
    deadNode->data = NULL;

    // If we've deleted the head of our list, re-assign it.
    if (deadNode == headRef) {
        headRef = nextNode;
        nextNode->prev = prevNode;
        myList->head = headRef;
    }

    // Return a variable based on the direction chosen
    if (direction == CLOCKWISE) {
        deadNode = nextNode;
        return deadNode;
    }

    if (direction == COUNTERCLOCKWISE) {
        deadNode = prevNode;
        return deadNode;
    }

    // if neither IF-condition was valid, we do not have valid iput to the function
    return NULL;
}



/* ----------------------------------------------------------------------------
	printList:

	Observe my solution executable to see how it should look
	You are really just writing the loop and calling the printData on each node
*/

void printList( CDLL list, int direction, int mode )
{
    /* Y O U R   C O D E    H E R E */
    // Setup initial variables
    CDLL_NODE *startNode = list.head;
    CDLL_NODE *iterator = list.head;

    // iterate and print  until we get back to the startNode
    if (direction == CLOCKWISE) {
        do
        {
            // ACCOUNT FOR MODE
            if (mode == BRIEF) {
                char * dataValue = iterator->data;
                //printf("%s ", dataValue);
                list.print(dataValue, mode);
            } // end-if
            else {
                char * dataValue = iterator->data;
                //printf("%s ", dataValue);
                list.print(dataValue, mode);
            }

            // Move iterator CW
            iterator = iterator->next;
            fflush(stdout);
        } while(iterator != startNode); // end-while
    } // end-if
    if (direction == COUNTERCLOCKWISE) {
        while(iterator->prev != startNode)
        {
            // ACCOUNT FOR MODE
            if (mode == BRIEF) {
                char * dataValue = iterator->data;
                printf("%s ", dataValue);
            } // end-if
            else {
                char * dataValue = iterator->data;
                printf("%s ", dataValue);
            } //end-else

            // Move iterator CCW
            iterator = iterator->prev;
            fflush(stdout);
        } // end-while
    } // end-iff
}

/* ----------------------------------------------------------------------------
	searchList:

	Scan list until you find a node that contains the data value passed in.
	If found return that pointer - otherwise return NULL
*/
CDLL_NODE * searchList( CDLL list, void * target )
{
    /* Y O U R   C O D E    H E R E */

    // First check that the head is not NULL.
    if (list.head == NULL) return NULL;

    // Then, setup variables so we can iterate
    CDLL_NODE *startNode = list.head;
    CDLL_NODE *iterator = list.head;
    // Check the data value of the iterator, until we reach start of loop again
    do {
        char *dataValue = iterator->data;
        if ( list.compare(dataValue,target) == 0 ) {
            return iterator;
        }
        iterator = iterator->next;
    } while (iterator->next != startNode);

    // if we get this far, match was NOT found
    return NULL;
}

// int main () {
//    return 0;
// }
