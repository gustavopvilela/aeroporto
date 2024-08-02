#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

int ultimoIdDecolagem = 0;
int ultimoIdAterrissagem = 1;

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

/* Estrutura para as pistas. */
typedef struct {
    char nome[50];
    int ocupado;
} Pista;

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

int cairAviao (Fila *fila) { /* Retornará a quantidade de aviões que caíram. */
    Apontador aux = fila->primeiro;
    Aviao aviao;
    int avioesCaidos = 0;
    
    if (filaVazia(*fila)) {
        return 0;
    }
    
    if (aux->proximo->aviao.unidadesDeTempo <= 0) {
        desenfileira(fila);
        avioesCaidos++;
    }
    
    return avioesCaidos;
}

Fila gerarAvioesAterrissagem () {
    Aviao novoAviao;
    
    Fila avioesAterrissagem;
    criarFila(&avioesAterrissagem);
    
    int qtdeAvioes = rand()%QTDE_GERAR_AVIOES; /* Gera números de 0 a 3, que é a qtde. de aviões que podem ser geradas. */
    
    for (int i = 0; i < qtdeAvioes; i++) {
        novoAviao.id = ultimoIdAterrissagem;
        novoAviao.tempoDeEspera = 0;
        novoAviao.unidadesDeTempo = rand()%20 + 1; /* Gera de 1 a 20 unidades
                                                    * de tempo. Não gero aviões
                                                    * com 0 de combustível pois
                                                    * seria impossível de ele
                                                    * chegar até as pistas em
                                                    * primeiro lugar.*/
        ultimoIdAterrissagem += 2; /* Atualiza o ID para o próximo avião gerado. */
        
        enfileira(novoAviao, &avioesAterrissagem);
    }
    
    return avioesAterrissagem;
}

