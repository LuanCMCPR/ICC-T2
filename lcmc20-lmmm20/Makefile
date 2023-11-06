#   Luan Carlos Maia Cruz - GRR20203891
#   Leonardo Marin Mendes Martin - GRR20205675

EX1="ajustePol"
EX2="ajustePolOtimizado"
DIR1="semOtimizacao"
DIR2="Otimizado"
DIR="lcmc20-lmmm20"
FILES=*.sh *.gp $(DIR).tar.gz

all:
	@echo "Compilando..."
	@make -sBC $(DIR1)
	@make -sBC $(DIR2)
	@cp "$(DIR1)/$(EX1)" .
	@cp "$(DIR2)/$(EX2)" .
	@echo "Pronto!"

# Compila com marcadores do likwid
markers:
	@echo "Compilando com marcadores..."
	@make CFLAGS="-I$LIKWID_INCLUDE -DLIKWID_PERFMON" -sBC $(DIR1)
	@make CFLAGS="-I$LIKWID_INCLUDE -DLIKWID_PERFMON" -sBC $(DIR2)
	@cp "$(DIR1)/$(EX1)" .
	@cp "$(DIR2)/$(EX2)" .
	@echo "Pronto!"

# Remove arquivos objeto, temporários e outros
clean:
	@echo "Limpando sujeira..."
	@make clean -sC $(DIR1)
	@make clean -sC $(DIR2)
	rm -f $(FILES)

# Remove arquivos objeto, temporários, executável e outros
purge: clean
	@echo "Faxina..."
	@make purge -sC $(DIR1)
	@make purge -sC $(DIR2)
	@rm -f $(EX1) $(EX2) $(FILES)

# Copia Script de Comparação
comparaTodas:
	@cp Scripts/comparaTodas.sh .
	@chmod u+x comparaTodas.sh

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

# Copia Script de Testar Diff
diff:
	@cp Scripts/testarDiff.sh .
	@chmod u+x testarDiff.sh

# Compila com flags de debug
debug:
	@echo "Compilando com Debug..."
	@make debug -sBC $(DIR1)
	@make debug -sBC $(DIR2)
	@cp "$(DIR1)/$(EX1)" .
	@cp "$(DIR2)/$(EX2)" .
	@echo "Pronto!"

# Gera um arquivo compactado com os arquivos necessários para execução do programa
compact:
	@echo "Compactando arquivos..."
	tar -czvf $(DIR).tar.gz $(addprefix ../$(DIR)/, *)

# Ativa flags de debug que mostram o resultado
results:
	@echo "Compilando com Debub de Resultados..."
	@make results -sBC $(DIR1)
	@make results -sBC $(DIR2)
	@cp "$(DIR1)/$(EX1)" .
	@cp "$(DIR2)/$(EX2)" .
	@echo "Pronto!"