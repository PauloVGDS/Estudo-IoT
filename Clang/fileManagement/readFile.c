#include "main.h"

int main()
{ SEP
   // Ponteiro para um arquivo
   FILE *fptr;
   char buffer[100];

   // O ponteiro agora aponta pro arquivo 'teste.txt', caso ele não exista ele cria o arquivo.
   fptr = fopen("teste.txt", "r");

   // Pega o conteúdo do arquivo pelo buffer e tamanho especificado.
   // fgets(buffer, tamanho de leitura, ponteiro do arquivo) Obs: O tamanho do buffer tem que bater com o tamanho de leitura, preferencialmente.
   // Só funciona quando o arquivo aberto em modo Read(r)
   // Pega apenas uma linha por vez, necessario estrutura de repetição
   // fgets(buffer, 100, fptr);
   while (fgets(buffer, 100, fptr)) // Retorna NULL logo falso no final do arquivo
   {
      printf("%s", buffer);
   }
   
   // Fecha o arquivo para liberar a memoria e o acesso de outros programas ao arquivo.
   fclose(fptr);
   SEP 
return 0;}