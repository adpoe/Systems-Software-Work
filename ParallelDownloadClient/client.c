/*
 * CS449 - Spring 2016
 * University of Pittsburgh
 * @author Anthony (Tony) Poerio
 * @email adp59@pitt.edu
 * Project #5 - Parallel Downloads for P2P Client
 */

///////////////////
//// INCLUDES /////
///////////////////

// C Library Standard Includes
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Threading Includes
#include <pthread.h>

// Socket Programming Includes
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Error handling includes
#include <errno.h>


//////////////////////
//// DEFINITIONS /////
//////////////////////

// DEFINE BOOLEANS
typedef int bool;
#define true 1
#define false 0

// GLOBAL OUTPUT VARS
bool moreDataExists = true;
FILE *outFile; // handle to our output file
int chunkNum = 0;  // keep chunks by number, treat them like array indices into file
int currentChunk; // holds value of the current chunk we're writing
int sizeOfChunk = 1024; // keep the size of our chunks as a variable
                        // so it's consistent throughout
                        //
char recvBuff[1025]; // the buffer where we receive chunks
pthread_mutex_t outputMutex = PTHREAD_MUTEX_INITIALIZER; // mutex lock for writing ouptut
pthread_mutex_t chunkMutex = PTHREAD_MUTEX_INITIALIZER; // mutex for controlling access to the individual chunks

int chunksWritten[1000]; // use this to keep track of which chunks were written

// GLOBAL THREADING VARS
int numThreads;
struct threadAddress {
    int threadID;
    char *threadIPAddr;
    int threadPort;
};

///////////////////////
////// FUNCTIONS //////
///////////////////////

/*
 * Method to handle output to a file
 */
void writeToFile(){
    // check if the current chunk has already been written
    if (chunksWritten[chunkNum] == 0) {
        // AND... write to the file if not
        // get our output file ready
        outFile = fopen("transferred_data.txt","a+"); // use "a+" because mutliple threads will touch file, and we want to append
        // move to the file location that corresponds to the chunk number
        // and write it
        fseek(outFile, chunkNum*sizeof(recvBuff), SEEK_SET);
        fwrite(recvBuff, 1, sizeof(recvBuff),outFile);
        fclose(outFile);

        // now, store a 1 in chunksWritten at our current index
        // this way, we can keep track of what has been written vs. not
        // this is to ensure nothing gets written more than once
        chunksWritten[chunkNum] = 1;
    }
    // else, we do nothing
    return;

}

/*
 * Function used to read a chunk of data from the server class
 *
 */
void *readChunk(void *threadAttributes) {
    printf("made it into readChunk\n");
    // get the thread attributes we passed in, and open a socket with them
    // we want a pointer to this, since it's already been allocated
    struct threadAddress *currentThreadAddress = threadAttributes;

    // use a while loop and run it until we get to end of the file
    while (moreDataExists) {

        // socket connection variables
        int sockfd; // initializers we need to work with sockets
        struct sockaddr_in serv_addr; // a struct for the server address

        // Use a mutex to contral access to the chunk numbers,
        // each time, before we access them
        pthread_mutex_lock(&chunkMutex);
        ////////  INSIDE MUTEX ////////
        currentChunk = chunkNum;
        ///////   END MUTEX  /////////
        pthread_mutex_unlock(&chunkMutex);

        // create the socket
        if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) <0)
        {
            printf("\n Error: could not create socket\n");
            perror("Could not create socket.");
            exit(errno);
        }

        // assign the port and ip number
        int serverPort = currentThreadAddress->threadPort;
        char *serverIP = currentThreadAddress->threadIPAddr;

        printf("serverPort=%d \n", serverPort);
        printf("serverIP=%s \n", serverIP);

        // update server address
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(serverPort);
        serv_addr.sin_addr.s_addr = inet_addr(serverIP);

        // make the connection
        int connReturn;
        connReturn = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        if(connReturn < 0)
        {
           printf("connReturn=%d",connReturn);
           printf("\n Error : Connect Failed \n");
           perror("Error making the connection");
           exit(errno);
        }
        printf("> Connection made \n");

        // clear out the old data in the buffer
        memset(&recvBuff, 0, sizeof(recvBuff));
        // fill a "message" string with the chunk number we want to grab,
        // and send this to the server
        char message[100];
        sprintf(message, "%d", currentChunk);
        // use send() to tell the server what chunk we want
        if (send(sockfd, message, sizeof(message), 0) < 0)
        {
            printf("Error : Could not SEND data to the server.");
            perror("Error on SEND");
            exit(errno);
        }
        // receive bytes from the server
        int bytesRecvd = recv(sockfd, recvBuff, sizeof(recvBuff), 0);
        printf("%s \n",recvBuff);
        // if we haven't received any bytes, exit
        if (bytesRecvd < 1){
            free(currentThreadAddress); // only thing malloc'd in this scope
            close(sockfd); // close the socket
            return (void *)0; // need to return some void pointer,
                              // so just return the value 0
                              // meaning 'no more bytes to read'
        }

        // handle file IO
        pthread_mutex_lock(&outputMutex);
        ///// INSIDE MUTEX /////
        writeToFile();
        chunkNum += 1; // after we write a chunk for a given number,
                       // increment the index, so we don't overwrite that
                       // that location and keep things in order

        // Ensure we don't get an array out of bounds error
        // when checking to ensure no chunks are overwritten
        int numberOfValidChunks = sizeof(chunksWritten)/sizeof(int);
        if (chunkNum >= numberOfValidChunks) {
            // resize the array, using realloc
            void *return_value_ptr = NULL;
            return_value_ptr = realloc(chunksWritten, 2*numberOfValidChunks*sizeof(int));
            if (return_value_ptr == NULL) exit(EXIT_FAILURE);
        }
        ///// MUTEX /////
        pthread_mutex_unlock(&outputMutex);

        // close the socket at end of the iteration,
        // so we can keep things straight
        close(sockfd);

    }
    // return 1, meaning we still have more bytes to read
    return (void*)1;
}

