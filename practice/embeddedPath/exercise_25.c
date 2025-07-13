#include "main.h"

typedef enum {
   DOMINGO,
   SEGUNDA,
   TERCA,
   QUARTA,
   QUINTA,
   SEXTA,
   SABADO
} weekDays;

char* nomeDia(weekDays d) {
   switch (d)
   {
   case DOMINGO:
      return "Domingo";
      break;
   case SEGUNDA:
      return "Segunda-Feira";
      break;
   case TERCA:
      return "Terça-Feira";
      break;
   case QUARTA:
      return "Quarta-Feira";
      break;
   case QUINTA:
      return "Quinta-Feira";
      break;
   case SEXTA:
      return "Sexta-Feira";
      break;
   case SABADO:
      return "Sabado";
      break;
   }
}

void main()
{ SEP

   int dia;
   printf("Digite o dia da semana: ");
   scanf("%d", &dia);
   if (dia < 1 || dia > 7) {
      printf("Dia invalido! Por favor, insira um numero entre 1 e 7.\n");
      return;
   }
   printf("%s", nomeDia(dia - 1));

SEP}
 