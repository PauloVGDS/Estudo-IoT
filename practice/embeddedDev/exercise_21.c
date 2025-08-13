#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <ctype.h>

#define SEP printf("\n==============================================================\n\n");
// O = 79
// X = 88
// Vitória se: X = 264 ou O = 237

int ganhador = 0;
int jogadas = 0;

void exibirJogada(char velha[3][3])
{
   for (int l = 0; l < 3; l++)
   {
      for (int c = 0; c < 3; c++)
      {

         printf("[ ");
         printf("%c", velha[l][c]);
         printf(" ] ");
      }
      printf("\n");
   }
}

void fazerJogada(char velha[3][3])
{
   int l = 0;
   int c = 0;
   char jogada;
   printf("\nEscolha a posicao da sua jogada!\n");

   printf("Linha(1 a 3): ");
   scanf("%i", &l);

   printf("Coluna(1 a 3): ");
   scanf("%i", &c);

   printf("\nFaca sua jogada: ");
   scanf(" %c", &jogada);
   jogada = toupper(jogada);
   printf("\n");
   if (velha[l - 1][c - 1] == ' ') {
      velha[l - 1][c - 1] = jogada;
      jogadas++;
   }
   else printf("\nJogada Invalida!\n");
   
}

int verificarVitoria(char velha[3][3])
{
   int somalin = 0;
   int somacol = 0;
   int somadiag1 = 0;
   int somadiag2 = 0;

   if (jogadas >= 9) return -1;
   for (int l = 0; l < 3; l++)
   {
      for (int c = 0; c < 3; c++)
      {
         somalin += velha[l][c]; // Linhas
         somacol += velha[c][l]; // Colunas
         somadiag1 += velha[c][c]; // Diagonal da Esquerda pra Direita
         somadiag2 += velha[3 - l][c]; // Diagonal da Direita pra Esquerda
      }
      
      switch (somalin)
      {
      case 264:
         printf("O Jogador 1 ganhou");
         return 1;
      
      case 237:
         printf("O Jogador 2 ganhou");
         return 1;
      } 

      switch (somacol)
         {
         case 264:
            printf("O Jogador 1 ganhou");
            return 1;
         
         case 237:
            printf("O Jogador 2 ganhou");
            return 1;
         }

      switch (somadiag1)
         {
         case 264:
            printf("O Jogador 1 ganhou");
            return 1;
         
         case 237:
            printf("O Jogador 2 ganhou");
            return 1;
         }

      switch (somadiag2)
         {
         case 264:
            printf("O Jogador 1 ganhou");
            return 1;
         
         case 237:
            printf("O Jogador 2 ganhou");
            return 1;
         }

      somalin = 0;
      somacol = 0;
      somadiag1 = 0;
      somadiag2 = 0;
   }

   return 0;
}

int main()
{
   SEP char velha[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};

   printf("Jogo da Velha\n");
   printf("Jogador 1: X\n");
   printf("Jogador 2: O\n");
   SEP
   exibirJogada(velha);
   int count = 0;
   while (ganhador == 0)
   {
      printf("\nVez do Jogador %i\n", (count % 2) + 1);
      count++;
      fazerJogada(velha);
      exibirJogada(velha);
      ganhador = verificarVitoria(velha);
   }

   if (ganhador == -1) printf("Deu velha!");
   

   SEP return 0;
}
// Verificar o número da jogada é válido
// Verificar se a posição não já foi usada