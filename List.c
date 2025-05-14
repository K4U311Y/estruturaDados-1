
#include "List.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX 2

typedef struct _no{
    int info;
    struct _no *prox;
}TNo;

TNo* TNo_createNFill(int);

struct _list{
    TNo* inicio;
};

struct _listS{
    unsigned int qty;
    int data[MAX];
 };

TLinkedList* list_create(){
    TLinkedList* nova = malloc(sizeof(TLinkedList));
    if(nova){
        nova->inicio = NULL;
    }
    return nova;
}

bool list_insert_begin(TLinkedList* lista, int info){
    TNo* novo = TNo_createNFill(info);
    if(novo == NULL) return false;
    if(lista->inicio != NULL)
        novo->prox = lista->inicio;
    lista->inicio = novo;
    return true;
}

bool list_insert_end(TLinkedList* lista, int info){
    TNo* novo = TNo_createNFill(info);
    if(novo == NULL) return false;
    //A lista está vazia?
    if(lista->inicio == NULL)
        lista->inicio = novo;
    else{
        //Lista nao vazia, temos que encontrar o último elemento
        TNo* aux = lista->inicio;
        while(aux->prox!=NULL)
            aux = aux->prox;
        aux->prox = novo;
    }
    return true;
}


void list_print(TLinkedList* lista){
    TNo* aux = lista->inicio;
    while(aux!=NULL){
        printf("[%d]->", aux->info);
        aux = aux->prox;
    }
    putchar('\n');
}

TNo* TNo_createNFill(int info){
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

bool list_delete_n(TLinkedList* lista, unsigned int n){
    while(n-->0){
        if(!list_delete_begin(lista))
            return false;
    }
    return true;
}

TSList* TSList_create(){
    TSList *novo = malloc(sizeof(TSList));
    //Definindo o estado de uma lista vazia
    if(novo)
        novo->qty = 0;
    return novo;
}

bool TSList_insert(TSList* lista, int valor){
    if(lista == NULL) return false;
    if(!TSList_is_full(lista)){
        lista->data[lista->qty] = valor;
        lista->qty++;
        return true;
    }
    return false;
}

void TSList_print(TSList* lista){
    if(lista!=NULL)
        for(int i=0; i<lista->qty; i++)
            printf("%d, ", lista->data[i]);
    putchar('\n');
}

unsigned int TSList_qty(TSList* lista){
    if(lista!=NULL)
        return lista->qty;
    return 0;
}

bool TSList_is_empty(TSList* lista){
    if(lista!=NULL)
        return lista->qty == 0;
    return true;
}

bool TSList_is_full(TSList* lista){
    if(lista!=NULL)
        return lista->qty == MAX;
    return false;
}