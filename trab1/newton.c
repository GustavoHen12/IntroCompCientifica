/************
* CI1164-Trabalho 1
*
* Autores:
*   Iago Mello Floriano - GRR:20196049 - imf19@inf.ufpr.br
*   Gustavo H. da S. Barbosa - GRR:20190485 - ghsb19@inf.ufpr.br 
* 
************/

#include "newton.h"

// ============ Funcões auxiliares ============

double getMaiorAbs (double *vet, int n){
  double maior = -1;
  for(int i = 0; i < n; i++){
    if(fabs(vet[i]) > maior)
      maior = fabs(vet[i]);
  }

  return maior;
}

// ============ Funcões para manipulação da libmatheval ============

// Aplica x_0, x_1, .., x_n em uma função f
// sendo f = a_0*x_0 + a_1*x_1 + ... + a_n*x_n
// Os valores de x, estão em ordem em x, e a função
// corresponde a func. Retorna o valor resultante
double aplicaFuncao(void *func, double *x, int tamanho){
  if(func == NULL){
    fprintf(stderr, "Ocorreu um erro ao calcular o valor\n");
    return 0;
  }
  char **variaveis;
  variaveis = malloc(sizeof(char **) * tamanho);
  for (int i = 0; i < tamanho; i++){
    variaveis[i] = malloc(sizeof(char *) * tamanho);
    sprintf(variaveis[i], "x%d", i+1);
  }
  return evaluator_evaluate(func, tamanho, variaveis, x);
}


// ============ Impressão da saída ============

void imprimeInicio(void **F, int num, FILE *saida){
  fprintf(saida, "%d\n", num);
  for (int i = 0; i < num; i++)
    fprintf(saida, "%s = 0\n", evaluator_get_string(F[i]));
}

void imprimeResultado(double *vet, char **variaveis, int tamanho, FILE *saida) {
  fprintf(saida, "#\n");
  for(int i = 0; i < tamanho; i++){
    fprintf(saida, "%s = %0.6f\n", variaveis[i], vet[i]);
  }
}

void imprimeDadosExecucao(DadosExecucao *dados, FILE *saida) {
  fprintf(saida, "###########\n");
  fprintf(saida, "# Tempo Total: %0.6f\n", dados->tempoTotal);
  fprintf(saida, "# Tempo Derivadas: %0.6f\n", dados->tempoDerivadas);
  fprintf(saida, "# Tempo Jacobiana: %0.6f\n", dados->tempoJacobianas);
  fprintf(saida, "# Tempo SL: %0.6f\n", dados->tempoSistemaLinear);
  fprintf(saida, "###########\n\n");

}


// ============ Funcões para manipulação dos dados dos vetores e matriz ============

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


// ============ Calculo SNL ============

// Recebe a matriz jacobiana ("matriz") de tamanho "n"x"n" e os termos independentes
// ("termos") e aplica. O resultado é colocado na matriz "resultado"
void aplicaTermosJacobiana(void ***matriz, double *termos, int n, double **resultado) {
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      resultado[i][j] = aplicaFuncao(matriz[i][j], termos, n);
    }
  }
}

// Aplica os termos "termos", na matriz "matriz" de tamanho "n"x"n".]
// O resultado de cada linha é colocado no vetor "resultado"
void aplicaTermosMatriz(void **matriz, double *termos, int n, double *resultado) {
  for(int i = 0; i < n; i++){
      resultado[i] = aplicaFuncao(matriz[i], termos, n) * -1;
  }
}

