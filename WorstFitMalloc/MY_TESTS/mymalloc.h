//
// Created by Tony P on 3/25/16.
//

// BACKUP!!!
////////////////////////
///// DECLARATIONS /////
////////////////////////

#ifndef MYMALLOC_MYMALLOC_H_H
#define MYMALLOC_MYMALLOC_H_H


void *my_worstfit_malloc(int size);
void my_free(void *ptr);



// Define class level variables
//int *currentBrk_ptr = sbrk(0);
#define BASE   sbrk(0)


///////////////////////////////////////
///// STRUCTURES AND GLOBAL VARS //////
///////////////////////////////////////

// Linked list structure we are using
typedef struct Node {
    // size of the chunk in memory
    size_t chunk_size;  // size in BYTES
    int is_free; // if==1, space is free. if==0, space is NOT free.
    // treat this like a boolean
    struct Node *next_ptr; // PTR to next struct in our linked list
    struct Node *prev_ptr; // PTR to prev struct in our linked list
} Node;

Node *memory_mgmt_list;



//////////////////////
///// FUNCTIONS //////
//////////////////////

// initialize our list
void *init_list() {
    // set our list so that its head points to the initial value of the heap
    Node *memory_list = BASE;
    return memory_list;
}


// My worst fit malloc function
// Algorithm:
//    -
void *my_worstfit_malloc(int size) {
    // get the current brk value
    Node *oldBrk_ptr = sbrk(0);
    // DEBUGGING STATEMENTS
    //printf("first brk value is %p\n", oldBrk_ptr);
    //printf("---------------------------------\n");


    Node *return_ptr;

    // start by initializing our list, if we need to
    if (memory_mgmt_list==NULL) {
        // init our list
        memory_mgmt_list = init_list();
        int node_size = sizeof(Node);
        // make room for our head node on the heap
        return_ptr = sbrk(node_size); // >>> FIRST CALL DOESN'T DO ANYTHING?
        return_ptr = sbrk(0); // confirm correct value
        // assign our new node value to the very beginning of heap
        Node *newNode = oldBrk_ptr;
        // assign our attributes for this new node
        newNode->is_free=0; // node is NOT free
        newNode->chunk_size=size; // we make space equal to the size we have allocated
        sbrk(size);
        newNode->prev_ptr=NULL; // because this is the very start of our list
        newNode->next_ptr=NULL; // there is no next node yet, it has not been created
        return return_ptr;  // return a ptr to address at beginning of allocated segment
        // right after our node data ends
    } // CONFIRM --> this segment works... we can start off correctly

    Node *my_mem_list = memory_mgmt_list;
    // SEARCH FOR THE PROPER NODE
    Node *currentNode_ptr = my_mem_list;
    Node *worst_fit_mem_addr_ptr = currentNode_ptr;
    int worst_fit_value = -1;
    int how_many_free_bytes = -1;
    // effectively searches through whole list
    while (NULL != currentNode_ptr->next_ptr) {
        // need to get ->next values because these are pointers
        currentNode_ptr = currentNode_ptr->next_ptr;
        // check size value of each node. then see if fit is a worse fit...
        how_many_free_bytes = currentNode_ptr->chunk_size - size;
        if (   currentNode_ptr->is_free==1 &&  // if the node is free
               how_many_free_bytes >= 16 &&  // and if our size actually fits in the node at all
               // NEED TO CHECK IF -- AT LEAST 16 because we need 16 addresses left for a node
               how_many_free_bytes > worst_fit_value // and if our fit is bigger than the worst fit
            // that is, there is MORE unused space than the previous worst fit
                )   // end conditionals

            // THEN, (if true) --> update our our worst fit data
        {
            worst_fit_mem_addr_ptr = currentNode_ptr; // update our worst-fit pointer
            worst_fit_value = how_many_free_bytes;    // make the new worst-fit the 'fit' we just found
            // (int value of how many free bytes)
        } // end-if
    } // end-while

// -------> search ended here <------------


    // currentNode_ptr will be the value at end of our list, if we need to allocate new space
    // should do that when we first make the node yet

    int *endBrk;

    //###########CREATE A NEW NODE ###################
    // what about if nothing available... or no good fits..
    // --> MAKE A NEW allocation of the size and put our node at the end of the list
    // can use our CURRENT NODE VALUE to reference
    if (worst_fit_value == -1) // if worst fit is -1, then we didn't find any fits, or nothing was free
    {

        //------CREATE BOOK-KEEPING--------
        // get size of our booking Node
        size_t bookkeeping_node_size = sizeof(struct Node);
        // create a bookkeeping node
        // and allocate its size to the heap
        Node *bookkeeping_node = sbrk(bookkeeping_node_size);
        return_ptr = sbrk(0);
        bookkeeping_node->chunk_size=size;
        bookkeeping_node->is_free=0;

        bookkeeping_node->next_ptr = NULL;   //newBrk_ptr; // --> should both of these be NULL?
        bookkeeping_node->prev_ptr = currentNode_ptr;   //oldBrk_ptr;
        //--------END BOOK-KEEPING----------
        // this case, we place our new node at the end of the list,
        // and we allocate memory there...
        // -> first for the node... .. means remove the initial allocation for this node, only store it when we need to
        currentNode_ptr->next_ptr = bookkeeping_node; // careful here.. we shouldn't allocate this space up front,
        // ......unless we need to
        // RETURN VALUE will be the return ptr from our sbrk() function, AFTER allocation
        // -> and then for the data
        // int *end_ptr = sbrk(size)
        // make room now
        endBrk = sbrk(size);

    } else {
        // ############ REPLACE AN EXISTING, FREE NODE ##########
        // and break in two, if need be
        // After we've found our worst fit... need to insert our bookkeeping node
        // within a linked list insert...

        // save values of the old node
        Node temp_split_node;
        temp_split_node.prev_ptr = currentNode_ptr->prev_ptr;
        temp_split_node.next_ptr = currentNode_ptr->next_ptr;
        temp_split_node.chunk_size = currentNode_ptr->chunk_size;
        temp_split_node.is_free = currentNode_ptr->is_free;

        // get a pointer to our new node, without changing the heap size,
        // use only values that are already allocated
        Node *updated_node = currentNode_ptr;
        //Node *end_updated_node = currentNode_ptr+sizeof(Node);
        updated_node->chunk_size=size;
        updated_node->is_free=0;

        // handle pointers for newNode
        updated_node->next_ptr = currentNode_ptr;
        updated_node->prev_ptr = currentNode_ptr->prev_ptr;

        // handle pointers for next and prev nodes
        currentNode_ptr->prev_ptr->next_ptr = updated_node; // set the prev_node to point at next
        currentNode_ptr->prev_ptr = updated_node; // then change the prev node pointer itself

        // set the values properly
        // new address is + NodeSize + Size up the stack
        Node *new_split_node_ptr = currentNode_ptr + sizeof(Node) + size;
        new_split_node_ptr->is_free = temp_split_node.is_free;
        // remaining chunk size is however much is left after subtracting the size passed in,
        // and the size of the node we just added
        new_split_node_ptr->chunk_size = temp_split_node.chunk_size - size - sizeof(Node);
        new_split_node_ptr->next_ptr = temp_split_node.next_ptr;
        new_split_node_ptr->prev_ptr= temp_split_node.prev_ptr;

        // finally, assign our return value. it's where our updated node ends
        return_ptr = currentNode_ptr + (16*sizeof(char)); // because a node is 16 bytes, and a char is 1 byte
    }

    // return the value to the previous break pointer
    return return_ptr;
}

