// Encontre o maior e o menor valor em um array de 8 números inteiros fornecidos pelo usuário.
#include <stdio.h>

void main() {
    int menor = 0;
    int maior = 0;
    int array[8];


    for (int i = 0; i < 8; i++) {
        printf("Digite um numero: ");
        scanf("%d", &array[i]);
    }

    menor = maior = array[7];

    for (int i = 0; i < 8; i++) {
        if (menor > array[i]) {
            menor = array[i];
        } 
        if (maior < array[i]) {
            maior = array[i];
        }
    }
    printf("Maior: %d\nMenor: %d", maior, menor);

}
