#include <stdio.h>

int main(int argc, char* argv[]) {
  int a = 0x13;
  int b = 0x55;

  printf("%x %x %x\n", a|0, a&1, ~~a);

  printf("%x %x %x\n", (a|a)&b, a|(a&b), a&(a|b));

  printf("%x %x\n", ~(a&b), ~a|~b);

  return 0;
}

