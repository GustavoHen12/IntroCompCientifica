#!/bin/bash
[[ ( -z $1 || $# -ne $(( $1 + 2 )) ) ]] && echo "Forma de uso: <quantidade de pasta para juntar informações> <pasta1> <pasta2> ... <pastaN> <pasta para saída>" && exit
DIRINICIAL=$PWD # diretorio de onde foi rodado o programa

TAMANHOS_TESTE=( 10 32 50 64 100 ) #128 200 250 256 300 400 512 600 1000 1024 2000 2048 3000 4096)

# Variaveis para diretorios
THISDIR=$(dirname $(realpath $0)) # diretorio onde está o script
OUT=${!#}
mkdir -p $OUT

cd $THISDIR

printf "tamanho, " > $OUT/L3.csv
printf "tamanho, " > $OUT/L2CACHE.csv
printf "tamanho, " > $OUT/FLOPS_DP.csv
count=1
for i in $@;do
  if [[ $count -ne 1 && $count -ne $# ]]; then
    COLUMN_INDEX=$(echo ${i} | sed 's/.*\///g')
    printf "${COLUMN_INDEX}_GeracaoJacobiana, ${COLUMN_INDEX}_MetodoNewton, ${COLUMN_INDEX}_CalculaJacobiana, ${COLUMN_INDEX}_SistemaLinear" >> $OUT/L3.csv
    printf "${COLUMN_INDEX}_GeracaoJacobiana, ${COLUMN_INDEX}_MetodoNewton, ${COLUMN_INDEX}_CalculaJacobiana, ${COLUMN_INDEX}_SistemaLinear" >> $OUT/L2CACHE.csv
    printf "${COLUMN_INDEX}_GeracaoJacobiana_dp, ${COLUMN_INDEX}_GeracaoJacobiana_avx, ${COLUMN_INDEX}_MetodoNewton_dp, ${COLUMN_INDEX}_MetodoNewton_avx, ${COLUMN_INDEX}_CalculaJacobiana_dp, ${COLUMN_INDEX}_CalculaJacobiana_avx, ${COLUMN_INDEX}_SistemaLinear_dp, ${COLUMN_INDEX}_SistemaLinear_avx" >> $OUT/FLOPS_DP.csv
  fi
  count=$(( $count + 1))
done
printf "\n" >> $OUT/L3.csv
printf "\n" >> $OUT/L2CACHE.csv
printf "\n" >> $OUT/FLOPS_DP.csv

#echo "tamanho, naoOpt_GeracaoJacobiana, naoOpt_MetodoNewton, naoOpt_CalculaJacobiana, naoOpt_SistemaLinear, opt_GeracaoJacobiana, opt_MetodoNewton, opt_CalculaJacobiana, opt_SistemaLinear" > $OUTCSV/L3.csv
#echo "tamanho, naoOpt_GeracaoJacobiana, naoOpt_MetodoNewton, naoOpt_CalculaJacobiana, naoOpt_SistemaLinear, opt_GeracaoJacobiana, opt_MetodoNewton, opt_CalculaJacobiana, opt_SistemaLinear" > $OUTCSV/L2CACHE.csv
#echo "tamanho, naoOpt_GeracaoJacobiana_dp, naoOpt_GeracaoJacobiana_avx, naoOpt_MetodoNewton_dp, naoOpt_MetodoNewton_avx, naoOpt_CalculaJacobiana_dp, naoOpt_CalculaJacobiana_avx, naoOpt_SistemaLinear_dp, naoOpt_SistemaLinear_avx, opt_GeracaoJacobiana_dp, opt_GeracaoJacobiana_avx, opt_MetodoNewton_dp, opt_MetodoNewton_avx, opt_CalculaJacobiana_dp, opt_CalculaJacobiana_avx, opt_SistemaLinear_dp, opt_SistemaLinear_avx" > $OUTCSV/FLOPS_DP.csv
#echo "tamanho, naoOpt_MetodoNewton, naoOpt_GeracaoJacobiana, naoOpt_CalculoJacobiana, naoOpt_SistemaLinear, opt_MetodoNewton, opt_GeracaoJacobiana, opt_CalculoJacobiana, opt_SistemaLinear" > $OUTCSV/TIME.csv
## Execução dos códigos salvando os testes
#cd $THISDIR
#for i in ${TAMANHOS_TESTE[@]}; do
#  echo "Gerando CSV para $(basename $i)"
#  # Formata saida para csv
#  echo "$i, $(grep 'L3 bandwidth \[MBytes/s\]' $OUTLIKWID/${i}L3.likwid |  grep -o -P '(?<=,).*(?=,)' | sed -z 's/\n/, /g;s/, $/\n/')" >> $OUTCSV/L3.csv
#  echo "$i, $(grep 'L2 miss ratio' $OUTLIKWID/${i}L2CACHE.likwid |  grep -o -P '(?<=,).*(?=,)' | sed -z 's/\n/, /g;s/, $/\n/')" >> $OUTCSV/L2CACHE.csv
#  echo "$i, $(grep 'DP \[MFLOP/s\]' $OUTLIKWID/${i}FLOPS_DP.likwid |  grep -o -P '(?<=,).*(?=,)' | sed -z 's/\n/, /g;s/, $/\n/')" >> $OUTCSV/FLOPS_DP.csv
#  TEMPO_NOPT=$(tail -n 6 $SAIDAS/naoOpt/out_$i.txt | head -n 4 | sed 's/# //g' | cut -d: -f 2 | sed -z 's/\n/, /g;s/, $/\n/;s/^ //g')
#  TEMPO_OPT=$(tail -n 6 $SAIDAS/opt/out_$i.txt | head -n 4 | sed 's/# //g' | cut -d: -f 2 | sed -z 's/\n/, /g;s/, $/\n/;s/^ //g')
#  echo "$i, $TEMPO_NOPT, $TEMPO_OPT" >> $OUTCSV/TIME.csv
#done
#
#
#cd $DIRINICIAL
