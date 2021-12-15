/************
* CI1164-Trabalho 1
*
* Autores:
*   Iago Mello Floriano - GRR:20196049 - imf19@inf.ufpr.br
*   Gustavo H. da S. Barbosa - GRR:20190485 - ghsb19@inf.ufpr.br 
* 
************/

#ifndef __LINEAR__
#define __LINEAR__

#include <stdlib.h>
#include <math.h>

double  *calculaSistemaLinear(double **sistema, double *termos_independentes, int tam);

// Faz as multiplicaçoes e somas necessárias para  zerar os termos abaixo do elemento [n][n]
// no sistema, atualiza também o valor nos termos independentes para nao alterar o resultado
// essa função considera q o termo n é o primeiro termo nao nulo da linha
void zeraDiagonal(double **sistema, double *termos_independentes, int tam);

#endif //__LINEAR__
