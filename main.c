// main.c
#include "individuo.h"
#include "config.h"
#include "populacao.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));

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
    fclose(file);

    Config config = ler_config("config.yaml");

    // 2. Criação do contexto do labirinto
    int penalidade = 20;
    Labirinto* lab = criar_contexto(labirinto, n, m, config.penalidade);
    if (!lab) {
        fprintf(stderr, "Erro: Falha ao criar contexto do labirinto\n");
        liberar_matriz(labirinto, n);
        return 1;
    }

    // 3. Exibição do labirinto
    labirinto_print(lab);

    ///////////////////////////////////////////////////////////////////
    //uint tamanho_populacao;
    //printf("Quantos individuos deseja que tenha na primeira populacao? ");
    //scanf("%u", &tamanho_populacao);
    //getchar();

    ///////////////////////////////////////////////////////////////////
    // 4. Criação da população
    // Agora passa o endereço da estrutura Config completa
    TLinkedList* populacao = criar_populacao(lab, &config); 
    if (!populacao) {
        fprintf(stderr, "Erro: Falha ao criar populacao\n");
        liberar_matriz(labirinto, n);
        free(lab);
        return 1;
    }

    // 5. Exibição da população
    printf("\n=== Populacao Inicial ===\n");
    print_populacao(populacao);

    //////////////////////////////////////////////////////////////////////////
    // 6. Simulação e exibição dos resultados
    //também passa o endereço completo de config
    simular_populacao(lab,populacao, &config);

    /////////////////////////////////////////////////////////////////////////
    printf("\n=== Iniciando o Algoritmo Genetico ===\n");

    /////////////////////////////////////////////////////////////////////////
    // 7. Executar o ciclo de evolução do Algoritmo Genético
    // A função 'evoluir_populacao' gerencia o loop de gerações e a substituição da população.
    evoluir_populacao(lab, populacao, &config); 

    // 8. Liberação de recursos (pode precisar ser ajustada dependendo de como evoluir_populacao gerencia a populacao)
    // Se evoluir_populacao sempre passa o 'populacao_atual' para a próxima geração,
    // então a última 'populacao' é a que foi criada ou a que restou no final do ciclo,
    // e precisa ser liberada aqui.
    liberar_populacao(populacao); 
    liberar_matriz(labirinto, n);
    free(lab);

    return 0;
}