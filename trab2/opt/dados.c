/************
* CI1164-Trabalho 1
*
* Autores:
*   Iago Mello Floriano - GRR:20196049 - imf19@inf.ufpr.br
*   Gustavo H. da S. Barbosa - GRR:20190485 - ghsb19@inf.ufpr.br 
* 
************/

#include "dados.h"

char *recebeNomeArquivoSaida (int argc, char *argv[]) {
  char *filename = malloc(sizeof(char) * STRING_SIZE);
  // Lê possível arquivo de saída
  int option;
  while ((option = getopt(argc, argv, "o:")) != -1) {
    if (option == 'o') {
      free(filename);
      filename = optarg;
    } else {
      free(filename);
      fprintf(stderr, "Nenhum arquivo para escrita especificado, usando saída padrão (stdout)\n");
      return NULL;
    }
  }

  return filename;
}

void novoSnl(SNL *snl, int tamanho) {
  if (tamanho == 0) { //erro
    return;
  }

  // Inicia variaveis
  snl->n = tamanho;
  snl->epsilon = 0.0;
  snl->max_iter = 0;

  //Realiza alocações
  // Aloca matriz para funções do sistema não linear
  snl->F = malloc(sizeof(void *) * tamanho);
  if(!snl->F)
    fprintf(stderr, "erro ao alocar f\n");
  
  // Aloca matriz para jacobiana
  // cada elemento da matriz corresponde a uma função (derivada parcial) 
  snl->Jacobiana = malloc(sizeof(void **) * tamanho);
  if(!snl->Jacobiana)
    fprintf(stderr, "erro ao alocar j\n");
  for (int i = 0; i < tamanho; i++) {
    snl->Jacobiana[i] = malloc(sizeof(void *) * tamanho);
    if(!snl->Jacobiana[i])
      fprintf(stderr, "erro ao alocar J[%d]\n", i);
  }

  // Aloca vetor para aproximação inicial
  snl->aprox_inicial = malloc(sizeof(double) * tamanho);
  if(!snl->aprox_inicial)
      fprintf(stderr, "erro ao alocar vetor para aproximação inicial\n");
}

FILE *abreEntrada(int argc, char *argv[]){
  char *nomeArquivoSaida = recebeNomeArquivoSaida(argc, argv);
  FILE *arqOut = fopen(nomeArquivoSaida, "w");
  if (!arqOut)
    arqOut = stdout;
  return (arqOut);
}

// A partir da funcão de entrada do sistema não linear
// calcula a derivada parcial para cáculo da matriz jacobiana
void calculaJacobiana(SNL *snl) {
  int tamanho = snl->n;
  int m = 4;

  // Faz o loop Unroll & Jam
  for (int i = 0; i < tamanho - (tamanho%m); i+=m) {
    //Salvar função para derivar
    void *funcAtual_1 = snl->F[i]; 
    void *funcAtual_2 = snl->F[i+1];
    void *funcAtual_3 = snl->F[i+2];
    void *funcAtual_4 = snl->F[i+3];
    
    for (int j = 0; j < tamanho; j++) {
      snl->Jacobiana[i][j] = evaluator_derivative(funcAtual_1, snl->nomes_variaveis[j]);
      snl->Jacobiana[i+1][j] = evaluator_derivative(funcAtual_2, snl->nomes_variaveis[j]);
      snl->Jacobiana[i+2][j] = evaluator_derivative(funcAtual_3, snl->nomes_variaveis[j]);
      snl->Jacobiana[i+3][j] = evaluator_derivative(funcAtual_4, snl->nomes_variaveis[j]);
    }
  }

  for (int i = tamanho - (tamanho%m); i < tamanho; i++) {
    void *funcAtual = snl->F[i]; //Salvar função para derivar
    for (int j = 0; j < tamanho; j++) {
      snl->Jacobiana[i][j] = evaluator_derivative(funcAtual, snl->nomes_variaveis[j]);
    }
  }
}

void **iniciaMatriz(int linhas, int colunas, int size){
  void **mat = malloc (linhas * size);
  if(mat == NULL){
    fprintf(stderr, "Ocorreu um erro ao criar a matriz\n");
    return NULL;
  }

  for (int i = 0; i < linhas; i++)
    mat[i] = malloc (colunas * size);

  return mat;
}

double iniciaSnlEntrada(SNL *snl) {
  int tamanho = snl->n;

  // Matriz do sistema não linear
  for (int i = 0; i < tamanho; i++) {
    char funcaoStr[INPUT_SIZE];
    if(scanf("%[^\t\n]s", funcaoStr) != 1){
      fprintf(stderr, "Ocorreu um erro ao realizar a leitura da entrada\n");
    }
    getchar();
    snl->F[i] = evaluator_create(funcaoStr);
  }

  // Aproximação inicial
  for (int i = 0; i < tamanho; i++) {
    if(scanf("%le", &snl->aprox_inicial[i]) != 1){
      fprintf(stderr, "Ocorreu um erro ao realizar a leitura da entrada\n");
    }
  }

  // Epsilon
  if(scanf("%le", &snl->epsilon) != 1){
    fprintf(stderr, "Ocorreu um erro ao realizar a leitura da entrada\n");
  }

  // Maximo de iterações
  if(scanf("%d", &snl->max_iter) != 1){
    fprintf(stderr, "Ocorreu um erro ao realizar a leitura da entrada\n");
  }
  getchar();

  // Inicia nomes das variaveis
  snl->nomes_variaveis = (char **) iniciaMatriz(tamanho, VARIABLE_NAME_SIZE, sizeof(char *));
  for (int i = 0; i < tamanho; i++){
    sprintf(snl->nomes_variaveis[i], "x%d", i+1);
  }

  double tempoDerivadas;
  tempoDerivadas = timestamp();
  LIKWID_MARKER_START("GeracaoJacobiana");
  calculaJacobiana(snl);
  LIKWID_MARKER_STOP("GeracaoJacobiana");
  tempoDerivadas = timestamp() - tempoDerivadas;

  return tempoDerivadas;
}

void encerraSNL(SNL *snl) {
  int tamanho = snl->n;

  // Destroi funções e libera matriz do SNL 
  for (int i = 0; i < tamanho; i++) {
    evaluator_destroy (snl->F[i]);
  }
  free(snl->F);

  // Libera aproximação inicial
  free(snl->aprox_inicial);

  // Destroi e libera matriz jacobiana
  for(int i = 0; i < tamanho; i++){
    for(int j = 0; j < tamanho; j++){
      evaluator_destroy (snl->Jacobiana[i][j]);
    }
    free(snl->Jacobiana[i]);
  }
  free(snl->Jacobiana);

  // Libera variaveis
  snl->epsilon = 0;
  snl->max_iter = 0;
  snl->n = 0;
}
