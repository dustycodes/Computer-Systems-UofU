#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  int i;

  for(i = 1; i < argc; i++) {
    int x = atoi(argv[i]);
    printf("%i = 0x%x\n", x, x);
  }

  return 0;
}
