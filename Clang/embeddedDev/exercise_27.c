#include "main.h"

typedef enum {
   TEMPERATURA,
   PRESSAO,
   UMIDADE,
   LUMINOSIDADE,
   NIVEL
} sensorTypes;

typedef struct
{
   char nome[20];
   sensorTypes tipo;
   float leitura;
} Sensor;

void infoSensores(Sensor sens[]) {
   for (int i = 0; i < 5; i++)
   {
      switch (sens[i].tipo)
      {
      case TEMPERATURA:
         printf("%s(Temperatura) = %.2f\n", sens[i].nome, sens[i].leitura);
         break;
      case PRESSAO:
         printf("%s(Pressao) = %.2f\n", sens[i].nome, sens[i].leitura);
         break;
      case UMIDADE:
         printf("%s(Umidade) = %.2f\n", sens[i].nome, sens[i].leitura);
         break;
      case NIVEL:
         printf("%s(Nivel) = %.2f\n", sens[i].nome, sens[i].leitura);
         break;
      case LUMINOSIDADE:
         printf("%s(Luminosidade) = %.2f\n", sens[i].nome, sens[i].leitura);
         break;
      }
      
   }
}

int main()
{ SEP

   Sensor sensors[5] = {{"Sensor0", TEMPERATURA, 24.6}, {"Sensor1", PRESSAO, 1.06}, {"Sensor2", UMIDADE, 14.7}, {"Sensor3", LUMINOSIDADE, 54.1}, {"Sensor4", NIVEL, 1.0}};
   infoSensores(sensors);


SEP return 0;}
 

