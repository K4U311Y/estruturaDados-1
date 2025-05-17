#include "labirinto.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL)); // Para movimento_aleatorio() funcionar corretamente

    // 1. Carregamento do labirinto
    FILE* file = fopen("labirinto.txt", "r");
    if (!file) {
        fprintf(stderr, "Erro: Nao foi possivel abrir o arquivo do labirinto\n");
        return 1;
    }
    
    // Leitura das dimensões
    uint n, m;
    if (fscanf(file, "%u %u\n", &n, &m) != 2) {
        fprintf(stderr, "Erro: Formato invalido do arquivo\n");
        fclose(file);
        return 1;
    }

    // Alocação e leitura da matriz
    char** labirinto = alocar_matriz(n, m);
    if (!labirinto) {
        fclose(file);
        return 1;
    }

    if (!criar_matrizLab(labirinto, n, m, file)) {
    fprintf(stderr, "Erro ao carregar labirinto do arquivo\n");
    liberar_matriz(labirinto, n);
    fclose(file);
    return 1;
    }

    // 2. Criação do contexto do labirinto
    int penalidade = 20;
    Labirinto* lab = criar_contexto(labirinto, n, m, penalidade);
    if (!lab) {
        fprintf(stderr, "Erro: Falha ao criar contexto do labirinto\n");
        liberar_matriz(labirinto, n);
        return 1;
    }

    // 3. Exibição do labirinto
    printf("=== LABIRINTO CARREGADO ===\n");
    matriz_print(labirinto, n, m);
    printf("\nDimensoes: %ux%u\n", n, m);
    printf("Posicao inicial (S): (%u, %u)\n", lab->inicio.i, lab->inicio.j);
    printf("Posicao destino (E): (%u, %u)\n", lab->saida.i, lab->saida.j);

    // 4. Criação da população (já com fitness calculado)
    TLinkedList* populacao = criar_populacao(lab);
    if (!populacao) {
        fprintf(stderr, "Erro: Falha ao criar populacao\n");
        liberar_matriz(labirinto, n);
        return 1;
    }


    // 5. Simulação e exibição dos resultados
    simular_populacao(lab, populacao);

    // 6. Liberação de recursos
    liberar_populacao(populacao);
    liberar_matriz(labirinto, n);

    return 0;
}
