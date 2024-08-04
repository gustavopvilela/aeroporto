#ifndef FILA_H
#define FILA_H

#ifdef __cplusplus
extern "C" {
#endif
    
#define SEM_COMBUSTIVEL 0
#define COMBUSTIVEL_CHEIO 20
#define QTDE_GERAR_AVIOES 4

#define ESC_KEY_CODE 27

#define PISTA_OCUPADA 1
#define PISTA_LIVRE 0

#define ROUND_ATERRISSAGEM 0
#define ROUND_DECOLAGEM 1

#define NAO_ATUALIZAR_COMBUSTIVEL 0
#define ATUALIZAR_COMBUSTIVEL 1

/* Estrutura do avião. */
typedef struct {
    int id;
    int unidadesDeTempo;
    int tempoDeEspera;
} Aviao;

/* Estrutura das células da fila. */
typedef struct Celula *Apontador;

typedef struct Celula {
    Aviao aviao;
    Apontador proximo;
} Celula;

/* Estrutura da fila. */
typedef struct {
    Apontador primeiro, ultimo;
    int qtdeAvioes; /* Com esse dado, será mais fácil decidir em que
                     * fila inserir. */
} Fila;

void criarFila (Fila* fila);
int filaVazia (Fila fila);
void enfileira (Aviao aviao, Fila* fila);
Aviao desenfileira (Fila *fila);
void imprimirFila (Fila fila);
void transferirAviao (Fila *fila1, Fila *fila2);
void alocarAvioes (Fila* filaAlocacao, Fila* fila1, Fila* fila2, Fila* fila3, Fila* fila4);
void adicionarTempoEspera (Fila fila, int *tempoEspera);


#ifdef __cplusplus
}
#endif

#endif /* FILA_H */

