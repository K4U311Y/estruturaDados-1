#ifndef INDIVIDUO_H
#define INDIVIDUO_H
#include "lista.h"
#include "labirinto.h"
#include "config.h" // pra ter acesso a FormaCaminho e Config

#include <stdint.h>
#include <stdio.h>
 
typedef struct _labirinto Labirinto;

typedef struct _individuo {
    Stack* caminho;      // Ponteiro para uma Pilha (Stack)
    int fitness;          
    int tamanho_caminho;  
} Individuo; 

/**
 * Gera um movimento aleatório para o caminho.
 * Por exemplo, pode retornar 'C' (Cima), 'B' (Baixo), etc.
 * @return char movimento aleatório ('C', 'B', 'E' ou 'D')
 */
char movimento_aleatorio();
/**
 * Gera movimento válido 
 * Exemplo: EX: no start tem parede em cima e na esquerda, as opções de movimento são só baixo e direita
 * @return char movimento aleatório 
 */
char movimento_valido_aleatorio(Labirinto* lab, Posicao atual);

/**
 * Simula o movimento de um indivíduo no labirinto e retorna sua posição final.
 * @param Labirinto* - ponteiro para o contexto do labirinto contendo S, E e dimensões
 * @param Individuo* indiv - Ponteiro para o indivíduo que tem os movimentos
 * @param int* colisoes - ponteiro que calcula a quantidade de colisões para fazer o calculo do fitness
 * @return Posicao - posição final após aplicar os movimentos
 */
Posicao simular_movimentos(const Labirinto*, Individuo*, int*, char**);

/**
 * Calcula o fitness de um indivíduo no labirinto
 * @param Labirinto* Ponteiro para o contexto do labirinto
 * @param Individuo* Ponteiro para o indivíduo a ser avaliado
 * @param FormaCaminho forma_geracao - indica a forma como o individuo foi gerado (importante pq muda a penalidade)
 * @param int w_distancia - o peso da distancia, lido do arquivo configuração
 */
void calcular_fitness(const Labirinto*, Individuo*, FormaCaminho, int);

///////////////////////////////////////////////////////////////////////////
/**
 * Clona um indivíduo existente (cópia profunda).
 * Cria um novo indivíduo e copia todos os dados, incluindo o caminho (pilha).
 * @param const Individuo* original - Ponteiro para o indivíduo a ser clonado.
 * @return Individuo - Uma nova estrutura Individuo com dados copiados, ou um indivíduo vazio em caso de falha.
 */
Individuo individuo_clone(const Individuo* original); 



#endif