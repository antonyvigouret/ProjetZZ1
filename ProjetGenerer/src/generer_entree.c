/*[INCLUDES]*/
/*Fichiers*/
#include "../include/generer_entree.h"



/*-------------------------------------------------------------------------------------------------------*/


/*[Génère automatiquement un fichier d'entree exploitable par la suite pour gurobi]
Entree : 
    - nbBinome (int) : Nombre de binôme souhaité dans le fichier
    - nbProjet (int) : Nombre de projet souhaité dans le fichier
    - nbChoix (int)  : Nombre de choix laissé à chaque binôme

Sortie : 
    - void
*/

void GenerateurDonneesEntree(int nbBinome, int nbProjet, int nbChoix)
{
    int i, j;
    FILE *fic1;
    FILE *fic2;
    
    srand(time(0));
    fic1 = fopen("TestGen1.txt", "w"); /*Création du fichier des choix*/
    
    fprintf(fic1, "%d %d %d\n", nbBinome, nbProjet, nbChoix); /*Ecriture première ligne*/

    for (i = 0; i < nbChoix; i++) /*Ecriture de la 2ieme ligne = poids allant de 1 à nbChoix*/
    {
        fprintf(fic1, "%d ", i+1);
    }
    fprintf(fic1, "\n");

    for (i = 0; i < nbBinome; i++) /*Ecriture de 'nbBinome' lignes avec 'nbProjet' colonnes*/ 
    {
        fprintf(fic1, "%d ", i+1);
        for (j = 0; j < nbChoix; j++)
        {
            fprintf(fic1, "%d ", rand()%nbProjet+1); /*Chaque coefficient est donné de facon aleatoire entre 1 et 'nbProjet'*/
        }
        fprintf(fic1, "\n"); /*Changement de ligne*/
    }

    fclose(fic1);

    fic2 = fopen("TestGen2.txt", "w");
    for (i = 0; i < nbProjet; i++)
    {
      fprintf(fic2, "%d ", i+1);
    }

    fclose(fic2);
}