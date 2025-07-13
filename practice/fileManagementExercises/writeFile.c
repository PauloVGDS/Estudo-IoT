#include "main.h"

int main()
{ SEP
   // Ponteiro para um arquivo
   FILE *fptr;
   // O ponteiro agora aponta pro arquivo 'teste.txt', caso ele não exista ele cria o arquivo.
   fptr = fopen("teste.txt", "a");

   // Passa o endereço e o texto para escrever no arquivo(se estiver aberto em modo 'a' ou 'w')
   // O modo Append(a) é para adicionar conteúdo conservando o existente.
   // O modo Write(w) adiciona escrevendo por cima de tudo.
   fprintf(fptr, "Paulo Vinicius Gomes da Silva\n");

   // Fecha o arquivo para liberar a memoria e o acesso de outros programas ao arquivo.
   fclose(fptr);

   SEP 
return 0;}


