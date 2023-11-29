#   Luan Carlos Maia Cruz - GRR20203891
#   Leonardo Marin Mendes Martin - GRR20205675

EX1="ajustePol"
EX2="ajustePolOtimizado"
DIR1="semOtimizacao"
DIR2="Otimizado"
DIR="lcmc20-lmmm20"
FILES=*.sh *.gp $(DIR).tar.gz gera_entrada

all:
	@echo "Compilando..."
	@make -BC $(DIR1)
	@make -BC $(DIR2)
	@cp "$(DIR1)/$(EX1)" .
	@cp "$(DIR2)/$(EX2)" .
	@echo "Pronto!"

# Remove arquivos objeto, temporários e outros
clean:
	@echo "Limpando sujeira..."
	@make clean -C $(DIR1)
	@make clean -C $(DIR2)
	rm -f $(FILES)

# Remove arquivos objeto, temporários, executável e outros
purge: clean
	@echo "Faxina..."
	@make purge -C $(DIR1)
	@make purge -C $(DIR2)
	@rm -f $(EX1) $(EX2) $(FILES)

# Copia Script de Comparação
comparaTodas:
	@cp Scripts/comparaTodas.sh .
	@chmod u+x comparaTodas.sh

# Copia Script de Comparação que utiliza o gerador de entradas
comparaTodas_geraEntradas:
	@cp Scripts/gera_entrada .
	@chmod u+x gera_entrada
	@cp Scripts/comparaTodas_geraEntradas.sh .
	@chmod u+x comparaTodas_geraEntradas.sh

# Copia Script de Comparação
compara:
	@cp Scripts/compara.sh .
	@chmod u+x compara.sh

# Copia Scripts de Gerar Graficos
gerarGrafico:
	@cp Scripts/geraGraficos.sh .
	@cp Scripts/plotarGrafico.gp .
	@chmod u+x plotarGrafico.gp
	@chmod u+x geraGraficos.sh

# Copia Script de Gerar Pontos
gerarPontos:
	@cp Scripts/perfctr_pontos.sh .
	@chmod u+x perfctr_pontos.sh

# Copia Script Gerar Pontos com o gerador de entradas
pontos_geraEntradas:
	@cp Scripts/gera_entrada .
	@chmod u+x gera_entrada
	@cp Scripts/pontos_geraEntradas.sh .
	@chmod u+x pontos_geraEntradas.sh

# Copia Script de Testar Diff
diff:
	@cp Scripts/testarDiff.sh .
	@chmod u+x testarDiff.sh

# Compila com flags de debug
debug:
	@echo "Compilando com Debug..."
	@make debug -BC $(DIR1)
	@make debug -BC $(DIR2)
	@cp "$(DIR1)/$(EX1)" .
	@cp "$(DIR2)/$(EX2)" .
	@echo "Pronto!"

# Gera um arquivo compactado com os arquivos necessários para execução do programa
compact:
	@echo "Compactando arquivos..."
	tar -czvf $(DIR).tar.gz $(addprefix ../$(DIR)/, *)

# Ativa flag de debug que mostra o resultado
result:
	@echo "Compilando com Debug de Resultados..."
	@make results -BC $(DIR1)
	@make results -BC $(DIR2)
	@cp "$(DIR1)/$(EX1)" .
	@cp "$(DIR2)/$(EX2)" .
	@echo "Pronto!"

# Ativa flag de debug que mostra o resíduo
residual:
	@echo "Compilando com Debug de Resíduo..."
	@make residual -BC $(DIR1)
	@make residual -BC $(DIR2)
	@cp "$(DIR1)/$(EX1)" .
	@cp "$(DIR2)/$(EX2)" .
	@echo "Pronto!"

# Ativa flag de debug que mostra o tempo
time:
	@echo "Compilando com Debug de Tempo..."
	@make time -BC $(DIR1)
	@make time -BC $(DIR2)
	@cp "$(DIR1)/$(EX1)" .
	@cp "$(DIR2)/$(EX2)" .
	@echo "Pronto!"

# Testa em máquina Pessoal
pessoal:
	@echo "Compilando para máquina pessoal..."
	@make pessoal -BC $(DIR1)
	@make pessoal -BC $(DIR2)
	@cp "$(DIR1)/$(EX1)" .
	@cp "$(DIR2)/$(EX2)" .
	@echo "Pronto!"

# Ativa marcadores do likwid e outras flags
marcadores:
	@echo "Compilando com marcadores..."
	@make marcadores -BC $(DIR1)
	@make marcadores -BC $(DIR2)
	@cp "$(DIR1)/$(EX1)" .
	@cp "$(DIR2)/$(EX2)" .
	@echo "Pronto!"

# Ativa somente os marcadores do likwid
pontos:
	@echo "Compilando com pontos..."
	@make pontos -BC $(DIR1)
	@make pontos -BC $(DIR2)
	@cp "$(DIR1)/$(EX1)" .
	@cp "$(DIR2)/$(EX2)" .
	@echo "Pronto!"