#include "populacao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_POPU 100
#define TAM_CAMINHO 50

typedef struct _individuo{
    char caminho[TAM_CAMINHO];
    int fitness;
};

typedef struct {
    unsigned int i, j;
} Posicao;

char movimento_aleatorio() {
    char movimentos[] = {'C', 'B', 'E', 'D'};
    return movimentos[rand() % 4];
}

TLinkedList* criar_pupulacao (Individuo populacao[]){
    for (int i = 0; i < TAM_POPU; i++)
            for (int j = 0; j < TAM_CAMINHO; j++)
                populacao[i].caminho[j] = movimento_aleatorio();
}