DadosExecucao *calculaSNL(SNL *snl, FILE *saida){
  //verificar se SNL não é NULL
  if (snl == NULL || !(snl->n > 0)) {
    fprintf(stderr, "Sistema não linear inválido !\n");
  }

  // Inicia variaveis utilizadas para calculo
  double *x = criaVetor(snl->n);
  double **tempJacobiana = criaMatriz(snl->n, snl->n);
  double *termosIndependentes = criaVetor(snl->n);
  if(x == NULL || tempJacobiana == NULL || termosIndependentes == NULL){
    return NULL;
  }

  // Pega os nomes da variaveis para impressão da saída parcial
  int tamanho = snl->n;
  char **variaveis;
  variaveis = malloc(sizeof(char **) * tamanho);
  for (int i = 0; i < tamanho; i++){
    variaveis[i] = malloc(sizeof(char *) * tamanho);
    sprintf(variaveis[i], "x%d", i+1);
  }

  // Imprime saída inicial desse sistema
  imprimeInicio(snl->F, tamanho, saida);


  // Variaveis para pegar os tempos
  DadosExecucao *dadosExec = inciaDadosExecucao();
  double tempoTotal, tempoJacobiana, tempoSL;

  // Copia vetor da aproximação incial para o com o valor de X
  copiaVetor(snl->aprox_inicial, x, snl->n);

  int iteracao = 0;
  tempoTotal = timestamp();
  while (iteracao < snl->max_iter) {
    // Calculo do delta
    // Montar sistema linear
    tempoJacobiana = timestamp();
    aplicaTermosJacobiana(snl->Jacobiana, x, snl->n, tempJacobiana);
    adicionaTempoJacobiana((timestamp() - tempoJacobiana), dadosExec);

    aplicaTermosMatriz(snl->F, x, snl->n, termosIndependentes);

    // Verifica critério de parada termos independentes
    if(getMaiorAbs(termosIndependentes, snl->n) < snl->epsilon){
      break;
    }

    // Imprime resultado parcial encontrado na iteracao anterior    
    imprimeResultado(x, variaveis, tamanho, saida);

    // Calcular sistema linear
    tempoSL = timestamp();
    double  *delta = calculculaSistemaLinear(tempJacobiana, termosIndependentes, snl->n);
    adicionaTempoSistemaLinear((timestamp() - tempoSL), dadosExec);  

    // Calcula X_i+1
    for(int i = 0; i < snl->n; i++){
      x[i] = x[i] + delta[i];
    }

    // Verifica critério de parada pelo delta
    if(getMaiorAbs(delta, snl->n) < snl->epsilon){
      break;
    }
    
    iteracao++;
  }
  adicionaTempoTotal((timestamp() - tempoTotal), dadosExec);

  imprimeResultado(x, variaveis, tamanho, saida);

  return dadosExec;
}

// ============ Funções para manipulação dos dados de execução ============

DadosExecucao *inciaDadosExecucao (){
  DadosExecucao *dados = malloc(sizeof(DadosExecucao));
  dados->tempoJacobianas = 0.0;
  dados->tempoSistemaLinear = 0.0;
  dados->tempoTotal = 0.0;
  dados->tempoDerivadas = 0.0;
  return dados;
}

void adicionaTempoJacobiana(double tempo, DadosExecucao *dados){
  if(dados == NULL || dados->tempoJacobianas < 0){
    fprintf(stderr, "Dados de execução não incializados\n");
    return;
  }

  dados->tempoJacobianas += tempo;
}

void adicionaTempoDerivadas(double tempo, DadosExecucao *dados){
  if(dados == NULL || dados->tempoDerivadas < 0){
    fprintf(stderr, "Dados de execução não incializados\n");
    return;
  }
  
  dados->tempoDerivadas += tempo;
}

void adicionaTempoSistemaLinear(double tempo, DadosExecucao *dados){
  if(dados == NULL || dados->tempoSistemaLinear < 0){
    fprintf(stderr, "Dados de execução não incializados\n");
    return;
  }
  
  dados->tempoSistemaLinear += tempo;
}

void adicionaTempoTotal(double tempo, DadosExecucao *dados){
  if(dados == NULL || dados->tempoTotal < 0){
    fprintf(stderr, "Dados de execução não incializados\n");
    return;
  }
  
  dados->tempoTotal += tempo;
}
