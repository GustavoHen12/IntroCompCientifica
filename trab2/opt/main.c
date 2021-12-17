/************
* CI1164-Trabalho 2
*
* Autores:
*   Iago Mello Floriano - GRR:20196049 - imf19@inf.ufpr.br
*   Gustavo H. da S. Barbosa - GRR:20190485 - ghsb19@inf.ufpr.br 
* 
************/

#include "newton.h"

int main(int argc, char *argv[]){
  LIKWID_MARKER_INIT;
  FILE *arqOut = abreEntrada(argc, argv);
  SNL snl;
  int tamanho = 0;
  while (!feof(stdin) && scanf("%d\n", &tamanho) == 1){
    // Cria sistema linear e inicia com valores lidos da entrada padrão
    novoSnl(&snl, tamanho);
    // Como as derivadas parciais não fazem parte do cáculo do sistema,
    // mas sim da leitura da entrada este é armazenado para ser impresso posteriormente.
    double tempoDerivadas = iniciaSnlEntrada(&snl);
    
    // Calcula o valor do sistema não linear
    DadosExecucao *dadosExecucao = calculaSNL(&snl, arqOut);

    // Adiciona o tempo das derivadas aos dados da execução e imprime
    adicionaTempoDerivadas(tempoDerivadas, dadosExecucao);
    imprimeDadosExecucao(dadosExecucao, arqOut);

    encerraSNL(&snl);
    encerraDadosExecucao(dadosExecucao);
  }
 fclose(arqOut);
 LIKWID_MARKER_CLOSE;
}
