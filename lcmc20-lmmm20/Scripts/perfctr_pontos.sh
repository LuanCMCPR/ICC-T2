#!/bin/bash

# Uso: ./perfect_pontos.sh <executavel> <diretorio de saida>

# Testa se possui argumentos
if [ $# -ne 2 ]
then
    echo "Uso: ./perfect_pontos.sh <executavel> <diretorio de saida>"
    exit 1
fi

# Metrica e Pontos a serem avaliados
METRICA="L2CACHE L3 ENERGY FLOPS_DP"
TAM="64 100 128 200 256 512 600 900 1024 2000 2048 3000 4000"
T="TEMPO"
CORE=3
EXEC=$1
D1="semOtimizacao"
D2="Otimizado"


# Gera arquivo com nomes diferentes, para facilitar a criacao dos graficos
# flag o é para gerar os arquivos de entrada do programa otimizado
if [ ${EXEC} == "./matmultOtimizado" ]; then
	make -sBC $D2
	cp "$D2/$EXEC" ./
	F1="multmatVetOTZ"
	F2="multmatMatOTZ"
	DIR=$D2
elif [ ${EXEC} == "./matmult" ]; then
	make -sBC $D1
	cp "$D1/$EXEC" ./
	F1="multmatVet"
	F2="multmatMat"
	DIR=$D1
else
	echo "Executavel invalido: ${EXEC}"
	echo "Executaveis: ./matmult e ./matmultOtimizado"
	exit 1
fi

echo "performance" > /sys/devices/system/cpu/cpufreq/policy$CORE/scaling_governor
# Compila o programa sem os marcadores, para obter o tempo de execucao

touch $F1$T.dat
touch $F2$T.dat

echo "Métrica: TEMPO"
for p in $TAM
do
	echo "	Testando ${p}"
	echo -n "$p " >> $F1$T.dat
	echo -n "$p " >> $F2$T.dat
	TEMPOS=$($EXEC $p)
	echo $TEMPOS | awk '{print $1}' >> $F1$T.dat
	echo $TEMPOS | awk '{print $2}' >> $F2$T.dat	
done

# Compila o programa com os marcadores do likwid, para poder obter as metricas
make CFLAGS="-I$LIKWID_INCLUDE -DLIKWID_PERFMON" -sBC $DIR
cp "$DIR/$EXEC" ./

for k in $METRICA	
do
	echo "Métrica: ${k}"
	touch $F1$k.dat
	touch $F2$k.dat
	for p in $TAM
	do	
		echo "	Testando ${p}"
		echo -n "$p " >> $F1$k.dat
		echo -n "$p " >> $F2$k.dat
     
		likwid-perfctr -C $CORE -g $k -m -o "$k.csv" $EXEC $p > /dev/null

		# Separa os dados para o group FLOPS_DP	
    	if [ "$k" == "FLOPS_DP" ]	
    	then
			AUX=$(grep -i "MFLOP/s" "$k.csv" | awk -F, '{print $2}')
        	echo -n $AUX | awk '{print $1}' | tr '\n' ' ' >> $F1$k.dat
			echo -n $AUX | awk '{print $2}' >> $F1$k.dat
			echo -n $AUX | awk '{print $3}' | tr '\n' ' ' >> $F2$k.dat
			echo -n $AUX | awk '{print $4}' >> $F2$k.dat
    	fi

    	# Separa os dados para o group ENERGY
    	if [ "$k" == "ENERGY" ]
    	then
			AUX=$(grep -i "ENERGY \[J\]" "$k.csv" | awk -F, '{print $2}')
        	echo -n $AUX | awk '{print $1}' >> $F1$k.dat
			echo -n $AUX | awk '{print $2}' >> $F2$k.dat  
    	fi		
		
		# Separa os dados para o group L3
    	if [ "$k" == "L3" ]
    	then
			AUX=$(grep -i "L3 bandwidth" "$k.csv" | awk -F, '{print $2}')
			echo -n $AUX | awk '{print $1}' >> $F1$k.dat
			echo -n $AUX | awk '{print $2}' >> $F2$k.dat
	   	fi		
		

    	if [ "$k" == "L2CACHE" ]
    	then
			AUX=$(grep -i "L2 miss ratio" "$k.csv" | awk -F, '{print $2}')
			echo -n $AUX | awk '{print $1}' >> $F1$k.dat
			echo -n $AUX | awk '{print $2}' >> $F2$k.dat
	   	fi
	done
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy$CORE/scaling_governor

DIR=$2
# Move arquivos para o diretório de saída
mkdir -p $DIR
mv *.csv $DIR
mv *.dat $DIR