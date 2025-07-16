#include "main.h"

#define BIT(num) (1 << num)

void setBitValor(uint8_t *valor, int bit);    // Seta o bit
void clearBitValor(uint8_t *valor, int bit);  // Limpa o bit  
int testBitValor(uint8_t valor, int bit);     // Testa o bit
void toggleBitValor(uint8_t *valor, int bit); // Inverte o bit

int main()
{ SEP

   uint8_t valor = 0x00;
   printf("%d", BIT(4) ^ valor);


SEP return 0;}

void setBitValor(uint8_t *valor, int bit) {
   return 
}


/*
Reutilize a struct Registro8b da semana anterior. Implemente funções que usem operadores
bit a bit em vez de campos da struct para as operações de setar/limpar/testar bits do registro.
Por exemplo, void setBitValor(uint8_t *valor, int bit) que usa as macros ou operadores para alterar o byte apontado. 
Compare essa abordagem com a de usar bit fields: qual pareceu mais simples ou eficiente? (Dica: bit fields isolam bits diretamente como membros,
mas podem sofrer influência de endianness e alinhamento; já as operações bitwise são explícitas
e portáveis, o que as torna muito usadas).
*/