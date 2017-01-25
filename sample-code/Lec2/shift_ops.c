#include <stdio.h>

int main(void) {
  int a = 0x1;
  int b = 0x11;
  int c = 0x80000000;

  printf("%i %i %i\n", a<<1, a<<2, a<<3);
  printf("%i %i\n", b<<5, b*32);

  printf("%x %x %x %x\n", b, b>>1, b>>2, b>>3);
  printf("%x %x %x %x\n", c, c>>1, c>>2, c>>3);

  return 0;
}
