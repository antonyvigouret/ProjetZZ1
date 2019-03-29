/*[INCLUDES]*/
/*Ficher*/
#include "matrice.h"


/*-------------------------------------------------------------------------------------------------------*/
/*[Affecter les données contenues dans le fichier d'entrée dans une structure]
Entree : 
    - fil (FILE *) : Pointeur du fichier d'entrée qui contient l'ensemble des données d'entrée

Sortie : 
    - Datas (Datas_t *) : Pointeur vers Datas_t
*/

Datas_t* FichierEnStructureDatas(FILE *fic)
{
    int i, j;
    Datas_t *Datas = malloc(sizeof(Datas_t));

    if(Datas == NULL)
    {
        fprintf(stderr, "Problème allocation d'un Datas_t *\n");
        exit(EXIT_FAILURE);
    }


    fscanf(fic, "%d %d %d", &(Datas->nbBinome), &(Datas->nbProjet), &(Datas->nbChoix)); /*Lecture première ligne du fichier*/

    Datas->poids = malloc(sizeof(int)*(Datas->nbChoix));
    if(Datas->poids == NULL)
    {
        fprintf(stderr, "Problème allocation\n");
        exit(EXIT_FAILURE);
    }

    /*Récupération des poids suivant l'importance du choix de projet*/
    for (i = 0; i < Datas->nbChoix; i++)
    {
        fscanf(fic, "%d", &(Datas->poids[i]));
    }

    Datas->choix = malloc(sizeof(int *)*(Datas->nbBinome));
    if(Datas->choix == NULL)
    {
        fprintf(stderr, "Problème allocation\n");
        exit(EXIT_FAILURE);
    }

    /*Récupération des choix de projet(j) pour chaque binôme(i)*/
    for (i = 0; i < Datas->nbBinome; i++)
    {
        Datas->choix[i] = malloc(sizeof(int)*(Datas->nbChoix+1));
        for (j = 0; j < Datas->nbChoix + 1; j++)
        {
            fscanf(fic, "%d", &(Datas->choix[i][j]));   
        }
    }


    Datas->res = malloc(sizeof(int *)*Datas->nbBinome);

    return Datas;
}


/*-------------------------------------------------------------------------------------------------------*/
/*[/*Création de la matrice d'entrée ( pour gurobi) à partir de la structure]
Entree : 
    - Datas (Datas_t *) : Pointeur de la structure de donnees

Sortie : 
    - matrice (int **) : double pointeur pour regrouper les données de la structure sous forme de matrice
*/

int** StructureEnMatrice(Datas_t *Datas)
{
    int   i, j, k;
    int **matrice = malloc(sizeof(int *)*Datas->nbBinome);

    if(matrice == NULL)
    {
        fprintf(stderr, "Porblème d'allocation\n");
        exit(EXIT_FAILURE);
    }


    for (i = 0; i < Datas->nbBinome; i++) /*Pour chaque binôme....*/
    {
        matrice[i] = malloc(sizeof(int)*Datas->nbProjet);
        for (j = 0; j < Datas->nbProjet; j++) /*Pour chaquye coefficient de la mtrice, on l'initialise à 0*/
        {
            matrice[i][j] = 0;
            for (k = 0; k < Datas->nbChoix; k++)
            {
                if (Datas->choix[i][k+1] == j+1) /*Si le projet est choisi par le binôme, on l'inscrit dans la matrice...*/
                {
                    matrice[i][j] = Datas->poids[k]; /*Puis on le pondère par la valeur du poids associé à son importance*/
                }
            }
        }
    }

    return matrice;
}


/*-------------------------------------------------------------------------------------------------------*/
/*[Donner le N° de projet du i_ème choix du j_ième binôme]
Entree : 
    - Datas (Datas_t *) : Pointeur de la structure de donnees
    - numBinome (int)   : N° du binôme auquel on veut connaitre un choix de projet
    - numChoix (int)    : N° choix du projet du binôme auquel on veut connaitre le N° de projet associé

Sortie : 
    - res (int) : N° projet de 'numChoix' choix du 'numBinome' binôme.
*/

int NumProjet_BinomeChoix(Datas_t *Datas, int numBinome, int numChoix)
{
    int i;
    int res = -1;
    for (i = 0; i < Datas->nbBinome; i++)
    {
        if (Datas->choix[i][0] == numBinome) 
            res = Datas->choix[i][numChoix];
    }
    return res; /*res vaudra -1 si un N° de choix et/ou un N° de binôme qui n'existe pas est/sont donné(s)*/
}


/*-------------------------------------------------------------------------------------------------------*/
/*[Donner le N° d'un choix d'un projet pour un binôme donné]
Entree : 
    - Datas (Datas_t *) : Pointeur de la structure de donnees
    - numBinome (int)   : N° du binôme auquel on veut connaitre un N° de choix à partir d'un N° de projet
    - numProjet (int)   : N° projet auquel on veut connaitre le N° de choix associé pour un binôme donné

Sortie : 
    - res (int) : N° projet de 'numChoix' choix du 'numBinome' binôme.
*/

int NumChoix_BinomeProjet(Datas_t *Datas, int numBinome, int numProjet)
{
    int i, j;
    int res = -1;
    for (i = 0; i < Datas->nbBinome; i++)
    {
        if (Datas->choix[i][0] == numBinome)
        {
            for (j = 1; j < Datas->nbChoix+1; j++)
            {
                if (Datas->choix[i][j] == numProjet) res = j;
            }
        }
    }
    return res; /*res vaudra -1 si le binôme n'a jamais souhaité ce projet*/
}


/*-------------------------------------------------------------------------------------------------------*/
/*[Creation du fichier du sortie sous forme de tableau (N° de binôme, N° de projet obtenu)]
Entree : 
    - Datas (Datas_t *) : Pointeur de la structure de donnees
    - sol (double *)    : Pointeur de double contenant les solutions (après résolution par gurobi)

Sortie : 
    - void
*/

void CreationFichierDeSortie(double *sol, Datas_t *Datas)
{
    int i;
    int taille = (Datas->nbBinome)*(Datas->nbProjet);

    FILE* fic = fopen("fichierDeSortie", "w"); /*Création de 'fichierDeSortie' s'il nexiste pas déjà*/

    for (i = 0; i < taille; i++)
    {
        if (sol[i] == 1) 
            fprintf(fic, "%d %d\n", Datas->choix[i/Datas->nbProjet][0], i%Datas->nbProjet + 1); /*Ecriture dans fichier*/
    }

    fclose(fic); /*Fermeture fichier*/
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
    srand(time(0)); /*Fonction pour aléatoire*/
    FILE *fic = fopen("Test_G", "w"); /*Création du fichier*/
    
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