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
      filename = optarg;
    } else {
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

  snl->n = tamanho;
  //Realiza alocações
  snl->F = malloc(sizeof(void *) * tamanho);
  if(!snl->F) printf("erro ao alocar f\n");
  snl->Jacobiana = malloc(sizeof(void **) * tamanho);
  if(!snl->Jacobiana) printf("erro ao alocar j\n");
  for (int i = 0; i < tamanho; i++) {
    snl->Jacobiana[i] = malloc(sizeof(void *) * tamanho);
    if(!snl->Jacobiana[i]) printf("erro ao alocar J[%d]\n", i);
  }
  snl->aprox_inicial = malloc(sizeof(double) * tamanho);
  snl->epsilon = 0.0;
  snl->max_iter = 0;
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
    printf("\t\t %s:\n", evaluator_get_string(funcAtual));
    for (int j = 0; j < tamanho; j++) {
      snl->Jacobiana[i][j] = evaluator_derivative(funcAtual, names[j]);
      printf("\t\t\t %s : %d\n", evaluator_get_string(snl->Jacobiana[i][j]), j+1);
    }
    printf("-----------------------\n");
  }
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

// void imprimeSNL(FILE *arqOut, SNL *snl) {
//   fprintf(arqOut, "SNL[%d]: \n", snl->n);
//   fflush(arqOut);

//   int tamanho = snl->n;
//   // Matriz do sistema não linear
//   /* Print variable names appearing in function. */
//   fprintf(arqOut, "sistema:\n");
//   for (int i = 0; i < tamanho; i++) {
//     fprintf(arqOut, "%s \n", evaluator_get_string(snl->F[i]));
//   }

//   //Imprimir da jacobiana REMOVER DEPOIS
//   fprintf(arqOut, "Jacobiana:\n");
//   for(int i = 0; i < tamanho; i++){
//     fprintf(arqOut, "\tLinha %d:\n", i);
//     for(int j = 0; j < tamanho; j++){
//       fprintf(arqOut, "\t\t%s \n", evaluator_get_string(snl->Jacobiana[i][j]));
//     }
//   }

//   // Aproximação inicial
//   fprintf(arqOut, "X0:");
//   for (int i = 0; i < tamanho; i++) {
//     fprintf(arqOut, " %le ", snl->aprox_inicial[i]);
//   }
//   fprintf(arqOut, "\n");

//   // Epsilon
//   fprintf(arqOut, "epsilon: %0.6f\n", snl->epsilon_1);

//   // Maximo de iterações
//   fprintf(arqOut, "maxIter: %d\n", snl->max_iter);
// }
