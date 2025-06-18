// config.h
#ifndef CONFIG_H
#define CONFIG_H

typedef enum {
    MOV_VALIDOS,
    ALEATORIO,
} FormaCaminho;

typedef struct {
    int tamanho_populacao;
    float prob_mut;
    float prob_cx;
    float elitismo;
    FormaCaminho forma_caminho;
    int w_distancia;
    int penalidade;
    int max_gen;
} Config;

Config ler_config(const char* filename);

#endif