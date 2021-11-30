#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <likwid.h>

#include "utils.h"
#include "libGaussJacobi.h"

int main(int argc, char ** argv) {
  LIKWID_MARKER_INIT;

  if (argc < 2) {
    printf("uso: %s file_in\n\n", argv[0]);
    return 1;
  }

  FILE *f_in = fopen(argv[1], "r");
  int n;
  fscanf(f_in, "%d", &n);
  rewind(f_in);

  FILE *f_out;

  f_out = fopen(markerName("out_gaussJacobi", n), "w");
  double tempoNaoOtimizado = timestamp();
  LIKWID_MARKER_START("naoOpt");
  gaussJacobi(f_in, f_out);
  LIKWID_MARKER_STOP("naoOpt");
  tempoNaoOtimizado = timestamp() - tempoNaoOtimizado;
  fclose(f_out);
  rewind(f_in);

  f_out = fopen(markerName("out_gaussJacobiOpt", n), "w");
  double tempoOtimizado = timestamp();
  LIKWID_MARKER_START("opt");
  gaussJacobiOpt(f_in, f_out);
  tempoOtimizado = timestamp() - tempoOtimizado;
  LIKWID_MARKER_STOP("opt");
  fclose(f_out);
  fclose(f_in);

  printf("%0.6f, %0.6f\n", tempoNaoOtimizado, tempoOtimizado);
  LIKWID_MARKER_CLOSE;
  return 0;
}

