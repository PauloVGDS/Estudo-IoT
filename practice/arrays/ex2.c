#include <stdio.h>
#include <stdio.h>

int main() {
    int numeros[10];
    int soma = 0;
    // TODO: leia 10 valores e acumule em 'soma'
    for(int i = 0; i < 10; i++) {
        printf("Digite um numero: ");
        scanf("%d", &numeros[i]);
        soma += numeros[i];
    }
    // TODO: exiba o resultado
    printf("Soma = %d\n", soma);
    return 0;
}
