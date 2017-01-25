#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[]) {
  int i, j, digit_count, decimal_value, factor;
  char *temp, str[1];

  for(i = 1; i < argc; i++) {

    digit_count = decimal_value = 0;
    temp = argv[i] + 1;
    while(*++temp != '\0')
      digit_count++;

    temp = argv[i] + 2;
    for(j = digit_count-1; j >= 0; j--, temp++) {

      switch(*temp) {
	case('A'): case('a'):
	  factor = 10;
	  break;
	case('B'): case('b'):
	  factor = 11;
	  break;
	case('C'): case('c'):
	  factor = 12;
	  break;
	case('D'): case('d'):
	  factor = 13;
	  break;
	case('E'): case('e'):
	  factor = 14;
	  break;
	case('F'): case('f'):
	  factor = 15;
	  break;
        default:
	  str[0] = *temp;
	  factor = atoi(str);
      }
	
      decimal_value += factor * pow(16, j);
   }

    printf("%s = %i\n", argv[i], decimal_value);
  }

  return 0;
}
