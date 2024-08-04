#ifndef AVIAO_H
#define AVIAO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fila.h"

int cairAviao (Fila *fila);
Fila gerarAvioesAterrissagem (int *ultimoIdAterrissagem);
Fila gerarAvioesDecolagem (int *ultimoIdDecolagem);
void atualizarInfosAviao (Fila *fila, int atualizarCombustivel);


#ifdef __cplusplus
}
#endif

#endif /* AVIAO_H */

