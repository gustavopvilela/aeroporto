#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fila.h"

void criarFila (Fila* fila) {
    /* No caso das filas deste trabalho, colocarei uma célula-cabeça para
     * facilitar sua implementação. */
    fila->primeiro = (Apontador)malloc(sizeof(Celula));
    fila->ultimo = fila->primeiro;
    fila->primeiro->proximo = NULL;
    fila->qtdeAvioes = 0;
}

int filaVazia (Fila fila) {
    return fila.primeiro == fila.ultimo;
}

void enfileira (Aviao aviao, Fila* fila) {
    fila->ultimo->proximo = (Apontador)malloc(sizeof(Celula));
    fila->ultimo->proximo->aviao = aviao;
    fila->ultimo = fila->ultimo->proximo;
    fila->ultimo->proximo = NULL;
    fila->qtdeAvioes++;
}

Aviao desenfileira (Fila *fila) {
    Aviao aviaoNulo;
    aviaoNulo.id = -1;
    aviaoNulo.unidadesDeTempo = -1;
    aviaoNulo.unidadesDeTempo = 0;
    
    Apontador item;
    
    if (filaVazia(*fila)) { /* A fila está vazia, o avião não existe. */
        return aviaoNulo;
    }
    
    item = fila->primeiro->proximo;
    
    fila->primeiro->proximo = item->proximo;
    fila->qtdeAvioes--;
    
    Aviao aviaoRemovido = item->aviao;
    free(item);
    
    if (fila->primeiro->proximo == NULL) {
        fila->ultimo = fila->primeiro;
    }
    
    return aviaoRemovido;
}

void imprimirFila (Fila fila) {
    Apontador aux = fila.primeiro;
    
    if (filaVazia(fila)) {
        printf("\t=== A FILA NÃO CONTÉM NENHUM AVIÃO ===\n");
    }
    else {
        printf("\tID\tCombustível\tTempo de espera\n");
        while (aux->proximo != NULL) {
            printf("\t%d\t%d\t\t%d\n", aux->proximo->aviao.id, aux->proximo->aviao.unidadesDeTempo, aux->proximo->aviao.tempoDeEspera);
            aux = aux->proximo;
        }
    }
}

void transferirAviao (Fila *fila1, Fila *fila2) {
    /* Fila 1 -> Fila 2. */
    
    Aviao aviaoTransferido;
    
    aviaoTransferido = desenfileira(fila1);
    
    if (aviaoTransferido.id != -1) {
        enfileira(aviaoTransferido, fila2);
    }
}

void alocarAvioes (Fila* filaAlocacao, Fila* fila1, Fila* fila2, Fila* fila3, Fila* fila4) {
    if (fila4 == NULL) { /* Significa que as filas são de decolagem. */
        while (!(filaVazia(*filaAlocacao))) {
            if (fila1->qtdeAvioes <= fila2->qtdeAvioes &&
                fila1->qtdeAvioes <= fila3->qtdeAvioes) { /* A fila 1 tem menos aviões. */
                
                transferirAviao(filaAlocacao, fila1);
            }
            else if (fila2->qtdeAvioes <= fila1->qtdeAvioes &&
                fila2->qtdeAvioes <= fila3->qtdeAvioes) {
                
                transferirAviao(filaAlocacao, fila2);
            }
            else if (fila3->qtdeAvioes <= fila1->qtdeAvioes &&
                fila3->qtdeAvioes <= fila1->qtdeAvioes) {
                
                transferirAviao(filaAlocacao, fila3);
            }
        }
    }
    else { /* Significa que as filas são de aterrissagem. */
        while (!(filaVazia(*filaAlocacao))) {
            if (fila1->qtdeAvioes <= fila2->qtdeAvioes &&
                fila1->qtdeAvioes <= fila3->qtdeAvioes &&
                fila1->qtdeAvioes <= fila4->qtdeAvioes) { /* A fila 1 tem menos aviões. */
                
                transferirAviao(filaAlocacao, fila1);
            }
            else if (fila2->qtdeAvioes <= fila1->qtdeAvioes &&
                     fila2->qtdeAvioes <= fila3->qtdeAvioes &&
                     fila2->qtdeAvioes <= fila4->qtdeAvioes) {
                
                transferirAviao(filaAlocacao, fila2);
            }
            else if (fila3->qtdeAvioes <= fila1->qtdeAvioes &&
                     fila3->qtdeAvioes <= fila2->qtdeAvioes &&
                     fila3->qtdeAvioes <= fila4->qtdeAvioes) {
                
                transferirAviao(filaAlocacao, fila3);
            }
            else if (fila4->qtdeAvioes <= fila1->qtdeAvioes &&
                     fila4->qtdeAvioes <= fila2->qtdeAvioes &&
                     fila4->qtdeAvioes <= fila3->qtdeAvioes) {
                
                transferirAviao(filaAlocacao, fila4);
            }
        }
    }
}

void adicionarTempoEspera (Fila fila, int *tempoEspera) {
    (*tempoEspera) += fila.primeiro->proximo->aviao.tempoDeEspera;
}