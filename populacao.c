#include "labirinto.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

char movimento_aleatorio() {
    char movimentos[] = {'C', 'B', 'E', 'D'};
    return movimentos[rand() % 4];
}

int calcular_distancia_manhattan(Posicao a, Posicao b) {
    return abs((int)a.i - (int)b.i) + abs((int)a.j - (int)b.j); //o abs é módilo, ou seja, a distância sempre vai dar positiva
}

TLinkedList* criar_populacao(Labirinto* lab) {
    if(!lab) return NULL;
    
    int dist = calcular_distancia_manhattan(lab->inicio, lab->saida);
    //if(dist <= 0) dist = 10; // Valor padrão se distância inválida
    
    TLinkedList* populacao = list_create();
    if(!populacao) return NULL;

    const int tamanho_populacao = 100;

    for(int i = 0; i < tamanho_populacao; i++) {
        Individuo ind = {
            .caminho = TSList_create(),
            .tamanho_caminho = dist + (rand() % (dist + 1)),
            .fitness = 0
        };

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
        Posicao final = simular_movimentos(lab, atual->info.caminho);
        
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

Posicao simular_movimentos(const Labirinto* lab, TSList* caminho) {
    if(!lab || !caminho) return lab->inicio;
    
    Posicao atual = lab->inicio;
    const uint n = lab->n, m = lab->m;

    for(uint i = 0; i < caminho->qty; i++) {
        Posicao proxima = atual;
        
        switch(caminho->data[i]) {
            case 'C': proxima.i--; break;
            case 'B': proxima.i++; break;
            case 'E': proxima.j--; break;
            case 'D': proxima.j++; break;
            default: continue; // Movimento inválido
        }
        
        if(proxima.i < n && proxima.j < m && 
           proxima.i >= 0 && proxima.j >= 0 && 
           lab->labirinto[proxima.i][proxima.j] != '#') {
            atual = proxima;
        }
    }
    return atual;
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
        printf("  Fitness: %d\n", atual->info.fitness);
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

void calcular_fitness(const Labirinto* lab, Individuo* indiv) {
    // Verificações de segurança
    if(!lab || !indiv || !indiv->caminho || !lab->labirinto) {
        if(indiv) indiv->fitness = 0;
        return;
    }

    // Simulação dos movimentos e contagem de colisões
    Posicao atual = lab->inicio;
    int colisoes = 0;
    const uint n = lab->n, m = lab->m;

    for(uint i = 0; i < indiv->caminho->qty; i++) {
        Posicao proxima = atual;
        
        switch(indiv->caminho->data[i]) {
            case 'C': proxima.i--; break;
            case 'B': proxima.i++; break;
            case 'E': proxima.j--; break;
            case 'D': proxima.j++; break;
            default: continue;
        }

        if(proxima.i < n && proxima.j < m && 
           lab->labirinto[proxima.i][proxima.j] != '#') {
            atual = proxima;
        } else {
            colisoes++;
        }
    }

    // Cálculo da distância MANHATTAN (mais adequada para labirintos)
    int distancia = calcular_distancia_manhattan(atual, lab->saida);


    // Cálculo do fitness
    // Quanto menor a distância e menos colisões, melhor o fitness
    int fitness = 1000 - distancia - (colisoes * lab->penalidade);
    indiv->fitness = (fitness > 0) ? fitness : 0;
}