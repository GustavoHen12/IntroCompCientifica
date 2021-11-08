#include"dados.h"

char *recebeNomeArquivoSaida(int argc, char *argv[]){
    char *filename = malloc(sizeof(char) * STRING_SIZE);
    // Lê possível arquivo de saída
   int option;
   while ((option = getopt (argc, argv, "o:")) != -1){
       if(option == 'o'){
           filename = optarg;
       }else{
           return 0;
       }
   }

   return filename;
}

// Função para iniciar uma função
// Recebe como parametro uma string e retorna um ponteiro para função
void* inicia_funcao(char* funcao){
    void* f = evaluator_create(funcao);
    return f;
}

void novoSnl(SNL *snl, int tamanho){
  if(tamanho == 0){ //erro
    return;
  }

  snl->n = tamanho;
  //Realiza alocações
  snl->F = malloc(sizeof(void) * tamanho);
  snl->Jacobiana = malloc(sizeof(void) * tamanho);
  for (int i = 0 ; i < tamanho; i++){
    snl->Jacobiana[i] = malloc(sizeof(void) * tamanho);
  }
  snl->aprox_inicial = malloc(sizeof(double) * tamanho);
  snl->epsilon_1 = 0.0;
  snl->epsilon_2 = 0.0;
  snl->max_iter = 0;
}

void calculaJacobiana(SNL *snl){
  int tamanho = snl->n;
  for(int i = 0; i < tamanho; i++){
    void *funcAtual = snl->F[i]; //Salvar função para derivar
    char **names;
    int count;
    evaluator_get_variables(funcAtual, &names, &count); //Informações sobre a função
    for(int j=0; j < count; j++){
      snl->Jacobiana[i][j] = evaluator_derivative(funcAtual, names[j]);
    }
  }

}

double iniciaSnlEntrada(SNL *snl){
  int tamanho = snl->n;

  // Matriz do sistema não linear
  for(int i = 0; i < tamanho; i++){
    char funcaoStr[INPUT_SIZE];
    scanf("%[^\t\n]s", funcaoStr);
    getchar();
    snl->F[i] = evaluator_create(funcaoStr);
  }

  // Aproximação inicial
  for(int i = 0; i < tamanho; i++){
    scanf("%le", &snl->aprox_inicial[i]);
  }

  // Epsilon
  scanf("%le", &snl->epsilon_1);

  // Maximo de iterações
  scanf("%d", &snl->max_iter);
  getchar();

  calculaJacobiana(snl);
  return 0;
}

void imprimeSNLiniciaSnlEntrada(SNL *snl){
  printf("SNL[%d]: \n", snl->n);
  fflush(stdout); 

  int tamanho = snl->n;
  // Matriz do sistema não linear
  /* Print variable names appearing in function. */
  printf("sistema:\n");
  for(int i = 0; i < tamanho; i++){
    printf("%s \n", evaluator_get_string(snl->F[i]));
  }

  // //Imprimir da jacobiana REMOVER DEPOIS
  // printf("Jacobiana:\n");
  // for(int i = 0; i < tamanho; i++){
  //   printf("\tLinha %d:\n", i);
  //   for(int j = 0; j < tamanho; j++){
  //     printf("\t\t%s \n", evaluator_get_string(snl->Jacobiana[i][j]));
  //   }
  // }

  // Aproximação inicial
  printf("X0:");
  for(int i = 0; i < tamanho; i++){
    printf(" %le ", snl->aprox_inicial[i]);
  }
  printf("\n");

  // Epsilon
  printf("epsilon: %0.6f\n", snl->epsilon_1);

  // Maximo de iterações
  printf("maxIter: %d\n", snl->max_iter);
}
