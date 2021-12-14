Organização:
(naoOpt, opt) pasta para cada programa
(saida)       pasta para as saidas, essa pasta contendo:
  (opt)       pasta para as saídas do teste programa otimizado
    (likwid)  pasta para a saída do likwid
    (csv)     pasta para o csv com os dados desejados
    (opt)     pasta para a saída do programa otimizado
  (naoOpt)    pasta para as saídas do teste programa não otimizado
    (likwid)  pasta para a saída do likwid
    (csv)     pasta para o csv com os dados desejados
    (naoOpt)  pasta para a saída do programa não otimizado
(graficos)    pasta para salvar os graficos como png
(entradas)    pasta que contém as entradas a serem usadas pelos programas
(broyden.sh)  script que gera as entradas a serem usadas pelos programas
(graficos.gp) grafico em gnuplot que gera os graficos utilizando os conteúdos de ./saida/csv
(broyden.sh)  script que faz a exucação dos programas usando a biblioteca likwid para obter a saída desejada
