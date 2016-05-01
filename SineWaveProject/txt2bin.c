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
	FILE* txtFile, *binFile, *infile;
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
    // Open the text file with fscanf, and read next item as a short
    while(fscanf(txtFile, "%hi", &shortVal) == 1) {
         // printf("Read Integer |%d|\n", shortVal);
         // Write each piece of input to binary file, whenever loop runs
         fwrite( &shortVal, sizeof(shortVal), 2, binFile );
    } // end read

    // Close both files
    fclose(txtFile);
    fclose(binFile);

    // Re-open binary file
    /*
    binFile = fopen( argv[2], "rb" );
	if (txtFile == NULL )
	{
		printf("Can't open %s for input. Program halting\n",argv[2] );
		return EXIT_FAILURE;
	} // end open and check that file is valid

    // Print each value to command line

    size_t size; // File size
    fseek(binFile, 0, SEEK_END);
    size = ftell(binFile);
    fseek(binFile, 0, SEEK_SET);
    buffer = (char *) malloc(size);  // Allocate space == size of file in bytes

    if (fread(&buffer, sizeof(*buffer), size, binFile) != size)
    {
        printf("Error reading the file: %s", argv[2]);
        return EXIT_FAILURE;
    } else {
        unsigned long i;
        for(i = 0; i < size; i++) {
            printf("%x", buffer[i]);
        }
    }
    */
   infile = fopen(argv[2], "rb");
   if (infile==NULL)
   {
        printf("Can't open %s for input.\n", argv[2] );
        exit( EXIT_FAILURE );
   }
   // short x;

   while(fread( &shortVal, sizeof(shortVal), 2, infile ))
   {
        printf(" %d \n", shortVal);
   }
   // Close binary file
    fclose( infile );

    //Ensure both files are closed
	fclose( txtFile );
	fclose( binFile );

	return EXIT_SUCCESS;  // 0 value return means no errors
}
