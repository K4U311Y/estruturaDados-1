#include "labirinto.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

char movimento_aleatorio() {
    char movimentos[] = {'C', 'B', 'E', 'D'};
    return movimentos[rand() % 4];
}

int calcular_distancia_manhattan(Posicao a, Posicao b) {
    return abs((int)a.i - (int)b.i) + abs((int)a.j - (int)b.j); //o abs é módulo, ou seja, a distância sempre vai dar positiva
}

TLinkedList* criar_populacao(Labirinto* lab, uint tamanho_populacao) {
    if(!lab) return NULL;
    int dist = calcular_distancia_manhattan(lab->inicio, lab->saida);
    TLinkedList* populacao = list_create();
    if(!populacao) return NULL;

    for(int i = 0; i < tamanho_populacao; i++) {
        Individuo ind;
        /*
        Quando fazemos rand() % (dist + 1), os possíveis valores passam a ser: 0, 1, 2, ..., dist
        o taminho pode ser o minimo = dist e o max de dist+dist
        */
        ind.tamanho_caminho = dist + (rand() % (dist + 1));
        ind.caminho = TSList_create(ind.tamanho_caminho);
        ind.fitness = 0;

        if(!ind.caminho) {
            liberar_populacao(populacao);
            return NULL;
        }

        for(int j = 0; j < ind.tamanho_caminho; j++) {
            if(!TSList_insert(ind.caminho, movimento_aleatorio())) {
                TSList_free(ind.caminho);
                liberar_populacao(populacao);
                return NULL;
            }
        }

        calcular_fitness(lab, &ind);

        if(!list_insert_end(populacao, ind)) {
            TSList_free(ind.caminho);
            liberar_populacao(populacao);
            return NULL;
        }
    }
    return populacao;
}

void simular_populacao(const Labirinto* lab, TLinkedList* populacao) {
    if(!lab || !populacao) {
        printf("Erro: Labirinto ou populacao invalidos\n");
        return;
    }

    printf("\n=== Simulacao da Populacao (com Fitness) ===\n");
    printf("Posicao inicial (S): (%u, %u)\n", lab->inicio.i, lab->inicio.j);
    printf("Posicao destino (E): (%u, %u)\n", lab->saida.i, lab->saida.j);
    printf("Penalidade por colisao: %d\n\n", lab->penalidade);

    TNo* atual = populacao->inicio;
    int contador = 1;
    
    while(atual != NULL) {
        int colisoes = 0;
        Posicao final = simular_movimentos(lab, &atual->info, &colisoes);
        
        printf("Individuo %03d\n", contador++);
        printf("Posicao final: (%u, %u)\n", final.i, final.j);
        printf("Fitness: %d\n", atual->info.fitness);
        printf("Status: ");
        
        if(final.i == lab->saida.i && final.j == lab->saida.j) {
            printf("Sucesso (atingiu o destino)\n");
        } else {
            printf("Falha (distancia: %d)\n", calcular_distancia_manhattan(final, lab->saida));
        }
        printf("\n");
        
        atual = atual->prox;
    }
}

void liberar_populacao(TLinkedList* populacao) {
    if(!populacao) return;
    
    TNo* atual = populacao->inicio;
    while(atual != NULL) {
        TNo* proximo = atual->prox;
        TSList_free(atual->info.caminho);
        free(atual);
        atual = proximo;
    }
    free(populacao);
}

void print_populacao(TLinkedList* populacao) {
    if(!populacao) {
        printf("População invalida!\n");
        return;
    }

    TNo* atual = populacao->inicio;
    int contador = 1;
    
    while(atual != NULL) {  // Verificação explícita
        printf("Individuo %d:\n", contador++);
        printf("  Tamanho do caminho: %d\n", atual->info.tamanho_caminho);
        printf("  Caminho: [");
        
        for(uint i = 0; i < atual->info.caminho->qty; i++) {
            printf("%c", atual->info.caminho->data[i]);
            if(i < atual->info.caminho->qty - 1) {
                printf(", ");
            }
        }
        printf("]\n\n");
        
        atual = atual->prox;  // Não esquecer de avançar
    }
}

Posicao simular_movimentos(const Labirinto* lab, Individuo* indiv, int* colisoes) {
    if(!lab || !indiv || !indiv->caminho) return lab->inicio;
    
    Posicao atual = lab->inicio;
    int colisao = 0;

    for(uint i = 0; i < indiv->tamanho_caminho; i++) {
        Posicao proxima = atual;
        
        switch(indiv->caminho->data[i]) {
            case 'C': proxima.i--; break;
            case 'B': proxima.i++; break;
            case 'E': proxima.j--; break;
            case 'D': proxima.j++; break;
            default: continue; // Movimento inválido
        }
        
        if(proxima.i < lab->n && proxima.j < lab->m && proxima.i >= 0 && proxima.j >= 0 && lab->labirinto[proxima.i][proxima.j] != '#') {
            atual = proxima;
        }else colisao ++;
    }
    if (colisoes) *colisoes = colisao;
    return atual;
}

void calcular_fitness(const Labirinto* lab, Individuo* indiv) {
    // Verificações de segurança
    if(!lab || !indiv || !indiv->caminho || !lab->labirinto) {
        if(indiv) indiv->fitness = 0;
        return;
    }
    int colisoes = 0;
    Posicao atual = simular_movimentos(lab, indiv, &colisoes);
    int distancia = calcular_distancia_manhattan(atual, lab->saida);
    int fitness = 1000 - distancia - (colisoes * lab->penalidade);
    /*
    Essa linha usa um operador ternário para garantir que o valor de fitness nunca seja negativo.
    Se fitness for maior que 0, o valor é mantido.
    Se falso (: 0) Se fitness for negativo ou zero, ele é substituído por 0.
    */
    indiv->fitness = (fitness > 0) ? fitness : 0;
}