#!/usr/bin/gnuplot -persist
SAIDAS_CSV = GPVAL_PWD."/csv"
GRAFICOS = GPVAL_PWD."/graficos"

# L3
set terminal png size 800,500 enhanced
set output GRAFICOS.'/GERACAOJACOBIANA_L3.png'
set xlabel "Tamanho"
set ylabel "MBytes/s"
set title "L3 Memory bandwidth - Geração Jacobiana"
plot SAIDAS_CSV."/L3.csv" u 1:2 w lines smooth unique title 'opt', \
    SAIDAS_CSV."/L3.csv" u 1:6 w lines smooth unique title 'naoOpt'

set terminal png size 800,500 enhanced
set output GRAFICOS.'/METODONEWTON_L3.png'
set xlabel "Tamanho"
set ylabel "MBytes/s"
set title "L3 Memory bandwidth -  Método Newton"
plot SAIDAS_CSV."/L3.csv" u 1:3 w lines smooth unique title 'opt', \
    SAIDAS_CSV."/L3.csv" u 1:7 w lines smooth unique title 'naoOpt'

set terminal png size 800,500 enhanced
set output GRAFICOS.'/CALCULAJACOBIANA_L3.png'
set xlabel "Tamanho"
set ylabel "MBytes/s"
set title "L3 Memory bandwidth - Calculo da Jacobiana "
plot SAIDAS_CSV."/L3.csv" u 1:4 w lines smooth unique title 'opt', \
    SAIDAS_CSV."/L3.csv" u 1:8 w lines smooth unique title 'naoOpt'

set terminal png size 800,500 enhanced
set output GRAFICOS.'/SISTEMALINEAR_L3.png'
set xlabel "Tamanho"
set ylabel "MBytes/s"
set title "L3 Memory bandwidth - Calculo Sistema Linear"
plot SAIDAS_CSV."/L3.csv" u 1:5 w lines smooth unique title 'opt', \
    SAIDAS_CSV."/L3.csv" u 1:9 w lines smooth unique title 'naoOpt'

# set term qt 1
# plot SAIDAS_CSV."/L3.csv" u 1:2 w lines smooth unique title 'opt', \
#     SAIDAS_CSV."/L3.csv" u 1:3 w lines smooth unique title 'naoOpt'

set terminal png size 800,500 enhanced
set output GRAFICOS.'/GERACAOJACOBIANA_L2CACHE.png'
set xlabel "Tamanho"
set ylabel "MBytes/s"
set title "Data cache miss ratio - Geração Jacobiana"
plot SAIDAS_CSV."/L2CACHE.csv" u 1:2 w lines smooth unique title 'opt', \
    SAIDAS_CSV."/L2CACHE.csv" u 1:6 w lines smooth unique title 'naoOpt'

set terminal png size 800,500 enhanced
set output GRAFICOS.'/METODONEWTON_L2CACHE.png'
set xlabel "Tamanho"
set ylabel "MBytes/s"
set title "Data cache miss ratio -  Método Newton"
plot SAIDAS_CSV."/L2CACHE.csv" u 1:3 w lines smooth unique title 'opt', \
    SAIDAS_CSV."/L2CACHE.csv" u 1:7 w lines smooth unique title 'naoOpt'

set terminal png size 800,500 enhanced
set output GRAFICOS.'/CALCULAJACOBIANA_L2CACHE.png'
set xlabel "Tamanho"
set ylabel "MBytes/s"
set title "Data cache miss ratio - Calculo da Jacobiana "
plot SAIDAS_CSV."/L2CACHE.csv" u 1:4 w lines smooth unique title 'opt', \
    SAIDAS_CSV."/L2CACHE.csv" u 1:8 w lines smooth unique title 'naoOpt'

set terminal png size 800,500 enhanced
set output GRAFICOS.'/SISTEMALINEAR_L2CACHE.png'
set xlabel "Tamanho"
set ylabel "MBytes/s"
set title "Data cache miss ratio - Calculo Sistema Linear"
plot SAIDAS_CSV."/L2CACHE.csv" u 1:5 w lines smooth unique title 'opt', \
    SAIDAS_CSV."/L2CACHE.csv" u 1:9 w lines smooth unique title 'naoOpt'

# set term qt 2
# plot SAIDAS_CSV."/L2CACHE.csv" u 1:2 w lines smooth unique title 'opt', \
#       SAIDAS_CSV."/L2CACHE.csv" u 1:3 w lines smooth unique title 'naoOpt'

