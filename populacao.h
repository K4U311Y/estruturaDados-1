#ifndef POPULACAO_H
#define POPULACAO_H
#include<stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "lista.h"
#include "config.h"

/**
 * Cria uma população inicial de indivíduos com caminhos aleatórios.
 * Por exemplo, gera uma população com tamanho baseado na distância de Manhattan.
 * @param Labirinto* - ponteiro para o contexto do labirinto contendo S, E e dimensões
 * @param uint - tamanho da população dado pelo usuário
 * @return TLinkedList* - lista encadeada contendo a população ou NULL em caso de erro
 * @see calcular_distancia_manhattan(), TSList_create(), list_create()
 */
TLinkedList* criar_populacao(Labirinto*, uint, FormaCaminho);

/**
 * Libera toda a memória alocada para uma população.
 * Por exemplo, libera todos os indivíduos e seus caminhos.
 * @param TLinkedList* ponteiro para a lista de população
 */
void liberar_populacao(TLinkedList*);

/**
 * Imprime toda a população no console, mostrando cada indivíduo com seu caminho e fitness.
 * Por exemplo, imprime "Indivíduo 1: Caminho: [C, B, E], Fitness: 10".
 * @param TLinkedList* ponteiro para a lista de população
 */
void print_populacao(TLinkedList*);

/**
 * Simula e avalia toda a população.
 * Calcula fitness e posições finais para todos os indivíduos.
 * 
 * @param Labirinto* Ponteiro para o contexto do labirinto já inicializado, contendo:
 *- labirinto: matriz do labirinto
 *- n, m: dimensões
 *- inicio: posição inicial (S)
 *- saida: posição destino (E)
 *- penalidade: valor da penalidade por colisão
 * @param TLinkedList Lista encadeada contendo a população a ser avaliada
 */
void simular_populacao(const Labirinto*, TLinkedList*);

////////////////////////////////////////////////////////////////////
int calcular_fitness_total(TLinkedList* populacao);

////////////////////////////////////////////////////////////////////
/**
 * @brief Seleciona um indivíduo da população usando o método da roleta.
 *
 * @param populacao A lista encadeada da população.
 * @param fitness_total A soma total dos fitness de todos os indivíduos na população.
 * @return Um ponteiro para o indivíduo selecionado na população.
 * Retorna NULL se a população for vazia ou o fitness_total for <= 0.
 */
Individuo* selecionar_pai_roleta(TLinkedList* populacao, int fitness_total);

Individuo crossover(Individuo pai1, Individuo pai2);
void mutar(Individuo* indiv, Labirinto* lab);
void preservar_elites(TLinkedList* populacao_antiga, TLinkedList* nova_geracao, int num_elites);
TLinkedList* criar_nova_geracao(TLinkedList* populacao_antiga, Labirinto* lab, Config config);
int calcular_fitness_total(TLinkedList* populacao);

Individuo copiar_individuo(const Individuo* original); 

#endif