#include "newton.h"

int verificaParada(SNL *snl) {
  return 0;
}

double aplicaFuncao(void *func, double *x){
  if(func == NULL){
    fprintf(stderr, "Ocorreu um erro ao calcular o valor\n");
    return 0;
  }
  char **variaveis;
  int tamanho;
  evaluator_get_variables(func, &variaveis, &tamanho);
  printf("> aplica %s  = %0.2f\n", evaluator_get_string(func), evaluator_evaluate(func, tamanho, variaveis, x));
  return evaluator_evaluate(func, tamanho, variaveis, x);
}

void aplicaTermosJacobiana(void ***matriz, double *termos, int n, double **resultado) {
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      resultado[i][j] = aplicaFuncao(matriz[i][j], termos);
    }
  }
}

void aplicaTermosMatriz(void **matriz, double *termos, int n, double *resultado) {
  for(int i = 0; i < n; i++){
      resultado[i] = aplicaFuncao(matriz[i], termos);
  }
}


double *criaVetor(int tamanho){
  double *vet = malloc(sizeof(double) * tamanho);
  if(vet == NULL){
    fprintf(stderr, "Ocorreu um erro ao criar um vetor !\n");
  }

  return vet;
}

double **criaMatriz(int colunas, int linhas){
  double **mat = malloc (linhas * sizeof (double*));
  if(mat == NULL){
    fprintf(stderr, "Ocorreu um erro ao criar a matriz\n");
    return NULL;
  }

  for (int i = 0; i < linhas; i++)
    mat[i] = malloc (colunas * sizeof (double));

  return mat;
}

void copiaVetor(double *vetA, double *vetB, int tam){
  if(vetA != NULL && vetB != NULL){
    for(int i = 0; i< tam; i++)
      vetB[i] = vetA[i];
  }
}

void imprimeMatriz(double **matriz, int n) {
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      printf(" %0.6f ", matriz[i][j]);
    }
    printf("\n");
  }
  printf("\n");

}

void imprimeVetor(double *vet, int n) {
  for(int i = 0; i < n; i++){
    printf(" %0.6f ", vet[i]);
  }
  printf("\n");
}


Resultado *calculaSNL(SNL *snl, FILE *saida){
  //verificar se SNL não é NULL
  if (snl == NULL || !(snl->n > 0)) {
    fprintf(stderr, "Sistema não linear inválido !\n");
  }

  double *x = criaVetor(snl->n);
  double **tempJacobiana = criaMatriz(snl->n, snl->n);
  double *termosIndependentes = criaVetor(snl->n);

  if(x == NULL || tempJacobiana == NULL || termosIndependentes == NULL){
    return NULL;
  }

  copiaVetor(snl->aprox_inicial, x, snl->n);

  int iteracao = 0;
  while (iteracao < snl->max_iter && !verificaParada(snl)) {
    // Calculo do delta
    // Montar sistema linear
    aplicaTermosJacobiana(snl->Jacobiana, x, snl->n, tempJacobiana);
    aplicaTermosMatriz(snl->F, x, snl->n, termosIndependentes);

    imprimeMatriz(tempJacobiana, snl->n);
    imprimeVetor(termosIndependentes, snl->n);
    // Calcular sistema linear

    // Calculo Xi+1

    // calcula delta iteracao
    iteracao++;
  }

  return NULL;
}