#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <math.h>

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

int main(){
  Diagonais d;
  double *independentes;
  int n, k;
  leParametrosDiagonais(&d, &n, &k);
  leTermosVetor(&independentes, n);
  
  imprime_diagonais(&d);
  imprime_vetor(independentes, n);

  return 0;
}
