#include <stdio.h>


int fourth (char *str) {
	return str[3];
}

int main() {
	char *x = "hello";
	fourth(x);
	return 0;
}
