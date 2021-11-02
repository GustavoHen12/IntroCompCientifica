#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <math.h>
#include "utils.h"
#include "dados.h"


void copiaVetor(double *vetA, double *vetB, int tam);

void imprimeResultado(double *X, int n);

// Inicializa D com k diagonais de matriz n x n
// aplica funções da entrada nessas diagonais com x sendo o indice do elemento na diagonal
void leParametrosDiagonais(Diagonais *D, int *n, int *k);

// Faz a leitura dos parâmetros para a execução do gauss-seidel
// Valor de epsilo e o número máximo de iterações
void leParametrosGaussSeildel(double *epsilon, int *max_iter);

// Maior diferença, em módulo, do valor de x anterior e o atual
double calculaErro(double *x_ant, double *x, int tam);

// Faz o cáullculo para UMA interação do método Gauss-Seidel
void iteracaoGaussSeildel(Diagonais *D, double *X, double *independentes);

// A partir das diagonais, valores de X iniciais, termos independentes calcula o valor da matriz
// Os critérios de parada levam em consideração se o erro é menor que o valor de epsilon ou se 
// foi atingido o número máximo de iterações
void gaussSeidel(Diagonais *diagonais, double *X, double *indenpendentes, int max_iter, double epsilon);

int main(){
  Diagonais d;
  double *independentes;
  double *x;
  int n, k;
  int max_iter;
  double epsilon;

  leParametrosDiagonais(&d, &n, &k);
  leTermosVetor(&independentes, n);
  leParametrosGaussSeildel(&epsilon, &max_iter);
  x = calloc(n, sizeof(double));
    
  // Calcula resultado
  double tempo;
  tempo = timestamp();

  gaussSeidel(&d, x, independentes, max_iter, epsilon);
  tempo = timestamp() - tempo;

  imprime_diagonais(&d);
  imprime_vetor(independentes, n);
  
  printf("----\n");

  printf("solução:");
  imprimeResultado(x, n);

  printf("tempo: %0.6f\n", tempo);
  return 0;
}

void copiaVetor(double *vetA, double *vetB, int tam){
  for(int i = 0; i < tam; i++){
    vetA[i] = vetB[i];
  }
}

void imprimeResultado(double *X, int n){
    for(int i = 0; i < n; i++){
      printf(" %0.6f", X[i]);
    }
    printf("\n");  
}

void leParametrosDiagonais(Diagonais *D, int *n, int *k){
  scanf("%d %d", n, k);
  getchar();
  inicializa_diagonais(D, *n, *k);
}

void leParametrosGaussSeildel(double *epsilon, int *max_iter){
  scanf("%lf", epsilon);
  scanf("%d", max_iter);
}

void iteracaoGaussSeildel(Diagonais *D, double *X, double *independentes){
  // para cada linha
  int p = D->k / 2; // superior
  int q = D->k / 2; // inferior
  double soma;
  for(int i = 0; i < D->n; i++){
    soma = 0;

    // Se linha possui elementos a direita da diagonal
    if(i < D->n){
      // Soma elementos a direita da diagonal principal
      for(int j = i+1; (j < D->n) && (j <= i+p); j++){
        soma += getValorDiagonal(D, j, i)*X[j];
      }
    }

    // Se linha possui elementos a esquerda da diagonal
    if(i > 0){
      // Soma elementos a esquerda da diagonal principal
      for(int j = i-1; (j >= 0) && (j >= (i-q)); j--){
        soma += getValorDiagonal(D, j, i)*X[j];
      }
    }

    // Subtrai dos termos independentes e divide
    double valor_iteracao = independentes[i] - soma;
    valor_iteracao = valor_iteracao / D->M[p][i];
    
    X[i] = valor_iteracao;
  }
}

double calculaErro(double *x_ant, double *x, int tam){
  double maior = 0;
  for(int i = 0; i < tam; i++){
    double erro = fabs((x[i] - x_ant[i]));
    if(erro > maior){
      maior = erro;
    }
  }

  return maior;
}

void gaussSeidel(Diagonais *diagonais, double *X, double *indenpendentes, int max_iter, double epsilon){
  int iteracao = 0;
  double erro = epsilon + 100;
  double *X_ant = calloc(diagonais->n, sizeof(double));
  while(iteracao < max_iter && !(erro < epsilon)){
    copiaVetor(X_ant, X, diagonais->n);

    iteracaoGaussSeildel(diagonais, X, indenpendentes);
    
    erro = calculaErro(X_ant, X, diagonais->n);
    iteracao++;
  }
}