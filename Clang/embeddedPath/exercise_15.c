#include "main.h"
/*

Array é um cojunto enfileirado de valores do mesmo tipo.

*/


int main()
{
   SEP
   printf("Aritmetica de Ponteiros.\n");
   int arr[3] = {10, 20, 30};
   int *p = arr;

   printf("%p\n", &p); // Endereço do ponteiro em si
   printf("%p\n", p); // O nome sozinho já é referência pro primeiro elemento
   printf("%p\n", p+1); // Somando 4 bytes a frente da referência inicial, temos o segundo termo. 1 número inteiro são 4 bytes.
   printf("%d\n", *p); // Acessando o valor referenciado que o ponteiro armazena
   printf("%d\n", *(p+1)); // Acessando o segundo termo.
   SEP return 0;
}
