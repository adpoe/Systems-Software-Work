#include <stdio.h>
#include <pthread.h>

#define BUFLEN 8000
#define MAX_THREADS 4
char buffer[BUFLEN];
int bufCnt=0;
int numThreads=0;

void * readFile(void * fname)
{
	int offset = numThreads*2000;

    // lock the threads before touching global variable
    pthread_mutex_t lock_g;
    pthread_mutex_lock(&lock_g);
    // --------------------------------------------------
	int threadNum = numThreads++; // global variable !!!
	int readCnt=0;
	char symbol='?';
	FILE *f = fopen( (char*)fname, "rt" );
	if ( !f )
	{
		fprintf( stderr, "Thread %d unable to open file %s\n",threadNum, (char*)fname );
		pthread_exit( (void*) -1 ); // something BAD
	}
	printf("thread %d reading file %s into buffer\n", threadNum, (char*) fname);
    // --------------------------------------------------
    // unlock the threads
    pthread_mutex_unlock(&lock_g);


    // lock again before we touch global var
    pthread_mutex_lock(&lock_g);
    // --------------------------------------------------
    while (fscanf(f, "%c",&symbol) == 1 )
	{
    	printf("%c",symbol);
		buffer[offset+readCnt++]=symbol;
		++bufCnt;  // global variable !!!
	}
    // --------------------------------------------------
    // unlock after global var is done
    pthread_mutex_unlock(&lock_g);


   	return (void*)readCnt;
}

int main(int argc, char ** argv)
{
	void* readCnts[MAX_THREADS];
	pthread_t threads[MAX_THREADS];     // thread objects
    for( int i=0 ; i<MAX_THREADS ; i++)
    {
        // lock before we start threading
        pthread_mutex_t lock_h;
        pthread_mutex_lock(&lock_h);
        //--------------------------
		pthread_create(&threads[i], NULL, readFile, (void *)argv[i+1]);
        //--------------------------
        // unlock the mutex afterwards
        pthread_mutex_unlock(&lock_h);
    }
	for( int i=0 ; i<MAX_THREADS ; i++)
    {
        // lock before we start threading
        pthread_mutex_t lock_h;
        pthread_mutex_lock(&lock_h);
        //--------------------------
		pthread_join( threads[i], &readCnts[i] );
        //--------------------------
        // unlock the mutex afterwards
        pthread_mutex_unlock(&lock_h);
    }

	int each = BUFLEN/MAX_THREADS;
	printf("DUMPING BUFFER: Should be %d '-' then %d '#' then %d '&' then %d '@'\n", each,each,each,each );
	for (int i=0 ; i < bufCnt  ; ++i )
		printf("%5d:%c\n", i, buffer[i]);
	printf( "\n");
	for (int i=0 ; i < MAX_THREADS ; ++i )
		printf( "Thread %d  read in %d symbols from file %s\n", i+1, (int)readCnts[i] , argv[i+1] );
	return 0;
}
