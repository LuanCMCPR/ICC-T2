#!/bin/bash

# Luan Carlos Maia Cruz - 20203891
# Leonardo Marin Mendes Martin - GRR20205675

# Modo de usar este script: ./ajustePol.sh <arquivo de entrada>

# Verifica se o numero de argumentos é igual a 1
if [ $# -eq 0 ]; then
    echo "Uso: $0 <arquivo_entrada>"
    exit 1
fi

# Variaveis para melhorar legibilidade
EXECUTAVEL="./ajustePol" #Nome do executavel
INPUT="$1" #Arquivo de entrada
CPU=3 #CPU a ser analisada
METRICA="FLOPS_DP ENERGY" #Metricas a serem analisadas
NUM_VEZES=6
NUM_CARAC=20

# Imprime um titulo com o caractere #, x vezes e o texto cetranlizado
imprimir_titulo() 
{
    local num_vezes=$1
    local texto=$2

    for ((i=1; i<=$num_vezes; i++)); do
        echo -n "- "
    done

    echo -n "$texto " 

    for ((i=1; i<=$num_vezes; i++)); do
        echo -n "- "
    done

    echo
}

# Imprime uma linha com o caractere #, x vezes
imprimir_linha() 
{
    local num_vezes=$1

    for ((i=1; i<=$num_vezes; i++)); do
        echo -n "- "
    done

    echo -e "\n"
}

make > /dev/null
$EXECUTAVEL < ${INPUT}

# Adiciona a flag de perfmon no arquivo Makefile e compila o programa
# Flag -B é para forçar a compilação, para evitar que não seja atualizado - "Nothing to be done for 'all'"
# Redireciona para /dev/null para não poluir a tela
#make -B CFLAGS="-DLIKWID_PERFMON" > /dev/null # Máquina de Testes
make -B CFLAGS="-I$LIKWID_INCLUDE -DLIKWID_PERFMON" > /dev/null # DINF 

# Aumenta a frequência do processador
echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

# Gera os arquivos de log e filtra dados
for k in ${METRICA}
do

    # Gera os arquivos de logs
    likwid-perfctr -C ${CPU} -g ${k} -m ${EXECUTAVEL} < ${INPUT} > ${k}.log

    # Separa os dados para o group FLOPS_DP
    if [ "$k" == "FLOPS_DP" ]
    then
        imprimir_titulo ${NUM_VEZES} "MÉTRICA FLOPS_DP"

        #cat ${k}.log | grep -i "Group\| DP \[MFLOP/s\]" | sed 's/,.*//' | sed 's/|//g' # Máquina de Testes
        cat ${k}.log | grep -i "Group\| DP \[MFLOP/s\]" | sed 's/,.*//' | sed 's/|//g' # DINF 
        
        imprimir_linha ${NUM_CARAC}
    fi

    # Separar os dados para o group ENERGY
    if [ "$k" == "ENERGY" ]
    then
        imprimir_titulo ${NUM_VEZES} "MÉTRICA ENERGY"
        cat ${k}.log | grep -i "Group\| ENERGY \[J\]" | sed 's/,.*//' | sed 's/|//g' # Máquina de Testes e DINF
        imprimir_linha ${NUM_CARAC}
    fi

    # Elimina os arquivos de logs
    rm ${k}.log
done

# Remove arquivos objeto, temporários e executavel
# Redireciona para /dev/null para não poluir a tela
make clean > /dev/null

# Diminui a frequência do processador
echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor