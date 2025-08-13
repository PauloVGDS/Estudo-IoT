#include "main.h"

// strcpy(pra onde eu quero que vá, de onde vai vim)
// 32 a diferença de minúsculo para maiúsculo

void copiaN(char *destino, char *origem, int N) {

   if (N == 0) {
      N = strlen(origem);
   }
   for (int i = 0; i < N && origem[i] != '\0'; i++)
   {
      destino[i] = origem[i];
   }
   destino[N] = '\0';
   printf("Valores copiados com sucesso!\n");
}


int main()
{ SEP
   char msg[30];
   char copy[sizeof(msg)];

   printf("Digite uma palavra: ");
   scanf("%s", msg);
   copiaN(copy, msg, 10);
   printf("Copia: %s\n", copy);

   SEP return 0;
}
