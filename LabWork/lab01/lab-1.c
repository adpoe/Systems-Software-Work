/* lab-1.c

   LAB FOR:  @adp59, Anthony Poerio
   CS449 - University of Pittsburgh


   demonstrates the following:

   - reading individual characters from stdin
   - converting the ASCII character code of a digit key to its numeric value
   - i.e. converting the char '5' to 5 via 5 x 10 to the zero
   - i.e converting the string "512" to 512 via 5*100 + 1*10 + 2*1
   - the keyword break to break out of the nearest enclosing loop
   - the loop models Horner's method of evaluating a polynomial

   FYI: If you compile and execute this file as is, it will print an infinite number of "> " prompts.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main( )
{
	int number=0;
    char charVar=0;

	printf("\nYou will be repeatedly prompted for a single digit key terminated with a RETURN\n");
	printf("As soon as you enter a non digit character the loop will terminate.\n\n");
	do
	{
		char digit; /* declare it in here since its not needed outside */
		printf("> ");
		fflush(stdout);
		/* read the digit into a char var using scanf with %c format (or use %d) */
        scanf("%c",&charVar);
		/* eat the RETURN from the stdin buffer using getchar() */
        getchar();
		/* test to see if its a digit. If not, break out of the loop via the keyword "break" */
        //if (!atoi(&charVar)) {
        //    break;
        //}
        if(!(charVar>='0' && charVar<='9')) {
            break;
        }
		/* if you read in via %c then convert the character to a number by subtracting '0' from it. */
        charVar = charVar - '0';
		/* update the running total */
        number = number * 10;
        number = number + charVar;
	} while ( 1 ); /* this is an infinte loop - we must break out */

	printf("\nFinal value of number: %d\n", number );

	return 0;

} /* END MAIN */
