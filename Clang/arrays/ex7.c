// Crie um array de 10 elementos e verifique se um número digitado pelo usuário está presente no array
#include <stdio.h>

void main() {
    int array[10] = {37, 82, 5, 69, 21, 44, 93, 16, 58, 73};
    int num;
    printf("Digite um numero: ");
    scanf("%d", &num);

    for (int i = 0; i < 10; i++) {
        if (num == array[i]) {
            printf("Numero Encontrado!");
            return;
        }
    }
    printf("Numero Nao Encontrado!");

}