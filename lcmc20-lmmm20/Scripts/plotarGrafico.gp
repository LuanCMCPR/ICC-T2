#!/usr/bin/gnuplot -c

# Luan Carlos Maia Cruz - GRR20203891
# Leonardo Marin Mendes Martin - GRR20205675 

# Uso: gnuplot -c plotarGrafico.gp <arquivo1> <arquivo2> <titulo do grafico> <nome_do_arquivo> <unidade eixo y> <metrica>
# 6 argumentos 

# variáveis para comparar não otimizado com otimizado
arqSemOtz = ARG1;
arqComOtz = ARG2;
metrica = ARG3;
nome = ARG4;
unidade = ARG5;
compara = ARG6;

# Configurações do gráfico
set terminal pngcairo enhanced font "arial,11" size 1600,900
set output nome.".png"

# Títulos e rótulos
set title metrica
set xlabel "pontos"
set ylabel unidade 

set logscale x

# Configuração do estilo de linha para cada conjunto de dados
set linetype 1 lc rgb 'red' lw 2 pt 7 ps 1.5   # Linha 1
set linetype 2 lc rgb 'green' lw 2 pt 7 ps 1.5   # Linha 2
set linetype 2 lc rgb 'green' lw 2 pt 7 ps 1.5   # Linha 2
# pt: Como o ponto é representado, existem vários representações 
# ps: Tamanho do ponto
# lw: Tamano da linha


# Para a metrica FLOPS DP o gráfico é diferente
if(compara ne "FLOPS DP"){
     if(compara eq "TEMPO"){
          set logscale y
     }

     plot arqSemOtz using 1:2 with linespoints title "sem otimizacao", \
          arqComOtz using 1:2 with linespoints title "com otimizacao"
}
else{
     # Escolhendo as cores para as outras linhas
     # set linetype 1 lc rgb 'red' lw 2 pt 7 ps 1.5   # Linha 1
     # set linetype 2 lc rgb 'orange' lw 2 pt 7 ps 1.5   # Linha 2
     # set linetype 3 lc rgb 'green' lw 2 pt 7 ps 1.5   # Linha 3
     # set linetype 4 lc rgb 'blue' lw 2 pt 7 ps 1.5   # Linha 4
     # Plotando gráfico para a metrica de FLOPS DP
     plot arqSemOtz using 1:2 with linespoints title "FLOPS sem otimizacao", \
          arqComOtz using 1:2 with linespoints title "FLOPS com otimizacao"
     set output nome." AVX.png"
     set title metrica." AVX"
     plot arqSemOtz using 1:3 with linespoints title "FLOPS AVX sem otimizacao", \
          arqComOtz using 1:3 with linespoints title "FLOPS AVX com otimizacao"
}
