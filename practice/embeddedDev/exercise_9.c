#include "main.h"


int fibonacci(int num)
{
   printf("%d\n", num);
   if (num == 0) return 0;
   else if (num == 1) return 1;
   
   return fibonacci(num - 1) + fibonacci(num - 2);
}

int main()
{
   SEP int num = 0;
   printf("Digite um numero: ");
   scanf("%i", &num);
   printf("%d\n", fibonacci(num));

   SEP return 0;
}
