#ifndef LISTA_H
#define LISTA_H
#include "individuo.h"
#include<stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef unsigned int uint;

typedef struct _listS {
    unsigned int qty; //quantos elementos foram inseridos
    unsigned int capacidade; //max elementos
    char* data; //vetor
} TSList;

typedef struct _no {
    Individuo info;
    struct _no *prox;
} TNo;

typedef struct _list {
    TNo* inicio;
} TLinkedList;

/**
 * Cria uma nova lista encadeada vazia.
 * Por exemplo, cria uma lista com ponteiro inicio apontando para NULL.
 * @return TLinkedList* ponteiro para a lista alocada ou NULL em caso de falha
 */
TLinkedList* list_create();

/**
 * Insere um elemento no início da lista.
 * Por exemplo, inserindo 1, 2, 3 resulta em [3] -> [2] -> [1].
 * @param TLinkedList* ponteiro para a lista
 * @param Individuo informação a ser armazenada
 * @return bool true se a inserção foi bem-sucedida
 */
bool list_insert_begin(TLinkedList*, Individuo);

/**
 * Insere um elemento no final da lista.
 * Por exemplo, inserindo 1, 2, 3 resulta em [1] -> [2] -> [3].
 * @param TLinkedList* ponteiro para a lista
 * @param Individuo informação a ser armazenada
 * @return bool true se a inserção foi bem-sucedida
 */
bool list_insert_end(TLinkedList*, Individuo);

/**
 * Imprime todos os elementos da lista no console.
 * Por exemplo, imprime [1] -> [2] -> [3] para a lista acima.
 * @param TLinkedList* ponteiro para a lista
 */
void list_print(TLinkedList*);

/**
 * Remove o primeiro elemento da lista.
 * Por exemplo, remove [1] de [1] -> [2] -> [3].
 * @param TLinkedList* ponteiro para a lista
 * @return bool true se a remoção foi bem-sucedida
 */
bool list_delete_begin(TLinkedList*);

/**
 * Remove os primeiros n elementos da lista.
 * Por exemplo, remover 2 elementos de [1,2,3] resulta em [3].
 * @param TLinkedList* ponteiro para a lista
 * @param unsigned int número de elementos a remover
 * @return bool true se a remoção foi bem-sucedida
 */
bool list_delete_n(TLinkedList*, unsigned int);

/**
 * Cria uma nova lista estática vazia.
 * Por exemplo, cria uma lista com qty = 0 e data vazio.
 * @param uint - tamanho máximo de elementos que podem ser armazenados 
 * @return TSList* ponteiro para a lista alocada ou NULL em caso de falha
 */
TSList* TSList_create(uint);

/**
 * Insere um caractere no final da lista estática.
 * Por exemplo, inserir 'A', 'B' resulta em data = ['A', 'B'].
 * @param TSList* ponteiro para a lista
 * @param char valor a ser inserido
 * @return bool true se a inserção foi bem-sucedida
 */
bool TSList_insert(TSList*, char);

/**
 * Imprime os elementos da lista estática no console.
 * Por exemplo, imprime "A, B" para a lista acima.
 * @param TSList* ponteiro para a lista
 */
void TSList_print(TSList*);

/**
 * Obtém a quantidade de elementos na lista estática.
 * Por exemplo, retorna 2 para a lista ['A', 'B'].
 * @param TSList* ponteiro para a lista
 * @return unsigned int número de elementos
 */
unsigned int TSList_qty(TSList*);

/**
 * Verifica se a lista estática está vazia.
 * Por exemplo, retorna true para uma lista recém-criada.
 * @param TSList* ponteiro para a lista
 * @return bool true se a lista está vazia
 */
bool TSList_is_empty(TSList*);

/**
 * Verifica se a lista estática está cheia.
 * Por exemplo, retorna true se qty == MAX (100).
 * @param TSList* ponteiro para a lista
 * @return bool true se a lista está cheia
 */
bool TSList_is_full(TSList*);

/**
 * Libera a memória alocada para a lista estática.
 * Por exemplo, libera a memória de uma lista com 2 elementos.
 * @param TSList* ponteiro para a lista
 */
void TSList_free(TSList*);

#endif