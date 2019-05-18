#ifndef __MATRICE_H__
#define __MATRICE_H__


/*[INCLUDES]*/
/*Librairies*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*-------------------------------------------------------------------------------------------------------*/
/* Structure qui regroupe l'ensemble des données d'entrée utiles */

/* choix : 
le tableau poids contiendra sur chaque ligne le numéro du binome suivi de ses choix de projets */

/* correspondance : 
le tableau correspondance fera correspondre à chaque N° de projet un N° entre 1 et nbProjet (plus pratique pour utiliser Gurobi ensuite) */

typedef struct donneesEntree
{
    int   nbBinome;
    int   nbProjet;
    int   nbChoix;
    int  *poids;
    int **choix;
    int **res;
    int **correspondance;
} Datas_t;


/*-------------------------------------------------------------------------------------------------------*/


/*[PROTOTYPES]*/

Datas_t* FichierEnStructureDatas(FILE *fic1, FILE *fic2);
int** StructureEnMatrice(Datas_t *Datas);
int NumProjet_BinomeChoix(Datas_t *Datas, int numBinome, int numChoix);
int NumChoix_BinomeProjet(Datas_t *Datas, int numBinome, int numProjet);
void CreationFichierDeSortie(Datas_t *Datas);


#endif