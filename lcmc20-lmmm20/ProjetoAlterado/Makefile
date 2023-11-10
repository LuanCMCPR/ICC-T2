#   Luan Carlos Maia Cruz - GRR20203891
#   Leonardo Marin Mendes Martin - GRR20205675

OBJS=libTime.o libRange.o libSL.o main.o
OUT=ajustePol
CC=gcc
LIKWID_INCLUDE = /usr/local/include/
LIKWID_LIB = /usr/local/lib/
FLAGS = -Wall -std=gnu99 -O3 -mavx -march=native
CFLAGS = -I$(LIKWID_INCLUDE) # -DLIKWID_PERFMON # Opção caso queira usar os marcadores do likwid
LFLAGS = -L$(LIKWID_LIB) -llikwid
FILES = *.c *.h *.sh LEIAME* Makefile 
DIR = lcmc20-lmmm20

all: ajustePol

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
