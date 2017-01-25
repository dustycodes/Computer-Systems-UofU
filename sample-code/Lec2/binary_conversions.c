#include <stdio.h>
#include <stdlib.h>

/* binary_conversions.c
 *
 * Based on user-input, this program performs binary to decimal 
 * and decimal to binary conversions.
 *
 * (Note: All binary integers are assumed to be 8-bit.) 
 */

int main()
{
  int choice;             /* menu choice, given by user */
  int binary_integer;     /* binary integer to be converted, given by user */
  int decimal_integer;    /* decimal integer to be converted, given by user */
  int i;                  /* for-loop counter */
  int j;                  /* for-loop counter */
  int power_10;           /* used in computing 10^x, for some integer x */
  int power_2;            /* used in computing 2^x, for some integer x */

  printf ( "Please choose: \n" );
  printf ( "\t 0  Binary to decimal conversion\n" );
  printf ( "\t 1  Decimal to binary conversion\n" );
  printf ( "\t -1 Quit\n" ); 

  scanf ( "%d", &choice );
  while (choice != -1) {
    
    if ( choice == 0 ) {
      printf ( "Enter an 8-bit binary integer:  " );
      scanf ( "%d", &binary_integer );

      if ( binary_integer > 11111111 ) {
	printf ( "The binary integer you entered has more than 8 binary digits, try again.\n" );
	continue;
      }

      decimal_integer = 0;
      for ( i=7; i>=0; i-- ) {

	/* after execution of this loop, power_10 = 10^i */
	power_10 = 1;
	for ( j=0; j<i; j++) {
	  power_10 *= 10;
	}

	/* if the ith digit of the binary integer is 1, add 2^i to decimal_integer */
	if ( binary_integer / power_10 == 1) {

	  /* after execution of this loop, power_2 = 2^i */
	  power_2 = 1;
	  for ( j=0; j<i; j++) {
	    power_2 *= 2;
	  }

	  decimal_integer += power_2;
	  binary_integer -= power_10;
	}	  
      }

      printf ( "The equivalent decimal integer is %d.\n", 
	       decimal_integer );
    }
    else {
      printf ( "Enter a nonnegative decimal integer:  " );
      scanf ( "%d", &decimal_integer );
      
      if ( decimal_integer < 0 ) {
	printf ( "You entered a negative integer, try again.\n" );
      }
      else if ( decimal_integer >= 256 ) {
	printf ( "The decimal integer you entered cannot be represented using only 8 binary digits, try again.\n" );
      }
      else {
	printf ( "The equivalent binary integer is ");

	for ( i=7; i>=0; i-- ) {

	  /* after execution of this loop, power_2 = 2^i */
	  power_2 = 1;
	  for ( j=0; j<i; j++) {
	    power_2 *= 2;
	  }
	  
	  /* if decimal_integer is divisible by 2^i, make the ith binary digit 1  */
	  if ( decimal_integer / power_2 == 1) {
	    printf ( "1");
	    decimal_integer -= power_2;
	  }
	  /* else make the ith binary digit 0 */
	  else {
	    printf ( "0" );
	  }
	}	    

	printf ( "\n" );
      }
    }

    printf ( "Please choose: \n" );
    printf ( "\t 0  Binary to decimal conversion\n" );
    printf ( "\t 1  Decimal to binary conversion\n" );
    printf ( "\t -1 Quit\n" ); 

    scanf ( "%d", &choice);
  } 

  return 0;
}
