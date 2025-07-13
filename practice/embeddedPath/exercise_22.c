#include "main.h"

// struct NomeDaStruct
struct Pessoa
{
   char nome[30];
   int idade;
};



int main()
{ SEP

   // struct NomeDaStruct NomeDaInstância
   struct Pessoa p1 = {"Paulo Vinicius", 21};
   struct Pessoa *ptr = &p1;
   // A variável é a própria struct.
   printf("%s\n", p1.nome);

   // A variável é um ponteiro para a struct.
   printf("%s\n", ptr->nome);
   // A variável é um ponteiro, logo se acessarmos o valor que esse ponteiro armazena(a struct), consiguimos usar o operador '.'
   // Logo isso é equivalente ao operador '->'
   printf("%s\n", (*ptr).nome);

   SEP 
return 0;}