# FLOPS_DP
set terminal png size 800,500 enhanced
set output GRAFICOS.'/GERACAOJACOBIANA_FLOPS_DP.png'
set xlabel "Tamanho"
set ylabel "MFLOP/s"
set title "Flops - Geração Jacobiana"
plot SAIDAS_CSV."/FLOPS_DP.csv" u 1:2 w lines smooth unique title 'dp-opt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:3 w lines smooth unique title 'avx-opt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:10 w lines smooth unique title 'dp-naoOpt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:11 w lines smooth unique title 'avx-naoOpt'

set terminal png size 800,500 enhanced
set output GRAFICOS.'/METODONEWTON_FLOPS_DP.png'
set xlabel "Tamanho"
set ylabel "MFLOP/s"
set title "Flops - Método Newton"
plot SAIDAS_CSV."/FLOPS_DP.csv" u 1:4 w lines smooth unique title 'dp-opt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:5 w lines smooth unique title 'avx-opt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:12 w lines smooth unique title 'dp-naoOpt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:13 w lines smooth unique title 'avx-naoOpt'

set terminal png size 800,500 enhanced
set output GRAFICOS.'/CALCULAJACOBIANA_FLOPS_DP.png'
set xlabel "Tamanho"
set ylabel "MFLOP/s"
set title "Flops - Calcula Jacobiana"
plot SAIDAS_CSV."/FLOPS_DP.csv" u 1:6 w lines smooth unique title 'dp-opt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:7 w lines smooth unique title 'avx-opt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:14 w lines smooth unique title 'dp-naoOpt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:15 w lines smooth unique title 'avx-naoOpt'

set terminal png size 800,500 enhanced
set output GRAFICOS.'/SISTEMALINEAR_FLOPS_DP.png'
set xlabel "Tamanho"
set ylabel "MFLOP/s"
set title "Flops - Calculo Sistema Linear"
plot SAIDAS_CSV."/FLOPS_DP.csv" u 1:8 w lines smooth unique title 'dp-opt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:9 w lines smooth unique title 'avx-opt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:16 w lines smooth unique title 'dp-naoOpt', \
      SAIDAS_CSV."/FLOPS_DP.csv" u 1:17 w lines smooth unique title 'avx-naoOpt'

# TEMPO
set terminal png size 800,500 enhanced
set output GRAFICOS.'/METODONEWTON_TIME.png'
set xlabel "Tamanho"
set ylabel "s"
set logscale y
set title "Tempo -  Método Newton"
plot SAIDAS_CSV."/TIME.csv" u 1:2 w lines smooth unique title 'opt', \
    SAIDAS_CSV."/TIME.csv" u 1:6 w lines smooth unique title 'naoOpt'

set terminal png size 800,500 enhanced
set output GRAFICOS.'/GERACAOJACOBIANA_TIME.png'
set xlabel "Tamanho"
set ylabel "s"
set logscale y
set title "Tempo - Geração Jacobiana"
plot SAIDAS_CSV."/TIME.csv" u 1:3 w lines smooth unique title 'opt', \
    SAIDAS_CSV."/TIME.csv" u 1:7 w lines smooth unique title 'naoOpt'

set terminal png size 800,500 enhanced
set output GRAFICOS.'/CALCULAJACOBIANA_TIME.png'
set xlabel "Tamanho"
set ylabel "s"
set logscale y
set title "Tempo - Calculo da Jacobiana "
plot SAIDAS_CSV."/TIME.csv" u 1:4 w lines smooth unique title 'opt', \
    SAIDAS_CSV."/TIME.csv" u 1:8 w lines smooth unique title 'naoOpt'

set terminal png size 800,500 enhanced
set output GRAFICOS.'/SISTEMALINEAR_TIME.png'
set xlabel "Tamanho"
set ylabel "s"
set logscale y
set title "Tempo - Calculo Sistema Linear"
plot SAIDAS_CSV."/TIME.csv" u 1:5 w lines smooth unique title 'opt', \
    SAIDAS_CSV."/TIME.csv" u 1:9 w lines smooth unique title 'naoOpt'

# set term qt 4
# plot SAIDAS_CSV."/TIME.csv" u 1:2 w lines smooth unique title 'opt', \
#       SAIDAS_CSV."/TIME.csv" u 1:3 w lines smooth unique title 'naoOpt'
