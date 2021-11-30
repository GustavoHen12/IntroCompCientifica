#!/bin/bash
TAMANHOS_TESTE=( 10 32 50 64 100 128 200 250 256 300 400 512 600 1000 1024 2000 2048 3000 4096)
#TAMANHOS_TESTE=( 10 32 50)
DIR_INICIAL="$PWD"

SRC="$PWD/sistema"
cd $SRC
echo "Rodando Make"
make geraSL avx
cd $DIR_INICIAL

ENTRADAS="$SRC/entradas-sistema"
[[ -d $ENTRADAS ]] ||  mkdir -p $ENTRADAS

SAIDAS="$SRC/saidas-sistema"
[[ -d $SAIDAS ]] || mkdir -p $SAIDAS
cd $SAIDAS

ENTRADAS="$SRC/entradas-sistema"
[[ -d $ENTRADAS ]] ||  mkdir -p $ENTRADAS

SAIDAS="$SRC/saidas-sistema"
[[ -d $SAIDAS ]] || mkdir -p $SAIDAS
cd $SAIDAS

echo "Inicia arquivos"
SAIDAS_CSV="$SRC/saida-csv"
[[ -d $SAIDAS_CSV ]] || mkdir -p $SAIDAS_CSV
touch $SAIDAS_CSV/L3.csv
echo 'Tamanho, naoOpt, opt' > $SAIDAS_CSV/L3.csv
touch $SAIDAS_CSV/L2CACHE.csv
echo 'Tamanho, naoOpt, opt' > $SAIDAS_CSV/L2CACHE.csv
touch $SAIDAS_CSV/FLOPS_DP.csv
echo 'Tamanho, dp_naoOpt, avx_naoOpt, dp_opt, avx_opt' > $SAIDAS_CSV/FLOPS_DP.csv
touch $SAIDAS_CSV/TIME.csv
echo 'Tamanho, naoOpt, opt' > $SAIDAS_CSV/TIME.csv

echo "Gerando Sistemas, Resultados e Fazendo o benchmark"
SAIDALIKWID="$SRC/saida-likwid"
[[ -d $SAIDALIKWID ]] || mkdir -p $SAIDALIKWID
echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor
for i in "${TAMANHOS_TESTE[@]}"; do
  $SRC/geraSL $i > $ENTRADAS/$i.sistema
  likwid-perfctr -C 3 -g L3       -o $SAIDALIKWID/${i}L3.likwid       -m $SRC/gaussJacobi-likwid $ENTRADAS/$i.sistema
  likwid-perfctr -C 3 -g L2CACHE  -o $SAIDALIKWID/${i}L2CACHE.likwid  -m $SRC/gaussJacobi-likwid $ENTRADAS/$i.sistema
  likwid-perfctr -C 3 -g FLOPS_DP -o $SAIDALIKWID/${i}FLOPS_DP.likwid -m $SRC/gaussJacobi-likwid $ENTRADAS/$i.sistema
  
  echo "$i, $($SRC/gaussJacobi-likwid $ENTRADAS/$i.sistema)" >> $SAIDAS_CSV/TIME.csv

  # Formata saida para csv
  echo "$i, $(grep 'L3 bandwidth \[MBytes/s\]' $SAIDALIKWID/${i}L3.likwid |  grep -o -P '(?<=,).*(?=,)' | sed -z 's/\n/, /g;s/, $/\n/')" >> $SAIDAS_CSV/L3.csv
  echo "$i, $(grep 'L2 miss ratio' $SAIDALIKWID/${i}L2CACHE.likwid |  grep -o -P '(?<=,).*(?=,)' | sed -z 's/\n/, /g;s/, $/\n/')" >> $SAIDAS_CSV/L2CACHE.csv
  echo "$i, $(grep 'DP \[MFLOP/s\]' $SAIDALIKWID/${i}FLOPS_DP.likwid |  grep -o -P '(?<=,).*(?=,)' | sed -z 's/\n/, /g;s/, $/\n/')" >> $SAIDAS_CSV/FLOPS_DP.csv
done
cd $DIR_INICIAL
echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor 

GRAFICOS="$DIR_INICIAL/graficos"
[[ -d $GRAFICOS ]] || mkdir -p $GRAFICOS

gnuplot -persist <<-EOFMarker
set terminal png size 800,500 enhanced
set output '$GRAFICOS/memoryBandwith.png'

set xlabel "Tamanho"
set ylabel "MBytes/s"
set title "L3 Memory bandwidth"
plot "$SAIDAS_CSV/L3.csv" u 1:2 w lines smooth unique title 'naoOpt', \
    "$SAIDAS_CSV/L3.csv" u 1:3 w lines smooth unique title 'opt'
EOFMarker

gnuplot -persist <<-EOFMarker
set terminal png size 800,500 enhanced
set output '$GRAFICOS/cacheMissRatio.png'

set xlabel "Tamanho"
set ylabel "Miss Ratio"
set title "Data cache miss ratio"
plot "$SAIDAS_CSV/L2CACHE.csv" u 1:2 w lines smooth unique title 'naoOpt', \
      "$SAIDAS_CSV/L2CACHE.csv" u 1:3 w lines smooth unique title 'opt'
EOFMarker

gnuplot -persist <<-EOFMarker
set terminal png size 800,500 enhanced
set output '$GRAFICOS/flops.png'

set xlabel "Tamanho"
set ylabel "MFLOP/s"
set title "Flops"
plot "$SAIDAS_CSV/FLOPS_DP.csv" u 1:2 w lines smooth unique title 'dp-naoOpt', \
      "$SAIDAS_CSV/FLOPS_DP.csv" u 1:3 w lines smooth unique title 'avx-naoOpt', \
      "$SAIDAS_CSV/FLOPS_DP.csv" u 1:4 w lines smooth unique title 'dp-opt', \
      "$SAIDAS_CSV/FLOPS_DP.csv" u 1:5 w lines smooth unique title 'avx-opt'
EOFMarker

gnuplot -persist <<-EOFMarker
set terminal png size 800,500 enhanced
set output '$GRAFICOS/tempo.png'

set xlabel "Tamanho"
set ylabel "s"
set title "Tempo"
plot "$SAIDAS_CSV/TIME.csv" u 1:2 w lines smooth unique title 'naoOpt', \
      "$SAIDAS_CSV/TIME.csv" u 1:3 w lines smooth unique title 'opt'
EOFMarker
