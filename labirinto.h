#ifndef LABIRINTO_H
#define LABIRINTO_H
#include<stdbool.h>

typedef struct _list TLinkedList;

// Carrega o labirinto do arquivo especificado para a matriz labirinto.
// Retorna o número de linhas lidas ou -1 em caso de erro.
TLinkedList* list_create(unsigned int, unsigned int);
bool list_insert_end(TLinkedList*, char, unsigned int, unsigned int);
void list_print_matrix(TLinkedList*);
void list_print(TLinkedList*);

#endif