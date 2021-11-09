/*
* CI1164-Trabalho 1
*
* Autores:
*   Iago Mello Floriano - GRR:20196049 - imf19@inf.ufpr.br
*   Gustavo H. da S. Barbosa - GRR:20190485 - ghsb19@inf.ufpr.br 
* 
*/

#include "dados.h"
#include "newton.h"

FILE *abreEntrada(int argc, char *argv[]);

int main(int argc, char *argv[]){
  FILE *arqOut = abreEntrada(argc, argv);
  SNL snl;
  int tamanho = 0;
  while (!feof(stdin) && scanf("%d\n", &tamanho) == 1){
    novoSnl(&snl, tamanho);
    double tempoDerivadas = iniciaSnlEntrada(&snl);
    imprimeSNL(arqOut, &snl);
    Resultado *param = calculaSNL(&snl, arqOut);
    //    imprimeResultado(nomeArquivoSaida);
    //    imprimeTempos(nomeArquivoSaida);
    //encerraSNL(&snl);
  }
}

FILE *abreEntrada(int argc, char *argv[]){
  char *nomeArquivoSaida = recebeNomeArquivoSaida(argc, argv);
  FILE *arqOut = fopen(nomeArquivoSaida, "w");
  if (!arqOut)
    arqOut = stdout;
  return (arqOut);
}
