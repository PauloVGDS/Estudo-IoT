// Conte quantos valores pares e ímpares existem em um array de 10 elementos fornecidos pelo usuário
#include <stdio.h>

void main() {
    int array[10]; 
    int pares, impares;

    pares = impares = 0;

    for (int i = 0; i < 10; i++) {
        printf("Digite um numero: ");
        scanf("%d", &array[i]);
    }
    


    for (int i = 0; i < 10; i++) {
        if (array[i] % 2 == 0) {
            pares++;
        } else {
            impares++;
        }
        
    }
    printf("Pares: %d\n", pares);
    printf("Impares %d\n", impares);   
}