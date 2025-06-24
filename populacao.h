#ifndef POPULACAO_H
#define POPULACAO_H
#include<stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "lista.h"
#include "config.h" //add

/**
 * Cria uma população inicial de indivíduos com caminhos aleatórios.
 * Por exemplo, gera uma população com tamanho baseado na distância de Manhattan.
 * @param Labirinto* - ponteiro para o contexto do labirinto contendo S, E e dimensões
 * @param const Config* - ponteiro para a configuração
 * @return TLinkedList* - lista encadeada contendo a população ou NULL em caso de erro
 * @see calcular_distancia_manhattan(), TSList_create(), list_create()
 */
TLinkedList* criar_populacao(Labirinto*, const Config*);////////////mudeei parametros

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
void simular_populacao(const Labirinto*, TLinkedList*, const Config*);

////////////////////////////////////////////////////////////////////
/**
 * Seleciona um indivíduo da população usando o método da roleta.
 * Indivíduos com maior fitness têm maior probabilidade de serem selecionados.
 * @param TLinkedList* populacao - Ponteiro para a lista encadeada da população.
 * @return TNo* - Ponteiro para o nó (TNo) do indivíduo selecionado, ou NULL em caso de erro.
 */
TNo* selecionar_individuo_roleta(TLinkedList* populacao);

/////////////////////////////////////////////////////////////////////
/**
 * Executa o ciclo principal do algoritmo genético para evoluir a população.
 * @param Labirinto* lab - Ponteiro para a estrutura do labirinto.
 * @param TLinkedList* populacao_atual - Ponteiro para a lista encadeada da população atual.
 * @param const Config* config - Ponteiro para a estrutura de configuração.
 */
void evoluir_populacao(Labirinto* lab, TLinkedList* populacao_atual, const Config* config); 

#endif