#!/bin/bash
TAMANHOS_TESTE=( 10 32 50 64 100 128 200 250 256 300 400 512 600 1000 1024 2000 2048 3000 4096)
DIR_INICIAL="$PWD"

SRC="$PWD/sistema"
cd $SRC
echo "Rodando Make"
make geraSL likwid
cd $DIR_INICIAL

ENTRADAS="$SRC/entradas-sistema"
[[ -d $ENTRADAS ]] ||  mkdir -p $ENTRADAS

SAIDAS="$SRC/saidas-sistema"
[[ -d $SAIDAS ]] || mkdir -p $SAIDAS
cd $SAIDAS

echo "Gerando Sistemas e Resultados"
for i in "${TAMANHOS_TESTE[@]}"; do
  $SRC/geraSL $i > $ENTRADAS/$i.sistema
  $SRC/gaussJacobi-likwid $ENTRADAS/$i.sistema
done
cd $DIR_INICIAL
