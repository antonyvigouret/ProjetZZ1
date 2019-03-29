/*Librairies*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Fichiers*/
#include "include/GUROBI.h"
#include "include/generer_entree.h"




/*------------------------------------------------------------------------------*/ 
/* ./prog 1 : lancement de gurobi avec le fichier d'entrée "entree.txt"
   ./prog 2 : lancement de la génération d'un fichier d'entrée en demandant dans un premier temps
        - donnez
            nbBinome =
            nbProjet = 
            nbChoix =
        Puis lancement de gurobi

*/


int main(int argc, char * argv[]) 
{

    if(argc != 2)
        fprintf(stderr, "ERREUR : Trop d'arguments donnés");
    else 
    {
        if(!isNumber(argv[1]))
            fprintf("ERREUR : Argument n'est pas un entier");
        else 
        {
            switch(atoi(argv[1])) 
            {
                case 1 :
                    LancementGurobi1("Test_NonG.txt");
                    break;
                case 2 :
                    LancementGurobi2();
                    break;
                default :
                    break;
            }
        }
    }

  return 0;
}

/*------------------------------------------------------------------------------*/