#include <stdio.h>
#include <math.h>
int main() {
   printf("Hello, World!");
   printf("\e[1;1H\e[2J");
   printf("Hello, World 2!");
   return 0;
}