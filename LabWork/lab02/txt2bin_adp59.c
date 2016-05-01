/*
  txt2bin.c

  MY NAME: Anthony Poerio
  MY PITT EMAIL: adp59@pitt.edu

  As you develop and test this file:

  use this command to compile: (you can name the executable whatever you like)
		gcc -W -Wall -Wextra -O2  txt2bin.c  -o txt2bin.exe

  use this command to execute:  (you will of course test on both input files)
		txt2bin.exe sine-1.bin
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main( int argc, char *argv[] )
{
	FILE* txtFile, *binFile;
	short int shortVal;


	if (argc<3)
	{
		printf("usage: %s  <infilename> <outfilename> (i.e. ./txt2bin.exe sine-1.txt sine-1.bin)\n", argv[0] );
		return EXIT_FAILURE;
	}

	txtFile = fopen( argv[1], "rt" );
	if (txtFile == NULL )
	{
		printf("Can't open %s for input. Program halting\n",argv[1] );
		return EXIT_FAILURE;
	}

	binFile = fopen( argv[2], "wb" );
	if (binFile == NULL )
	{
		printf("Can't open %s for output. Program halting\n",argv[2] );
		return EXIT_FAILURE;
	}

	/* YOUR CODE HERE

		read each value from the text file using fscanf as a short int and write it out to the binary file using fwrite
	*/

    /*Version 1*/
    // fscanf(txtFile, "%hi", &(shortVal));
    // fwrite(&shortVal, sizeof(shortVal), 1, binFile);
    // End Version 1
    // WRONG: Only writes one character - we need a LOOP to write whole file.

    /*Version2*/
     while( fscanf(txtFile, "%hi", &shortVal) == 1 ) {
         fwrite(&shortVal, sizeof(shortVal), 1, binFile);
     }
    // End Version 2
    // CORRECT: This one passes a diff, as long as I use the -w tag. 
    //          Seems like this adds an extra trailing space to each entry.
    //          But it's the only one which can be correct. 

    /*Version 3*/
    // while (feof(txtFile) == 0) {
    //     fscanf(txtFile, "%hi", &shortVal);
    //     fwrite(&shortVal, sizeof(shortVal), 1, binFile);
    // }
    // End Version 3
    // WRONG: Outputs the LAST value twice 

    /*Version 4*/
    // fscanf(txtFile, "%hi", &shortVal);
    // while (feof(txtFile) == 0) {
    //     fwrite(&shortVal, sizeof(shortVal), 1, binFile);
    //     //printf("%d\n",shortVal);
    //     fscanf(txtFile, "%hi", &shortVal);
    // }
    // End Version 4
    // WRONG:  Because it also prints each value, and that is NOT part of the specification.
    
    fclose( txtFile );
	fclose( binFile );
    
   // READ DATA AND PRINT IT, TO CHECK
   //printf("%s\n", argv[2]);
   FILE * infile = fopen(argv[2], "rb");
   if (infile==NULL)
   {
        printf("Can't open %s for input.\n", argv[2] );
        exit( EXIT_FAILURE );
   }

   // size_t result; 
   // result = fread(&shortVal, sizeof(shortVal), 1, infile);
   // printf("%d\n", result);
   while(fread( &shortVal, sizeof(shortVal), 1, infile ))
   {
        printf(" %d \n", shortVal);
   }
   // Close binary file
   fclose( infile );
    
   return EXIT_SUCCESS; // 0 value return means no errors
}
