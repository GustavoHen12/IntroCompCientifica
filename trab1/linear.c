#include "linear.h"

double  *calculculaSistemaLinear(double **sistema, double *termos_independentes, int tam){
  int *transformacoes = malloc(tam * sizeof(int));
  // Zerar triangulo inferior
  for(int i = 0; i < tam; i++){
    pivoteia(sistema, termos_independentes, transformacoes, i, tam);
    zeraAbaixo(sistema, termos_independentes, i, tam);
  }
  // Criação do vetor das variáveis
  double *variaveis = malloc(tam * sizeof(double));

  // Cálculo das variáveis
  for(int i = tam -1; i >= 0; i--){
    double total = 0;
    for(int j = i+1; j < tam; j++)
      total += sistema[i][j] * variaveis[j];
    printf("total: %0.2f\n", total);
    variaveis[i] = (termos_independentes[i] - total) / sistema[i][i];
  }

  // Despivoteia o sistema inteiro para retornar as variáveis corretas
  despivoteia(sistema, termos_independentes, transformacoes, variaveis, tam);

  return variaveis;
}

void pivoteia(double **sistema, double *termos_independentes, int *transformacoes, int n, int tam){
  int max = n; // Valor para salvar o índice da linha com maior enésimo elemento
  for(int i = n; i < tam; i++)
    if (sistema[i][n] > sistema[max][n]) max = i;

  // modifica vetor de transformacoes
  int temp = transformacoes[max];
  transformacoes[max] = transformacoes[n];
  transformacoes[n] = temp;

  // faz alteração necessária no sistema
  double *tempS = sistema[max];
  sistema[max] = sistema[n];
  sistema[n] = tempS;

  // faz alteração necessária nos termos independentes
  double tempI = termos_independentes[max];
  termos_independentes[max] = termos_independentes[n];
  termos_independentes[n] = tempI;
}

void despivoteia(double **sistema, double *termos_independentes, int *transformacoes, double *variaveis, int tam){
  for(int i = 0; i < tam - 1; i++){
    int trocar_com = transformacoes[i];

    // faz alteração necessária no sistema
    double *tempS = sistema[trocar_com];
    sistema[trocar_com] = sistema[i];
    sistema[i] = tempS;

    // faz alteração necessária nos termos independentes
    double tempD = termos_independentes[trocar_com];
    termos_independentes[trocar_com] = termos_independentes[i];
    termos_independentes[i] = tempD;

    // modifica vetor nas variaveis que foram calculadas
    tempD = variaveis[trocar_com];
    variaveis[trocar_com] = variaveis[i];
    variaveis[i] = tempD;

    // modifica vetor de transformacoes
    int temp = transformacoes[trocar_com];
    transformacoes[trocar_com] = transformacoes[i];
    transformacoes[i] = temp;

  }
}

void zeraAbaixo(double **sistema, double *termos_independentes, int n, int tam){
  for(int i = n+1; i < tam; i++){
    double m = -(sistema[i][n] / sistema[n][n]);
    sistema[i][n] = 0;
    for(int j = n + 1; j < tam; j++){
      sistema[i][j] += m * sistema[n][j];
    }
    termos_independentes[i] += m * termos_independentes[n];
  }
}
