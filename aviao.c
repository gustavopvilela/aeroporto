#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "aviao.h"

int cairAviao (Fila *fila) { /* Retornará a quantidade de aviões que caíram. */
    Apontador aux = fila->primeiro;
    Aviao aviao;
    int avioesCaidos = 0;
    
    if (filaVazia(*fila)) {
        return 0;
    }
    
    /* Aqui, eu posso desenfileirar em cadeia caso haja aviões sem combustível
     * em sequência. Perceba que não faço alterações no meio da fila. Estou
     * usando apenas a função de desenfileirar. Caso ocorra um avião sem
     * combustível no meio ou fim da fila, ele não será retirado até que
     * chegue na primeira posição. */
    while (fila->primeiro->proximo != NULL && fila->primeiro->proximo->aviao.unidadesDeTempo <= 0) {
        aviao = desenfileira(fila);
        avioesCaidos++;
        
        //Mensagem mostrando que o avião caiu.
        printf("\n\t-\tO avião de ID %d caiu (sem combustível há %d ciclos).", aviao.id, aviao.unidadesDeTempo * (-1));
    }
    
    return avioesCaidos;
}

Fila gerarAvioesAterrissagem (int *ultimoIdAterrissagem) {
    Aviao novoAviao;
    
    Fila avioesAterrissagem;
    criarFila(&avioesAterrissagem);
    
    int qtdeAvioes = rand()%QTDE_GERAR_AVIOES; /* Gera números de 0 a 3, que é a qtde. de aviões que podem ser geradas. */
    
    for (int i = 0; i < qtdeAvioes; i++) {
        novoAviao.id = (*ultimoIdAterrissagem);
        novoAviao.tempoDeEspera = 0;
        novoAviao.unidadesDeTempo = rand()%20 + 1; /* Gera de 1 a 20 unidades
                                                    * de tempo. Não gero aviões
                                                    * com 0 de combustível pois
                                                    * seria impossível de ele
                                                    * chegar até as pistas em
                                                    * primeiro lugar.*/
        (*ultimoIdAterrissagem) += 2; /* Atualiza o ID para o próximo avião gerado. */
        
        enfileira(novoAviao, &avioesAterrissagem);
    }
    
    return avioesAterrissagem;
}

Fila gerarAvioesDecolagem (int *ultimoIdDecolagem) {
    Aviao novoAviao;
    
    Fila avioesDecolagem;
    criarFila(&avioesDecolagem);
    
    int qtdeAvioes = rand()%QTDE_GERAR_AVIOES; /* Gera números de 0 a 3, que é a qtde. de aviões que podem ser geradas. */
    
    for (int i = 0; i < qtdeAvioes; i++) {
        novoAviao.id = (*ultimoIdDecolagem);
        novoAviao.tempoDeEspera = 0;
        novoAviao.unidadesDeTempo = COMBUSTIVEL_CHEIO;
        (*ultimoIdDecolagem) += 2; /* Atualiza o ID para o próximo avião gerado. */
        
        enfileira(novoAviao, &avioesDecolagem);
    }
    
    return avioesDecolagem;
}

void atualizarInfosAviao (Fila *fila, int atualizarCombustivel) {
    /* Atualiza informações de tempo de
    * espera e de combustível nos aviões
    * de determinada fila. */
    Apontador aux = fila->primeiro;
    
    if (filaVazia(*fila)) {
        return;
    }
    
    while (aux->proximo != NULL) {
        if (atualizarCombustivel) aux->proximo->aviao.unidadesDeTempo--;
        aux->proximo->aviao.tempoDeEspera++;
        
        aux = aux->proximo;
    }
}

