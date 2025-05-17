#ifndef LABIRINTO_H
#define LABIRINTO_H
#include<stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef unsigned int uint;

typedef struct _listS {
    unsigned int qty;
    char data[100];
} TSList;

typedef struct _individuo {
    TSList* caminho;      // Lista de movimentos ('C', 'B', 'E', 'D')
    int fitness;          // Valor que indica a qualidade da solução
    int tamanho_caminho;  // Número de movimentos no caminho
} Individuo;

typedef struct _no {
    Individuo info;
    struct _no *prox;
} TNo;

typedef struct _list {
    TNo* inicio;
} TLinkedList;

typedef struct {
    unsigned int i, j;
} Posicao;

typedef struct _labirinto {
    char** labirinto;
    uint n, m;
    Posicao inicio;
    Posicao saida;
    int penalidade;
} Labirinto;

/**
 * Aloca uma matriz de caracteres com dimensões n x m.
 * Por exemplo, para um labirinto 6x10, aloca 6 linhas de 10 caracteres cada.
 * @param uint número de linhas da matriz
 * @param uint número de colunas da matriz
 * @return char** ponteiro para a matriz alocada ou NULL em caso de falha
 */
char** alocar_matriz(uint, uint);

/**
 * Libera a memória de uma matriz previamente alocada.
 * Por exemplo, libera uma matriz 6x10 alocada por alocar_matriz().
 * @param char** ponteiro para a matriz
 * @param uint número de linhas da matriz
 */
void liberar_matriz(char**, uint);

/**
 * @brief Exibe informações completas sobre o labirinto carregado
 * 
 * @param char** Matriz de caracteres representando o labirinto
 * @param uint Número de linhas do labirinto
 * @param uint Número de colunas do labirinto
 * @param Labirinto* Ponteiro para a estrutura Labirinto com metadados
 */
int labirinto_print(char**, uint, uint, const Labirinto*);

/**
 * Carrega um labirinto a partir de um arquivo já aberto para a matriz.
 * @param labirinto Matriz destino (já alocada)
 * @param n Número de linhas da matriz
 * @param m Número de colunas da matriz
 * @param arquivo Ponteiro FILE* já aberto no modo leitura ("r")
 * @return bool true se o labirinto foi carregado com sucesso
 */
bool criar_matrizLab(char**, uint, uint, FILE*);

/**
 * Cria uma população inicial de indivíduos com caminhos aleatórios.
 * Por exemplo, gera uma população com tamanho baseado na distância de Manhattan.
 * @param Labirinto* - ponteiro para o contexto do labirinto contendo S, E e dimensões
 * @param uint - tamanho da população dado pelo usuário
 * @return TLinkedList* - lista encadeada contendo a população ou NULL em caso de erro
 * @see calcular_distancia_manhattan(), TSList_create(), list_create()
 */
TLinkedList* criar_populacao(Labirinto*, uint);

/**
 * Gera um movimento aleatório válido para o caminho.
 * Por exemplo, pode retornar 'C' (Cima), 'B' (Baixo), etc.
 * @return char movimento aleatório ('C', 'B', 'E' ou 'D')
 */
char movimento_aleatorio();

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
 * Simula o movimento de um indivíduo no labirinto e retorna sua posição final.
 * @param char** labirinto - matriz do labirinto
 * @param uint n - número de linhas
 * @param uint m - número de colunas
 * @param TSList* caminho - listda de movimentos do inivíduo
 * @param Posicao inicio - posição inicial (S)
 * @return Posicao - posição final após aplicar os movimentos
 */
Posicao simular_movimentos(const Labirinto*, TSList*);

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

/**
 * Encontra as posições de 'S' (início) e 'E' (saída) no labirinto.
 * @param char** labirinto - matriz do labirinto
 * @param uint n - número de linhas
 * @param uint m - número de colunas
 * @param Posicao* inicio - ponteiro para armazenar a posição de 'S'
 * @param Posicao* saida - ponteiro para armazenar a posição de 'E'
 * @return bool - true se ambas as posições foram encontradas
 */
bool encontrar_posicoes_SE(char**, uint, uint, Posicao*, Posicao*);

/**
 * Calcula o fitness de um indivíduo no labirinto
 * @param lab Ponteiro para o contexto do labirinto
 * @param indiv Ponteiro para o indivíduo a ser avaliado
 */
void calcular_fitness(const Labirinto*, Individuo*);

/**
 * Cria e inicializa uma estrutura de contexto do labirinto com todas as informações necessárias.
 * Por exemplo, armazena as dimensões do labirinto, posições de início/saída e penalidade.
 * @param char** labirinto - matriz do labirinto
 * @param uint n - número de linhas do labirinto
 * @param uint m - número de colunas do labirinto
 * @param int penalidade - valor da penalidade por colisão com paredes
 * @return ContextoLabirinto* - ponteiro para a estrutura alocada ou NULL em caso de falha
 */
Labirinto* criar_contexto(char**, uint, uint, int);

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
 * @return TSList* ponteiro para a lista alocada ou NULL em caso de falha
 */
TSList* TSList_create();

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