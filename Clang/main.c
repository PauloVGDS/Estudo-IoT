#include "main.h"

#define BIT(bit) (1 << (bit))

void criptoText(char text[], int key) {
   for (int i = 0; i < strlen(text); i++)
  {
   text[i] ^= key;
  }
  printf("\nSua frase criptografada e: %s", text);
}


int main()
{ SEP
   char text[100]; // Buffer para armazenar a mensagem
   int key = 0;

  do {
    printf("\nDigite sua frase: ");
    fgets(text, sizeof(text), stdin); // Lê o input do usuário
    text[strcspn(text, "\n")] = 0; // Remove o caractere de linha nova se presente'
  } while (strlen(text) == 0); // Repete se o input ou chave está vazio

  do
  {
   printf("\nDigite a chave: ");
   scanf("%d", &key);
  } while (key <= 0 || key >= 256);
  
  criptoText(text, key);
  criptoText(text, key);

SEP return 0;}
