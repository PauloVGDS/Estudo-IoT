// Remova um elemento de um array de 8 posições (dado um índice), deslocando os elementos para a esquerda
#include <stdio.h>

void main() {
    int array[8] = {37, 82, 5, 69, 21, 44, 93, 16};
    int index;

    printf("Digite um numero: ");
    scanf("%d", &index);

    for (int i = 0; i < 8; i++) {
        if (i >= index) {
                array[i] = array[i+1];
        }
        if (i == 7) {
            array[i] = 0;
        }
        printf("%d ", array[i]);
    }
    
}