#include "main.h"

// strlen = length
/*
As funções strlen e sizeof se comportam de forma diferente, uma vez que a função sizeof considera o caractere \0 de fim de linha.
*/

// strcpy = copiar
/*
Copia o conteudo da string str1 para str2
str1 = Paulo; str2 = "";
strcpy(str1, str2) 
str2 = Paulo;
*/

// strcat = concatenar
/*
Concatena a string str2 para a str1
str1 = Paulo; str2 = Vinicius;
strcat(str1, str2) == Paulo Vinicius 
*/

// Bônus strcmp = comparar
/*
Compara a string str1 com a str2 
strcmp(str1, str2)
Retorna 0 se elas forem iguais
Retorna um numero negativo se elas forem diferentes
*/

char msg[100];
char msgCopy[100];

int main()
{
   SEP
   printf("Digite uma palavra: ");
   scanf("%s", msg);

   printf("\nInformacoes da string:\n");
   printf("Tamanho: %i\n", strlen(msg));
   strcpy(msgCopy, msg);
   printf("Copia da palavra: %s\n", msgCopy);
   strcat(msg, " Vinicius");
   printf("String Concatenada: %s", msg);
   SEP return 0;
}

