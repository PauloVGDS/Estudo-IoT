#include "main.h"

// PONTEIRO = [ENDEREÇO][VALOR]

// Trocar os valores entre A e B
void swap(int *a, int *b) {

   // Alocação da variável temporária
   int *temp = malloc(sizeof(int));

   // Copiando o valor de A
   temp = *a;

   // Copiando B
   
   a = b;

   // Trocando B para A
   b = &temp;

   // Liberando a memória
   free(temp);
   printf("A = %i\nB = %i\n", *a, *b);


}



int main()
{
   SEP
   int *a = 10;
   int *b = 20;

   printf("A = %i\nB = %i\n", a, b);
   swap(&a, &b);

   SEP return 0;
}
