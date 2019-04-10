#ifndef __MATRICE_H__
#define __MATRICE_H__


/*[INCLUDES]*/
/*Librairies*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*-------------------------------------------------------------------------------------------------------*/
/*Structure qui regroupe l'ensemble des données d'entrée utiles*/

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

Datas_t* FichierEnStructureDatas(FILE *fic);
int** StructureEnMatrice(Datas_t *Datas);
int NumProjet_BinomeChoix(Datas_t *Datas, int numBinome, int numChoix);
int NumChoix_BinomeProjet(Datas_t *Datas, int numBinome, int numProjet);
void CreationFichierDeSortie(double *sol, Datas_t *Datas);


#endif
