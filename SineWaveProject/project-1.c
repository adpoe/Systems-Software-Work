/*
  project-1.c

  MY NAME: Anthony Poerio
  MY PITT EMAIL: adp59@pitt.edu

  As you develop and test this file:

  use this command to compile: (you can name the executable whatever you like)
  gcc -W -Wall -Wextra -O2   project-1.c   -o project-1.exe

  use this command to execute:  (you will of course test on both input files)

  ./project-1.exe  sine-1.bin
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char *argv[])
{
  FILE * inFile= NULL;

  // DECLARE ALL ADDITIONAL NEEDED VARIABLES
    short int prev;
    short int current;
    short int next;
    short int initWave;
  // DECLARE VARIABLES FOR REPORTING
    short int endSample;
    short int endValue;
    short int peakValue;
    short int nadirValue;
    int countWave;
    int countTotal;

  /* recommend declaring two short int i.e prev & current so that you can compare
     them to look for zero crossings or changes in direction and sign.
     You will also need some int counters etc.
  */

  if (argc < 2)
    {
      fprintf(stderr,"\nMust specify a binary file on the command line. Please try again.\n");
      exit(EXIT_FAILURE);
    }
  if  ((inFile = fopen(argv[1],"rb")) == NULL)
    {
      fprintf(stderr,"Can't open %s for input\n", argv[1] );
      exit(EXIT_FAILURE);
    }

  // YOUR CODE HERE - READ EACH VALUE FROM THE BINARY FILE ONE AT A TIME AND LOOK FOR ZERO CROSSINGS TO DETECT WAVES

    // initialize values
    initWave = 0;
    countWave = 0;
    countTotal = 0;

    // Read in values, three at a time so we can see crossings, peaks, nadirs

    // Read in a current value
    fread(&current, sizeof(current), 2, inFile);
    countWave++;
    countTotal++;
    // Read in a previous value
    fread(&prev, sizeof(prev), 2, inFile);
    countWave++;
    countTotal++;


    //printf("PREV == %d\n", prev);
    //printf("CURRENT == %d\n", current);
    /*
     * Correct output on second one. But not first. Need to ID better way to always find start of wave.
     */
    while(fread(&next, sizeof(next), 2, inFile)) {
           // printf("I'm in the loop [%d]: \n\tPREV: %d\n\tCURR: %d\n\t NEXT: %d\n", countTotal, prev, current, next);
           // printf("Initialized? :: %d\n", initWave);

        // Check for end of sine wave, which we know is full
        if( (initWave == 1) && prev <= 0 && current <= 0 && next > 0 ) {
            // Then we have the END of a sine wave, by zero crossing
            endSample = countTotal;
            endValue = current;
            if (current > 0) {
                countWave++;
                // NEED TO:  Check terminating condition
                // of 110, and see why it should be 37,
                // instead of 38. Maybe this isn't correct
            }
            printf("%d\t%d\t%d\t%d\t%d\n", endSample, endValue, countWave, peakValue, nadirValue);
            initWave= 0;
            countWave = -1; // make this -1, because it will get incremented at the end of the loop
            peakValue = 0;
            nadirValue = 0;
        }

        // Check for end of sine wave, which was not initialized in our loop
        if ( (initWave == 0) && prev <= 0 && current <= 0 && next > 0)  {
            // Then we have the START of a sine wave, by zero crossing
            initWave = 1;
            countWave = 0;
            if (current >= 0) {
                countWave++;
            }
        }

        //Find Peak value for the wave
        if (current > peakValue) {
            peakValue = current;
        }

        // Find the nadir value
        if (current < nadirValue) {
            nadirValue = current;
        }

        // After checking all conditions, we make PREV the CURRENT value, and repeat
        countWave++;
        countTotal++;
        prev = current;
        current = next;
        // printf("countWave: %d\n", countWave);
    }
  /* recommended strategy:
     ** read in the first value of the file (prev) before the loop starts.
     ** Then in the while loop read next short in.
     In the loop you are to be looking for critical events:  zero crossings or sign changes.
     Before you start writing that code start out just echoing each value and the sample # associate with it.
     Once that is right start printing an alert whenever zero is touched/crossed OR or the direction changes.
     Zero crossings and direction changes are the critical events you must correctly detect.
     Once that is right then try to detect the start of the first wave.
     Once that is right add code to  detect the end of the first wave.
     Then start coding to detect every wave - counting samples per wave and
     keeping track of the highest and lowest value in that wave.
  */




  fclose(inFile); /* after the read loop is done. close file */

  return EXIT_SUCCESS;  // this is just a zero value. In C a zero return often indicates no error
}
