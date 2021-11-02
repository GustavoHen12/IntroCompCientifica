#ifndef __DADOS__
#define __DADOS__

#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>

#define INPUT_SIZE 256

typedef struct {
  double **M; //Matriz onde terá as diagonais
  int n; //Tamanho da maior diagonal
  int k; //Quantidade de diagonais
} Diagonais;

// Inicia D com n diagonais sendo a maior de tamanho tam
// Também aplica as funções dadas na entrada
void inicializa_diagonais(Diagonais *D, int n, int k);

// Inicializa vetor de tamanho n
// Aplica função da entrada em seus elementos, sendo x o indice do elemento
void leTermosVetor(double **vetor, int n);

// Imprime vetor de doubles com 6 casas decimais
void imprime_vetor(double *vetor, int n);

// Aplica função da entrada no vetor passado, o indice do numero no vetor será o valor de x
void aplica_func(double *vetor, int n);

// Imprime diagonais com 6 casa decimais
void imprime_diagonais(Diagonais *D);

// Recebe uma linha e coluna da matrix
// e a partir das diagonais D retorna o elemento correspondente
double getValorDiagonal(Diagonais *D, int coluna, int linha);

#endif
