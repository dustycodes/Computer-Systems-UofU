#include <stdio.h>
#include <limits.h>

int main(void) {
  printf("char %i to %i\n", CHAR_MIN, CHAR_MAX);
  printf("unsigned char 0 to %i\n", UCHAR_MAX);

  printf("short %i to %i\n", SHRT_MIN, SHRT_MAX);
  printf("unsigned short 0 to %i\n", USHRT_MAX);

  printf("int %i to %i\n", INT_MIN, INT_MAX);
  printf("unsigned int 0 to %u\n", UINT_MAX);  // why use %u instead of %i?

  printf("long %ld to %ld\n", LONG_MIN, LONG_MAX);  // why use %ld?
  printf("unsigned long 0 to %lu\n", ULONG_MAX);    // why use %lu?

  return 0;
}
