#!/bin/bash

# Luan Carlos Maia Cruz - GRR20203891
# Leonardo Marin Mendes Martin - GRR20205675 

# Uso: ./geraGraficos.sh <diretorio_de_entrada_pontos_nao_otimizado > <diretorio_de_entrada_pontos_otimizados> <diretorio_salvar_gráficos>
# 3 argumentos

# Testa se possui 3 argumentos
if [ $# -ne 3 ]
then
    echo "Uso: ./geraGraficos.sh <diretorio_de_entrada_pontos_nao_otimizado > <diretorio_de_entrada_pontos_otimizados> <diretorio_salvar_gráficos>"
    exit 1
fi

# Diretório de entrada e saída
IN_NOT_OPT=$1
IN_OPT=$2
OUTPUT_DIR=$3
mkdir -p $OUTPUT_DIR

# Metrica e Unidades
METRICAS=("TEMPO" "L3" "L2CACHE" "FLOPS DP")
UNIDADES=("ms" "Mbytes/s" "Miss ratio" "Mflops/s")
TAM=${#METRICAS[@]}

# Arquivos de entrada
GERALSL=("$IN_NOT_OPT/geraSL_TEMPO.dat" "$IN_NOT_OPT/geraSL_L3.dat" "$IN_NOT_OPT/geraSL_L2CACHE.dat" "$IN_NOT_OPT/geraSL_FLOPS_DP.dat")
SOLUCAOSL=("$IN_NOT_OPT/solucaoSL_TEMPO.dat" "$IN_NOT_OPT/solucaoSL_FLOPS_DP.dat")
RESIDUOSL=("$IN_NOT_OPT/residuoSL_TEMPO.dat" "$IN_NOT_OPT/residuoSL_L3.dat" "$IN_NOT_OPT/residuoSL_L2CACHE.dat" "$IN_NOT_OPT/residuoSL_FLOPS_DP.dat")
GERALSL_OTZ=("$IN_OPT/geraSL_OTZTEMPO.dat" "$IN_OPT/geraSL_OTZL3.dat" "$IN_OPT/geraSL_OTZL2CACHE.dat" "$IN_OPT/geraSL_OTZFLOPS_DP.dat")
SOLUCAOSL_OTZ=("$IN_OPT/solucaoSL_OTZTEMPO.dat" "$IN_OPT/solucaoSL_OTZFLOPS_DP.dat")
RESIDUOSL_OTZ=("$IN_OPT/residuoSL_OTZTEMPO.dat" "$IN_OPT/residuoSL_OTZL3.dat" "$IN_OPT/residuoSL_OTZL2CACHE.dat" "$IN_OPT/residuoSL_OTZFLOPS_DP.dat") 

# Iteração sobre os pares de arquivos 
for ((i=0; i < ${TAM}; i++))
do
    K=${METRICAS[i]}
    U=${UNIDADES[i]}
    NOME1="COMPARACAO-GERACAO-${K}"
    NOME2="COMPARACAO-SOLUCAO-${K}"
    NOME3="COMPARACAO-RESIDUO-${K}"

    # Executa o script Gnuplot para o par de arquivos
    gnuplot -c plotarGrafico.gp "${GERALSL[i]}" "${GERALSL_OTZ[i]}" "GERACAO - $K" "$NOME1" "$U" "$K"
    if [ "$K" == "TEMPO" ]; then
        gnuplot -c plotarGrafico.gp "${SOLUCAOSL[0]}" "${SOLUCAOSL_OTZ[0]}" "SOLUCAO - $K" "$NOME2" "$U" "$K"
    fi
    if [ "$K" == "FLOPS DP" ]; then
        gnuplot -c plotarGrafico.gp "${SOLUCAOSL[1]}" "${SOLUCAOSL_OTZ[1]}" "SOLUCAO - $K" "$NOME2" "$U" "$K"
    fi
    gnuplot -c plotarGrafico.gp "${RESIDUOSL[i]}" "${RESIDUOSL_OTZ[i]}" "RESIDUO - $K" "$NOME3" "$U" "$K"
done    

mv *.png $OUTPUT_DIR
