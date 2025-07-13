// Insira um número em um array ordenado (de até 10 elementos), mantendo a ordem crescente
#include <stdio.h>

int main() {
    int array[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int newArray[11];
    int num;
    int stop;
    printf("Digite um numero: ");
    scanf("%d", &num);


    for (int i = 0; i < 11; i++)
    {
        if (array[i] < num && i != 10) {
            newArray[i] = array[i];
            printf("%d ", newArray[i]);
        } else {
            newArray[i] = num;
            stop = i;
            printf("%d ", newArray[i]);
            break;
        }
        
    }

    for (int i = stop + 1; i < 11; i++) {
        newArray[i] = array[i-1];
        printf("%d ", newArray[i]);
    }
    printf("\n");
    return 0;
}