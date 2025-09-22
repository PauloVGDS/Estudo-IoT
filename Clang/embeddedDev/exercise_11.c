#include "main.h"

void mudaLuz(int estado)
{
   if (estado == 0)
   {
      printf("Verde\n");
   }
   else if (estado == 1)
   {
      printf("Vermelho\n");
   }
   else if (estado == -1)
   {
      printf("Amarelo\n");
   }
}

void aguardar(int tempo)
{
   return Sleep(tempo);
}

int main()
{
   SEP

       printf("================SEMAFORO================\n");

   while (1)
   {
      mudaLuz(0);
      aguardar(10000);
      mudaLuz(-1);
      aguardar(5000);
      mudaLuz(1);
      aguardar(15000);
   }
   SEP return 0;
}
