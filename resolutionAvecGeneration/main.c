/*Librairies*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Fichiers*/
#include "include/GUROBI.h"
#include "include/generer_entree.h"




/*------------------------------------------------------------------------------*/ 
/*
   ./prog : lancement de la génération d'un fichier d'entrée en demandant dans un premier temps
        - donnez
            nbBinome =
            nbProjet = 
            nbChoix =
        Puis lancement de gurobi

*/


int main(int argc, char * argv[]) 
{
   LancementGurobi2();

  return 0;
}

/*------------------------------------------------------------------------------*/