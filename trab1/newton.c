#include "newton.h"

int verificaParada(SNL *snl) {
  return 0;
}

void aplicaTermosJacobiana(void ***matriz, double *termos, int n, double **resultado)
{
}

void aplicaTermosMatriz(void **matriz, double *termos, int n, double *resultado)
{
}

Resultado *calculaSNL(SNL *snl, FILE *saida){
  //verificar se SNL não é NULL
  if (snl == NULL || !snl->n > 0)
  {
    fprintf(stderr, "Sistema não linear inválido !\n");
  }

  double *x = copiaVetor(snl->aprox_inicial);
  double **tempJacobiana = criaMatriz(snl->n, snl->n);
  double *termosIndependentes = criaVetor(snl->n);

  int iteracao = 0;
  while (iteracao < snl->max_iter && !verificaParada(snl)) {
    aplicaTermosJacobiana(snl->Jacobiana, x, snl->n, tempJacobiana);
    aplicaTermosMatriz(snl->F, x, snl->n, termosIndependentes);

    // Calculo do delta
    // Montar sistema linear
    // Calcular sistema linear

    // Calculo Xi+1

    // calcula delta iteracao
  }
}