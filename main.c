#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "fila.h"
#include "aviao.h"
#include "pista.h"



int main(int argc, char** argv) {
    srand(time(NULL)); /* Seed geradora de números aleatórios. */
    
    int contador = 0;
    
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
    
    /* Variáveis de informações. */
    int avioesCaidosRound = 0;
    int avioesCaidosTotal = 0;
    int avioesPousadosRound = 0;
    int avioesPousadosTotal = 0;
    int avioesDecoladosRound = 0;
    int avioesDecoladosTotal = 0;
    
    int ultimoIdDecolagem = 0;
    int ultimoIdAterrissagem = 1;
    
    int avioesPousadosSemReservaDeCombustivel = 0;
    
    int tempoEsperaDecolagemTotal = 0;
    int tempoEsperaAterrissagemTotal = 0;
    
    float mediaEsperaDecolagem;
    float mediaEsperaAterrissagem;
    
    int escape; /* Usado para terminar o programa. */
    int round = ROUND_ATERRISSAGEM; /* Define se o round será de pouso ou decolagem. */
    
    /* Simulação em si. */
    do {
        /* Antes de processarmos pousos e decolagens, é gerado as filas
         * temporárias dos aviões (função de gerar aviões), que serão
         * transferidos para suas respectivas filas (função alocarAvioes()). */
        
        tempAterrissagem = gerarAvioesAterrissagem(&ultimoIdAterrissagem);
        tempDecolagem = gerarAvioesDecolagem(&ultimoIdDecolagem);
        
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
                if (filaVazia(filaAterrissagem1) == 0 && filaVazia(filaAterrissagem2) == 1) {
                    adicionarTempoEspera(filaAterrissagem1, &tempoEsperaAterrissagemTotal);
                    
                    if (filaAterrissagem1.primeiro->proximo->aviao.unidadesDeTempo == 1) avioesPousadosSemReservaDeCombustivel++;
                    
                    pousarAviao(&filaAterrissagem1, &pista1);
                    avioesPousadosRound++;
                }
                else if (filaVazia(filaAterrissagem1) == 1 && filaVazia(filaAterrissagem2) == 0) {
                    adicionarTempoEspera(filaAterrissagem2, &tempoEsperaAterrissagemTotal);
                    
                    if (filaAterrissagem2.primeiro->proximo->aviao.unidadesDeTempo == 1) avioesPousadosSemReservaDeCombustivel++;
                    
                    pousarAviao(&filaAterrissagem2, &pista1);
                    avioesPousadosRound++;
                }
                else if (filaVazia(filaAterrissagem1) == 0 && filaVazia(filaAterrissagem2) == 0) {
                    
                    if ((filaAterrissagem1.primeiro->proximo->aviao.unidadesDeTempo
                        + filaAterrissagem2.primeiro->proximo->aviao.unidadesDeTempo) / 2 == 1) {

                        adicionarTempoEspera(filaAterrissagem1, &tempoEsperaAterrissagemTotal);
                        adicionarTempoEspera(filaAterrissagem2, &tempoEsperaAterrissagemTotal);
                        
                        /* Nesse caso, eu vou pousar os dois aviões:
                        * um na pista para ele, e outro na pista 3. Nesse
                        * passo, já está verificado que as duas filas possuem
                        * aviões. */
                       pousarAviao(&filaAterrissagem1, &pista1);
                       pousarAviao(&filaAterrissagem2, &pista3);
                       
                       avioesPousadosSemReservaDeCombustivel += 2;
                       
                       avioesPousadosRound++;
                       avioesPousadosRound++;
                    }
                    else if (filaAterrissagem1.primeiro->proximo->aviao.unidadesDeTempo <=
                             filaAterrissagem2.primeiro->proximo->aviao.unidadesDeTempo) {
                        
                        if (filaAterrissagem1.primeiro->proximo->aviao.unidadesDeTempo == 1) {
                            /* Aqui, pousamos os dois aviões de uma vez,
                             * um na pista 1 e outro na pista 3. */
                            adicionarTempoEspera(filaAterrissagem1, &tempoEsperaAterrissagemTotal);
                            adicionarTempoEspera(filaAterrissagem2, &tempoEsperaAterrissagemTotal);
                            
                            pousarAviao(&filaAterrissagem1, &pista3);
                            pousarAviao(&filaAterrissagem2, &pista1);
                            
                            avioesPousadosRound += 2;
                            avioesPousadosSemReservaDeCombustivel++;
                        }
                        else {
                            /* Como nenhum dos dois aviões está com 1 de
                             * combustível, pousamos o que tem a menor
                             * quantidade somente. */
                            adicionarTempoEspera(filaAterrissagem1, &tempoEsperaAterrissagemTotal);
                            
                            pousarAviao(&filaAterrissagem1, &pista1);
                            avioesPousadosRound++;
                        }
                    }
                    else if (filaAterrissagem2.primeiro->proximo->aviao.unidadesDeTempo <
                             filaAterrissagem1.primeiro->proximo->aviao.unidadesDeTempo) {
                        
                        if (filaAterrissagem2.primeiro->proximo->aviao.unidadesDeTempo == 1) {
                            /* Aqui, pousamos os dois aviões de uma vez,
                             * um na pista 1 e outro na pista 3. */
                            adicionarTempoEspera(filaAterrissagem2, &tempoEsperaAterrissagemTotal);
                            adicionarTempoEspera(filaAterrissagem1, &tempoEsperaAterrissagemTotal);
                            
                            pousarAviao(&filaAterrissagem2, &pista3);
                            pousarAviao(&filaAterrissagem1, &pista1);
                            
                            avioesPousadosRound += 2;
                            avioesPousadosSemReservaDeCombustivel++;
                        }
                        else {
                            /* Como nenhum dos dois aviões está com 1 de
                             * combustível, pousamos o que tem a menor
                             * quantidade somente. */
                            adicionarTempoEspera(filaAterrissagem2, &tempoEsperaAterrissagemTotal);
                            
                            pousarAviao(&filaAterrissagem2, &pista1);
                            avioesPousadosRound++;
                        }
                    }
                }
                
                /* Verificando as filas 3 e 4. */
                if (filaVazia(filaAterrissagem3) == 0 && filaVazia(filaAterrissagem4) == 1) {
                    adicionarTempoEspera(filaAterrissagem3, &tempoEsperaAterrissagemTotal);
                    
                    if (filaAterrissagem3.primeiro->proximo->aviao.unidadesDeTempo == 1) avioesPousadosSemReservaDeCombustivel++;
                    
                    pousarAviao(&filaAterrissagem3, &pista2);
                    avioesPousadosRound++;
                }
                else if (filaVazia(filaAterrissagem3) == 1 && filaVazia(filaAterrissagem4) == 0) {
                    adicionarTempoEspera(filaAterrissagem4, &tempoEsperaAterrissagemTotal);
                    
                    if (filaAterrissagem4.primeiro->proximo->aviao.unidadesDeTempo == 1) avioesPousadosSemReservaDeCombustivel++;
                    
                    pousarAviao(&filaAterrissagem4, &pista2);
                    avioesPousadosRound++;
                }
                else if (filaVazia(filaAterrissagem3) == 0 && filaVazia(filaAterrissagem4) == 0) {
                    
                    if ((filaAterrissagem3.primeiro->proximo->aviao.unidadesDeTempo
                        + filaAterrissagem4.primeiro->proximo->aviao.unidadesDeTempo) / 2 == 1) {

                        /* Nesse caso, eu vou pousar os dois aviões:
                        * um na pista para ele, e outro na pista 3. Nesse
                        * passo, já está verificado que as duas filas possuem
                        * aviões. Como neste passo não há 100% de certeza de
                        * que a pista 3 estrá livre, preciso verificar isso
                        * antes de inserir nela. Caso esteja atualmente
                        * ocupada, nada será feito. */
                        adicionarTempoEspera(filaAterrissagem3, &tempoEsperaAterrissagemTotal);
                        pousarAviao(&filaAterrissagem3, &pista2);
                        avioesPousadosRound++;
                        avioesPousadosSemReservaDeCombustivel++;
                       
                       if (pista3.ocupado == PISTA_LIVRE) {
                           adicionarTempoEspera(filaAterrissagem4, &tempoEsperaAterrissagemTotal);
                           pousarAviao(&filaAterrissagem4, &pista3);
                           avioesPousadosRound++;
                           avioesPousadosSemReservaDeCombustivel++;
                       }
                    }
                    else if (filaAterrissagem3.primeiro->proximo->aviao.unidadesDeTempo <=
                             filaAterrissagem4.primeiro->proximo->aviao.unidadesDeTempo) {
                        
                        if (filaAterrissagem3.primeiro->proximo->aviao.unidadesDeTempo == 1) {
                            /* Aqui, pousamos os dois aviões de uma vez,
                             * um na pista 2 e outro na pista 3, caso esteja livre. */
                            if (pista3.ocupado == PISTA_LIVRE) {
                                adicionarTempoEspera(filaAterrissagem3, &tempoEsperaAterrissagemTotal);
                                adicionarTempoEspera(filaAterrissagem4, &tempoEsperaAterrissagemTotal);

                                pousarAviao(&filaAterrissagem3, &pista3);
                                pousarAviao(&filaAterrissagem4, &pista2);

                                avioesPousadosRound += 2;
                                avioesPousadosSemReservaDeCombustivel++;
                            }
                            else {
                                adicionarTempoEspera(filaAterrissagem3, &tempoEsperaAterrissagemTotal);
                                pousarAviao(&filaAterrissagem3, &pista2);
                                avioesPousadosRound++;
                                avioesPousadosSemReservaDeCombustivel++;
                            }
                        }
                        else {
                            /* Como nenhum dos dois aviões está com 1 de
                             * combustível, pousamos o que tem a menor
                             * quantidade somente. */
                            adicionarTempoEspera(filaAterrissagem3, &tempoEsperaAterrissagemTotal);
                            
                            pousarAviao(&filaAterrissagem3, &pista2);
                            avioesPousadosRound++;
                        }
                    }
                    else if (filaAterrissagem4.primeiro->proximo->aviao.unidadesDeTempo <
                             filaAterrissagem3.primeiro->proximo->aviao.unidadesDeTempo) {
                        
                        if (filaAterrissagem4.primeiro->proximo->aviao.unidadesDeTempo == 1) {
                            /* Aqui, pousamos os dois aviões de uma vez,
                             * um na pista 2 e outro na pista 3, caso esteja livre. */
                            if (pista3.ocupado == PISTA_LIVRE) {
                                adicionarTempoEspera(filaAterrissagem4, &tempoEsperaAterrissagemTotal);
                                adicionarTempoEspera(filaAterrissagem3, &tempoEsperaAterrissagemTotal);

                                pousarAviao(&filaAterrissagem4, &pista3);
                                pousarAviao(&filaAterrissagem3, &pista2);

                                avioesPousadosRound += 2;
                                avioesPousadosSemReservaDeCombustivel++;
                            }
                            else {
                                adicionarTempoEspera(filaAterrissagem4, &tempoEsperaAterrissagemTotal);
                                pousarAviao(&filaAterrissagem4, &pista2);
                                avioesPousadosRound++;
                                avioesPousadosSemReservaDeCombustivel++;
                            }
                        }
                        else {
                            /* Como nenhum dos dois aviões está com 1 de
                             * combustível, pousamos o que tem a menor
                             * quantidade somente. */
                            adicionarTempoEspera(filaAterrissagem4, &tempoEsperaAterrissagemTotal);
                            
                            pousarAviao(&filaAterrissagem4, &pista2);
                            avioesPousadosRound++;
                        }
                    }
                }
                
                /* Verificando disponibilidade de pistas para decolagem. */
                if (filaVazia(filaDecolagem1) == 0 && pista1.ocupado == PISTA_LIVRE) {
                    adicionarTempoEspera(filaDecolagem1, &tempoEsperaDecolagemTotal);
                    decolarAviao(&filaDecolagem1, &pista1);
                    avioesDecoladosRound++;
                }
                if (filaVazia(filaDecolagem2) == 0 && pista2.ocupado == PISTA_LIVRE) {
                    adicionarTempoEspera(filaDecolagem2, &tempoEsperaDecolagemTotal);
                    decolarAviao(&filaDecolagem2, &pista2);
                    avioesDecoladosRound++;
                }
                if (filaVazia(filaDecolagem3) == 0 && pista3.ocupado == PISTA_LIVRE) {
                    adicionarTempoEspera(filaDecolagem3, &tempoEsperaDecolagemTotal);
                    decolarAviao(&filaDecolagem3, &pista3);
                    avioesDecoladosRound++;
                }
                
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
                if (filaVazia(filaAterrissagem1) == 0 &&
                     filaAterrissagem1.primeiro->proximo->aviao.unidadesDeTempo == 1) {
                    
                    if (pista1.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem1, &pista1);
                        avioesPousadosRound++;
                        avioesPousadosSemReservaDeCombustivel++;
                    }
                    else if (pista3.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem1, &pista3);
                        avioesPousadosRound++;
                        avioesPousadosSemReservaDeCombustivel++;
                    }
                }
                
                if (filaVazia(filaAterrissagem2) == 0 &&
                     filaAterrissagem2.primeiro->proximo->aviao.unidadesDeTempo == 1) {
                    
                    if (pista1.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem2, &pista1);
                        avioesPousadosRound++;
                        avioesPousadosSemReservaDeCombustivel++;
                    }
                    else if (pista3.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem2, &pista3);
                        avioesPousadosRound++;
                        avioesPousadosSemReservaDeCombustivel++;
                    }
                }
                
                if (filaVazia(filaAterrissagem3) == 0 &&
                     filaAterrissagem3.primeiro->proximo->aviao.unidadesDeTempo == 1) {
                    
                    if (pista2.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem3, &pista2);
                        avioesPousadosRound++;
                        avioesPousadosSemReservaDeCombustivel++;
                    }
                    else if (pista3.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem3, &pista3);
                        avioesPousadosRound++;
                        avioesPousadosSemReservaDeCombustivel++;
                    }
                }
                
                if (filaVazia(filaAterrissagem4) == 0 &&
                     filaAterrissagem4.primeiro->proximo->aviao.unidadesDeTempo == 1) {
                    if (pista2.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem4, &pista2);
                        avioesPousadosRound++;
                        avioesPousadosSemReservaDeCombustivel++;
                    }
                    else if (pista3.ocupado == PISTA_LIVRE) {
                        pousarAviao(&filaAterrissagem4, &pista3);
                        avioesPousadosRound++;
                        avioesPousadosSemReservaDeCombustivel++;
                    }
                }
                
                /* Passadas essas condições, podemos verificar a
                 * disponibilidade das pistas para que os aviões
                 * decolem. Como temos uma fila para cada pista,
                 * fica mais fácil a verificação. */
                if (filaVazia(filaDecolagem1) == 0 && pista1.ocupado == PISTA_LIVRE) {
                    adicionarTempoEspera(filaDecolagem1, &tempoEsperaDecolagemTotal);
                    decolarAviao(&filaDecolagem1, &pista1);
                    avioesDecoladosRound++;
                }
                if (filaVazia(filaDecolagem2) == 0 && pista2.ocupado == PISTA_LIVRE) {
                    adicionarTempoEspera(filaDecolagem2, &tempoEsperaDecolagemTotal);
                    decolarAviao(&filaDecolagem2, &pista2);
                    avioesDecoladosRound++;
                }
                if (filaVazia(filaDecolagem3) == 0 && pista3.ocupado == PISTA_LIVRE) {
                    adicionarTempoEspera(filaDecolagem3, &tempoEsperaDecolagemTotal);
                    decolarAviao(&filaDecolagem3, &pista3);
                    avioesDecoladosRound++;
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
        avioesDecoladosTotal += avioesDecoladosRound;
        
        atualizarInfosAviao(&filaDecolagem1, NAO_ATUALIZAR_COMBUSTIVEL);
        atualizarInfosAviao(&filaDecolagem2, NAO_ATUALIZAR_COMBUSTIVEL);
        atualizarInfosAviao(&filaDecolagem3, NAO_ATUALIZAR_COMBUSTIVEL);
        
        /* Caso nenhum avião tenha pousado ou decolado no round (por as
         * filas estarem vazias), mostramos uma mensagem para que o usuári
         * não fique perdido. */
        if (avioesPousadosRound == 0 && avioesDecoladosRound == 0) {
            printf("\n\t-\tNENHUM avião pousou ou decolou nesta iteração.\n\n");
        }
        
        printf("\n\n\n");
        
        /* Imprimindo o conteúdo das filas. */
        if (filaVazia(filaAterrissagem1) == 0) {
            printf("\n\tFila 1 (A):\n");
            imprimirFila(filaAterrissagem1);
        }
        if (filaVazia(filaAterrissagem2) == 0) {
            printf("\n\tFila 2 (A):\n");
            imprimirFila(filaAterrissagem2);
        }
        if (filaVazia(filaAterrissagem3) == 0) {
            printf("\n\tFila 3 (A):\n");
            imprimirFila(filaAterrissagem3);
        }
        if (filaVazia(filaAterrissagem4) == 0) {
            printf("\n\tFila 4 (A):\n");
            imprimirFila(filaAterrissagem4);
        }
        if (filaVazia(filaDecolagem1) == 0) {
            printf("\n\tFila 1 (D):\n");
            imprimirFila(filaDecolagem1);
        }
        if (filaVazia(filaDecolagem2) == 0) {
            printf("\n\tFila 2 (D):\n");
            imprimirFila(filaDecolagem2);
        }
        if (filaVazia(filaDecolagem3) == 0) {
            printf("\n\tFila 3 (D):\n");
            imprimirFila(filaDecolagem3);
        }
        
        avioesPousadosRound = 0; 
        avioesDecoladosRound = 0;
        
        if (round == ROUND_ATERRISSAGEM) round = ROUND_DECOLAGEM;
        else round = ROUND_ATERRISSAGEM;
        
        printf("\n\nAperte qualquer tecla + ENTER!! para continuar ou ESC + ENTER para sair.\n");
        escape = getchar();
        getchar();
        
        //printf("\e[1;1H\e[2J");
        printf("\033[2J\033[H");
        
        contador++;
    }
    while (escape != ESC_KEY_CODE);
    
    /* Cálculo das estatísticas restantes. */
    float porcentagemAvioesCaidos = (avioesCaidosTotal * 100) / (avioesCaidosTotal + avioesPousadosTotal);
    mediaEsperaAterrissagem = tempoEsperaAterrissagemTotal / avioesPousadosTotal;
    mediaEsperaDecolagem = tempoEsperaDecolagemTotal / avioesDecoladosTotal;
    
    /* Estatísticas finais após a simulação. */
    printf("\n\t=== ESTATÍSTICAS ===");
    printf("\n\tTotal de aviões caídos: %d", avioesCaidosTotal);
    printf("\n\tTotal de aviões aterrissados: %d", avioesPousadosTotal);
    printf("\n\tTotal de aviões decolados: %d", avioesDecoladosTotal);
    printf("\n\tRazão de aviões caídos por total de aviões: %.2f%%", porcentagemAvioesCaidos);
    printf("\n\tMédia de espera para aterrissagem: %.2f ciclos", mediaEsperaAterrissagem);
    printf("\n\tMédia de espera para decolagem: %.2f ciclos", mediaEsperaDecolagem);
    printf("\n\tTotal de aviões que pousaram sem reserva de combustível: %d", avioesPousadosSemReservaDeCombustivel);
    
    return (EXIT_SUCCESS);
}