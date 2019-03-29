# PROJECTS NAMES
EXEC = prog

# COMPILER
CC = gcc
# Compilation Options
C_FLAGS = -L/home/Downloads/gurobi801/linux64/lib -I/home/Downloads/gurobi801/linux64/include -lgurobi80 -lm -Wall -ansi -pedantic -Wextra -g -O2
# Compilation Libs
LIBS =
# Libs Options
L_FLAGS =

# All sources files
MAIN = main.c
SOURCES = $(wildcard src/*.c)

# All objects files from sources
OBJ = $(SOURCES:.c=.o) $(MAIN:.c=.o)


###   BUILDS   ####
all: $(EXEC)

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) $(LIBS) $(L_FLAGS) -o $@

clean:
	rm -f $(OBJ)

# Build object file
matrice.o: include/matrice.h src/matrice.c
	$(CC) -c matrice.c $(C_FLAGS)
generer_entree.o: include/generer_entree.h src/generer_entree.c
	$(CC) -c generer_entree.c $(C_FLAGS)
GUROBI.o: include/matrice.h include/gurobi_c.h include/GUROBI.h src/GUROBI.c
	$(CC) -c GUROBI.c $(C_FLAGS)
main.o : include/GUROBI.h include/generer_entree.h main.c
	$(CC) -c main.c $(C_FLAGS)
