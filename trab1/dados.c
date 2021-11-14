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
  char **names;
  names = malloc(sizeof(char **) * tamanho);

  for (int i = 0; i < tamanho; i++){
    names[i] = malloc(sizeof(char *) * tamanho);
    sprintf(names[i], "x%d", i+1);
  }

  for (int i = 0; i < tamanho; i++) {
    void *funcAtual = snl->F[i]; //Salvar função para derivar
    for (int j = 0; j < tamanho; j++) {
      snl->Jacobiana[i][j] = evaluator_derivative(funcAtual, names[j]);
    }
  }

  // Desalocação da memória
  for (int i = 0; i < tamanho; i++)
    free(names[i]);
  free(names);
}

double iniciaSnlEntrada(SNL *snl) {
  int tamanho = snl->n;

  // Matriz do sistema não linear
  for (int i = 0; i < tamanho; i++) {
    char funcaoStr[INPUT_SIZE];
    scanf("%[^\t\n]s", funcaoStr);
    getchar();
    snl->F[i] = evaluator_create(funcaoStr);
  }

  // Aproximação inicial
  for (int i = 0; i < tamanho; i++) {
    scanf("%le", &snl->aprox_inicial[i]);
  }

  // Epsilon
  scanf("%le", &snl->epsilon);

  // Maximo de iterações
  scanf("%d", &snl->max_iter);
  getchar();

  double tempoDerivadas;
  tempoDerivadas = timestamp();
  calculaJacobiana(snl);
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
