#include "main.h"




int main()
{
   SEP

   int arr[10];
   int acc = 0;
   int min = 0, max = 0;


   for (int i = 0; i < 10; i++) 
   {
      printf("Digite um numero: ");
      scanf("%i", &arr[i]);
      if (i == 0) min = arr[i];
      if (arr[i] > max) max = arr[i];
      if (arr[i] < min) min = arr[i];
      acc += arr[i];
      printf("\n");
   }
   
   printf("Media: %i\n", acc/10);
   printf("Minimo: %i\n", min);
   printf("Maximo: %i\n", max);

   SEP return 0;
}
