#!/bin/bash

# Uso: ./pontos_geraEntrada.sh <executavel> <diretorio de saida>

# Testa se possui argumentos
if [ $# -ne 2 ]
then
    echo "Uso: ./pontos_geraEntrada.sh <executavel> <diretorio de saida>"
    exit 1
fi

# Metrica e Pontos a serem avaliados
METRICA="L2CACHE L3 FLOPS_DP"
TAM="64 100 128 200 256 512 600 900 1024 2000 3000 4096 6000 7000 10000 50000 $((10**5)) $((10**6)) $((10**7)) $((10**8))"
T="TEMPO"
CORE=3
EXEC=$1
D1="semOtimizacao"
D2="Otimizado"
EXE="ajustePol"
ENTRADA="gera_entrada"



# Gera arquivo com nomes diferentes, para facilitar a criacao dos graficos
# flag o é para gerar os arquivos de entrada do programa otimizado
# Compila o programa sem os marcadores, para obter o tempo de execucao
if [ ${EXEC} == "./${EXE}Otimizado" ]; then
	make time -sBC $D2
	cp "$D2/$EXEC" ./
	F1="geraSL_OTZ"
	F2="solucaoSL_OTZ"
	F3="residuoSL_OTZ"
	DIR=$D2
elif [ ${EXEC} == "./${EXE}" ]; then
	make time -sBC $D1
	cp "$D1/$EXEC" ./
	F1="geraSL_"
	F2="solucaoSL_"
	F3="residuoSL_"
	DIR=$D1
else
	echo "Executavel invalido: ${EXEC}"
	echo "Executaveis: ./${EXE} e ./${EXE}Otimizado"
	exit 1
fi

echo "performance" > /sys/devices/system/cpu/cpufreq/policy$CORE/scaling_governor

touch $F1$T.dat
touch $F2$T.dat
touch $F3$T.dat

echo "Métrica: TEMPO"
for p in $TAM
do
	echo "	Testando ${p}"
	echo -n "$p " >> $F1$T.dat
	echo -n "$p " >> $F2$T.dat
	echo -n "$p " >> $F3$T.dat
	TEMPOS=$(./${ENTRADA} $p | $EXEC)
	echo $TEMPOS | awk '{print $1}' >> $F1$T.dat
	echo $TEMPOS | awk '{print $2}' >> $F2$T.dat
	echo $TEMPOS | awk '{print $3}' >> $F3$T.dat
done

# Compila o programa com os marcadores do likwid, para poder obter as metricas
# make CFLAGS="-I$LIKWID_INCLUDE -DLIKWID_PERFMON" -sBC $DIR
make pontos -sBC $DIR
cp "$DIR/$EXEC" ./

for k in $METRICA	
do
	echo "Métrica: ${k}"
	touch $F1$k.dat
	if [ "$k" == "FLOPS_DP" ]
	then
		touch $F2$k.dat
	fi
	touch $F3$k.dat
	for p in $TAM
	do	
		echo "	Testando ${p}"
		echo -n "$p " >> $F1$k.dat
		if [ "$k" == "FLOPS_DP" ]
		then
			echo -n "$p " >> $F2$k.dat
		fi
		echo -n "$p " >> $F3$k.dat
     
		./${ENTRADA} $p | likwid-perfctr -C $CORE -g $k -m -o "$k$p.csv" "$EXEC" > /dev/null

		# Separa os dados para o group FLOPS_DP	
    	if [ "$k" == "FLOPS_DP" ]	
    	then
			AUX=$(grep -i "MFLOP/s" "$k$p.csv" | awk -F, '{print $2}')
        	echo -n $AUX | awk '{print $1}' | tr '\n' ' ' >> $F1$k.dat
			echo -n $AUX | awk '{print $2}' >> $F1$k.dat
			echo -n $AUX | awk '{print $3}' | tr '\n' ' ' >> $F2$k.dat
			echo -n $AUX | awk '{print $4}' >> $F2$k.dat
			echo -n $AUX | awk '{print $5}' | tr '\n' ' ' >> $F3$k.dat
			echo -n $AUX | awk '{print $6}' >> $F3$k.dat
    	fi

    	# Separa os dados para o group ENERGY
    	# if [ "$k" == "ENERGY" ]
    	# then
		# 	AUX=$(grep -i "ENERGY \[J\]" "$k.csv" | awk -F, '{print $2}')
        # 	echo -n $AUX | awk '{print $1}' >> $F1$k.dat
		# 	echo -n $AUX | awk '{print $2}' >> $F2$k.dat
		# 	echo -n $AUX | awk '{print $3}' >> $F3$k.dat  
    	# fi		
		
		# Separa os dados para o group L3
    	if [ "$k" == "L3" ]
    	then
			AUX=$(grep -i "L3 bandwidth" "$k$p.csv" | awk -F, '{print $2}')
			echo -n $AUX | awk '{print $1}' >> $F1$k.dat
			echo -n $AUX | awk '{print $3}' >> $F3$k.dat
	   	fi		
		
		# Separa os dados para o group L2CACHE
    	if [ "$k" == "L2CACHE" ]
    	then
			AUX=$(grep -i "L2 miss ratio" "$k$p.csv" | awk -F, '{print $2}')
			echo -n $AUX | awk '{print $1}' >> $F1$k.dat
			echo -n $AUX | awk '{print $3}' >> $F3$k.dat
	   	fi
	done
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy$CORE/scaling_governor

DIR=$2
# Move arquivos para o diretório de saída
mkdir -p $DIR
rm *.csv
mv *.dat $DIR