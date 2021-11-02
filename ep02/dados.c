#include"dados.h"

void leTermosVetor(double **vetor, int n){
  *vetor = malloc(n * sizeof(double));
  aplica_func(*vetor, n);
}

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

void imprime_vetor(double *vetor, int n){
  for (int i = 0; i < n; i++){
    printf("%0.0f ", vetor[i]);
  }
  printf("\n");
}

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

void imprime_diagonais(Diagonais *D){
  int nMeio = D->k / 2;
  for (int i = 0; i < D->k; i++){
    int tamAtual = D->n - abs(i - nMeio);
    imprime_vetor(D->M[i], tamAtual);
  }
}

double getValorDiagonal(Diagonais *D, int coluna, int linha){
  // Verifica se esta dentro da matriz
  if(coluna >= D->n || linha >= D->n){
    return 0;
  }

  // Calcula em qual diagonal esta
  int diagonal = ((coluna - linha) * -1) + D->k/2;
  if(diagonal < 0 || diagonal >= D->k){
    return 0;
  }

  // Calcula qual o indice do elemento na diagonal
  int ind_diag = (coluna - linha) > 0 ? coluna - (coluna - linha) : coluna;  
  int tam_diag = D->n - abs(coluna - linha) - 1;
  if(ind_diag > tam_diag){
    return 0;
  }

  return D->M[diagonal][ind_diag];
}