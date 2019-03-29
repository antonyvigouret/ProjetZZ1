FLAGS = -I/home/antony/Bureau/gurobi810/linux64/include -L/home/antony/Bureau/gurobi810/linux64/lib -lgurobi81 -lm -g -Wall -Wextra -ansi -pedantic

prog: main.o matrice.o generer_entree.o GUROBI.o
	gcc -o prog main.o matrice.o generer_entree.o GUROBI.o ${FLAGS}

main.o: main.c 
	gcc -o main.o -c main.c ${FLAGS}

matrice.o: src/matrice.c
	gcc -o matrice.o -c src/matrice.c ${FLAGS}

generer_entree.o : src/generer_entree.c
	gcc -o generer_entree.o -c src/generer_entree.c ${FLAGS}

GUROBI.o : src/GUROBI.c
	gcc -o GUROBI.o -c src/GUROBI.c ${FLAGS}
