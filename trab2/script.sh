#!/bin/bash
DIRINICIAL=$PWD # diretorio de onde foi rodado o programa

TAMANHOS_TESTE=( 10 32 50 64 ) #100 128 200 250 256 300 400 512 600 1000 1024 2000 2048 3000 4096)

# Variaveis para diretorios
THISDIR=$(dirname $(realpath $0)) # diretorio onde está o script
SRCNOPT=$THISDIR/naoOpt           # pasta para diretorio do codigo nao otimizado
SRCOPT=$THISDIR/opt               # pasta para diretorio do codigo nao otimizado
SAIDAS=$THISDIR/saida             # pasta onde terão todas as saidas
OUTLIKWID=$SAIDAS/likwid          # pasta para os resultados obtidos com likwid
OUTNOPT=$SAIDAS/naoOpt            # pasta para a saída do programa nao otimizado
OUTOPT=$SAIDAS/opt                # pasta para a saída do programa otimizado
OUTCSV=$SAIDAS/csv                # pasta para salvar os csvs para serem usados por graficos.gp
ENTRADAS=$THISDIR/entradas        # pasta para as saídas de ./broyden.sh

cd $THISDIR

# Criação das pastas onde serão salvos os resultados
mkdir -p $OUTLIKWID $OUTNOPT $OUTOPT $OUTCSV $ENTRADAS

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

# Execução make
cd $SRCNOPT
make
cd $SRCOPT
make

# Execução dos códigos salvando os testes
cd $THISDIR
echo "Fazendo testes"
for i in ${TAMANHOS_TESTE[@]}; do
  echo "Executando testes para tamanho $(basename $i)"

  # Teste para versão não otimizada
  $SRCNOPT/newtonSNL -o $OUTNOPT/out_$i.txt < $ENTRADAS/$i.sistema
  likwid-perfctr -O -C 3 -g L3       -o $OUTLIKWID/${i}NOPTL3.likwid       -m $SRCNOPT/newtonSNL -o /dev/null < $ENTRADAS/$i.sistema
  likwid-perfctr -O -C 3 -g L2CACHE  -o $OUTLIKWID/${i}NOPTL2CACHE.likwid  -m $SRCNOPT/newtonSNL -o /dev/null < $ENTRADAS/$i.sistema
  likwid-perfctr -O -C 3 -g FLOPS_DP -o $OUTLIKWID/${i}NOPTFLOPS_DP.likwid -m $SRCNOPT/newtonSNL -o /dev/null < $ENTRADAS/$i.sistema

  # Teste para versão otimizada
  $SRCOPT/newtonSNL -o $OUTOPT/out$i.txt < $ENTRADAS/$i.sistema
  likwid-perfctr -O -C 3 -g L3       -o $OUTLIKWID/${i}OPTL3.likwid        -m $SRCOPT/newtonSNL -o /dev/null < $ENTRADAS/$i.sistema
  likwid-perfctr -O -C 3 -g L2CACHE  -o $OUTLIKWID/${i}OPTL2CACHE.likwid   -m $SRCOPT/newtonSNL -o /dev/null < $ENTRADAS/$i.sistema
  likwid-perfctr -O -C 3 -g FLOPS_DP -o $OUTLIKWID/${i}OPTFLOPS_DP.likwid  -m $SRCOPT/newtonSNL -o /dev/null < $ENTRADAS/$i.sistema

  # Junta saída da versão nao otimizada com a versão otimizada
  echo "NAO OTIMIZADO" > $OUTLIKWID/${i}L3.likwid
  echo "NAO OTIMIZADO" > $OUTLIKWID/${i}L2CACHE.likwid
  echo "NAO OTIMIZADO" > $OUTLIKWID/${i}FLOPS_DP.likwid
  cat $OUTLIKWID/${i}NOPTL3.likwid       >> $OUTLIKWID/${i}L3.likwid
  cat $OUTLIKWID/${i}NOPTL2CACHE.likwid  >> $OUTLIKWID/${i}L2CACHE.likwid
  cat $OUTLIKWID/${i}NOPTFLOPS_DP.likwid >> $OUTLIKWID/${i}FLOPS_DP.likwid
  echo "OTIMIZADO" >> $OUTLIKWID/${i}L3.likwid
  echo "OTIMIZADO" >> $OUTLIKWID/${i}L2CACHE.likwid
  echo "OTIMIZADO" >> $OUTLIKWID/${i}FLOPS_DP.likwid
  cat $OUTLIKWID/${i}OPTL3.likwid       >> $OUTLIKWID/${i}L3.likwid
  cat $OUTLIKWID/${i}OPTL2CACHE.likwid  >> $OUTLIKWID/${i}L2CACHE.likwid
  cat $OUTLIKWID/${i}OPTFLOPS_DP.likwid >> $OUTLIKWID/${i}FLOPS_DP.likwid
done


cd $DIRINICIAL
