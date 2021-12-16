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

// Desaloca o espaço ocupado pelo vetor vet
void destroiVetor(double *vet){
  free(vet);
}

// Copia um vetor A em um vetor B
void copiaVetor(double *vetA, double *vetB, int tam){
  if(vetA != NULL && vetB != NULL){
    for(int i = 0; i< tam; i++)
      vetB[i] = vetA[i];
  }
}

// ============ Funcões para manipulação da libmatheval ============

// Aplica x_0, x_1, .., x_n em uma função f
// sendo f = a_0*x_0 + a_1*x_1 + ... + a_n*x_n
// Os valores de x, estão em ordem em x, e a função
// corresponde a func. Retorna o valor resultante
double aplicaFuncao(void *func, double *x, int tamanho, char **variaveis){
  if(func == NULL){
    fprintf(stderr, "Ocorreu um erro ao calcular o valor\n");
    return 0;
  }

  double resultado = evaluator_evaluate(func, tamanho, variaveis, x);

  return resultado;
}

// ============ Calculo SNL ============
DadosExecucao *calculaSNL(SNL *snl, FILE *saida){
  //verificar se SNL não é NULL
  if (snl == NULL || !(snl->n > 0)) {
    fprintf(stderr, "Sistema não linear inválido !\n");
  }

  // Inicia variaveis utilizadas para calculo
  double *x = criaVetor(snl->n);
  double **tempJacobiana = (double **) iniciaMatriz(3, snl->n, sizeof(double));
  double *termosIndependentes = criaVetor(snl->n);
  if(x == NULL || tempJacobiana == NULL || termosIndependentes == NULL){
    return NULL;
  }

  // Pega os nomes da variaveis para impressão da saída parcial
  int tamanho = snl->n;

  // Imprime saída inicial desse sistema
  imprimeInicio(snl->F, tamanho, saida);


  // Variaveis para pegar os tempos
  DadosExecucao *dadosExec = iniciaDadosExecucao();
  double tempoTotal, tempoJacobiana, tempoSL;

  // Copia vetor da aproximação inicial para o com o valor de X
  copiaVetor(snl->aprox_inicial, x, snl->n);

  int iteracao = 0;
  LIKWID_MARKER_START("MetodoNewton");
  tempoTotal = timestamp();
  while (iteracao < snl->max_iter) {
    // Calculo do delta
    // Montar sistema linear
    for(int i = 0; i < tamanho; i++){
      // Aplica termos jacobiana
      LIKWID_MARKER_START("CalculaJacobiana");
      tempoJacobiana = timestamp();

      tempJacobiana[0][i] = aplicaFuncao(snl->Jacobiana[0][i], x, tamanho, snl->nomes_variaveis);
      tempJacobiana[1][i] = aplicaFuncao(snl->Jacobiana[1][i], x, tamanho, snl->nomes_variaveis);
      tempJacobiana[2][i] = aplicaFuncao(snl->Jacobiana[2][i], x, tamanho, snl->nomes_variaveis);

      adicionaTempoJacobiana((timestamp() - tempoJacobiana), dadosExec);
      LIKWID_MARKER_STOP("CalculaJacobiana");

      //Aplica termos matriz
      termosIndependentes[i] = aplicaFuncao(snl->F[i], x, tamanho, snl->nomes_variaveis) * -1;
    }

    // Verifica critério de parada termos independentes
    if(getMaiorAbs(termosIndependentes, snl->n) < snl->epsilon){
      break;
    }

    LIKWID_MARKER_STOP("MetodoNewton");
    // Imprime resultado parcial encontrado na iteracao anterior    
    adicionaTempoTotal((timestamp() - tempoTotal), dadosExec);
    imprimeResultado(x, snl->nomes_variaveis, tamanho, saida);
    tempoTotal = timestamp();
    LIKWID_MARKER_START("MetodoNewton");

    // Calcular sistema linear
    LIKWID_MARKER_START("SistemaLinear");
    tempoSL = timestamp();
    double  *delta = calculaSistemaLinear(tempJacobiana, termosIndependentes, snl->n);
    adicionaTempoSistemaLinear((timestamp() - tempoSL), dadosExec);  
    LIKWID_MARKER_STOP("SistemaLinear");

    // Calcula X_i+1
    for(int i = 0; i < snl->n; i++){
      x[i] = x[i] + delta[i];
    }

    // Verifica critério de parada pelo delta
    if(getMaiorAbs(delta, snl->n) < snl->epsilon){
      free(delta);
      break;
    }
    free(delta);
    
    iteracao++;
  }
  LIKWID_MARKER_STOP("MetodoNewton");
  adicionaTempoTotal((timestamp() - tempoTotal), dadosExec);

  imprimeResultado(x, snl->nomes_variaveis, tamanho, saida);

  // Desalocação de espaço na memória
  destroiVetor(x);
  destroiVetor(termosIndependentes);
  encerraMatriz((void **) tempJacobiana, snl->n);

  return dadosExec;
}

// ============ Funções para manipulação dos dados de execução ============

DadosExecucao *iniciaDadosExecucao (){
  DadosExecucao *dados = malloc(sizeof(DadosExecucao));
  dados->tempoJacobianas = 0.0;
  dados->tempoSistemaLinear = 0.0;
  dados->tempoTotal = 0.0;
  dados->tempoDerivadas = 0.0;
  return dados;
}

void adicionaTempoJacobiana(double tempo, DadosExecucao *dados){
  if(dados == NULL || dados->tempoJacobianas < 0){
    fprintf(stderr, "Dados de execução não inicializados\n");
    return;
  }

  dados->tempoJacobianas += tempo;
}

void adicionaTempoDerivadas(double tempo, DadosExecucao *dados){
  if(dados == NULL || dados->tempoDerivadas < 0){
    fprintf(stderr, "Dados de execução não inicializados\n");
    return;
  }
  
  dados->tempoDerivadas += tempo;
}


void adicionaTempoSistemaLinear(double tempo, DadosExecucao *dados){
  if(dados == NULL || dados->tempoSistemaLinear < 0){
    fprintf(stderr, "Dados de execução não inicializados\n");
    return;
  }
  
  dados->tempoSistemaLinear += tempo;
}

void adicionaTempoTotal(double tempo, DadosExecucao *dados){
  if(dados == NULL || dados->tempoTotal < 0){
    fprintf(stderr, "Dados de execução não inicializados\n");
    return;
  }
  
  dados->tempoTotal += tempo;
}

// função para dar free no dados execução
void encerraDadosExecucao(DadosExecucao *dados){
  free(dados);
}
