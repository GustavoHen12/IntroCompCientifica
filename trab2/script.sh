#!/bin/bash
DIRINICIAL=$PWD # diretorio de onde foi rodado o programa

[[ $# -ne 2 ]] && echo "Forma de usar: $0 <pasta com código fonte e makefile> <pasta para saída>" && exit

TAMANHOS_TESTE=( 10 32 50 64 100 128 200 250 256 300 400 512 600 1000 1024 2000 2048 3000 4096)
echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

# Variaveis para diretorios
THISDIR=$(dirname $(realpath $0)) # diretorio onde está o script
GRAFICOS=$THISDIR/graficos        # pasta para diretorio com os arquivos dos graficos gerados
SRC=$1                            # pasta para diretorio do codigo
SAIDAS=$2/$(basename $1)          # pasta onde terão todas as saidas
OUTLIKWID=$SAIDAS/likwid          # pasta para os resultados obtidos com likwid
OUTSRC=$SAIDAS/$1                 # pasta para a saída do programa
OUTCSV=$SAIDAS/csv                # pasta para salvar os csvs para serem usados por graficos.gp
ENTRADAS=$THISDIR/entradas        # pasta para as saídas de ./broyden.sh

cd $THISDIR

# Criação das pastas onde serão salvos os resultados
mkdir -p $SAIDAS $OUTLIKWID $OUTSRC $OUTCSV $ENTRADAS

# Geração dos sistemas, cada sistema em um arquivo para ser mais fácil separar os testes por tamanho
cd $ENTRADAS
echo "Gerando entradas"
source $THISDIR/broyden.sh > ./all.sistema
csplit all.sistema '/^$/+1' '{*}' > /dev/null
rm "xx${#TAMANHOS_TESTE[@]}"
for i in $(ls xx*); do
  mv $i "$(head -n 1 $i).sistema"
done
rm all.sistema
cd $THISDIR

# Execução make
cd $SRC
make

# Execução dos códigos salvando os testes
cd $THISDIR
echo "Fazendo testes"
for i in ${TAMANHOS_TESTE[@]}; do
  echo "Executando testes para tamanho $(basename $i)"

  # Teste para versão passada na linha de comando
  $SRC/newtonSNL -o $OUTSRC/out_$i.txt < $ENTRADAS/$i.sistema
  likwid-perfctr -O -C 3 -g L3       -o $OUTLIKWID/${i}L3.likwid       -m $SRC/newtonSNL -o /dev/null < $ENTRADAS/$i.sistema
  likwid-perfctr -O -C 3 -g L2CACHE  -o $OUTLIKWID/${i}L2CACHE.likwid  -m $SRC/newtonSNL -o /dev/null < $ENTRADAS/$i.sistema
  likwid-perfctr -O -C 3 -g FLOPS_DP -o $OUTLIKWID/${i}FLOPS_DP.likwid -m $SRC/newtonSNL -o /dev/null < $ENTRADAS/$i.sistema

done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor
cd $DIRINICIAL
