#include "labirinto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

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

int matriz_print(char** list, uint n, uint m){
    for (uint i = 0; i < n; i++) {
        for (uint j = 0; j < m; j++) {
            printf("%c ", list[i][j]);
        }
        printf("\n");
    }

    return 0;
}

bool criar_matrizLab (char** labirinto, uint n, uint m){

    FILE* f = fopen("labirinto.txt", "r");
    if(f){
        uint n, m;
        if(fscanf(f, "%u %u", &n, &m) == 2){
            for (uint i = 0; i < n; i++) {
                for (uint j = 0; j < m; j++) {
                    labirinto[i][j] = fgetc(f);
                }
                fgetc(f); // consome o '\n' ao fim da linha
            }
            fclose(f);
        }else{
            puts("Erro ao ler o numero de linhas e colunas da primeira linha");
        }
    }else{
        puts("Houve um problema ao abrir o arquivo input1.txt");
    }
}


