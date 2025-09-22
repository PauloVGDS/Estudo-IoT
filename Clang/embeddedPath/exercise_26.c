#include "main.h"

#define LOOP_RANGE 8

struct Registro8B {
   unsigned int bit0 : 1; 
   unsigned int bit1 : 1; 
   unsigned int bit2 : 1; 
   unsigned int bit3 : 1; 
   unsigned int bit4 : 1; 
   unsigned int bit5 : 1; 
   unsigned int bit6 : 1; 
   unsigned int bit7 : 1;

   uint8_t valor;
   
};

void lerBits(struct Registro8B *r) {
   
   printf("Representacao Decimal: %d", r->valor);
   r->bit0 = (r->valor >> 0) & 1;
   r->bit1 = (r->valor >> 1) & 1;
   r->bit2 = (r->valor >> 2) & 1;
   r->bit3 = (r->valor >> 3) & 1;
   r->bit4 = (r->valor >> 4) & 1;
   r->bit5 = (r->valor >> 5) & 1;
   r->bit6 = (r->valor >> 6) & 1;
   r->bit7 = (r->valor >> 7) & 1;
   printf("\nRepresentacao binaria: %d %d %d %d %d %d %d %d \n", r->bit7, r->bit6, r->bit5, r->bit4, r->bit3, r->bit2, r->bit1, r->bit0);
}

void atualizaBits(struct Registro8B *r) {
   
   r->valor = 0;

   if (r->bit0 == 1) r->valor+= 1;
   if (r->bit1 == 1) r->valor+= 2;
   if (r->bit2 == 1) r->valor+= 4;
   if (r->bit3 == 1) r->valor+= 8;
   if (r->bit4 == 1) r->valor+= 16;
   if (r->bit5 == 1) r->valor+= 32;
   if (r->bit6 == 1) r->valor+= 64;
   if (r->bit7 == 1) r->valor+= 128;
   // r->bit0 |= (r->valor << numero-do-bit); também dá


   printf("Atualizacao do valor: %d\n", r->valor);
   
}



void main()
{ SEP
   struct Registro8B* registro = malloc(sizeof(struct Registro8B));

   printf("Digite um numero: ");
   scanf("%d", &registro->valor);

   lerBits(registro);

   registro->bit1 = 0;
   registro->bit3 = 0;
   registro->bit4 = 1;

   atualizaBits(registro);

   lerBits(registro);
   free(registro);

SEP}
 

