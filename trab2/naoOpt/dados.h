/************
* CI1164-Trabalho 1
*
* Autores:
*   Iago Mello Floriano - GRR:20196049 - imf19@inf.ufpr.br
*   Gustavo H. da S. Barbosa - GRR:20190485 - ghsb19@inf.ufpr.br 
* 
************/

#ifndef __DADOS__
#define __DADOS__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <matheval.h>
#include "utils.h"
#include <likwid.h>

#define STRING_SIZE 100
#define INPUT_SIZE 256

typedef struct {
  void **F; // Sistema não linear
  void ***Jacobiana; // Matriz jacobiana
  double *aprox_inicial; // Aproximação inicial
  double epsilon; // epsilon
  int max_iter; // número máximo de iterações
  int n; // Tamanho do sistema linear
} SNL;

// Le opções de entrada e retorna o nome do arquivo de saída
// Se não tiver nenhuma opção retorna NULL
char *recebeNomeArquivoSaida(int argc, char *argv[]);

// Abre arquivo de entrada e retorna ponteiro para arquivo
// Se nenhum arquivo for selecionado retorna stdout
FILE *abreEntrada(int argc, char *argv[]);

// Cria um novo snl, que possui tamanho "tamanho"
// Todas as alocações são realizados dentro da função
void novoSnl(SNL *snl, int tamanho);

// A partir de um sistema não linear já inicializado "snl"
// Le valores da entrada padrão e inicia SNL
// O valor de retorno corresponde ao tempo para o 
// calculo das derivadas da matriz Jacobiana
double iniciaSnlEntrada(SNL *snl);

// Finaliza funções e desaloca dados do SNL
void encerraSNL(SNL *snl);

#endif
