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

echo "Gerando Sistemas, Resultados e Fazendo o benchmark"
SAIDALIKWID="$SRC/saida-likwid"
[[ -d $SAIDALIKWID ]] || mkdir -p $SAIDALIKWID
for i in "${TAMANHOS_TESTE[@]}"; do
  $SRC/geraSL $i > $ENTRADAS/$i.sistema
  likwid-perfctr -C 3 -g L3       -o $SAIDALIKWID/${i}L3.likwid       -m $SRC/gaussJacobi-likwid $ENTRADAS100.sistema
  likwid-perfctr -C 3 -g L2CACHE  -o $SAIDALIKWID/${i}L2CACHE.likwid  -m $SRC/gaussJacobi-likwid $ENTRADAS100.sistema
  likwid-perfctr -C 3 -g FLOPS_DP -o $SAIDALIKWID/${i}FLOPS_DP.likwid -m $SRC/gaussJacobi-likwid $ENTRADAS100.sistema
done
cd $DIR_INICIAL
