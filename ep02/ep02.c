#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <math.h>

#define INPUT_SIZE 256

typedef struct {
  double **M; //Matriz onde terá as diagonais
  int n; //Tamanho da maior diagonal
  int k; //Quantidade de diagonais
} Diagonais;

// Inicia D com n diagonais sendo a maior de tamanho tam
// Todas as diagonais serão preenchidas com 0
void inicializa_diagonais(Diagonais *D, int n, int k){
  D->M = (double **)malloc(k * sizeof(double *));
  int tamAtual = n - (k / 2);
  int incrTam = 1;
  for(int i=0; i < k; i++){
    D->M[i] = (double *)calloc(tamAtual, sizeof(double));
    if (tamAtual == n) incrTam = -1;
    tamAtual += incrTam;
  }
  D->k = k;
  D->n = n;
}

// Imprime diagonais com 6 casa decimais
void imprime_diagonais(Diagonais *D){
  int nMeio = D->k / 2;
  for (int i = 0; i < D->k; i++){
    int tamAtual = D->n - abs(i - nMeio);
    for (int j = 0; j < tamAtual; j++){
      printf("[%0.6f] ", D->M[i][j]);
    }
    printf("\n");
  }
}

// Função para iniciar uma função
// Recebe como parametro uma string e retorna um ponteiro para função
void* inicia_funcao(char* funcao){
    void* f = evaluator_create(funcao);
    return f;
}

// Lê função de stdin e aplica nas diagonais
// valor de x é igual ao indice do elemento na diagonal
void funcoesStdin_diagonais(Diagonais *D){
  int nMeio = D->k / 2;
  void *f;
  for (int i = 0; i < D->k; i++){
    char func[INPUT_SIZE];
    scanf("%[^\t\n]s", func);
    getchar();
    f = inicia_funcao(func);

    int tamAtual = D->n - abs(i - nMeio);
    for (int j = 0; j < tamAtual; j++){
      D->M[i][j] = evaluator_evaluate_x(f, j);
    }
  }
}

void leParametrosDiagonais(Diagonais *D, int *n, int *k){
  scanf("%d %d", n, k);
  getchar();
  inicializa_diagonais(D, *n, *k);
  funcoesStdin_diagonais(D);
}

int main(){
  Diagonais d;
  int n, k;
  leParametrosDiagonais(&d, &n, &k);
  
  imprime_diagonais(&d);

  return 0;
}
