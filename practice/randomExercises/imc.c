/* Escreva um programa que calcule o índice de massa corporal (IMC). Solicite ao usuário
o peso (kg) e a altura (m), então calcule o IMC ( peso/(altura*altura) ) e mostre em qual
faixa a pessoa se encontra (baixo peso, peso normal, sobrepeso ou obesidade). Isso exige usar
float e printf com formatação para uma ou duas casas decimais. */

#include <stdio.h>
#include <math.h>

float calcImc(float peso, float altura);
int faixasImc(float imc);

int main() {
    float peso = 0;
    float altura = 0;
    float result = 0;

    printf("Calculo de indice de massa corporal(IMC)\n");
    printf("Digite seu peso: ");
    scanf("%f", &peso);

    printf("Digite sua altura: ");
    scanf("%f", &altura);

    printf("Sua altura: %.2f\nSeu peso: %.2f\n", altura, peso);

    result = calcImc(peso, altura);

    printf("Seu IMC: %.2f\n", result);
    faixasImc(result);
    return 0;
}


float calcImc(float peso, float altura) {
    float imc = peso/pow(altura, 2);
    return imc;
}

int faixasImc(float imc) {
    if (imc < 18.5) {
        printf("Estado de Magreza\n");
        printf("Procure um médico\n");
    } else if (imc > 18.5 && imc < 24.9) {
        printf("Estado Saudavel\n");
    } else if (imc > 25.0 && imc < 29.9) {
        printf("Estado de sobrepeso\n");
    } else if (imc > 30.0 && imc < 34.9) {
        printf("Estado de Obsidade Grau I\n");
    } else if (imc > 35 && imc < 39.9) {
        printf("Estado de Obsidade Grau II(severa)\n");
    } else if (imc > 40.0) {
        printf("Estado de obsidade Grau III(morbida)\n");
    }
    return 0;
}