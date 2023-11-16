#!/bin/bash

# Uso: ./geraEntradas.sh <diretorio de saida>

# Testa se possui argumentos
if [ $# -ne 1 ]
then
    echo "Uso: ./geraEntradas.sh <diretorio de saida>"
    exit 1
fi

# Pontos a serem avaliados
TAM=(64 100 128 200 256 512 600 900 1024 2000 3000 4096 6000 7000 10000 50000 $((10**5)) $((10**6)) $((10**7)) $((10**8)))
DIR=$1
EXEC="gera_entrada"

cp Scripts/$EXEC.c ./
make -sB $EXEC

for k in "${TAM[@]}"
do
    echo "Gerando entrada de tamanho ${k}"
    ./${EXEC} ${k} > $ENTRADA$k.in

done


mkdir -p $DIR
mv *.in $DIR
rm $EXEC $EXEC.c