/*
 * Method to create all the threads we need for the client
 *
 */
void createThreads(pthread_t *threadArray, int argCount, char *argList[]) {
    printf("made it into create threads\n");
    // create threads
    // increment by 2 so we can can get everything done in the same loop iteration
    int threadNum = 0;
    printf("argCount=%d\n",argCount);
    int argNum;
    for (argNum=0; argNum+1<argCount; argNum+=2) {
        printf("Arg %d (unused)= %s\n", argNum, argList[argNum]);
        printf("Arg %d (IP)= %s\n", argNum+1, argList[argNum+1]);
        printf("Arg %d (Port)= %s\n", argNum+2, argList[argNum+2]);
        // need to get the thread parameters (server address) into a void pointer,
        // so we can pass it into the readChunk() method within a pthread_create() call.
        struct threadAddress *threadParams = malloc(sizeof(struct threadAddress));
        threadParams->threadIPAddr = argList[argNum+1];
        threadParams->threadPort = atoi(argList[argNum+2]);
        // add all of our threads to an array and pass all the data
        // that we need to pthread create
        pthread_create(&threadArray[threadNum], NULL, readChunk, threadParams);
        threadNum++;
    }
}

//////////////////////////////
////// MAIN ENTRY POINT //////
//////////////////////////////
int main(int argc, char *argv[])
{
    // PARSE CMD LINE ARGS
    // first check if we have enough arguments
    int argsModTwo = argc % 2;
    if (argc < 3) {
         printf("Too few arguments.\n");
         printf("\n Usage: %s <Server 1: IP>  <Server 1: port number>  <Server 1: IP>  <Server 1: port number> ... etc\n",argv[0]);
         return 1;
    }
    // next, check if we have the right number, must be multiple of 2
    else if (argsModTwo != 1) {
         printf("argsModTwo=%d \n", argsModTwo);
         printf("argc=%d \n", argc);
         printf("Please specify an IP and PORT NUMBER for each server.\n");
         printf("\n Usage: %s <Server 1: IP>  <Server 1: port number>  <Server 1: IP>  <Server 1: port number> ... etc\n",argv[0]);
         return 2;
    }
    //else, get number of threads --> one for EACH server
    else {
        numThreads = argc/2;
        printf("numThreads=%d \n",numThreads);
    }
    //////////// PREPARE OUTPUT FILE ////////////
    // reset previous file data we transferred, since are appending
    outFile = fopen("transferred_data.txt","w");
    fwrite(" ", 1, 1, outFile);
    fclose(outFile);
    ////////////// END OUTPUT FILE PREP /////////
    printf("Made it past prepare output file\n");

    // Initalize our record keeping array
    memset(chunksWritten, 0, 1000 * sizeof(chunksWritten[0]));
    
    ////////// HANDLE THREADING ////////////
    // and create an array containing all of our threads,
    // so we can access them sequentially
    pthread_t myThreads[numThreads];
    // createThreads() will update the myThreads array,
    // so that it contains all the threads we we're calling
    createThreads(myThreads, argc, argv);
    // next, join all the threads
    int threadNum;
    for (threadNum=0; threadNum < numThreads; threadNum++)
        pthread_join(myThreads[threadNum], NULL);
    //////// END HANDLE THREADING //////////

    // And, finally, exit!
    return 0;
}

