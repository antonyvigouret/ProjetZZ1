# Pour trouver le chemin vers gurobi il faut utiliser : whereis gurobi.sh
INC = -I/opt/gurobi/gurobi810/linux64/include
LIB = -L/opt/gurobi/gurobi810/linux64/lib -lgurobi81 
FLAGS = ${INC} ${LIB} -lm -g -Wall -Wextra -ansi -pedantic -O2

prog: main.o matrice.o GUROBI.o
	gcc -o prog edition/main.o edition/matrice.o edition/GUROBI.o ${FLAGS}

main.o: main.c 
	gcc -o edition/main.o -c main.c ${FLAGS}

matrice.o: src/matrice.c
	gcc -o edition/matrice.o -c src/matrice.c ${FLAGS}

GUROBI.o : src/GUROBI.c
	gcc -o edition/GUROBI.o -c src/GUROBI.c ${FLAGS}
	
clean :
	rm -f edition/*.o
