#!/bin/bash
DIRINICIAL=$PWD # diretorio de onde foi rodado o programa

TAMANHOS_TESTE=( 10 32 50 64 100 128 200 250 256 300 400 512 600 1000 1024 2000 2048 3000 4096)

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
if [[ "$1" != "--nogen" ]]; then 
  cd $ENTRADAS
  echo "Gerando entradas"
  source $THISDIR/broyden.sh > ./all.sistema
  csplit all.sistema '/^$/+1' '{*}' > /dev/null
  rm "xx${#TAMANHOS_TESTE[@]}"
  for i in $(ls xx*); do
    mv $i "$(head -n 1 $i).sistema"
  done
  rm all.sistema
fi

cd $DIRINICIAL
