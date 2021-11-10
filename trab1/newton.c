#include "newton.h"

int verificaParada(SNL *snl) {
  return 0;
}

// Aplica x_0, x_1, .., x_n em uma função f
// sendo f = a_0*x_0 + a_1*x_1 + ... + a_n*x_n
// Os valores de x, estão em ordem em x, e a função
// corresponde a func. Retorna o valor resultante
double aplicaFuncao(void *func, double *x){
  if(func == NULL){
    fprintf(stderr, "Ocorreu um erro ao calcular o valor\n");
    return 0;
  }
  char **variaveis;
  int tamanho;
  evaluator_get_variables(func, &variaveis, &tamanho);
  return evaluator_evaluate(func, tamanho, variaveis, x);
}

// Recebe a matriz jacobiana ("matriz") de tamanho "n"x"n" e os termos independentes
// ("termos") e aplica. O resultado é colocado na matriz "resultado"
void aplicaTermosJacobiana(void ***matriz, double *termos, int n, double **resultado) {
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      resultado[i][j] = aplicaFuncao(matriz[i][j], termos);
    }
  }
}

// Aplica os termos "termos", na matriz "matriz" de tamanho "n"x"n".]
// O resultado de cada linha é colocado no vetor "resultado"
void aplicaTermosMatriz(void **matriz, double *termos, int n, double *resultado) {
  for(int i = 0; i < n; i++){
      resultado[i] = aplicaFuncao(matriz[i], termos) * -1;
  }
}

// Cria um vetor de tamanho "tamanho"
// se ocorrer um erro, adiciona em sterr e retorna NULL
double *criaVetor(int tamanho){
  double *vet = malloc(sizeof(double) * tamanho);
  if(vet == NULL){
    fprintf(stderr, "Ocorreu um erro ao criar um vetor !\n");
  }

  return vet;
}

// Cria matriz de "colunas" x "linhas"
// Se ocorrer algum erro, retorna NULL
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

// Copia um vetor A em um vetor B
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

double getMaiorAbs (double *vet, int n){
  double maior = -1;
  for(int i = 0; i < n; i++){
    if(fabs(vet[i]) > maior)
      maior = fabs(vet[i]);
  }

  return maior;
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
  while (iteracao < snl->max_iter) {
    
    printf("Iteracao %d ", iteracao);
    imprimeVetor(x, snl->n);

    // Calculo do delta
    // Montar sistema linear
    aplicaTermosJacobiana(snl->Jacobiana, x, snl->n, tempJacobiana);
    aplicaTermosMatriz(snl->F, x, snl->n, termosIndependentes);

    // Parada
    if(getMaiorAbs(termosIndependentes, snl->n) < snl->epsilon_1){
      break;
    }

    
    // Calcular sistema linear
    double  *delta = calculculaSistemaLinear(tempJacobiana, termosIndependentes, snl->n);    

    // Calculo Xi+1
    for(int i = 0; i < snl->n; i++){
      x[i] = x[i] + delta[i];
    }

    // Parada
    if(getMaiorAbs(delta, snl->n) < snl->epsilon_1){
      break;
    }
    
    iteracao++;
  }

  printf("Resultado ");
  imprimeVetor(x, snl->n);

  return NULL;
}