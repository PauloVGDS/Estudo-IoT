#include "main.h"

int main() { SEP

   float temp = 0;

   printf("Conversor: Celsius -> Fahrenheit\n");
   printf("Digite a temperatura:");
   scanf("%f", &temp);

   printf("Temperatura convertida: %.2fF", temp * 9/5 + 32);
SEP  return 0;
}
    