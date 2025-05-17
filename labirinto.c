#include "labirinto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
pq usar o malloc? quando fui criar a matriz para o labirinto o código deu erro, depois de pesquisar sobre o erro
vi que, ou eu colocava o segundo valor da matriz [][esse], como um valor pré definido ou eu usava ponteiro e malloc
acabei optando por essa opção pois o professor disse que o código teria que funcionar com um labirinto de qualquer 
tamanho
*/
char** alocar_matriz (uint n, uint m){
    char** labirinto = malloc (n * sizeof(char*));
    if (!labirinto) 
        return NULL;
    
        for (uint i = 0; i < n; i++) {
            labirinto[i] = malloc(m * sizeof(char));
            if (!labirinto[i]) {
                // desaloca tudo em caso de erro
                for (uint j = 0; j < i; j++) free(labirinto[j]);
                free(labirinto);
                return NULL;
        }
    }
    return labirinto;
}

void liberar_matriz(char** matriz, uint n) {
    for (uint i = 0; i < n; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

int labirinto_print(char** list, uint n, uint m, const Labirinto* lab){
    for (uint i = 0; i < n; i++) {
        for (uint j = 0; j < m; j++) {
            printf("%c", list[i][j]);
        }
        printf("\n");
    }
    printf("=== LABIRINTO CARREGADO ===\n");
    printf("\nDimensoes: %ux%u\n", n, m);
    printf("Posicao inicial (S): (%u, %u)\n", lab->inicio.i, lab->inicio.j);
    printf("Posicao destino (E): (%u, %u)\n", lab->saida.i, lab->saida.j);

    return 0;
}

// Modificar a função para receber FILE*
bool criar_matrizLab(char** labirinto, uint n, uint m, FILE* f) {
    if (!f) return false;

    for (uint i = 0; i < n; i++) {
        for (uint j = 0; j < m; j++) {
            int c = fgetc(f);
            // Remove tratamento extra de newline que pode pular caracteres válidos
            if (c == EOF) {
                return false;
            }
            labirinto[i][j] = (char)c;
        }
        // Pular newline no final de cada linha
        if (fgetc(f) != '\n' && !feof(f)) {
            return false;
        }
    }
    return true;
}

bool encontrar_posicoes_SE(char** labirinto, uint n, uint m, Posicao* inicio, Posicao* saida) {
    bool encontrouS = false, encontrouE = false;
    
    for(uint i = 0; i < n; i++) {
        for(uint j = 0; j < m; j++) {
            if(labirinto[i][j] == 'S') {
                inicio->i = i;
                inicio->j = j;
                encontrouS = true;
            }
            if(labirinto[i][j] == 'E') {
                saida->i = i;
                saida->j = j;
                encontrouE = true;
            }
        }
    }
    
    return encontrouS && encontrouE;
}

// Criar uma vez e reutilizar
Labirinto* criar_contexto(char** labirinto, uint n, uint m, int penalidade) {
    Labirinto* lab = malloc(sizeof(Labirinto));
    if (!lab) return NULL;
    
    lab->labirinto = labirinto;
    lab->n = n;
    lab->m = m;
    lab->penalidade = penalidade;
    
    if (!encontrar_posicoes_SE(labirinto, n, m, &lab->inicio, &lab->saida)) {
        free(lab);
        return NULL;
    }
    
    return lab;
}


