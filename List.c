#include "individuo.h"
#include <stdlib.h>
#include <stdio.h>

TNo* TNo_createNFill(Individuo);

TLinkedList* list_create() {
    TLinkedList* nova = malloc(sizeof(TLinkedList));
    if(nova) nova->inicio = NULL;
    return nova;
}

bool list_insert_begin(TLinkedList* lista, Individuo info){
    TNo* novo = TNo_createNFill(info);
    if(novo == NULL) return false;
    if(lista->inicio != NULL)
        novo->prox = lista->inicio;
    lista->inicio = novo;
    return true;
}

bool list_insert_end(TLinkedList* lista, Individuo info) {
    TNo* novo = malloc(sizeof(TNo));
    if(!novo) return false;
    
    novo->info = info;
    novo->prox = NULL;

    if(lista->inicio == NULL) {
        lista->inicio = novo;
    } else {
        TNo* aux = lista->inicio;
        while(aux->prox != NULL) aux = aux->prox;
        aux->prox = novo;
    }
    return true;
}

void list_print(TLinkedList* lista){
    if(!lista) return;
    
    TNo* aux = lista->inicio;
    while(aux != NULL){
        printf("[Fitness: %d]->", aux->info.fitness);
        aux = aux->prox;
    }
    putchar('\n');
}

TNo* TNo_createNFill(Individuo info){
    TNo* novo = malloc(sizeof(TNo));
    if(novo){
        novo->info = info;
        novo->prox = NULL;
    }
    return novo;
}

TLinkedList* list_concatenate(TLinkedList* l1, TLinkedList* l2){
    TLinkedList* l_result = list_create();
    if(l_result){
        TNo* aux = l1->inicio;
        while(aux!=NULL){
            list_insert_end(l_result, aux->info);
            aux = aux->prox;
        }
        aux = l2->inicio;
        while(aux!=NULL){
            list_insert_end(l_result, aux->info);
            aux = aux->prox;
        }
    }
    return l_result;
}

bool list_delete_begin(TLinkedList* lista){
    if(lista->inicio == NULL)
        return false;
    TNo* aux = lista->inicio;
    lista->inicio = lista->inicio->prox;
    free(aux);
    return true;
}

Stack* Stack_create(){
    Stack* nova = malloc(sizeof(Stack));
    if(nova!=NULL)
        nova->qty = 0;
    return nova;
}

bool Stack_push(Stack* pilha, int info){
    if(Stack_is_full(pilha))
        return false;
    //Codigo para se a pilha nao estiver cheia
    pilha->data[pilha->qty++] = info;
    return true;
}
bool Stack_pop(Stack* pilha, int* info){
    if(Stack_is_empty(pilha))
        return false;
    *info = pilha->data[--(pilha->qty)];
    return true;
}

bool Stack_is_empty(Stack* pilha){
    return pilha->qty == 0;
}

bool Stack_is_full(Stack* pilha){
    return pilha->qty == pilha->capacidade;
}

unsigned int Stack_size(Stack* pilha){
    return pilha->qty;
}

void Stack_print(Stack* pilha) {
    if(!pilha) return;
    
    printf("[");
    for(int i = pilha->qty-1 - 1; i >= 0; i--) {
        printf("%c", pilha->data[i]);
        if(i > 0) printf(", ");
    }
    printf("]");
}