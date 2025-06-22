// populacao.c
#include "populacao.h"
#include "individuo.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Função auxiliar para copiar matriz
char** copiar_matriz(char** original, uint n, uint m) {
    char** copia = malloc(n * sizeof(char*));
    for(uint i = 0; i < n; i++) {
        copia[i] = malloc(m * sizeof(char));
        for(uint j = 0; j < m; j++) {
            copia[i][j] = original[i][j];
        }
    }
    return copia;
}
////////////////////////////////////////////////////////////////////////////
Individuo copiar_individuo(const Individuo* original) {
    Individuo copia;
    if (!original) {
        // Retorna um indivíduo "nulo" ou lida com erro
        copia.caminho = NULL; // Ou inicialize todos os campos para um estado inválido
        copia.fitness = 0;
        copia.tamanho_caminho = 0;
        copia.id = -1;
        return copia;
    }

    copia.id = original->id;
    copia.fitness = original->fitness;
    copia.tamanho_caminho = original->tamanho_caminho;

    // Chama Stack_copy para fazer uma cópia profunda da pilha do caminho
    copia.caminho = Stack_copy(original->caminho); 

    return copia; // Retorna a struct Individuo por valor
}
int calcular_distancia_manhattan(Posicao a, Posicao b) {
    return abs((int)a.i - (int)b.i) + abs((int)a.j - (int)b.j);
}

char movimento_aleatorio() {
    char movimentos[] = {'C', 'B', 'E', 'D'};
    return movimentos[rand() % 4];
}

char movimento_valido_aleatorio(Labirinto* lab, Posicao atual) {
    char movimentos[] = {'C', 'B', 'E', 'D'};
    char movimentos_validos[4];
    int num_validos = 0;
    
    for(int i = 0; i < 4; i++) {
        Posicao prox = atual;
        switch(movimentos[i]) {
            case 'C': prox.i--; break;
            case 'B': prox.i++; break;
            case 'E': prox.j--; break;
            case 'D': prox.j++; break;
        }
        
        if(prox.i < lab->n && prox.j < lab->m && 
           prox.i >= 0 && prox.j >= 0 && 
           lab->labirinto[prox.i][prox.j] != '#') {
            movimentos_validos[num_validos++] = movimentos[i];
        }
    }
    
    return (num_validos > 0) ? 
        movimentos_validos[rand() % num_validos] : 
        movimento_aleatorio();
}
//separar aqui a geração de movimento aleatório e a de movimentos válidos, o de movimento aleatório, so o primeiro movimente será somente para a direira ou para baixo
//que é o caso desse labirinto, a o movimentos válidos não terá penalidade por colisão pois ele considera o movimento que vai bater na parede
Posicao simular_movimentos(const Labirinto* lab, Individuo* indiv, int* colisoes, char** lab_copia) {
    if(!lab || !indiv || !indiv->caminho) return lab->inicio;
    
    Posicao atual = lab->inicio;
    int colisao = 0;

    // Se lab_copia foi fornecido, marcar posição inicial
    if(lab_copia && lab_copia[atual.i][atual.j] != 'S') {
        lab_copia[atual.i][atual.j] = '@';
    }

    for(uint i = 0; i < indiv->caminho->qty; i++) {
        char mov = indiv->caminho->data[i];
        Posicao proxima = atual;
        
        switch(mov) {
            case 'C': proxima.i--; break;
            case 'B': proxima.i++; break;
            case 'E': proxima.j--; break;
            case 'D': proxima.j++; break;
            default: continue;
        }
        
        // Verificar movimento válido
        if(proxima.i < lab->n && proxima.j < lab->m && 
           proxima.i >= 0 && proxima.j >= 0 && 
           lab->labirinto[proxima.i][proxima.j] != '#') {
            atual = proxima;
            
            // Se lab_copia foi fornecido, marcar posição
            if(lab_copia && lab_copia[atual.i][atual.j] != 'S' && 
               lab_copia[atual.i][atual.j] != 'E') {
                lab_copia[atual.i][atual.j] = '@';
            }
        } else {
            colisao++;
        }
    }
    
    if(colisoes) *colisoes = colisao;
    return atual;
}

