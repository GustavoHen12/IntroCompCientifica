/************
* CI1164-Trabalho 1
*
* Autores:
*   Iago Mello Floriano - GRR:20196049 - imf19@inf.ufpr.br
*   Gustavo H. da S. Barbosa - GRR:20190485 - ghsb19@inf.ufpr.br 
* 
************/

#include "linear.h"
#include <stdio.h>

double  *calculaSistemaLinear(double **sistema, double *termos_independentes, int tam){
  // Zerar triangulo inferior
  printf("sistema antes de zera diagonais\n");
  for (int j = 0; j < tam; j++){
    for (int i = 0; i < 3; i++){
      printf("[%0.6f] ", sistema[i][j]);
    }
    printf("\n");
  }
  zeraDiagonal(sistema, termos_independentes, tam);
  printf("sistema depois de zera diagonais\n");
  for (int j = 0; j < tam; j++){
    for (int i = 0; i < 3; i++){
      printf("[%0.6f] ", sistema[i][j]);
    }
    printf("\n");
  }
  // Criação do vetor das variáveis
  double *variaveis = malloc(tam * sizeof(double));

  // Cálculo das variáveis
  variaveis[tam-1] = termos_independentes[tam-1] / sistema[1][tam-1];
  for(int i = tam -2; i >= 0; i--){
    double total = 0;
    total += sistema[2][i] * variaveis[i + 1];
    variaveis[i] = (termos_independentes[i] - total) / sistema[1][i];
  }
  printf("Variaveis:\n");
  for (int i = 0; i < tam; i++)
    printf("[%0.6f] ", variaveis[i]);
  printf("\ntermos_independentes:\n");
  for (int i = 0; i < tam; i++)
    printf("[%0.6f] ", termos_independentes[i]);

  return variaveis;
}

void zeraDiagonal(double **sistema, double *termos_independentes, int tam){
  for(int i = 0; i < tam - 1; i++){
    double m = sistema[0][i+1] / sistema[1][i];
    sistema[0][i+1] = 0;
    sistema[1][i+1] -= sistema[2][i] * m;
    termos_independentes[i+1] -= termos_independentes[i] * m;
  }
}
