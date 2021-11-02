#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <math.h>
#include "utils.h"

#define INPUT_SIZE 256

typedef struct {
  double **M; //Matriz onde terá as diagonais
  int n; //Tamanho da maior diagonal
  int k; //Quantidade de diagonais
} Diagonais;

// Imprime vetor de doubles com 6 casas decimais
void imprime_vetor(double *vetor, int n){
  for (int i = 0; i < n; i++){
    printf("[%0.6f] ", vetor[i]);
  }
  printf("\n");
}

// Aplica função da entrada no vetor passado, o indice do numero no vetor será o valor de x
void aplica_func(double *vetor, int n){
  void *f;
  char func[INPUT_SIZE];
  scanf("%[^\t\n]s", func);
  getchar();
  f = evaluator_create(func);

  for (int i = 0; i < n; i++){
    vetor[i] = evaluator_evaluate_x(f, i);
  }
}

// Inicializa vetor de tamanho n
// Aplica função da entrada em seus elementos, sendo x o indice do elemento
void leTermosVetor(double **vetor, int n){
  *vetor = malloc(n * sizeof(double));
  aplica_func(*vetor, n);
}

// Inicia D com n diagonais sendo a maior de tamanho tam
// Também aplica as funções dadas na entrada
void inicializa_diagonais(Diagonais *D, int n, int k){
  D->M = (double **)malloc(k * sizeof(double *));
  int tamAtual = n - (k / 2);
  int incrTam = 1;
  for(int i=0; i < k; i++){
    leTermosVetor(&(D->M[i]), tamAtual);
    if (tamAtual == n) incrTam = -1;
    tamAtual += incrTam;
  }
  D->k = k;
  D->n = n;
}

// Imprime diagonais com 6 casa decimais
void imprime_diagonais(Diagonais *D){
  int nMeio = D->k / 2;
  for (int i = 0; i < D->k; i++){
    int tamAtual = D->n - abs(i - nMeio);
    imprime_vetor(D->M[i], tamAtual);
  }
}

// Lê função de stdin e aplica nas diagonais
// valor de x é igual ao indice do elemento na diagonal
void funcoesStdin_diagonais(Diagonais *D){
  int nMeio = D->k / 2;
  for (int i = 0; i < D->k; i++){
    printf("Vai aplicar func\n");
    int tamAtual = D->n - abs(i - nMeio);
    printf("Tamanho diagonal: %d\n", tamAtual);
    aplica_func(D->M[i], tamAtual);
  }
}

// Inicializa D com k diagonais de matriz n x n
// aplica funções da entrada nessas diagonais com x sendo o indice do elemento na diagonal
void leParametrosDiagonais(Diagonais *D, int *n, int *k){
  scanf("%d %d", n, k);
  getchar();
  inicializa_diagonais(D, *n, *k);
}

/*******/
//Funções para calculo do gauss seidel

double getValorDiagonal(Diagonais *D, int coluna, int linha){
  if(coluna >= D->n || linha >= D->n){
    return 0;
  }

  int diagonal = ((coluna - linha) * -1) + D->k/2;

  if(diagonal < 0 || diagonal >= D->k){
    return 0;
  }

  int elem_diag = (coluna - linha) > 0 ? coluna - (coluna - linha) : coluna;  

  int tam_diag = D->n - abs(coluna - linha) - 1;
  if(elem_diag > tam_diag){
    return 0;
  }

  return D->M[diagonal][elem_diag];
}

void iteracaoGaussSeildel(Diagonais *D, double *X, double *independentes, int iteracao){
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
    //printf("Soma direita: %0.6f \n", soma);

    // Se linha possui elementos a esquerda da diagonal
    if(i > 0){
      // Soma elementos a esquerda da diagonal principal
      for(int j = i-1; (j >= 0) && (j >= (i-q)); j--){
        soma += getValorDiagonal(D, j, i)*X[j];
      }
    }
    //printf("Soma total: %0.6f \n", soma);

    // Subtrai dos termos independentes e divide
    double valor_iteracao = independentes[i] - soma;
    valor_iteracao = valor_iteracao / D->M[p][i];
    
    //printf("X%d: %0.6f \n", i, valor_iteracao);
    X[i] = valor_iteracao;
  }
}

int criterioParada(double erro, double epsilon){
  return 0;
}

double calculaErro(Diagonais* d){
  return 1.0;
}

void gaussSeidel(Diagonais *diagonais, double *X, double *indenpendentes, int max_iter, double epsilon){
  int iteracao = 0;
  double erro = epsilon + 100;
  while(iteracao < max_iter && !criterioParada(erro, epsilon)){
    iteracaoGaussSeildel(diagonais, X, indenpendentes, iteracao);

    printf("> %d :", iteracao);
    for(int i = 0; i < diagonais->n; i++){
      printf(" X[%d] = %0.6f", i, X[i]);
    }
    printf("\n");

    erro = calculaErro(diagonais);
    iteracao++;
  }
}

int main(){
  Diagonais d;
  double *independentes;
  double *x;
  int n, k;
  int max_iter;
  double epsilon;

  leParametrosDiagonais(&d, &n, &k);
  leTermosVetor(&independentes, n);
  scanf("%lf", &epsilon);
  scanf("%d", &max_iter);
  x = malloc(n * sizeof(double));

  // Zera vetor com resultados
  for(int i = 0; i < n; i++){
    x[i] = 0;
  }

  // Imprime "Matriz"
  for(int i = 0; i < n; i++){
    printf("|");
    for(int j = 0; j < n; j++){
      printf(" %0.2f ", getValorDiagonal(&d, j, i));
      fflush(stdout);
    }
    printf("|   | %0.2f |\n", independentes[i]);
    
  }
  
  // Calcula resultado
  double tempo;
  tempo = timestamp();

  gaussSeidel(&d, x, independentes, max_iter, epsilon);
  tempo = timestamp() - tempo;

  imprime_diagonais(&d);
  imprime_vetor(independentes, n);

  printf("solução:");
  for(int i = 0; i < n; i++){
    printf(" %0.6f", x[i]);
  }
  printf("\n");
  printf("tempo: %0.6f\n", tempo);
  return 0;
}
