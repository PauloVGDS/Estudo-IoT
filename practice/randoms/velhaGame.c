#include <stdio.h>

int main() {
    int rodadas = 0;
    int firstPlayerWins = 0;
    int secondPlayerWins = 0;
    int firstJogada = 0;
    int secondJogada = 0;
    printf("Pedra / Papel / Tesoura\n");
    printf("O primeiro a vencer 3 rodadas ganha!\n");

    while (rodadas < 3) {
        printf("Para jogar, digite:\n");
        printf("1 - Pedra\n2 - Papel\n3 - Tesoura\n");
    
    
        printf("Player 1: ");
        scanf("%i", &firstJogada);
    
        printf("Player 2: ");
        scanf("%i", &secondJogada);
        
        // Empate
        if (firstJogada == secondJogada) {
            printf("Empate! Repetindo Rodada.\n");
            continue;
        }
        
        // Pedra
        if (firstJogada == 1 && secondJogada == 3) {
            firstPlayerWins++;
            printf("Vitoria do Jogador 1\n");
            printf("Pedra > Tesoura\n");
        } else if (firstJogada == 1 && secondJogada == 2) {
            secondPlayerWins++;
            printf("Vitoria do Jogador 2\n");
            printf("Pedra < Papel\n");
        }
    
        // Papel
        if (firstJogada == 2 && secondJogada == 1) {
            firstPlayerWins++;
            printf("Vitoria do Jogador 1\n");
            printf("Papel > Pedra\n");
        } else if (firstJogada == 2 && secondJogada == 3) {
            secondPlayerWins++;
            printf("Vitoria do Jogador 2\n");
            printf("Papel < Tesoura\n");
        }
    
        // Tesoura
        if (firstJogada == 3 && secondJogada == 2) {
            firstPlayerWins++;
            printf("Vitoria do Jogador 1\n");
            printf("Tesoura > Papel\n");
        } else if (firstJogada == 3 && secondJogada == 1) {
            secondPlayerWins++;
            printf("Vitoria do Jogador 2\n");
            printf("Tesoura < Pedra\n");
        }
        rodadas++;
    }
    
    if (firstPlayerWins >= 2) {
        printf("Jogador 1 foi o melhor das 3 rodadas com %i vitorias!\n", firstPlayerWins);
    }
    if (secondPlayerWins >= 2) {
        printf("Jogador 2 foi o melhor das 3 rodadas com %i vitorias!\n", secondPlayerWins);

    }

    return 0;
}      