void calcular_fitness(const Labirinto* lab, Individuo* indiv) {
    if(!lab || !indiv || !indiv->caminho) {
        if(indiv) indiv->fitness = 0;
        return;
    }

    int w_distancia = 1000;
    
    int colisoes = 0;
    // Chamada única para simular movimentos
    Posicao final = simular_movimentos(lab, indiv, &colisoes, NULL);
    
    int distancia = calcular_distancia_manhattan(final, lab->saida);
    int fitness = w_distancia - distancia - (colisoes * lab->penalidade);
    indiv->fitness = (fitness > 0) ? fitness : 0;
}

void liberar_individuo(Individuo* indiv) {
    if (indiv && indiv->caminho) {
        free(indiv->caminho->data);
        free(indiv->caminho);
        indiv->caminho = NULL;
    }
}

void liberar_populacao(TLinkedList* populacao) {
    free(populacao);
}

void print_populacao(TLinkedList* populacao) {
    if(!populacao) {
        printf("População invalida!\n");
        return;
    }

    TNo* atual = populacao->inicio;
    int contador = 1;
    
    while(atual != NULL) {
        printf("Individuo %d:\n", contador++);
        printf("  Fitness: %d\n", atual->info.fitness);
        printf("  Caminho: ");
        Stack_print(atual->info.caminho);
        printf("\n\n");
        
        atual = atual->prox;
    }
}

TLinkedList* criar_populacao(Labirinto* lab, uint tamanho_populacao, FormaCaminho forma_caminho) {
    if (!lab) return NULL;

    int dist = calcular_distancia_manhattan(lab->inicio, lab->saida);
    TLinkedList* populacao = list_create();
    if (!populacao){
        print("erro");
    } return NULL;

    for (int i = 0; i < tamanho_populacao; i++) {
        Individuo ind;
        ind.id = i + 1;
        printf("DEBUG: Em criar_populacao: Tentando inserir Individuo ID %d. List size ANTES: %u\n", ind.id, list_size(populacao));
        if (!list_insert_end(populacao, ind)) {
    printf("ERRO FATAL: Falha ao inserir Individuo ID %d na lista.\n", ind.id);
    // ... tratamento de erro ...
}
           printf("DEBUG: Em criar_populacao: Individuo ID %d inserido. List size DEPOIS: %u. Inicio da lista ID: %d\n", ind.id, list_size(populacao),
           populacao->inicio ? populacao->inicio->info.id : -1);
        printf("Criando individuo ID: %d\n", ind.id); //verificação de numeração de indivíduos
        ind.tamanho_caminho = dist + (rand() % (dist + 1));
        ind.caminho = Stack_create(ind.tamanho_caminho);
        ind.fitness = 0;

        if (!ind.caminho) {
            liberar_populacao(populacao);
            return NULL;
        }

        // Gera movimentos baseados em forma_caminho
        Posicao atual = lab->inicio;
        for (int j = 0; j < ind.tamanho_caminho; j++) {
            char mov;
            if (forma_caminho == MOV_VALIDOS) {
                mov = movimento_valido_aleatorio(lab, atual);
                // Atualiza a posição atual para o próximo movimento
                Posicao prox = atual;
                switch (mov) {
                    case 'C': prox.i--; break;
                    case 'B': prox.i++; break;
                    case 'E': prox.j--; break;
                    case 'D': prox.j++; break;
                }
                if (prox.i < lab->n && prox.j < lab->m) {  // Verifica limites
                    atual = prox;
                }
            } else {
                mov = movimento_aleatorio();
            }
            Stack_push(ind.caminho, mov);
        }

        calcular_fitness(lab, &ind);
        if (!list_insert_end(populacao, ind)) {
            liberar_individuo(&ind); 
            liberar_populacao(populacao);
            return NULL;
        }
    }
    
    return populacao;
}


