INC = -I/home/antony/Bureau/gurobi810/linux64/include
LIB = -L/home/antony/Bureau/gurobi810/linux64/lib -lgurobi81 
FLAGS = ${INC} ${LIB} -lm -g -Wall -Wextra -ansi -pedantic -O2

prog: main.o matrice.o GUROBI.o
	gcc -o prog main.o matrice.o GUROBI.o ${FLAGS}

main.o: main.c 
	gcc -o main.o -c main.c ${FLAGS}

matrice.o: src/matrice.c
	gcc -o matrice.o -c src/matrice.c ${FLAGS}

GUROBI.o : src/GUROBI.c
	gcc -o GUROBI.o -c src/GUROBI.c ${FLAGS}
