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
    labirinto_print(labirinto, n, m, lab);

    uint tamanho_populacao;
    printf("Quantos individuos deseja que tenha na populacao?");
    scanf ("%d", &tamanho_populacao);
    getchar();

    // 4. Criação da população (já com fitness calculado)
    TLinkedList* populacao = criar_populacao(lab, tamanho_populacao);
    if (!populacao) {
        fprintf(stderr, "Erro: Falha ao criar populacao\n");
        liberar_matriz(labirinto, n);
        return 1;
    }

    // 5. printar a população
    print_populacao(populacao);

    // 6. Simulação e exibição dos resultados
    simular_populacao(lab, populacao);

    // 7. Liberação de recursos
    liberar_populacao(populacao);
    liberar_matriz(labirinto, n);

    return 0;
}
