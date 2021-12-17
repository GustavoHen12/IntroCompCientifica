#!/bin/bash
[[ $# -ne 3 ]] && echo "Forma de uso (mande a pasta com maior numero de entradas primeiro): <pasta1> <pasta2> <pasta para saída>" && exit
DIRINICIAL=$PWD # diretorio de onde foi rodado o programa

TAMANHOS_TESTE=( 10 32 50 64 100 128 200 250 256 300 400 512 600 1000 1024 2000 2048 3000 4096)

# Variaveis para diretorios
THISDIR=$(dirname $(realpath $0)) # diretorio onde está o script
OUT=$3
mkdir -p $OUT

cd $THISDIR

printf "tamanho, " > $OUT/L3.csv
printf "tamanho, " > $OUT/L2CACHE.csv
printf "tamanho, " > $OUT/FLOPS_DP.csv
printf "$(basename $1)_GeracaoJacobiana, $(basename $1)_MetodoNewton, $(basename $1)_CalculaJacobiana, $(basename $1)_SistemaLinear" >> $OUT/L3.csv
printf "$(basename $1)_GeracaoJacobiana, $(basename $1)_MetodoNewton, $(basename $1)_CalculaJacobiana, $(basename $1)_SistemaLinear" >> $OUT/L2CACHE.csv
printf "$(basename $1)_GeracaoJacobiana_dp, $(basename $1)_GeracaoJacobiana_avx, $(basename $1)_MetodoNewton_dp, $(basename $1)_MetodoNewton_avx, $(basename $1)_CalculaJacobiana_dp, $(basename $1)_CalculaJacobiana_avx, $(basename $1)_SistemaLinear_dp, $(basename $1)_SistemaLinear_avx" >> $OUT/FLOPS_DP.csv
printf "$(basename $2)_GeracaoJacobiana, $(basename $2)_MetodoNewton, $(basename $2)_CalculaJacobiana, $(basename $2)_SistemaLinear" >> $OUT/L3.csv
printf "$(basename $2)_GeracaoJacobiana, $(basename $2)_MetodoNewton, $(basename $2)_CalculaJacobiana, $(basename $2)_SistemaLinear" >> $OUT/L2CACHE.csv
printf "$(basename $2)_GeracaoJacobiana_dp, $(basename $2)_GeracaoJacobiana_avx, $(basename $2)_MetodoNewton_dp, $(basename $2)_MetodoNewton_avx, $(basename $2)_CalculaJacobiana_dp, $(basename $2)_CalculaJacobiana_avx, $(basename $2)_SistemaLinear_dp, $(basename $2)_SistemaLinear_avx" >> $OUT/FLOPS_DP.csv
printf "\n" >> $OUT/L3.csv
printf "\n" >> $OUT/L2CACHE.csv
printf "\n" >> $OUT/FLOPS_DP.csv
echo "tamanho, $(basename $1)_MetodoNewton, $(basename $1)_GeracaoJacobiana, $(basename $1)_CalculaJacobiana, $(basename $1)_SistemaLinear, $(basename $2)_MetodoNewton, $(basename $2)_GeracaoJacobiana, $(basename $2)_CalculaJacobiana, $(basename $2)_SistemaLinear" > $OUT/TIME.csv

echo "Criando arquivos temporários"
for i in ${TAMANHOS_TESTE[@]}; do
  echo "Criando para $i"
  cat $1/likwid/${i}L3.likwid       > $OUT/${i}L3.temp
  cat $1/likwid/${i}L2CACHE.likwid  > $OUT/${i}L2CACHE.temp
  cat $1/likwid/${i}FLOPS_DP.likwid > $OUT/${i}FLOPS_DP.temp
  cat $2/likwid/${i}L3.likwid       >> $OUT/${i}L3.temp
  cat $2/likwid/${i}L2CACHE.likwid  >> $OUT/${i}L2CACHE.temp
  cat $2/likwid/${i}FLOPS_DP.likwid >> $OUT/${i}FLOPS_DP.temp
done
## Execução dos códigos salvando os testes
for i in ${TAMANHOS_TESTE[@]}; do
  echo "Gerando CSV para $(basename $i)"
  # Formata saida para csv
  echo "$i, $(grep 'L3 bandwidth \[MBytes/s\]' $OUT/${i}L3.temp |  grep -o -P '(?<=,).*(?=,)' | sed -z 's/\n/, /g;s/, $/\n/')" >> $OUT/L3.csv
  echo "$i, $(grep 'L2 miss ratio' $OUT/${i}L2CACHE.temp |  grep -o -P '(?<=,).*(?=,)' | sed -z 's/\n/, /g;s/, $/\n/')" >> $OUT/L2CACHE.csv
  echo "$i, $(grep 'DP \[MFLOP/s\]' $OUT/${i}FLOPS_DP.temp |  grep -o -P '(?<=,).*(?=,)' | sed -z 's/\n/, /g;s/, $/\n/')" >> $OUT/FLOPS_DP.csv
  TEMPO_1=$(tail -n 6 $1/$(basename $1)/out_$i.txt | head -n 4 | sed 's/# //g' | cut -d: -f 2 | sed -z 's/\n/, /g;s/, $/\n/;s/^ //g')
  TEMPO_2=$(tail -n 6 $2/$(basename $2)/out_$i.txt | head -n 4 | sed 's/# //g' | cut -d: -f 2 | sed -z 's/\n/, /g;s/, $/\n/;s/^ //g')
  echo "$i, $TEMPO_1, $TEMPO_2" >> $OUT/TIME.csv
done

rm $OUT/*.temp
