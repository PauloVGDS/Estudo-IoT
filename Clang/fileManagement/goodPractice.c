#include "main.h"

int main()
{ SEP
   // Ponteiro para um arquivo
   FILE *fptr;

   // O ponteiro agora aponta pro arquivo 'teste.txt', caso ele não exista ele cria o arquivo.
   fptr = fopen("teste.txt", "r");

   // Good Practice
   // A função fopen no modo Read(r) retorna NULL, caso o arquivo não exista.
   if (fptr == NULL) {
      printf("Arquivo inexistente!");
   }

   SEP 
return 0;}


