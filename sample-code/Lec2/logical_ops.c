#include <stdio.h>

int main(int argc, char* argv[]) {
  int a = 0x0;
  int b = 0x93;

  printf("%x %x\n", !a, !b);

  printf("%x %x %x %x\n", a&&b, a||b, !a||!b, !a&&!b);

  return 0;
}
