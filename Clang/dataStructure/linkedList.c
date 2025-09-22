#include "main.h"

struct Node {
   int num;
   struct Node* ptr;
};

void printNodes(struct Node* list) {
   while (list != NULL)
   {
      printf("%d\n", list->num);
      list = list->ptr;
   }
   
}

struct Node* createNode(struct Node* list, int num) {
   // Se for o primeiro da lista.
   if (list == NULL) {
      list = malloc(sizeof(struct Node));
      list->num = num;
      list->ptr = NULL;
      return list;
   }

   struct Node* listAux = list;
   while (listAux->ptr != NULL) listAux = listAux->ptr; // Avança até o último nó com valor.

   struct Node* novoNo = malloc(sizeof(struct Node)); // Cria o nó que será adicionado.
   novoNo->num = num;
   novoNo->ptr = NULL;

   //Adionar o novo nó
   listAux->ptr = novoNo;

   return list;
}

struct Node* insertNode(struct Node* list, int num) {
   // Insere um novo nó no inicio.
   struct Node* novoNo = malloc(sizeof(struct Node));
   novoNo->num = num;
   novoNo->ptr = list;
   return novoNo;
}

void freeNodes(struct Node* list) {
   struct Node* noAux = NULL;
   while (list != NULL) {
      noAux = list;
      list = list->ptr;
      free(noAux);
   }
   

}

void removeByValue(struct Node* list) {
   struct Node* listAux = list;
   struct Node* prevNode;
   prevNode = list;
   listAux = listAux->ptr;
   free(list);
   list = NULL;
   printf("\n%d\n", list->num);
   printf("\n%d\n", prevNode->num);
   printf("\n%d\n", listAux->num);


}

int main()
{ SEP

   struct Node* lista = NULL;
   lista = createNode(lista, 10);
   lista = createNode(lista, 20);
   lista = createNode(lista, 30);
   lista = createNode(lista, 40);
   lista = createNode(lista, 50);
   lista = insertNode(lista, 5);
   printNodes(lista);

   removeByValue(lista);
   printNodes(lista);

   freeNodes(lista);
   lista = NULL;

SEP return 0;}

/*
Inserir no início - FEITO
Remover por valor
Buscar elemento - FEITO
Contar nós - FEITO
Inverter lista
*/


// list -> ptr -> ptr -> ptr