#ifndef INDIVIDUO_H
#define INDIVIDUO_H
#include "lista.h"
#include "labirinto.h"
#include<stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct _individuo {
    TSList* caminho;      // Lista de movimentos ('C', 'B', 'E', 'D')
    int fitness;          // Valor que indica a qualidade da solução
    int tamanho_caminho;  // Número de movimentos no caminho
} Individuo;

/**
 * Gera um movimento aleatório válido para o caminho.
 * Por exemplo, pode retornar 'C' (Cima), 'B' (Baixo), etc.
 * @return char movimento aleatório ('C', 'B', 'E' ou 'D')
 */
char movimento_aleatorio();

/**
 * Simula o movimento de um indivíduo no labirinto e retorna sua posição final.
 * @param Labirinto* - ponteiro para o contexto do labirinto contendo S, E e dimensões
 * @param Individuo* indiv - Ponteiro para o indivíduo que tem os movimentos
 * @param int* colisoes - ponteiro que calcula a quantidade de colisões para fazer o calculo do fitness
 * @return Posicao - posição final após aplicar os movimentos
 */
Posicao simular_movimentos(const Labirinto*, Individuo*, int*);

/**
 * Calcula o fitness de um indivíduo no labirinto
 * @param Labirinto* Ponteiro para o contexto do labirinto
 * @param Individuo* Ponteiro para o indivíduo a ser avaliado
 */
void calcular_fitness(const Labirinto*, Individuo*);


#endif