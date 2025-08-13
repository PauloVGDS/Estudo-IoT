#include "main.h"

int main()
{
   SEP int cmd = 0;
   float num = 0;
   do
   {
      printf("\n1 - Calcular o modulo de um numero\n");
      printf("\n2 - Calcular o cubo de um numero\n");
      printf("\n3 - Calcular a raiz de um numero\n");
      printf("\n4 - Sair\n");
      printf("Digite sua escolha: ");
      scanf("%i", &cmd);

      switch (cmd)
      {
      case 1:
         printf("Digite um numero: ");
         scanf("%f", &num);
         printf("\nO modulo e: %i\n", (int)num % 2);
         break;
      case 2:
         printf("Digite um numero: ");
         scanf("%f", &num);
         printf("\nO cubo e: %.2f\n", pow(num, 3));
         break;
      case 3:
         printf("Digite um numero: ");
         scanf("%f", &num);
         printf("\nA raiz e: %.2f\n", pow(num, 2));
         break;
      case 4:
         printf("\nSaindo...\n");
         break;

      default:
         printf("Opcao invalida!\n");
         break;
      }

   } while (cmd != 4);

   SEP return 0;
}
