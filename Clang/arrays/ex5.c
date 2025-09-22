// Dado um array de 6 elementos, inverta a ordem dos elementos e imprima o resultado.
#include <stdio.h>


void main() {
    int array[6] = {1, 2, 3, 4, 5, 6};
    int aux[6];

    for (int i = 0; i < 6; i++)
    {
        aux[i] = array[5-i];
        
    }

    for (int i = 0; i < 6; i++)
    {
        array[i] = aux[i];
        printf("%d ", array[i]);
    }
}