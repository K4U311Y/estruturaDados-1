#include "labirinto.h"
#include <stdio.h>

typedef unsigned int uint;
int main(){
    uint n, m;

    FILE* f = fopen("labirinto.txt", "r");

    // aloca matriz dinamicamente
    char** matriz = alocar_matriz(n, m);
    if (!matriz) {
        printf("Erro ao alocar matriz\n");
        return 1;
    }

    // preenche matriz com os dados do arquivo
    if (!criar_matrizLab(matriz, n, m)) {
        liberar_matriz(matriz, n);
        return 1;
    }

    // imprime a matriz exatamente como está no arquivo
    matriz_print(matriz, n, m);

    liberar_matriz(matriz, n);

    return 0;
}
