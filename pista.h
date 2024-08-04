#ifndef PISTA_H
#define PISTA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fila.h"
    
/* Estrutura para as pistas. */
typedef struct {
    char nome[50];
    int ocupado;
} Pista;

void criarPista (Pista* pista, char* nome);
void pousarAviao (Fila* fila, Pista* pista);
void decolarAviao (Fila* fila, Pista* pista);


#ifdef __cplusplus
}
#endif

#endif /* PISTA_H */

