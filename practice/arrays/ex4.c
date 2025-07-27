// Calcule a média de uma turma de 5 alunos, lendo as notas e mostrando a média no final
// &variavel = passa o endereço da variável
// *variavel = acessa ou altera o valor dessa variável
#include <stdio.h>

void main() {
    int array[5];
    int media = 0;
    char *text = "Paulo";

    for (int i = 0; i < 5; i++) {
        printf("Digite um numero: ");
        scanf("%d", &array[i]);
        media += array[i];
    }
    
    printf("Media: %d\n", media / 5);
}