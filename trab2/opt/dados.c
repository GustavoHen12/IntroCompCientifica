/************
* CI1164-Trabalho 1
*
* Autores:
*   Iago Mello Floriano - GRR:20196049 - imf19@inf.ufpr.br
*   Gustavo H. da S. Barbosa - GRR:20190485 - ghsb19@inf.ufpr.br 
* 
************/

#include "dados.h"

// char **iniciaMatriz(int linhas, int colunas, int size){
//   char **mat = malloc (linhas * size);
//   if(mat == NULL){
//     fprintf(stderr, "Ocorreu um erro ao criar a matriz\n");
//     return NULL;
//   }
// 
//   mat[0] = malloc(linhas * colunas * size);
//   for (int i = 1; i < linhas; i++)
//     mat[i] = &(mat[i-1][i * colunas]);
// 
//   return mat;
// }

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
  // essa sera uma matriz tridiagonal
  snl->Jacobiana = (void ***)malloc(3 * sizeof(void **));
  snl->Jacobiana[0] = (void **)malloc(3 * tamanho * sizeof(void *));
  snl->Jacobiana[1] = snl->Jacobiana[0] + tamanho;
  snl->Jacobiana[2] = snl->Jacobiana[0] + tamanho*2;

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
  snl->Jacobiana[0][0] = evaluator_derivative(snl->F[0], "b"); // usado para criar uma função nula
  snl->Jacobiana[1][0] = evaluator_derivative(snl->F[0], snl->nomes_variaveis[0]);
  snl->Jacobiana[2][0] = evaluator_derivative(snl->F[0], snl->nomes_variaveis[1]);
  for (int i = 1; i < tamanho-1; i++){
    void *funcAtual = snl->F[i]; 
    snl->Jacobiana[0][i]= evaluator_derivative(funcAtual, snl->nomes_variaveis[i-1]);
    snl->Jacobiana[1][i]= evaluator_derivative(funcAtual, snl->nomes_variaveis[i  ]);
    snl->Jacobiana[2][i]= evaluator_derivative(funcAtual, snl->nomes_variaveis[i+1]);
  }
  snl->Jacobiana[0][tamanho - 1] = evaluator_derivative(snl->F[tamanho - 1], snl->nomes_variaveis[tamanho - 2]);
  snl->Jacobiana[1][tamanho - 1] = evaluator_derivative(snl->F[tamanho - 1], snl->nomes_variaveis[tamanho - 1]);
  snl->Jacobiana[2][tamanho - 1] = snl->Jacobiana[0][0]; // usado para usar uma função nula já gerada
  
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
  free(snl->Jacobiana[0]);
  snl->Jacobiana[1] = NULL;
  snl->Jacobiana[2] = NULL;
  free(snl->Jacobiana);

  // Libera nome das variaveis
  for (int i=0; i < snl->n; i++)
    free(snl->nomes_variaveis[i]);

  // Libera variaveis
  snl->epsilon = 0;
  snl->max_iter = 0;
  snl->n = 0;
}
