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
    ///////////////////////////////////////////////////////////////////
    //int penalidade = 20;
    Labirinto* lab = criar_contexto(labirinto, n, m, config.penalidade);
    if (!lab) {
        fprintf(stderr, "Erro: Falha ao criar contexto do labirinto\n");
        liberar_matriz(labirinto, n);
        return 1;
    }

    // 3. Exibição do labirinto
    labirinto_print(lab);
//////////////////////////////////////////////////////////////////
    //uint tamanho_populacao;
    //printf("Quantos individuos deseja que tenha na primeira populacao? ");
    //scanf("%u", &tamanho_populacao);
    //getchar();

    // 4. Criação da população
    TLinkedList* populacao = criar_populacao(lab, config.tamanho_populacao, config.forma_caminho); //arrumar o .h
    if (!populacao) {
        fprintf(stderr, "Erro: Falha ao criar populacao\n");
        liberar_matriz(labirinto, n);
        return 1;
    }

    // 5. Avaliação inicial da população
    simular_populacao(lab, populacao);

    ////////////////////////////////////////////////////////////
     // 6. Loop do algoritmo genético
    int geracao = 0;
    while (geracao < config.max_gen) {
        printf("\n=== Geracao %d/%d ===\n", geracao + 1, config.max_gen);
        
        // Seleção e reprodução
        TLinkedList* nova_geracao = criar_nova_geracao(populacao, lab, config);
        //int fitness_total = calcular_fitness_total(populacao);

        liberar_populacao(populacao);
        populacao = nova_geracao;

        // Avaliação a cada 10 gerações ou na última
        if (geracao % 10 == 0 || geracao == config.max_gen - 1) {
            simular_populacao(lab, populacao);
        }
        geracao++;
    }

    // .7 Exibição da população
    printf("\n=== Populacao Inicial ===\n");
    print_populacao(populacao);

    // 7. Resultado final (SIMPLIFICADO)
    printf("\n=== RESULTADO FINAL ===\n");
    simular_populacao(lab, populacao);
    
    // 8. Liberação de recursos
    liberar_populacao(populacao);
    liberar_matriz(labirinto, n);
    free(lab);

    return 0;
}