#   Luan Carlos Maia Cruz - GRR20203891
#   Leonardo Marin Mendes Martin - GRR20205675

OBJS=libTime.o libRange.o libSL.o main.o
OUT=ajustePolOtimizado
CC=gcc
# LIKWID_INCLUDE = /usr/local/include/ # Máquina Pessoal
# LIKWID_LIB = /usr/local/lib/ # Máquina Pessoal
LIKWID_INCLUDE = /home/soft/likwid/include # Máquina DINF
LIKWID_LIB = /home/soft/likwid/lib # Máquina DINF
FLAGS = -Wall -g -std=gnu99 -O3 -mavx -march=native
CFLAGS = -I$(LIKWID_INCLUDE) # -DLIKWID_PERFMON # Opção caso queira usar os marcadores do likwid
LFLAGS = -L$(LIKWID_LIB) -llikwid
FILES = *.c *.h *.sh LEIAME* Makefile 
DIR = lcmc20-lmmm20

all: FLAGS += -DTIME -DRESULT -DRESIDUAL
all: ajustePol

marcadores: CFLAGS += -DLIKWID_PERFMON
marcadores: all

pontos: CFLAGS += -DLIKWID_PERFMON
pontos: ajustePol

ajustePol: $(OBJS)
	$(CC) $(FLAGS) $(CFLAGS) $(OBJS) -o $(OUT) $(LFLAGS) -lm

libTime.o: libTime.c libTime.h
	$(CC) $(FLAGS) -c libTime.c

libRange.o: libRange.c libRange.h
	$(CC) $(FLAGS) -c libRange.c

libSL.o: libSL.c libSL.h
	$(CC) $(FLAGS) -c libSL.c

main.o: main.c
	$(CC) $(FLAGS) $(CFLAGS) -c main.c

# Remove arquivos objeto, temporários e executável
clean:
	@echo "Removendo sujeira..."
	rm -f *.o *~

purge: clean
	rm -f $(OUT)

# Gera um arquivo compactado com os arquivos necessários para execução do programa
compact:
	@echo "Compactando arquivos..."
	tar -czvf $(DIR).tar.gz $(addprefix ../$(DIR)/, $(FILES))

# Ativa flags de debug
debug: FLAGS += -DRESULT -DRESIDUAL
debug: ajustePol

# Ativa flag de debug que mostra o resultado
results: FLAGS += -DRESULT
results: ajustePol

# Ativa flag de debug que mostra o residuo
residual: FLAGS += -DRESIDUAL
residual: ajustePol

# Ativa flag de debug que mostra o tempo de execução
time: FLAGS += -DTIME
time: ajustePol

# Teste na máquina Pessoal
pessoal:
	$(eval LIKWID_INCLUDE = /usr/local/include/) # Máquina Pessoal
	$(eval LIKWID_LIB = /usr/local/lib/) # Máquina Pessoal
pessoal: marcadores