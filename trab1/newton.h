/************
* CI1164-Trabalho 1
*
* Autores:
*   Iago Mello Floriano - GRR:20196049 - imf19@inf.ufpr.br
*   Gustavo H. da S. Barbosa - GRR:20190485 - ghsb19@inf.ufpr.br 
* 
* Newton: Implemeta cálculo
************/

#ifndef __NEWTON__
#define __NEWTON__

#include"dados.h"

typedef struct {
  double tempoTotal;
  double tempoJacobianas;
  double tempoSistemaLinear;
} Resultado;

Resultado *calculaSNL(SNL *snl, FILE *saida);

#endif
