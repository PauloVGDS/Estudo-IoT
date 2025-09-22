#include "main.h"

int main()
{
   SEP
   printf("Alocacao dinamica de Array.\n");

   int *ptr = malloc(5 * sizeof(int));
   float med;

   for (int i = 0; i < 5; i++)
   {
      printf("Digite um numero: ");
      scanf("%d", ptr + i);
      printf("\n");
   }
   
   printf("Numeros escolhidos: ");
   printf("%d, ", *ptr);
   printf("%d, ", *(ptr + 1));
   printf("%d, ", *(ptr + 2));
   printf("%d, ", *(ptr + 3));
   printf("%d\n", *(ptr + 4));

   for (int i = 0; i < 5; i++)
   {
      med += *(ptr + i);
   }
   

   printf("Media: %.2f", med / 5);
   SEP return 0;
}
