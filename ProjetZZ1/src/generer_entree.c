/*[INCLUDES]*/
/*Fichiers*/
#include "../include/generer_entree.h"




/*-------------------------------------------------------------------------------------------------------*/
/*Savoir si la chaine de charactère est un nombre*/


Bool isNumber(char* p_str)
{
  int i = 0;
  int length = strlen(p_str);
  Bool check = true;

  /* Supprime le /n a la fin de la chaine */
  if (p_str[length - 1] == '\n') {
       p_str[length - 1] = '\0';
       length--;
  }

  while(check && i < length)
  {
    if(p_str[i] < '0' || p_str[i] > '9')
    {
      check = false;
    }

    i++;
  }

  return check;
}


/*-------------------------------------------------------------------------------------------------------*/


/*[Génère automatiquement un fichier d'entree exploitble par la suite pour gurobi]
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
    srand(time(0));
    FILE *fic = fopen("Test_G.txt", "w"); /*Création du fichier*/
    
    fprintf(fic, "%d %d %d\n", nbBinome, nbProjet, nbChoix); /*Ecriture première ligne*/

    for (i = 0; i < nbChoix; i++) /*Ecriture de la 2ieme ligne = poids allant de 1 à nbChoix*/
    {
        fprintf(fic, "%d ", i+1);
    }
    fprintf(fic, "\n");

    for (i = 0; i < nbBinome; i++) /*Ecriture de 'nbBinome' lignes avec 'nbProjet' colonnes*/ 
    {
        fprintf(fic, "%d ", i+1);
        for (j = 0; j < nbChoix; j++)
        {
            fprintf(fic, "%d ", rand()%nbProjet+1); /*Chaque coefficient est donné de facon aleatoire entre 1 et 'nbProjet'*/
        }
        fprintf(fic, "\n"); /*Changement de ligne*/
    }

    fclose(fic);
}