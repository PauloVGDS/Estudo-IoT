#include "main.h"

#define SIGNE(num) (num >> 31)
#define INV(num) (num ^ 0xFFFFFFFF) // Comparação bit a bit XOR
// Também poderia ser (~num), comparação NOT, apenas inverte o bit.  



int absolute(int num) {
   int positivo = SIGNE(num);
   return (num ^ positivo) - positivo;
}


int main()
{ SEP
   int num;

   printf("Digite um numero: ");
   scanf("%d", &num);
   printf("\n");
   printf("Valor absoluto: %d\n", absolute(num));



SEP return 0;}

/*
Faça um programa que recebe do usuário um número inteiro e exibe seu valor absoluto. 
Porém, não use funções prontas ou operador de comparação – utilize operações bit a bit. 
Dica: Use o fato de que em C o tipo int é com sinal na representação complemento de dois. 
Você pode obter o bit de sinal (mais significativo) deslocando à direita por 31 (assumindo 32 bits). 
Em seguida, pode usar esse bit como máscara para condicionar a inversão de todos os bits e
adicionar 1 (que é como se calcula o negativo de um número em complemento de dois). Esse
exercício é avançado e explora como números negativos são representados, portanto encare
como desafio opcional para raciocínio.

*/