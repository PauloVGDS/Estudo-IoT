#include "main.h"

#define BIT(num) (1 << num) // Desloca o 0x01 'num' vezes pra esquerda
#define UPBIT(valor, bit) (valor |= BIT(bit)) // Comparação OR pra ligar o bit
#define DOWNBIT(valor, bit) (valor &= ~BIT(bit)) // Comparação AND com bit invertido pra desligar o bit 
#define TOGGLEBIT(valor, bit) (valor ^= BIT(bit)) // Liga ou desliga o bit com o operador XOR que inverte o estado
#define TESTBIT(valor, bit) ((valor) & BIT(bit)) // Operação AND pra testar se o bit está ativo, se retorna qualquer valor diferente de 0 o bit está ativo


int main()
{ SEP

   uint8_t valor = 0x01;
   DOWNBIT(valor, 7);
   UPBIT(valor, 7);
   printf("%d \n", valor);
   printf("%d", TESTBIT(valor, 2));

SEP return 0;}