#include "main.h"

#define BIT(n) (1 << (n)) // Liga o bit N
#define SET_BIT(reg, bit) (reg |= BIT(bit)) // Ligando um BIT específico
#define INV_BIT(reg, bit) (reg ^= BIT(bit)) // Invertendo
#define CLEAR_BIT(reg, bit) (reg &= ~BIT(bit)) // Desligando
#define CHECK_BIT(reg, bit) (reg & BIT(bit)) // Verificando, retorna (true para != 0 e false para == 0)
// 7 6 5 4 3 2 1 0
// 0 0 0 1 0 1 0 0
// 1 1 1 1 1 0 1 1
// 0 0 0 1 0 0 0 0 

int main()
{ SEP
   int reg = 0x00;
   printf("Inicio: %d \n", reg);

   reg = SET_BIT(reg, 2);
   printf("Bit 2 ativo: %d \n", reg);
   
   reg = INV_BIT(reg, 2);
   printf("Bit 2 limpo: %d \n", reg);

   reg = SET_BIT(reg, 4);
   printf("Bit 4 ligado: %d \n", reg);
   
   
   reg = SET_BIT(reg, 5);
   printf("Bit 5 ligado: %d \n", reg);
   
   reg = CLEAR_BIT(reg, 4);
   printf("Bit 4 desligado: %d \n", reg);
   
   printf("Fim: %d \n", reg);
   


SEP return 0;}


/*
Utilizando macros, defina: #define BIT(n) (1 << (n)) . Use essa macro para criar
máscaras para bits de 0 a 7 facilmente. Depois defina macros para manipulação de registrador,
por exemplo:
#define BIT(n) (1 << (n))
#define SET_BIT(reg, bit) ((reg) |= BIT(bit))
#define CLEAR_BIT(reg, bit) ((reg) &= ~BIT(bit))
#define TOGGLE_BIT(reg, bit) ((reg) ^= BIT(bit))
#define CHECK_BIT(reg, bit) ((reg) & BIT(bit))
Crie uma variável uint8_t reg = 0x00; e teste essas macros: ligue alguns bits, verifique se
CHECK_BIT acusa verdadeiro, desligue bits, etc., imprimindo o valor de reg a cada passo em
hexadecimal. Essas macros são semelhante
*/