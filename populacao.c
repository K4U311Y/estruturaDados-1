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
//separar aqui a geração de movimento aleatório e a de movimentos válidos, o de movimento aleatório, so o primeiro movimente será somente para a direira ou para baixo
//que é o caso desse labirinto, a o movimentos válidos não terá penalidade por colisão pois ele considera o movimento que vai bater na parede
Posicao simular_movimentos(const Labirinto* lab, Individuo* indiv, int* colisoes, char** lab_copia) {
    if(!lab || !indiv || !indiv->caminho) return lab->inicio;
    
    Posicao atual = lab->inicio;
    int colisao = 0;

    // Se lab_copia foi fornecido, marcar posição inicial
    if(lab_copia && lab_copia[atual.i][atual.j] != 'S') {
        lab_copia[atual.i][atual.j] = '@';
    }

    for(uint i = 0; i < indiv->caminho->qty; i++) {
        char mov = indiv->caminho->data[i];
        Posicao proxima = atual;
        
        switch(mov) {
            case 'C': proxima.i--; break;
            case 'B': proxima.i++; break;
            case 'E': proxima.j--; break;
            case 'D': proxima.j++; break;
            default: continue;
        }
        
        // Verificar movimento válido
        if(proxima.i < lab->n && proxima.j < lab->m && 
           proxima.i >= 0 && proxima.j >= 0 && 
           lab->labirinto[proxima.i][proxima.j] != '#') {
            atual = proxima;
            
            // Se lab_copia foi fornecido, marcar posição
            if(lab_copia && lab_copia[atual.i][atual.j] != 'S' && 
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

//////////////////////////////////////
void calcular_fitness(const Labirinto* lab, Individuo* indiv, FormaCaminho forma_geracao, int w_distancia) {
    if(!lab || !indiv || !indiv->caminho) {
        if(indiv) indiv->fitness = 0;
        return;
    }
    /////////////////AQUI//////////////////
    //int w_distancia = 1000;
    
    int colisoes = 0;
    // Chamada única para simular movimentos
    Posicao final = simular_movimentos(lab, indiv, &colisoes, NULL);
    
    int distancia = calcular_distancia_manhattan(final, lab->saida);
    int fitness = 0;

    ////////////calculando fitness com base na forma de geração
    if(forma_geracao == MOV_VALIDOS){
        fitness = w_distancia - distancia; // Usando w_distancia do labirinto (que vem do config)
    } else { // no caso o ALEATORIO
        fitness = w_distancia -  distancia - (colisoes * lab->penalidade);
    }
    indiv->fitness = (fitness > 0) ? fitness : 0; //pq fitness não  pode ser negativo
}

// populacao.c (função modificada)
TLinkedList* criar_populacao(Labirinto* lab, const Config* config) {////////////////////////////
    if(!lab || !config) return NULL;
    int dist = calcular_distancia_manhattan(lab->inicio, lab->saida);
    TLinkedList* populacao = list_create();
    if(!populacao) return NULL;

    for(int i = 0; i < config->tamanho_populacao; i++) {
        Individuo ind;
        ind.tamanho_caminho = dist + (rand() % (dist + 1));
        ind.caminho = Stack_create(ind.tamanho_caminho);
        ind.fitness = 0;

        if(!ind.caminho) {
            TNo* current = populacao->inicio;
            while (current != NULL) {
                //////////////////////////////////////////////////////
                TNo* next = current->prox;
                free(current->info.caminho->data); // Liberar o array de dados da pilha
                free(current->info.caminho);       // Liberar a estrutura da pilha
                free(current);                     // Liberar o nó da lista
                current = next;
            }
            free(populacao);
            return NULL;
        }
        ////////////////////////////////////////////////////
        Posicao current_pos = lab->inicio; // Posição para rastrear durante a geração do caminho
        
        //////////////////////////GERAÇÃO DE CAMINHO COM BASE EM 'forma_caminho'
        if (config->forma_caminho == ALEATORIO){
            for(int j = 0; j < ind.tamanho_caminho; j++){
                char mov =  movimento_aleatorio();
                Stack_push(ind.caminho, mov);
            }
        } else if (config->forma_caminho == MOV_VALIDOS){
            for(int j = 0; j < ind.tamanho_caminho; j++){
                char mov = movimento_valido_aleatorio(lab, current_pos);
                Stack_push(ind.caminho, mov);

                ///////////////////////////////////////////////////////////////
                //este bloco garante que quando está gerando um indivíduo do tipo MOV_VALIDOS,
                //todos os movimentos são gerados com movimento_valido_aleatorio() (e a current_pos é atualizada).
                //é meio que salvar a posição que parou
                Posicao next_pos = current_pos;
                switch(mov) {
                    case 'C': next_pos.i--; break;
                    case 'B': next_pos.i++; break;
                    case 'E': next_pos.j--; break;
                    case 'D': next_pos.j++; break;
                }
                // Atualiza a posição apenas se o movimento realmente foi válido.
                // Isso guia a geração de movimentos válidos em sequência.
                if(next_pos.i < lab->n && next_pos.j < lab->m && 
                   next_pos.i >= 0 && next_pos.j >= 0 && 
                   lab->labirinto[next_pos.i][next_pos.j] != '#') {
                    current_pos = next_pos;
                }
            }
        }
         
        //calculando o fitness ja com os parâmetros de configuração
        calcular_fitness(lab, &ind, config->forma_caminho, config->w_distancia);

        ///////////////////////////////////////////////////////////////////////
        //mudei na tentativ de solucionar o vazamento de memória
        if(!list_insert_end(populacao, ind)) {
            free(ind.caminho->data);
            free(ind.caminho);
            TNo* current = populacao->inicio;
            //percorre e libera cada indivíduo
            while (current != NULL){
                TNo* next = current->prox;
                //libera a memória do caminho 
                free(current->info.caminho->data);
                free(current->info.caminho);
                free(current);
                //libera nó da lista
                current = next;
            }
            free(populacao);
            //aí sim libera a lista
            return NULL;
        }
    }
    return populacao;
}

void simular_populacao(const Labirinto* lab, TLinkedList* populacao, const Config* config) {
    if(!lab || !populacao || !config) {
        printf("Erro: Labirinto ou populacao invalidos\n");
        return;
    }

     printf("\n=== Simulacao da Populacao (com Fitness) ===\n");
    printf("Posicao inicial (S): (%u, %u)\n", lab->inicio.i, lab->inicio.j);
    printf("Posicao destino (E): (%u, %u)\n", lab->saida.i, lab->saida.j);
    printf("Penalidade por colisao: %d\n", lab->penalidade); 
    printf("Peso da Distancia (w_distancia): %d\n\n", config->w_distancia); // P MOSTRAR O BAGUIII DA CASA DAS CARAMBOLAS

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

