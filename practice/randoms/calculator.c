#include <stdio.h>

int main() {
    double num1, num2, resultado;
    char operador;

    printf("==== CALCULADORA EM C ====\n");
    printf("Digite a operação no formato: número operador número (ex: 5 + 3)\n");
    printf("Operadores disponíveis: +  -  *  /\n\n");

    printf("Digite a operação: ");
    scanf("%lf %c %lf", &num1, &operador, &num2);

    switch (operador) {
        case '+':
            resultado = num1 + num2;
            printf("Resultado: %.2lf\n", resultado);
            break;
        case '-':
            resultado = num1 - num2;
            printf("Resultado: %.2lf\n", resultado);
            break;
        case '*':
            resultado = num1 * num2;
            printf("Resultado: %.2lf\n", resultado);
            break;
        case '/':
            if (num2 != 0) {
                resultado = num1 / num2;
                printf("Resultado: %.2lf\n", resultado);
            } else {
                printf("Erro: divisão por zero!\n");
            }
            break;
        default:
            printf("Operador inválido!\n");
            break;
    }

    return 0;
}
