/************
* CI1164-Trabalho 1
*
* Autores:
*   Iago Mello Floriano - GRR:20196049 - imf19@inf.ufpr.br
*   Gustavo H. da S. Barbosa - GRR:20190485 - ghsb19@inf.ufpr.br 
* 
* Dados: Define estrutura de dados utlizada, bem como métodos de leitura de entrada e escrita da saída
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

#define STRING_SIZE 100
#define INPUT_SIZE 256

typedef struct {
  void **F; // Sistema não linear
  void ***Jacobiana; // Matriz jacobiana
  double *aprox_inicial; // Aproximação inicial
  double epsilon_1; // epsilon X
  double epsilon_2; // epsilon Y
  int max_iter; // número máximo de iterações
  int n; // Tamanho do sistema linear
} SNL;

char *recebeNomeArquivoSaida(int argc, char *argv[]);

void novoSnl(SNL *snl, int tamanho);

double iniciaSnlEntrada(SNL *snl);

// void imprimeResultado(char *nomeArquivoSaida);

// void imprimeTempos(char *nomeArquivoSaida);

void imprimeSNLiniciaSnlEntrada(FILE *arqOut, SNL *snl);

// int fimDaEntrada();

#endif
