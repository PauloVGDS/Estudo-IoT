#include <stdio.h>

int main() {
    int num;
    int aux = 1;
    printf("Digite um numero: ");
    scanf("%d", &num);

    while(aux <= num) {
        int aux2 = 0;
        while (aux2 < aux) {
            printf("%d ", aux2+1);
            aux2++;
        }
        printf("\n");
        aux++;
    }
    return 0;
}

