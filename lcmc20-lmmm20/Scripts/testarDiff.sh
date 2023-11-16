#!/bin/bash

# Luan Carlos Maia Cruz - GRR20203891
# Leonardo Marin Mendes Martin - GRR20205675

# Uso: ./testarDiff.sh

EX1="ajustePol"
EX2="ajustePolOtimizado"
D1="semOtimizacao"
D2="Otimizado"
CORE=3


PONTOS="64 100 128 200 256 512 600 900 1024 2000 3000 4096 6000 7000 10000 50000 $((10**5)) $((10**6)) $((10**7)) $((10**8))"


make debug -sBC $D1
cp "$D1/$EX1" ./

make debug -sBC $D2
cp "$D2/$EX2" ./

echo "performance" > /sys/devices/system/cpu/cpufreq/policy${CORE}/scaling_governor

for i in ${PONTOS}
do
    echo "Testando $i"
    "./$EX1" < $i >> res1.txt
    "./$EX2" < $i >> res2.txt
done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy${CORE}/scaling_governor

diff res1.txt res2.txt
rm  ers1.txt res2.txt