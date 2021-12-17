/************
* CI1164-Trabalho 2
*
* Autores:
*   Iago Mello Floriano - GRR:20196049 - imf19@inf.ufpr.br
*   Gustavo H. da S. Barbosa - GRR:20190485 - ghsb19@inf.ufpr.br 
* 
************/

#ifndef __NEWTON__
#define __NEWTON__

#include <math.h>
#include"dados.h"
#include"linear.h"

typedef struct {
  double tempoTotal;
  double tempoJacobianas;
  double tempoSistemaLinear;
  double tempoDerivadas;
} DadosExecucao;

#define UNROLL_SIZE 4

// Função responsável por cacular o resultado do sistema não linear "snl"
// através do método de newton. Os dados dos resultados parciais são
// impressos no arquivo "saida"
DadosExecucao *calculaSNL(SNL *snl, FILE *saida);

// Imprime os tempos de execução no arquivo saida
void imprimeDadosExecucao(DadosExecucao *dados, FILE *saida);

// Retorna um ponteiro para uma nova estrutura 
// com os tempos de execução zerados
DadosExecucao *iniciaDadosExecucao ();

// Cada uma das funções abaixo adicionam o tempo "tempo"
// aos dados de execução, se os dados não foram inicializados
// aponta erro
void adicionaTempoJacobiana(double tempo, DadosExecucao *dados);

void adicionaTempoDerivadas(double tempo, DadosExecucao *dados);

void adicionaTempoSistemaLinear(double tempo, DadosExecucao *dados);

void adicionaTempoTotal(double tempo, DadosExecucao *dados);

void encerraDadosExecucao(DadosExecucao *dados);

#endif