// What's the algorithm here?
void my_free(void *ptr) {
    //printf("called\n");

    Node *check_base_node = ptr;
    check_base_node = check_base_node - 1;
    //printf("breaks before successfully subtracting from ptr\n");
    // HANDLE BASE CASE
    if (memory_mgmt_list == check_base_node) {
        //printf("base case >>\n");
        Node *base_node = check_base_node;
        base_node->is_free = 1;

        // if the next node is free, coalesce with it
        if (base_node->next_ptr->is_free) {
            Node *next_node = base_node->next_ptr;
            base_node->chunk_size += next_node->chunk_size + sizeof(Node);
            base_node->next_ptr = next_node->next_ptr;
            base_node->next_ptr->prev_ptr = base_node;
        }
       return;
    }


    // HANDLE ALL OTHER CASES
    // get a reference to our pointer, assert that our node is free
    // our pointer is to start of data segment... to access the actual node, we need to move back 16 bytes
    Node *free_node = ptr;
    free_node = free_node - 1; // go back the size of one node to access its contents and data
    free_node->is_free=1;
    //printf("okay during the node assignment\n");

    // if next is free, coalesce
    int next_node_free;
    if (NULL != free_node->next_ptr) {
         next_node_free = free_node->next_ptr->is_free; // if this is 1, then next node is free
    } else {
        // if the next node is NULL, it CAN'T be free, so must be 0
        next_node_free = 0;
    }
    //printf("breaks because we try to get a next and hit a null\n");
    if (1 == next_node_free) {
        //printf("stuck in coalesce NEXT\n");
        // coalesce
        Node *next_node = free_node->next_ptr;
        next_node->chunk_size += free_node->chunk_size + sizeof(Node);
        next_node->is_free = 1;
        next_node->prev_ptr = free_node->prev_ptr;
        // next ptr doesn't change

    }

    //-------------------works------------
    // ensure we don't hit a null point below when we delete the head node
    if (free_node == memory_mgmt_list) {
        // printf("hypothesis:  this case breaks it\n");
        return;
    }


    // if prev is free, coalesce
    int prev_node_free = free_node->prev_ptr->is_free; // if this is 1, prev node is free
    //-------------------end----------------

    if (1 == prev_node_free) {
        // printf("stuck in coalesce PREV\n");
        // coalesce
        Node *prev_node = free_node->prev_ptr;
        prev_node->chunk_size += free_node->chunk_size + sizeof(Node);
        prev_node->is_free = 1;
        prev_node->next_ptr = free_node->next_ptr;
        // prev pointer doesn't change
   }

}

#endif 


