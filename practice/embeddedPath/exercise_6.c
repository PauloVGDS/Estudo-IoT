#include "main.h"

int main()
{
   SEP
   
   printf("================SEMAFORO================\n");
   while (1)
   {
      printf("Verde\n");
      Sleep(10000);
      printf("Amarelo\n");
      Sleep(2500);
      printf("Vermelho\n");
      Sleep(15000);
   }
   

   SEP return 0;
}
