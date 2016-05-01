#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mymalloc.h"
#include "mallocdrv.c"

int main() {

    // confirm we have the system setup
    printf("=====CONFIRM C ENVIRONMENT WORKS======\n");
    printf("Hello, World!\n");
    // confirm worst fit malloc does work
    printf("=====CONFIRM MY_MALLOC WORKS=====\n");
    // baseNode was used for debugging, so I could see the whole list, starting from the beginning
    int *baseNode = sbrk(0);

    int *two_byte_addr1 = my_worstfit_malloc(16); // 00
    strcpy(two_byte_addr1, "one");
    printf("1st allocation value, should start at 010: %p\n\tvalue=%s\n", two_byte_addr1, two_byte_addr1);

    int *two_byte_addr2 = my_worstfit_malloc(16); // 02
    strcpy(two_byte_addr2, "two");
    printf("2nd allocation value, should start at 030: %p\n\tvalue=%s\n", two_byte_addr2, two_byte_addr2);

    int *two_byte_addr3 = my_worstfit_malloc(16); // 04
    strcpy(two_byte_addr3, "three");
    printf("3rd allocation value, should start at 050: %p\n\tvalue=%s\n", two_byte_addr3, two_byte_addr3);

    int *six_byte_addr1 = my_worstfit_malloc(48); // a0
    strcpy(six_byte_addr1, "four score and seven years ago");
    printf("4th allocation value, should start at 070: %p\n\tvalue=%s\n", six_byte_addr1, six_byte_addr1);

    my_worstfit_malloc(0);  // just to check last value


    printf("=====CONFIRM MY_FREE WORKS======\n");
    // count the values and confirm what they **SHOULD** be... confirm you get those values...
    // and if you do, you'll be okay. it may just take a day to figure out what the values have to be.
    // run the test program we are given
    // TEST 2
    printf("After freeing 2nd value, we see: \n");  // chunk size=16, doesn't change
    my_free(two_byte_addr2);  // this will just free, won't coalesce
    printf("\t 2nd node chunk size should be 16\n");
    printf("\t but really, it is: %d\n", two_byte_addr2[-4]);
    // TEST 2
    printf("After freeing 3rd value, we see: \n");   // chunk size=48, for 2ND node
    my_free(two_byte_addr3); // this will coalesce with prev
    printf("\t 2nd node chunk size should be 48\n");
    printf("\t but really, it is: %d\n", two_byte_addr2[-4]);
    // TEST 3
    printf("After freeing 1st value, we see: \n");   // chunk size=64 for 1st node -- only last node is left, and it's 48
    my_free(two_byte_addr1); // this will coalesce with next
    printf("\t 1st node chunk size should be 80\n");
    printf("\t but really, it is: %d\n", two_byte_addr1[-4]);
    // printf("After freeing 1st value again, we see: \n");
    my_free(two_byte_addr1); // this will check that base case is okay, and we can free head node
                             // and okay to free a node that's already freed
    printf("After freeing 4th (and final) value, we see: \n"); // chunk size=144 and only one node left
    my_free(six_byte_addr1);
    printf("\t 1st node chunk size should be 144\n");
    printf("\t but really, it is: %d\n", two_byte_addr1[-4]);

    printf("====CONFIRM MALLOC WORKS *AFTER* FREEING====\n");
    int *new_addr = my_worstfit_malloc(16);
    printf("\t 1st node chunk should be of size 16\n");
    printf("\t but really, it is: %d\n", new_addr[-4]);

    printf("====PROF'S TEST PROGRAM=====\n");
    printf("uncomment line below in the src file to run the prof's test program from here.\n");
    //run_tests();
    return 0;

}