Fila gerarAvioesDecolagem () {
    Aviao novoAviao;
    
    Fila avioesDecolagem;
    criarFila(&avioesDecolagem);
    
    int qtdeAvioes = rand()%QTDE_GERAR_AVIOES; /* Gera números de 0 a 3, que é a qtde. de aviões que podem ser geradas. */
    
    for (int i = 0; i < qtdeAvioes; i++) {
        novoAviao.id = ultimoIdDecolagem;
        novoAviao.tempoDeEspera = 0;
        novoAviao.unidadesDeTempo = COMBUSTIVEL_CHEIO;
        ultimoIdDecolagem += 2; /* Atualiza o ID para o próximo avião gerado. */
        
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

void imprimirFila (Fila fila) {
    Apontador aux = fila.primeiro;
    
    /*if (filaVazia(fila)) {
        printf("\t=== A FILA NÃO CONTÉM NENHUM AVIÃO ===\n");
    }
    else {
        printf("\tID\tCombustível\tTempo de espera\n");
        while (aux->proximo != NULL) {
            printf("\t%d\t%d\t\t%d\n", aux->proximo->aviao.id, aux->proximo->aviao.unidadesDeTempo, aux->proximo->aviao.tempoDeEspera);
            aux = aux->proximo;
        }
    } */
    
    while (aux->proximo != NULL) {
        printf("<(%d) ", aux->proximo->aviao.unidadesDeTempo);
        aux = aux->proximo;
    }
    
    printf("\n");
}

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

int main(int argc, char** argv) {
    srand(time(NULL)); /* Seed geradora de números aleatórios. */
    
    /* Gerando as filas de aterrissagem. */
    Fila filaAterrissagem1; criarFila(&filaAterrissagem1);
    Fila filaAterrissagem2; criarFila(&filaAterrissagem2);
    Fila filaAterrissagem3; criarFila(&filaAterrissagem3);
    Fila filaAterrissagem4; criarFila(&filaAterrissagem4);
    
    /* Gerando as filas de decolagem. */
    Fila filaDecolagem1; criarFila(&filaDecolagem1);
    Fila filaDecolagem2; criarFila(&filaDecolagem2);
    Fila filaDecolagem3; criarFila(&filaDecolagem3);
    
    /* Gerando as pistas. */
    Pista pista1; criarPista(&pista1, "Pista 1");
    Pista pista2; criarPista(&pista2, "Pista 2");
    Pista pista3; criarPista(&pista3, "Pista 3");
    
    /* Gerando as filas que gerarão os aviões para aterrissagem e decolagem. */
    Fila tempAterrissagem; criarFila(&tempAterrissagem);
    Fila tempDecolagem; criarFila(&tempDecolagem);
    
    int avioesCaidosRound = 0;
    int avioesCaidosTotal = 0;
    int avioesPousadosRound = 0;
    int avioesPousadosTotal = 0;
    int escape; /* Usado para terminar o programa. */
    int round = ROUND_ATERRISSAGEM; /* Define se o round será de pouso ou decolagem. */
    
    do {
        /* Antes de processarmos pousos e decolagens, é gerado as filas
         * temporárias dos aviões (função de gerar aviões), que serão
         * transferidos para suas respectivas filas (função alocarAvioes()). */
        
        tempAterrissagem = gerarAvioesAterrissagem();
        tempDecolagem = gerarAvioesDecolagem();
        
        alocarAvioes(&tempAterrissagem, &filaAterrissagem1, &filaAterrissagem2, &filaAterrissagem3, &filaAterrissagem4);
        alocarAvioes(&tempDecolagem, &filaDecolagem1, &filaDecolagem2, &filaDecolagem3, (Fila*)NULL);
        /* Nessa segunda alocação, temos de fazer o cast para o tipo Fila* do
         * quinto argumento da função, uma vez que para as filas de decolagem
         * só existem três delas. */
        
        /* Depois, caímos os aviões que ficaram sem combustível e
         * atualizamos suas variáveis.  */
        avioesCaidosRound = cairAviao(&filaAterrissagem1) +
                            cairAviao(&filaAterrissagem2) +
                            cairAviao(&filaAterrissagem3) +
                            cairAviao(&filaAterrissagem4);
        avioesCaidosTotal += avioesCaidosRound;
        
        printf("Fila 1 (A): "); imprimirFila(filaAterrissagem1);
        printf("Fila 2 (A): "); imprimirFila(filaAterrissagem2);
        printf("Fila 3 (A): "); imprimirFila(filaAterrissagem3);
        printf("Fila 4 (A): "); imprimirFila(filaAterrissagem4);
        
        printf("Fila 1 (D): "); imprimirFila(filaDecolagem1);
        printf("Fila 2 (D): "); imprimirFila(filaDecolagem2);
        printf("Fila 3 (D): "); imprimirFila(filaDecolagem3);
        
        printf("\n==============================================================================\n");
        
        switch (round) {
            case ROUND_ATERRISSAGEM:
                /* Decidi que quando estiver no round de aterrissagem, os
                 * aviões somente aterrisarão. Não haverá a possibilidade
                 * de algum avião decolar, a menos que sobre alguma pista
                 * no final, o que é altamente improvável. */
                
                /* Podemos ter até 3 aviões em situação de estar sem
                 * combustível. Como olhamos as prateleiras de dois
                 * em dois, para conferir se apenas um avião está oom 1
                 * de combustível, podemos tirar a média dos dois. Caso
                 * a média seja 1, significa que os dois tem 1 de combustível
                 * e um deles utilizará a 3ª pista de pouso. Caso a média seja
                 * diferente de 1, significa que um deles ou os dois possui
                 * mais que 1 de combustível, dessa forma, pousaremos o que
                 * tem menor quantidade. */
                
                /* Como nesse round eu só estou pousando aviões, no primeiro
                 * if, eu posso ter a certeza de que todas as pistas estarão
                 * livres.*/
                
                /* Verificando as filas 1 e 2. */
                if (!filaVazia(filaAterrissagem1) && filaVazia(filaAterrissagem2)) {
                    pousarAviao(&filaAterrissagem1, &pista1);
                    avioesPousadosRound++;
                }
                else if (filaVazia(filaAterrissagem1) && !filaVazia(filaAterrissagem2)) {
                    pousarAviao(&filaAterrissagem2, &pista1);
                    avioesPousadosRound++;
                }
                else if (!filaVazia(filaAterrissagem1) && !filaVazia(filaAterrissagem2)) {
                    
                    if ((filaAterrissagem1.primeiro->proximo->aviao.unidadesDeTempo
                        + filaAterrissagem2.primeiro->proximo->aviao.unidadesDeTempo) / 2 == 1) {

                       /* Nesse caso, eu vou pousar os dois aviões:
                        * um na pista para ele, e outro na pista 3. Nesse
                        * passo, já está verificado que as duas filas possuem
                        * aviões. */
                       pousarAviao(&filaAterrissagem1, &pista1);
                       pousarAviao(&filaAterrissagem2, &pista3);
                       
                       avioesPousadosRound++;
                       avioesPousadosRound++;
                    }
                    else if (filaAterrissagem1.primeiro->proximo->aviao.tempoDeEspera <=
                             filaAterrissagem2.primeiro->proximo->aviao.tempoDeEspera) {
                        
                        pousarAviao(&filaAterrissagem1, &pista1);
                        avioesPousadosRound++;
                    }
                    else if (filaAterrissagem2.primeiro->proximo->aviao.tempoDeEspera <
                             filaAterrissagem1.primeiro->proximo->aviao.tempoDeEspera) {
                        
                        pousarAviao(&filaAterrissagem2, &pista1);
                        avioesPousadosRound++;
                    }
                }
                
                /* Verificando as filas 3 e 4. */
                if (!filaVazia(filaAterrissagem3) && filaVazia(filaAterrissagem4)) {
                    pousarAviao(&filaAterrissagem3, &pista2);
                    avioesPousadosRound++;
                }
                else if (filaVazia(filaAterrissagem3) && !filaVazia(filaAterrissagem4)) {
                    pousarAviao(&filaAterrissagem4, &pista2);
                    avioesPousadosRound++;
                }
                else if (!filaVazia(filaAterrissagem3) && !filaVazia(filaAterrissagem4)) {
                    
                    if ((filaAterrissagem3.primeiro->proximo->aviao.unidadesDeTempo
                        + filaAterrissagem4.primeiro->proximo->aviao.unidadesDeTempo) / 2 == 1) {

                       /* Nesse caso, eu vou pousar os dois aviões:
                        * um na pista para ele, e outro na pista 3. Nesse
                        * passo, já está verificado que as duas filas possuem
                        * aviões. Como neste passo não há 100% de certeza de
                        * que a pista 3 estrá livre, preciso verificar isso
                        * antes de inserir nela. Caso esteja atualmente
                        * ocupada, nada será feito. */
                       pousarAviao(&filaAterrissagem3, &pista2);
                       avioesPousadosRound++;
                       
                       if (pista3.ocupado == PISTA_LIVRE) {
                           pousarAviao(&filaAterrissagem4, &pista3);
                           avioesPousadosRound++;
                       }
                    }
                    else if (filaAterrissagem3.primeiro->proximo->aviao.tempoDeEspera <
                             filaAterrissagem4.primeiro->proximo->aviao.tempoDeEspera) {
                        
                        pousarAviao(&filaAterrissagem3, &pista2);
                        avioesPousadosRound++;
                    }
                    else if (filaAterrissagem4.primeiro->proximo->aviao.tempoDeEspera <
                             filaAterrissagem3.primeiro->proximo->aviao.tempoDeEspera) {
                        
                        pousarAviao(&filaAterrissagem4, &pista2);
                        avioesPousadosRound++;
                    }
                }
                
                /* Verificando disponibilidade de pistas para decolagem. */
                if (pista1.ocupado == PISTA_LIVRE) decolarAviao(&filaDecolagem1, &pista1);
                if (pista2.ocupado == PISTA_LIVRE) decolarAviao(&filaDecolagem2, &pista2);
                if (pista3.ocupado == PISTA_LIVRE) decolarAviao(&filaDecolagem3, &pista3);
                
                /* Por fim, neste round, atualizamos a variával para que
                 * no próximo round os aviões decolem. */
                //round = ROUND_DECOLAGEM;
            break;
            
            case ROUND_DECOLAGEM:
                /* No round de decolagem, os aviões que devem decolar terão
                 * prioridade, entretanto, caso tenha algum avião que necessite
                 * pousar imediatamente (combustível 1), ele terá prioridade
                 * no pouso. Isso pode ser para qualquer das três pistas. */
                
                /* Primeiro, verificaremos se há algum avião nas prateleiras
                 * esperando para pousar e tem combustível 1. */
                if (!filaVazia(filaAterrissagem1) &&
                     filaAterrissagem1.primeiro->proximo->aviao.unidadesDeTempo == 1) {
                    
                    if (pista1.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem1, &pista1);
                        avioesPousadosRound++;
                    }
                    else if (pista3.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem1, &pista3);
                        avioesPousadosRound++;
                    }
                }
                
                if (!filaVazia(filaAterrissagem2) &&
                     filaAterrissagem2.primeiro->proximo->aviao.unidadesDeTempo == 1) {
                    
                    if (pista1.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem2, &pista1);
                        avioesPousadosRound++;
                    }
                    else if (pista3.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem2, &pista3);
                        avioesPousadosRound++;
                    }
                }
                
                if (!filaVazia(filaAterrissagem3) &&
                     filaAterrissagem3.primeiro->proximo->aviao.unidadesDeTempo == 1) {
                    
                    if (pista2.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem3, &pista2);
                        avioesPousadosRound++;
                    }
                    else if (pista3.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem3, &pista3);
                        avioesPousadosRound++;
                    }
                }
                
                if (!filaVazia(filaAterrissagem4) &&
                     filaAterrissagem4.primeiro->proximo->aviao.unidadesDeTempo == 1) {
                    if (pista2.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem4, &pista2);
                        avioesPousadosRound++;
                    }
                    else if (pista3.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem4, &pista3);
                        avioesPousadosRound++;
                    }
                }
                
                /* Passadas essas condições, podemos verificar a
                 * disponibilidade das pistas para que os aviões
                 * decolem. Como temos uma fila para cada pista,
                 * fica mais fácil a verificação. */
                if (!filaVazia(filaDecolagem1) && pista1.ocupado == PISTA_LIVRE) {
                    decolarAviao(&filaDecolagem1, &pista1);
                }
                if (!filaVazia(filaDecolagem2) && pista2.ocupado == PISTA_LIVRE) {
                    decolarAviao(&filaDecolagem2, &pista2);
                }
                if (!filaVazia(filaDecolagem3) && pista3.ocupado == PISTA_LIVRE) {
                    decolarAviao(&filaDecolagem3, &pista3);
                }
                
                /* Por fim, neste round, atualizamos a variával para que
                 * no próximo round os aviões aterrisem. */
                //round = ROUND_ATERRISSAGEM;
            break;
        }
        
        /* Após todo o processo ser feito, atualizamos as informações de
         * todas as prateleiras.*/
        atualizarInfosAviao(&filaAterrissagem1, ATUALIZAR_COMBUSTIVEL);
        atualizarInfosAviao(&filaAterrissagem2, ATUALIZAR_COMBUSTIVEL);
        atualizarInfosAviao(&filaAterrissagem3, ATUALIZAR_COMBUSTIVEL);
        atualizarInfosAviao(&filaAterrissagem4, ATUALIZAR_COMBUSTIVEL);
        atualizarInfosAviao(&filaDecolagem1, NAO_ATUALIZAR_COMBUSTIVEL);
        atualizarInfosAviao(&filaDecolagem2, NAO_ATUALIZAR_COMBUSTIVEL);
        atualizarInfosAviao(&filaDecolagem3, NAO_ATUALIZAR_COMBUSTIVEL);
        
        /* Também atualizamos as pistas para que elas
         * fiquem desponíveis no próximo round. */
        pista1.ocupado = PISTA_LIVRE;
        pista2.ocupado = PISTA_LIVRE;
        pista3.ocupado = PISTA_LIVRE;
        
        avioesPousadosTotal += avioesPousadosRound;
        
        atualizarInfosAviao(&filaDecolagem1, NAO_ATUALIZAR_COMBUSTIVEL);
        atualizarInfosAviao(&filaDecolagem2, NAO_ATUALIZAR_COMBUSTIVEL);
        atualizarInfosAviao(&filaDecolagem3, NAO_ATUALIZAR_COMBUSTIVEL);
        
        /* printf("Aviões caídos nesta iteração: %d\n", avioesCaidosRound);
        printf("Aviões caídos no total: %d\n", avioesCaidosTotal);
        printf("Aviões pousados nesta iteração: %d\n", avioesPousadosRound);
        printf("Aviões pousados no total: %d\n", avioesPousadosTotal); */
        
        avioesPousadosRound = 0; 
        
        printf("\n\tROUND: %s\n", round == ROUND_ATERRISSAGEM ? "Aterrissagem" : "Decolagem");
        
        printf("Fila 1 (A): "); imprimirFila(filaAterrissagem1);
        printf("Fila 2 (A): "); imprimirFila(filaAterrissagem2);
        printf("Fila 3 (A): "); imprimirFila(filaAterrissagem3);
        printf("Fila 4 (A): "); imprimirFila(filaAterrissagem4);
        
        printf("Fila 1 (D): "); imprimirFila(filaDecolagem1);
        printf("Fila 2 (D): "); imprimirFila(filaDecolagem2);
        printf("Fila 3 (D): "); imprimirFila(filaDecolagem3);
        
        printf("\n!!===== AQUI ACABA UMA ITERAÇÃO =====!!\n");
        
        if (round == ROUND_ATERRISSAGEM) round = ROUND_DECOLAGEM;
        else round = ROUND_ATERRISSAGEM;
        
        printf("Aperte qualquer tecla + ENTER para continuar ou ESC + ENTER para sair.\n");
        escape = getchar();
        getchar();
        
        //printf("\e[1;1H\e[2J");
        printf("\033[2J\033[H");
    }
    while (escape != ESC_KEY_CODE);
    
    return (EXIT_SUCCESS);
}

