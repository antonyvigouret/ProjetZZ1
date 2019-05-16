/*Librairies*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Fichiers*/
#include "include/GUROBI.h"




/*------------------------------------------------------------------------------*/ 
/* Donner en argument le fichier d'affectation et le fichier contenant la liste des projets */


int main(int argc, char * argv[]) 
{
    if(argc != 3)
        fprintf(stderr, "ERREUR : Nombre n'arguments incorrect\n");
    else 
    {
        LancementGurobi(argv[1], argv[2]);
    }

  return 0;
}

/*------------------------------------------------------------------------------*/