#ifndef POPULACAO_H
#define POPULACAO_H
#include<stdbool.h>
#include "List.h"

typedef struct _individuo Individuo;

TLinkedList* criar_pupulacao (Individuo);
char movimento_aleatorio();

#endif