#!/bin/bash

# Luan Carlos Maia Cruz - GRR20203891
# Leonardo Marin Mendes Martin - GRR20205675 

# Uso: ./comparaTodas <ponto>

if [ $# -ne 1 ]
then
	echo "Uso: ./comparaTodas <ponto>"
	exit 1
fi

METRICA="L2CACHE L3 ENERGY FLOPS_DP"
CORE=3  
P=$1
D1="semOtimizacao"
D2="Otimizado"
EX1="matmult"
EX2="matmultOtimizado"


echo "performance" > /sys/devices/system/cpu/cpufreq/policy${CORE}/scaling_governor

	
	make -sBC $D1
	cp "$D1/$EX1" .

	make -sBC $D2
	cp "$D2/$EX2" .

	T1=$("./$EX1" $P)
	T2=$("./$EX2" $P)

	echo -e "TEMPO"
	echo $D1
	echo -n $T1 | awk '{print $1}'
	echo -n $T1 | awk '{print $2}'

	echo $D2
	echo -n $T2 | awk '{print $1}'
	echo -n $T2 | awk '{print $2}'

	make -sB CFLAGS="-I$LIKWID_INCLUDE -DLIKWID_PERFMON" -C $D1
	cp "$D1/$EX1" .

	make -sB CFLAGS="-I$LIKWID_INCLUDE -DLIKWID_PERFMON" -C $D2
	cp "$D2/$EX2" .


    for M in ${METRICA}
    do
		D=$D1
	    EX="./$EX1"
	    echo -e "\n${M}"
	    for i in {1..2}
		do  
			echo ${D}
			likwid-perfctr -C $CORE -g $M -m -o "$M.csv" $EX $P > /dev/null

			# Separa os dados para o group FLOPS_DP	
    		if [ "$M" == "FLOPS_DP" ]	
    		then
				AUX=$(grep -i "MFLOP/s" "$M.csv" | awk -F, '{print $2}')
        		echo -n $AUX | awk '{print $1}' | tr '\n' ' '
				echo -n $AUX | awk '{print $2}'
				echo -n $AUX | awk '{print $3}' | tr '\n' ' '
				echo -n $AUX | awk '{print $4}'
    		fi

    		# Separa os dados para o group ENERGY
    		if [ "$M" == "ENERGY" ]
    		then
				AUX=$(grep -i "ENERGY \[J\]" "$M.csv" | awk -F, '{print $2}')
        		echo -n $AUX | awk '{print $1}'
				echo -n $AUX | awk '{print $2}'  
    		fi		

			# Separa os dados para o group L3
    		if [ "$M" == "L3" ]
    		then
				AUX=$(grep -i "L3 bandwidth" "$M.csv" | awk -F, '{print $2}')
				echo -n $AUX | awk '{print $1}'
				echo -n $AUX | awk '{print $2}'
	   		fi		

			# Separa os dados para o group L2CACHE
    		if [ "$M" == "L2CACHE" ]
    		then
				AUX=$(grep -i "L2 miss ratio" "$M.csv" | awk -F, '{print $2}')
				echo -n $AUX | awk '{print $1}'
				echo -n $AUX | awk '{print $2}'
	   		fi

			D=$D2
			EX="./$EX2"
			rm ${M}.csv
		done
	done		
echo "powersave" > /sys/devices/system/cpu/cpufreq/policy${CORE}/scaling_governor