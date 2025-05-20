
#include "labirinto.h"
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

TSList* TSList_create(uint capacidade) {
    TSList *novo = malloc(sizeof(TSList));

    novo->data = malloc (sizeof (TSList));
    if(novo) novo->qty = 0;
    novo->capacidade=  capacidade;
    return novo;
}

bool TSList_insert(TSList* lista, char valor) {
    if(lista == NULL || lista->qty >= lista->capacidade) return false;
    lista->data[lista->qty++] = valor;
    return true;
}

void TSList_print(TSList* lista){
    if(lista == NULL) return;
    
    printf("[");
    for(uint i = 0; i < lista->qty; i++) {
        printf("%c", lista->data[i]);
        if(i < lista->qty - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

bool TSList_is_empty(TSList* lista){
    if(lista!=NULL)
        return lista->qty == 0;
    return true;
}

bool TSList_is_full(TSList* lista){
    if(lista!=NULL)
        return lista->qty == 100;
    return false;
}

void TSList_free(TSList* lista) {
    if(lista) {
        free(lista->data);
        free(lista);
    }
}