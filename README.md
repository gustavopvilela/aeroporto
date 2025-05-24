# Trabalho de Estruturas de Dados I: _Simulação de um aeroporto_

![Image example](https://images.ctfassets.net/u73tyf0fa8v1/2DyATBgdpUywalG5xR63aE/4d43be3d4244ad62b0afec3b725f0199/3___OPERATE_YOUR_AIRPORT_AND_TERMINAL_copie1.jpg?w=3840&q=75)

Este repositório se refere ao trabalho prático da disciplina de Estruturas de Dados I, que visava construir uma simulação de um aeroporto em C utilizando as estruturas básicas, como filas.

## Contexto

O objetivo do trabalho é simular os padrões de aterrissagem e decolagem em um aeroporto. Suponha um aeroporto que possui três pistas, numeradas como 1, 2 e 3. Existem quatro “prateleiras” de espera para aterrissagem, duas para cada uma das pistas 1 e 2. Aeronaves que se aproximam do aeroporto devem integrar-se a uma das prateleiras (filas) de espera, sendo que estas filas devem procurar manter o mesmo tamanho. Assim que um avião entra em uma fila de aterrissagem, ele recebe um número de identificação ID e outro número inteiro que indica a quantidade de unidades de tempo que o avião pode permanecer na fila antes que ele tenha de descer (do contrário, seu combustível termina e ele cai).


Existem também filas para decolagem, uma para cada pista. Os aviões que chegam nessas filas também recebem uma identificação ID. Essas filas também devem procurar manter o mesmo tamanho.

## A simulação

O controle do simulador é realizado por _UT_ (Unidade de Tempo), sendo que, a cada unidade de tempo:

- de zero a três aeronaves podem chegar nas filas de decolagem;
- de zero a três aeronaves podem chegar nas prateleiras;
- cada pista pode ser usada para um pouso ou uma decolagem, sendo que a pista 3 em geral só é usada para decolagens, a não ser que um dos aviões nas prateleiras esteja para aterrissar e fique sem combustível, quando então ela deve ser imediatamente usada para pouso.

**OBS.:** se apenas uma aeronave está com falta de combustível, ela pousará na pista 3; se mais de um avião estiver nesta situação, as outras pistas poderão ser utilizadas (a cada unidade de tempo, no máximo três aviões poderão estar nesta situação).

Números inteiros pares sucessivos são usados para a identificação dos aviões chegando nas filas de decolagem. Números inteiros ímpares sucessivos são usados da mesma forma para os que chegam nas filas de aterrissagem. A cada unidade de tempo, os aviões entram nessas filas antes que as operações de decolagem e aterrissagem ocorram.

O algoritmo não permite o crescimento excessivo das filas, mantendo-as com tamanho parecido. Além disso, as filas não podem ser reordenadas, uma vez que o trabalho não implementa a estrutura de dados de fila de prioridade.

## Saída

A cada unidade de tempo, o programa indica o que está ocorrendo, imprimindo:

- o conteúdo de cada fila;
- o tempo médio de espera para decolagem;
- o tempo médio de espera para aterrissagem;
- o número de aviões que aterrissam sem reserva de combustível (chegam ao solo de forma segura restando apenas uma UT para acabar o combustível).

O segundo e o terceiro item são calculados para os aviões que já pousaram ou decolaram.

## Entrada

A entrada é feita gerando números pseudoaleatórios. Ou seja, para cada UT, são gerados:

- o número de aviões (0-3) chegando nas filas de aterrissagem com as respectivas reservas de combustível (1-20 unidades de tempo);
- o número de aviões (0-3) chegando nas filas de decolagem.

## Modularização

O código é modularizado, separando cada parte da aplicação (`aviao`, `fila` e `pista`) nos arquivos de código (`.c`) e nos arquivos de cabeçalho (`.h`), além do arquivo `main.c`.