#ifndef LIST_H
#define LIST_H
#include<stdbool.h>

typedef struct _list TLinkedList;
typedef struct _listS TSList;

/**
 * Cria instâncias da lista simplesmente encadeada.
 * @return TLinkedList* ponteiro para a lista alocada
 * */
TLinkedList* list_create();
/**
 * Tenta fazer a inserção no início da lista. 
 * Por exemplo, se a sequência de inserção for 1, 2, 3, 
 * a lista será [3] -> [2] -> [1]
 * @param TLinkedList* ponteiro para uma instância da lista
 * @param int informação a ser armazenada na lista
 * @return bool informando o sucesso da operação
 */
bool list_insert_begin(TLinkedList*, int);
/**
 * Tenta fazer a inserção no fim da lista. 
 * Por exemplo, se a sequência de inserção for 1, 2, 3, 
 * a lista será [1] -> [2] -> [3]
 * @param TLinkedList* ponteiro para uma instância da lista
 * @param int informação a ser armazenada na lista
 * @return bool informando o sucesso da operação
 */
bool list_insert_end(TLinkedList*, int);
/**
 * Faz a impressão da lista para checagem.
 * @param TLinkedList* ponteiro para uma instância da lista
 */

 /**
 * Tenta deletar o elemento do início. 
 * Por exemplo, se a lista é [1] -> [2] -> [3], apos a chamada, 
 * a Lista se tornará [2] -> [3]
 * @param TLinkedList* ponteiro para uma instância da lista
 * @return bool informando o sucesso da operação
 */
bool list_delete_begin(TLinkedList*);

void list_print(TLinkedList*);
/**
 * Concatenta duas listas.
 * @param TLinkedList* lista 1
 * @param TLinkedList* lista 2
 * @return TLinkedList* instância da terceira lista concatenada
 */
TLinkedList* list_concatenate(TLinkedList*, TLinkedList*);

/**
 * Tenta remover os n primeiros elementos da lista.
 * @param TLinkedList* lista 1
 * @param unsigned int n 
 * @return bool informando o sucesso da operação
 */
bool list_delete_n(TLinkedList*, unsigned int);

/**
 * Cria uma nova lista estática.
 * @return TSList* Ponteiro para a instância da lista criada. Retorna NULL em caso de falha na alocação.
 */
TSList* TSList_create();

/**
 * Insere um novo elemento com o valor fornecido no final da lista.
 * @param TSList* list Ponteiro para a lista na qual o elemento será inserido.
 * @param int value Valor inteiro a ser inserido na lista.
 * @return bool Retorna true se a inserção foi bem-sucedida, false caso contrário (ex: lista nula ou falha na alocação de memória).
 */
bool TSList_insert(TSList*, int);

/**
 * Imprime os elementos da lista, separados por espaço.
 * @param TSList* list Ponteiro para a lista a ser impressa. Se a lista for nula, nada é impresso.
 */
void TSList_print(TSList* );

/**
 * Retorna a quantidade de elementos presentes na lista.
 * @param TSList* list Ponteiro para a lista da qual se deseja obter a quantidade de elementos.
 * @return unsigned int Número de elementos na lista. Retorna 0 se a lista for nula.
 */
unsigned int TSList_qty(TSList* );

/**
 * Verifica se a lista está vazia (não contém nenhum elemento).
 * @param TSList* list Ponteiro para a lista a ser verificada.
 * @return bool Retorna true se a lista estiver vazia, false caso contrário ou se o ponteiro da lista for nulo.
 */
bool TSList_is_empty(TSList* );

/**
 * Verifica se a lista está cheia. A implementação de listas simplesmente encadeadas dinâmicas geralmente não tem um limite fixo,
 * portanto, esta função pode sempre retornar false a menos que haja alguma restrição de implementação específica.
 * @param TSList* list Ponteiro para a lista a ser verificada.
 * @return bool Retorna true se a lista estiver cheia (de acordo com alguma possível restrição de implementação), false caso contrário ou se o ponteiro da lista for nulo.
 */
bool TSList_is_full(TSList* );

#endif
