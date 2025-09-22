#include "main.h"

// Forma 1:
/*
struct Person
{
   char nome[50];
   int idade;
   float peso;
}; 
*/
// Refere-se como: struct Person [nome-da-instância] 


// Forma 2:
/* 
typedef struct 
{
   char nome[50];
   int idade;
   float peso;
} Person; 
*/
// Refere-se como: Person [nome-da-instância]




typedef struct
{
   char nome[50];
   int idade;
   float peso;
} Person;

void printPerson(Person *persona) {
   printf("Nome: %s\nIdade: %i\nPeso: %.2f\n", persona->nome, persona->idade, persona->peso);
}


int main()
{ SEP
   Person p1 = {"Paulo Vinicius", 21, 69.7};

   printPerson(&p1);

   SEP 
return 0;}


