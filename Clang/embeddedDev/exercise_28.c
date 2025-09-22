#include "main.h"

// TRUTH TABLE
// Operador OR (pelo menos um)
// 0 | 0 = 0
// 1 | 0 = 1
// 0 | 1 = 1
// 1 | 1 = 1

// Operador AND (só os dois)
// 0 & 0 = 0
// 1 & 0 = 0
// 0 & 1 = 0
// 1 & 1 = 1

#define BIT_MASK(num) (1 << (num - 1))

void printBits(uint8_t num) {

   printf("Binario: ");
   for (int i = 7; i >= 0; i--)
   {
      if (i == 3) {
         printf(" ");
      }

      // Desloca um bit por vez pro lado e compara com 1, retornando o mesmo
      printf("%d", (num >> i) & 1);
      
   }

   printf("\n");
}

uint8_t turnOnBit(uint8_t num, int pos) {
   // 1010(num)
   // 0100(mask bit do segundo indice)
   // 1010 | 0100 OR 1010 | 0x01 << 2
   return num | BIT_MASK(pos);
}

uint8_t turnOffBit(uint8_t num, int pos) {
   // 1010(num)
   // 0010(mask bit do primeiro indice)
   // 1010 & ~0010 OR 1010 & ~(1 << 2)
   return num & ~BIT_MASK(pos); // num AND NOT(1 lshift (pos - 1))
   // 1 < 5 = 100000
}

uint8_t flipBit(uint8_t num, int pos) {
   // 1010
   // 0001
   // 1001
   return num ^ BIT_MASK(pos);
}

void checkBit(uint8_t num, int pos) {
   // 1010
   // 1010 >> 4 == 1
   if (num & BIT_MASK(pos)) {
      printf("Bit %d LIGADO!\n", pos);
   } else printf("Bit %d DESLIGADO!\n", pos);

}


int main()
{ SEP
   // 0000 0000
   uint8_t valor = 19; 
   printf("Decimal: %d\n", valor);
   printBits(valor);

   printf("Ligando Bit de Indice 2!\n");
   valor = turnOnBit(valor, 2);
   printBits(valor);

   printf("Desligando Bit de Indice 5!\n");
   valor = turnOffBit(valor, 5);
   printBits(valor);
   
   printf("Invertendo Bit de Indice 0!\n");
   valor = flipBit(valor, 1);
   printBits(valor);

   printf("Verificando bit de indice 7!\n");
   checkBit(valor, 7);

   printf("Ligando Bit de Indice 7!\n");
   valor = turnOnBit(valor, 8);
   printBits(valor);

   printf("Verificando bit de indice 7!\n");
   checkBit(valor, 7);

SEP return 0;}
 

/*
Considere um byte (8 bits) representado por uma variável uint8_t valor . Escreva código
para: 
(a) setar/ligar o bit de índice 2 (terceiro bit, valendo 1<<2), 
(b) limpar/desligar o bit de índice 5, 
(c) inverter o bit de índice 0, 
(d) testar se o bit de índice 7 está em 1 ou 0. Faça isso usando operações | , & , ^ e shifts. 
Para (d), você pode criar uma máscara e usar if (valor & mascara) para condição (não-zero significa bit=1). 
Teste seu código atribuindo valores em valor e imprimindo o resultado em binário/hexadecimal a cada operação. 
Dica: Para imprimir em binário, você pode escrever uma função que itere pelos bits de um uint8_t
e use printf para cada bit (ou use %d em expressões (valor>>i)&1 ).
*/
