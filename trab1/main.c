/*
* CI1164-Trabalho 1
*
* Autores:
*   Iago Mello Floriano - GRR:20196049 - imf19@inf.ufpr.br
*   Gustavo H. da S. Barbosa - GRR:20190485 - ghsb19@inf.ufpr.br 
* 
*/

#include"dados.h"
#include"newton.h"

int main(int argc, char *argv[]){
   char *nomeArquivoSaida = recebeNomeArquivoSaida(argc, argv);
   SNL snl;
   int tamanho = 0;
   while(!feof(stdin) && scanf("%d\n", &tamanho) == 1){
       novoSnl(&snl, tamanho);
       double tempoMatrixJacobiana = iniciaSnlEntrada(&snl);
       imprimeSNLiniciaSnlEntrada(&snl);
    //   Resultado param = calculaSNL(snl);
    //    imprimeResultado(nomeArquivoSaida);
    //    imprimeTempos(nomeArquivoSaida);
        //encerraSNL(&snl);
   }
}