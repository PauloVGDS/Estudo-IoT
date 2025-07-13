#include "main.h"

int main() { SEP

   int num = 0;

   printf("Digite um numero: ");
   scanf("%i", &num);

   if (num % 2 == 0) {
      printf("Esse numero e PAR!");
   } else {
      printf("Esse numero e IMPAR!");
   }

SEP  return 0;
}
    