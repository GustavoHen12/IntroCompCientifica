#!/usr/bin/gnuplot -persist
SAIDAS_CSV = GPVAL_PWD."/sistema/saida-csv"
GRAFICOS = GPVAL_PWD."/graficos"

set terminal png size 800,500 enhanced
set output GRAFICOS.'/L3.png'
set xlabel "Tamanho"
set ylabel "MBytes/s"
set title "L3 Memory bandwidth"
plot SAIDAS_CSV."/L3.csv" u 1:2 w lines smooth unique title 'naoOpt', \
    SAIDAS_CSV."/L3.csv" u 1:3 w lines smooth unique title 'opt'

set term qt 1
plot SAIDAS_CSV."/L3.csv" u 1:2 w lines smooth unique title 'naoOpt', \
    SAIDAS_CSV."/L3.csv" u 1:3 w lines smooth unique title 'opt'


set terminal png size 800,500 enhanced
set output GRAFICOS.'/L2CACHE.png'
set xlabel "Tamanho"
set ylabel "Miss Ratio"
set title "Data cache miss ratio"
plot SAIDAS_CSV."/L2CACHE.csv" u 1:2 w lines smooth unique title 'naoOpt', \
      SAIDAS_CSV."/L2CACHE.csv" u 1:3 w lines smooth unique title 'opt'

set term qt 2
plot SAIDAS_CSV."/L2CACHE.csv" u 1:2 w lines smooth unique title 'naoOpt', \
      SAIDAS_CSV."/L2CACHE.csv" u 1:3 w lines smooth unique title 'opt'


set terminal png size 800,500 enhanced
set output GRAFICOS.'/FLOPS_DP.png'
set xlabel "Tamanho"
set ylabel "MFLOP/s"
set title "Flops"
plot SAIDAS_CSV."/FLOPS_DP.csv" u 1:2 w lines smooth unique title 'dp-naoOpt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:3 w lines smooth unique title 'avx-naoOpt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:4 w lines smooth unique title 'dp-opt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:5 w lines smooth unique title 'avx-opt'

set term qt 3
plot SAIDAS_CSV."/FLOPS_DP.csv" u 1:2 w lines smooth unique title 'dp-naoOpt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:3 w lines smooth unique title 'avx-naoOpt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:4 w lines smooth unique title 'dp-opt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:5 w lines smooth unique title 'avx-opt'

set terminal png size 800,500 enhanced
set output GRAFICOS.'/TIME.png'
set xlabel "Tamanho"
set logscale y
set ylabel "s"
set title "Tempo"
plot SAIDAS_CSV."/TIME.csv" u 1:2 w lines smooth unique title 'naoOpt', \
      SAIDAS_CSV."/TIME.csv" u 1:3 w lines smooth unique title 'opt'

set term qt 4
plot SAIDAS_CSV."/TIME.csv" u 1:2 w lines smooth unique title 'naoOpt', \
      SAIDAS_CSV."/TIME.csv" u 1:3 w lines smooth unique title 'opt'
