#include "main.h"

// 32 a diferença de minúsculo para maiúsculo

int contaVogais(char string[]) {
   int count = 0;
   for (int i = 0; i < strlen(string); i++)
   {
      if (string[i] > 96) {
         if ((string[i] == 'a') || (string[i] == 'e') || (string[i] == 'i') || (string[i] == 'o') || (string[i] == 'u')) count++;
      }
      
      if ((string[i] == 'A') || (string[i] == 'E') || (string[i] == 'I') || (string[i] == 'O') || (string[i] == 'U')) count++;
   }
   return count;
   
}


int main()
{
   SEP
   char msg[20];
   printf("Digite uma palavra: ");
   scanf("%s", msg);

   printf("%i", contaVogais(msg));
   SEP return 0;
}

