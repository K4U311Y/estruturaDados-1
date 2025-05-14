#ifndef LABIRINTO_H
#define LABIRINTO_H
#include<stdbool.h>

char** alocar_matriz (uint, uint);
void liberar_matriz (char**, uint);
int matriz_print(char**, uint, uint);
bool criar_matrizLab (char**, uint, uint);

#endif