void simular_populacao(const Labirinto* lab, TLinkedList* populacao) {
    if(!lab || !populacao) {
        printf("Erro: Labirinto ou populacao invalidos\n");
        return;
    }

    printf("\n=== Simulacao da Populacao (com Fitness) ===\n");
    printf("Posicao inicial (S): (%u, %u)\n", lab->inicio.i, lab->inicio.j);
    printf("Posicao destino (E): (%u, %u)\n", lab->saida.i, lab->saida.j);
    printf("Penalidade por colisao: %d\n\n", lab->penalidade);

    TNo* atual = populacao->inicio;
    //int contador = 1;
    
    while(atual != NULL) {
        int colisoes = 0;
        
        // Cria cópia do labirinto para marcar caminho
        char** lab_copia = copiar_matriz(lab->labirinto, lab->n, lab->m);
        Posicao final = simular_movimentos(lab, &atual->info, &colisoes, lab_copia);
        
        printf("Individuo %03d\n", atual->info.id); 
        printf("Posicao final: (%u, %u)\n", final.i, final.j);
        printf("Fitness: %d\n", atual->info.fitness);
        printf("Status: ");
        
        if(final.i == lab->saida.i && final.j == lab->saida.j) {
            printf("Sucesso (atingiu o destino)\n");
        } else {
            printf("Falha (distancia: %d)\n", calcular_distancia_manhattan(final, lab->saida));
        }
        
        // Exibe labirinto com caminho marcado
        printf("Labirinto com caminho percorrido:\n");
        for(uint i = 0; i < lab->n; i++) {
            for(uint j = 0; j < lab->m; j++) {
                printf("%c", lab_copia[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        
        // Libera cópia do labirinto
        for(uint i = 0; i < lab->n; i++) free(lab_copia[i]);
        free(lab_copia);
        
        atual = atual->prox;
    }
}

////////////////////////////////////////////////////////////////////////////////////
int calcular_fitness_total(TLinkedList* populacao) {
    if (!populacao || !populacao->inicio) return 0;
    
    int total = 0;
    TNo* atual = populacao->inicio;
    
    while (atual != NULL) {
        total += atual->info.fitness;
        atual = atual->prox;
    }
    
    return total;
}

//////////////////////////////////////////////////////////////////////////////
Individuo* selecionar_pai_roleta(TLinkedList* populacao, int fitness_total) {
    if (!populacao || !populacao->inicio) {
        // População vazia ou inválida, não há pais para selecionar
        return NULL;
    }

    if (fitness_total <= 0) {
        // Se o fitness total for zero ou negativo (o que não deveria acontecer com fitness > 0)
        // Seleciona um indivíduo aleatoriamente para evitar divisão por zero/loop infinito.
        // Isso é um "fallback" para cenários onde a evolução travou ou todos são igualmente ruins.
        int rand_idx = rand() % list_size(populacao); // assume list_size está correta
        TNo* atual = populacao->inicio;
        for (int i = 0; i < rand_idx; i++) {
            if (atual->prox == NULL) break; // Evitar ir além do final da lista
            atual = atual->prox;
        }
        return &atual->info;
    }

    // Gerar um número aleatório no intervalo [0, fitness_total) com precisão de ponto flutuante
    // É mais robusto usar double para a roleta
    double valor_sorteado = (double)rand() / RAND_MAX * fitness_total;

    double soma_acumulada = 0.0;
    TNo* atual = populacao->inicio;

    while (atual != NULL) {
        soma_acumulada += atual->info.fitness;
        // Verifica se a soma acumulada atingiu ou ultrapassou o valor sorteado
        if (soma_acumulada >= valor_sorteado) {
            return &atual->info; // Retorna o ponteiro para o indivíduo encontrado
        }
        atual = atual->prox;
    }

    // Em casos excepcionais (por exemplo, erro de ponto flutuante),
    // pode ser que o loop termine sem encontrar. Como fallback, retorna o último.
    // Isso é raro se o `valor_sorteado` for [0, total_fitness) e `total_fitness > 0`.
    // Ou retorna NULL para indicar que algo inesperado aconteceu.
    return &populacao->inicio->info; // Retorna o primeiro como último recurso, ou melhor, NULL
}

////////////////////////////////////////////////////////
// Implementação da mutação
void mutar(Individuo* indiv, Labirinto* lab) {
    if (indiv->caminho->qty == 0) return;
    
    int pos = rand() % indiv->caminho->qty;
    char movimentos[] = {'C', 'B', 'E', 'D'};
    indiv->caminho->data[pos] = movimentos[rand() % 4];
}

////////////////////////////////////////////////////////////////////
Individuo crossover(Individuo pai1, Individuo pai2) {
    Individuo filho;
    filho.caminho = Stack_create(pai1.caminho->capacidade);
    filho.fitness = 0;
    
    // Ponto de corte aleatório
    int corte = rand() % pai1.caminho->qty;
    
    // Primeira parte do pai1
    for (int i = 0; i < corte; i++) {
        Stack_push(filho.caminho, pai1.caminho->data[i]);
    }
    
    // Segunda parte do pai2
    for (int i = corte; i < pai2.caminho->qty; i++) {
        Stack_push(filho.caminho, pai2.caminho->data[i]);
    }
    
    return filho;
}

///////////////////////////////////////////////////////////////////////////////
void preservar_elites(TLinkedList* populacao_antiga, TLinkedList* nova_geracao, int num_elites) {
    TNo* atual = populacao_antiga->inicio;
    for (int i = 0; i < num_elites && atual != NULL; i++) {
        list_insert_end(nova_geracao, atual->info);
        atual = atual->prox;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Função principal para criar nova geração
TLinkedList* criar_nova_geracao(TLinkedList* populacao_antiga, Labirinto* lab, Config config) {
    TLinkedList* nova_geracao = list_create();
    if (!nova_geracao) return NULL;

    int fitness_total = calcular_fitness_total(populacao_antiga);
    
    // 1. Elitismo: Preserva os melhores indivíduos (CÓPIAS PROFUNDAS!)
    // Primeiro, precisa ordenar a população por fitness para selecionar os "melhores"
    // (a função `preservar_elites` atual assume que os melhores já estão no início,
    // o que requer uma ordenação prévia da `populacao_antiga`).
    // assumido que `populacao_antiga` já está ordenada do maior fitness para o menor.
    TNo* atual_elite = populacao_antiga->inicio;
    int num_elites = (int)(config.tamanho_populacao * config.elitismo); // Arredonda para baixo

    for (int i = 0; i < num_elites && atual_elite != NULL; i++) {
        Individuo elite_copia = copiar_individuo(&atual_elite->info); // CÓPIA PROFUNDA AQUI
        if (elite_copia.caminho == NULL);
        elite_copia.id = i + 1; 
        if (!list_insert_end(nova_geracao, elite_copia)) {
            liberar_individuo(&elite_copia);
            // Tratar erro: liberar nova_geracao e retornar NULL
            liberar_populacao(nova_geracao);
            return NULL;
        }
        atual_elite = atual_elite->prox;
    }
    
    unsigned int individuos_criados = list_size(nova_geracao);
    
    // 2. Crossover e Mutação: Preenche o restante da população
    while (individuos_criados < config.tamanho_populacao) {
        // Seleciona os pais usando a roleta
        Individuo* pai1_ptr = selecionar_pai_roleta(populacao_antiga, fitness_total);
        Individuo* pai2_ptr = selecionar_pai_roleta(populacao_antiga, fitness_total);

        // Se a seleção falhar por algum motivo (e.g., população muito pequena ou fitness 0)
        if (!pai1_ptr || !pai2_ptr) {
            // Se não puder selecionar pais, pode parar ou tentar novamente
            // Para simplicidade, vamos pular esta iteração e continuar com a próxima
            // Se for um problema persistente, pode levar a um loop infinito
            printf("Aviso: Falha ao selecionar pais. Tentando novamente...\n");
            break; // Ou continue, dependendo da sua estratégia para casos limites.
                   // Se quebrar, pode resultar em uma população menor que o desejado.
        }

        Individuo filho;
        // Aplica crossover com a probabilidade configurada
        if ((float)rand() / RAND_MAX < config.prob_cx) {
            // O crossover recebe as structs, a função crossover deve fazer a cópia da Stack
            filho = crossover(*pai1_ptr, *pai2_ptr); 
        } else {
            // Se não houver crossover, um dos pais pode ser clonado para o filho (ou outra estratégia)
            filho = copiar_individuo(pai1_ptr); // Clona o pai1
        }
        
        // Atribui um ID único ao novo filho
        filho.id = individuos_criados + 1; 

        // Aplica mutação com a probabilidade configurada
        if ((float)rand() / RAND_MAX < config.prob_mut) {
            mutar(&filho, lab); // A mutação altera o caminho da `filho` local
        }
        
        // Calcula o fitness do novo filho
        calcular_fitness(lab, &filho);
        
        // Insere o filho na nova geração
        if (!list_insert_end(nova_geracao, filho)) {
            liberar_individuo(&filho); // Libera o filho se a inserção falhar
            // Tratar erro: liberar nova_geracao e retornar NULL
            liberar_populacao(nova_geracao);
            return NULL;
        }
        individuos_criados++;
    }
    
    return nova_geracao;
}

