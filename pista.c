#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "pista.h"

void criarPista (Pista* pista, char* nome) {
    strcpy(pista->nome, nome);
    pista->ocupado = PISTA_LIVRE; /* A pista vem desocupada de início. */
}

void pousarAviao (Fila* fila, Pista* pista) {
    Aviao aviao = desenfileira(fila);
    
    if (aviao.id != -1) {
        pista->ocupado = PISTA_OCUPADA;
    }
}

void decolarAviao (Fila* fila, Pista* pista) {
    Aviao aviao = desenfileira(fila);
    
    if (aviao.id != -1) {
        pista->ocupado = PISTA_OCUPADA;
    }
}