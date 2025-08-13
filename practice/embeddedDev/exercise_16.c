#include "main.h"

void reverse(char *string) {
   char *init = string;
   char *end = string + (strlen(string) - 1);
   char temp;

   for (int i = 0; init < end; i++)
   {
      temp = *init;
      *init = *end;
      *end = temp;

      init++;
      end--;
   }
}



int main()
{
   SEP
   printf("Funcao de inversao de strings.\n");
   char msg[] = "Paulo Vinicius Gomes da Silva";
   
   reverse(msg);

   printf("\n%s\n", msg);
   SEP return 0;
}
