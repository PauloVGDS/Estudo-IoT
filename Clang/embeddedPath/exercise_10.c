#include "main.h"

int soma(int a, int b) {return a + b;}
int sub(int a, int b) {return a - b;}
int multi(int a, int b) {return a * b;}
float divs(float a, float b) {return a / b;}

int main()
{
   SEP int num1, num2;
   char operator;

   printf("Digite um numero: ");
   scanf("%i", &num1);
   printf("\n");

   printf("Digite um operador: ");
   scanf(" %c", &operator);
   printf("\n");

   printf("Digite um segundo numero: ");
   scanf("%i", &num2);
   printf("\n");

   switch (operator)
   {
   case '+':
      printf("%d + %d = %d", num1, num2, soma(num1, num2));
      break;

   case '-':
      printf("%d - %d = %d", num1, num2, sub(num1, num2));
      break;

   case '*':
      printf("%d * %d = %d", num1, num2, multi(num1, num2));
      break;

   case '/':
      printf("%d / %d = %f", num1, num2, div(num1, num2));
      break;

   default:
      printf("Operacao invalida!");
      break;
   }

   SEP return 0;
}
