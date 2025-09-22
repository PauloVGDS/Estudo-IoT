#include "main.h"
/*

int *p; // Ponteiro
printf("%p", p); // Endereço que o ponteiro armazena
printf("%p", *p) // Valor que o endereço que o ponteiro armazena contém.
printf("%p", &p) // Endereço do próprio ponteiro

Isso da na mesma coisa:
p == *(&p)

*/

// [endereço_ptr][endereço][valor]
// &p = endereço_ptr
// p = endereço
// *p = valor

// A referência de uma variável é o seu endereço
//

int main()
{
   SEP
   printf("Alterando o valor de X pelo ponteiro P.\n");
   int x;
   printf("Digite um numero: ");
   scanf("%d", &x);


   int *p = &x; // referenciando X.

   // Ao acessar o valor do endereço no qual o ponteiro armazena estamos 'desreferenciando' o ponteiro
   printf("Antes: %d\n", x);
   *p = 256; // Desreferência com atribuição
   printf("Depois: %d\n", x);

   SEP return 0;
}
