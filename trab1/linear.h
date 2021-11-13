#ifndef __LINEAR__
#define __LINEAR__

#include <stdlib.h>
#include <math.h>

double  *calculculaSistemaLinear(double **sistema, double *termos_independentes, int tam);

// Faz linha n de sistema com tam dimensoes
// ter o enésimo elemento ser o maior quando comparado as linhas com n maior
// salva qual mudança foi feita no vetor transformacoes
// o vetor transformacoes deverá conter elementos de 0 até n-1
void pivoteia(double **sistema, double *termos_independentes, int *transformacoes, int n, int tam);

// Desfaz as trasnformações descritas no vetor transformações
// o vetor transformacoes deverá conter elementos de 0 até n-1
void despivoteia(double **sistema, double *termos_independentes, int *transformacoes, double *variaveis, int tam);

// Faz as multiplicaçoes e somas necessárias para  zerar os termos abaixo do elemento [n][n]
// no sistema, atualiza também o valor nos termos independentes para nao alterar o resultado
// essa função considera q o termo n é o primeiro termo nao nulo da linha
void zeraAbaixo(double **sistema, double *termos_independentes, int n, int tam);

#endif //__LINEAR__
