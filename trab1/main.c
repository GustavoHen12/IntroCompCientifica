/************
* CI1164-Trabalho 1
*
* Autores:
*   Iago Mello Floriano - GRR:20196049 - imf19@inf.ufpr.br
*   Gustavo H. da S. Barbosa - GRR:20190485 - ghsb19@inf.ufpr.br 
* 
************/

#include "newton.h"

int main(int argc, char *argv[]){
  FILE *arqOut = abreEntrada(argc, argv);
  SNL snl;
  int tamanho = 0;
  while (!feof(stdin) && scanf("%d\n", &tamanho) == 1){
    // Cria sistema linear e inicia com valores lidos da entrada padrão
    novoSnl(&snl, tamanho);
    double tempoDerivadas = iniciaSnlEntrada(&snl);
    
    DadosExecucao *dadosExecucao = calculaSNL(&snl, arqOut);

    adicionaTempoDerivadas(tempoDerivadas, dadosExecucao);

    imprimeDadosExecucao(dadosExecucao, arqOut);

    encerraSNL(&snl);
  }
}
