FLAGS = -I/home/antony/Bureau/gurobi810/linux64/include -L/home/antony/Bureau/gurobi810/linux64/lib -lgurobi81 -lm -g -Wall -Wextra -ansi -pedantic -O2

prog: main.o matrice.o generer_entree.o GUROBI.o
	gcc -o prog edition/main.o edition/matrice.o edition/generer_entree.o edition/GUROBI.o ${FLAGS}

main.o: main.c 
	gcc -o edition/main.o -c main.c ${FLAGS}

matrice.o: src/matrice.c
	gcc -o edition/matrice.o -c src/matrice.c ${FLAGS}

generer_entree.o : src/generer_entree.c
	gcc -o edition/generer_entree.o -c src/generer_entree.c ${FLAGS}

GUROBI.o : src/GUROBI.c
	gcc -o edition/GUROBI.o -c src/GUROBI.c ${FLAGS}
	
clean :
	rm -f edition/*.o
