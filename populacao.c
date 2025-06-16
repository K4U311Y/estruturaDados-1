// populacao.c
#include "populacao.h"
#include "individuo.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Função auxiliar para copiar matriz
char** copiar_matriz(char** original, uint n, uint m) {
    char** copia = malloc(n * sizeof(char*));
    for(uint i = 0; i < n; i++) {
        copia[i] = malloc(m * sizeof(char));
        for(uint j = 0; j < m; j++) {
            copia[i][j] = original[i][j];
        }
    }
    return copia;
}

char movimento_aleatorio() {
    char movimentos[] = {'C', 'B', 'E', 'D'};
    return movimentos[rand() % 4];
}

char movimento_valido_aleatorio(Labirinto* lab, Posicao atual) {
    char movimentos[] = {'C', 'B', 'E', 'D'};
    char movimentos_validos[4];
    int num_validos = 0;
    
    for(int i = 0; i < 4; i++) {
        Posicao prox = atual;
        switch(movimentos[i]) {
            case 'C': prox.i--; break;
            case 'B': prox.i++; break;
            case 'E': prox.j--; break;
            case 'D': prox.j++; break;
        }
        
        if(prox.i < lab->n && prox.j < lab->m && 
           prox.i >= 0 && prox.j >= 0 && 
           lab->labirinto[prox.i][prox.j] != '#') {
            movimentos_validos[num_validos++] = movimentos[i];
        }
    }
    
    return (num_validos > 0) ? 
        movimentos_validos[rand() % num_validos] : 
        movimento_aleatorio();
}

int calcular_distancia_manhattan(Posicao a, Posicao b) {
    return abs((int)a.i - (int)b.i) + abs((int)a.j - (int)b.j);
}

void calcular_fitness(const Labirinto* lab, Individuo* indiv) {
    if(!lab || !indiv || !indiv->caminho) {
        if(indiv) indiv->fitness = 0;
        return;
    }
    
    // Simula movimentos sem cópia do labirinto
    int colisoes = 0;
    Posicao atual = lab->inicio;
    
    for(uint i = 0; i < indiv->caminho->qty-1; i++) {
        char mov = indiv->caminho->data[i];
        Posicao proxima = atual;
        
        switch(mov) {
            case 'C': proxima.i--; break;
            case 'B': proxima.i++; break;
            case 'E': proxima.j--; break;
            case 'D': proxima.j++; break;
            default: continue;
        }
        
        if(proxima.i < lab->n && proxima.j < lab->m && 
           proxima.i >= 0 && proxima.j >= 0 && 
           lab->labirinto[proxima.i][proxima.j] != '#') {
            atual = proxima;
        } else {
            colisoes++;
        }
    }
    
    int distancia = calcular_distancia_manhattan(atual, lab->saida);
    int fitness = 1000 - distancia - (colisoes * lab->penalidade);
    indiv->fitness = (fitness > 0) ? fitness : 0;
}

TLinkedList* criar_populacao(Labirinto* lab, uint tamanho_populacao) {
    if(!lab) return NULL;
    int dist = calcular_distancia_manhattan(lab->inicio, lab->saida);
    TLinkedList* populacao = list_create();
    if(!populacao) return NULL;

    for(int i = 0; i < tamanho_populacao; i++) {
        Individuo ind;
        ind.tamanho_caminho = dist + (rand() % (dist + 1));
        ind.caminho = Stack_create(ind.tamanho_caminho);
        ind.fitness = 0;

        if(!ind.caminho) {
            free(populacao);
            return NULL;
        }

        for(int j = 0; j < ind.tamanho_caminho; j++) {
            char mov;
            if(j == 0) {
                // Garante primeiro movimento válido
                mov = movimento_valido_aleatorio(lab, lab->inicio);
            } else {
                mov = movimento_aleatorio();
            }
            
            if(!Stack_push(ind.caminho, mov)) {
                free(ind.caminho);
                free(populacao);
                return NULL;
            }
        }

        calcular_fitness(lab, &ind);

        if(!list_insert_end(populacao, ind)) {
            free(ind.caminho);
            free(populacao);
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
        
        // Cria cópia do labirinto para marcar caminho
        char** lab_copia = copiar_matriz(lab->labirinto, lab->n, lab->m);
        Posicao final = simular_movimentos(lab, &atual->info, &colisoes, lab_copia);
        
        printf("Individuo %03d\n", contador++);
        printf("Posicao final: (%u, %u)\n", final.i, final.j);
        printf("Fitness: %d\n", atual->info.fitness);
        printf("Status: ");
        
        if(final.i == lab->saida.i && final.j == lab->saida.j) {
            printf("Sucesso (atingiu o destino)\n");
        } else {
            printf("Falha (distancia: %d)\n", calcular_distancia_manhattan(final, lab->saida));
        }
        
        // Exibe labirinto com caminho marcado
        printf("Labirinto com caminho percorrido:\n");
        for(uint i = 0; i < lab->n; i++) {
            for(uint j = 0; j < lab->m; j++) {
                printf("%c", lab_copia[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        
        // Libera cópia do labirinto
        for(uint i = 0; i < lab->n; i++) free(lab_copia[i]);
        free(lab_copia);
        
        atual = atual->prox;
    }
}

void liberar_populacao(TLinkedList* populacao) {
    free(populacao);
}

void print_populacao(TLinkedList* populacao) {
    if(!populacao) {
        printf("População invalida!\n");
        return;
    }

    TNo* atual = populacao->inicio;
    int contador = 1;
    
    while(atual != NULL) {
        printf("Individuo %d:\n", contador++);
        printf("  Fitness: %d\n", atual->info.fitness);
        printf("  Caminho: ");
        Stack_print(atual->info.caminho);
        printf("\n\n");
        
        atual = atual->prox;
    }
}

Posicao simular_movimentos(const Labirinto* lab, Individuo* indiv, int* colisoes, char** lab_copia) {
    if(!lab || !indiv || !indiv->caminho) return lab->inicio;
    
    Posicao atual = lab->inicio;
    int colisao = 0;

    // Marca posição inicial (se não for S)
    if(lab_copia[atual.i][atual.j] != 'S') {
        lab_copia[atual.i][atual.j] = '@';
    }

    // Percorre movimentos na ordem de inserção
    for(uint i = 0; i < indiv->caminho->qty-1; i++) {
        char mov = indiv->caminho->data[i];
        Posicao proxima = atual;
        
        switch(mov) {
            case 'C': proxima.i--; break;
            case 'B': proxima.i++; break;
            case 'E': proxima.j--; break;
            case 'D': proxima.j++; break;
            default: continue;
        }
        
        // Verifica movimento válido
        if(proxima.i < lab->n && proxima.j < lab->m && 
           proxima.i >= 0 && proxima.j >= 0 && 
           lab->labirinto[proxima.i][proxima.j] != '#') {
            atual = proxima;
            
            // Marca posição (exceto S e E)
            if(lab_copia[atual.i][atual.j] != 'S' && 
               lab_copia[atual.i][atual.j] != 'E') {
                lab_copia[atual.i][atual.j] = '@';
            }
        } else {
            colisao++;
        }
    }
    
    if(colisoes) *colisoes = colisao;
    return atual;
}

