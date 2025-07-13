#include "main.h"


typedef struct {
   int x;
   int y;
} Ponto2D;

//  sqrt((x1-x2)^2 + (y1-y2)^2)
double distancia(Ponto2D *a, Ponto2D *b) {
   return sqrt(pow((a->x-a->y), 2) + pow((b->x-b->y), 2));
}



void main()
{ SEP

   // cords.x = 10;
   // cords.y = 20
   Ponto2D *ponto1;
   ponto1->x = 10;
   ponto1->y = 20;
   Ponto2D ponto2 = {40, 10};
   printf("%.2f", distancia(ponto1, &ponto2));

SEP}


