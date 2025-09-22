#include "main.h"

int main() { SEP

   double num;

   printf("Digite um numero: ");
   scanf("%lf", &num);

   for (double i = num - 1; i > 0; i--) {
      
      num *= i;
   }
   
   printf("O fatorial e %.2lf", num);

SEP  return 0;
}
    
