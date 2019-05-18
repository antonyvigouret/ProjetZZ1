/*[INCLUDES]*/

/*Librairies*/
#include <stdlib.h> 
#include <stdio.h>

/*Fichiers*/
#include "../include/gurobi_c.h"
#include "../include/matrice.h"
#include "../include/GUROBI.h"
#include "../include/generer_entree.h"





/* 
- Entree :
    Fichier1 (char*) : fichier d'entrée contenant les choix des binomes
    Fichier2 (char*) : fichier contenant la liste de tout les projets
- sortie : 
    void
*/

void LancementGurobi(char *Fichier1, char *Fichier2)
{
  /*Declarations des variables*/
  GRBenv   *env   = NULL;
  GRBmodel *model = NULL;
  double   *sol;
  double   *val;
  double   *obj;
  double    objval;
  int     **matrice;
  int      *ind;
  int       i, j, k;
  int       taille;
  int       optimstatus;
  int       error = 0;
  char     *vtype;
  FILE     *fic1 = NULL;
  FILE     *fic2 = NULL;
  Datas_t  *Datas;


  fic1 = fopen(Fichier1, "r"); /*Lecture du fichier d'entree aves les choix*/
  fic2 = fopen(Fichier2, "r"); /*Lecture du fichier avec la liste des projets*/
  if (fic1 == NULL || fic2 == NULL) 
  {
    printf("erreur ouverture fichier(s)\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    Datas = FichierEnStructureDatas(fic1, fic2); /*Affecter les données contenues dans les fichiers d'entrées dans une structure*/
    matrice = StructureEnMatrice(Datas); /*Création de la matrice d'entrée (pour gurobi) à partir de la structure*/
    taille = (Datas->nbBinome)*(Datas->nbProjet); /*Il est nécessaire de connaitre la taille n*m pour les tableaux en 1D utilisés par Gurobi*/

    /* Alocation des tableaux utiles pour gurobi */
    sol = malloc(sizeof(double)*taille);
    ind = malloc(sizeof(int)*Datas->nbProjet);
    val = malloc(sizeof(double)*Datas->nbProjet);
    obj = malloc(sizeof(double)*taille);
    vtype = malloc(sizeof(char)*taille);


    if((sol == NULL) || (ind == NULL) || (val == NULL) || (obj == NULL) || (vtype == NULL))
    {
      fprintf(stderr, "Problème d'allocation des variables\n");
      exit(EXIT_FAILURE);
    }
    else
    {
      /* Création de l'environnement (gurobi) */
      error = GRBloadenv(&env, "ProjetZZ1.log");
      if (error) goto QUIT;

      /* Création d'un modèle vide (gurobi) */
      error = GRBnewmodel(env, &model, "ProjetZZ1", 0, NULL, NULL, NULL, NULL, NULL);
      if (error) goto QUIT;


      /* Ajout des coefficients de la matrice d'entrée vers les variables obj et vtype (gurobi) */
      /* Pour gurobi, il est necessaire que la matrice (2D) soit transformée en 1D */
      k = 0;
      for (i = 0; i < Datas->nbBinome; i++)
      {
        for (j = 0; j < Datas->nbProjet; j++)
        {
          obj[k] = matrice[i][j];
          vtype[k] = GRB_BINARY;
          k++;
        }
      }


      /* Ajout des variables dans le model */
      error = GRBaddvars(model, taille, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
      if (error) goto QUIT;



      /*[[CONTRAINTES]]*/
      
      /* Ajoute les contraintes sur les lignes */
      for (i = 0; i < Datas->nbBinome; i++)
      {
        for (j = 0; j < Datas->nbProjet; j++)
        {
          ind[j] = i*Datas->nbProjet + j;
          val[j] = 1;
        }
        error = GRBaddconstr(model, Datas->nbProjet, ind, val, GRB_EQUAL, 1.0, "contrainteLigne"); /*Création de la contrainte ligne*/
        if (error) goto QUIT;
      }

      /* Ajoute les contraintes sur les colonnes */
      for (j = 0; j < Datas->nbProjet; j++)
      {
        for (i = 0; i < Datas->nbBinome; i++)
        {
          ind[i] = i*Datas->nbProjet + j;
          val[i] = 1;
        }
        error = GRBaddconstr(model, Datas->nbBinome, ind, val, GRB_LESS_EQUAL, 1.0, "contrainteColonne"); /*Création de la contrainte colonne*/
        if (error) goto QUIT;
      }


      /* Optimise le model */
      error = GRBoptimize(model);
      if (error) goto QUIT;

      /* Décrit le model dans 'ProjetZZ1.lp' */
      error = GRBwrite(model, "ProjetZZ1.lp");
      if (error) goto QUIT;

      /* Capture les informations de la solution */
      error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
      if (error) goto QUIT;

      error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
      if (error) goto QUIT;

      error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, taille, sol);
      if (error) goto QUIT;

      printf("\nOptimization complete\n");
      if (optimstatus == GRB_OPTIMAL) 
      {
        j = 0;
        printf("Optimal objective: %.4e\n", objval);
        /* Affichage du résultat dans le terminal */
        printf("\nVoici les affectations (choix = -1 : le projet ne fait pas parti de leurs choix):\n");
        for (i = 0; i < taille; i++)
        {
          if (sol[i] == 1) 
          {
            printf("le binome %d à recu le projet %d (choix °%d)\n",Datas->choix[i/Datas->nbProjet][0], Datas->correspondance[i%Datas->nbProjet][1], 
                                                              NumChoix_BinomeProjet(Datas, Datas->choix[i/Datas->nbProjet][0], i%Datas->nbProjet + 1));

            /* Création tableau resultat : 3 colonnes */
            Datas->res[i/Datas->nbProjet] = malloc(sizeof(int)*3);
            Datas->res[i/Datas->nbProjet][0] = Datas->choix[i/Datas->nbProjet][0];
            Datas->res[i/Datas->nbProjet][1] = Datas->correspondance[i%Datas->nbProjet][1];
            Datas->res[i/Datas->nbProjet][2] = NumChoix_BinomeProjet(Datas, Datas->choix[i/Datas->nbProjet][0], i%Datas->nbProjet + 1);
          }
        }
        printf("\nLes affectations sont également disponible dans le fichier 'fichierDeSortie'\n");
      }
      else if (optimstatus == GRB_INF_OR_UNBD) 
      {
        printf("Modele infaisable ou hors bornes\n");
      } 
      else 
      {
        printf("Optimisation stoppée trop tôt\n");
      }

      /* Ecriture des affectations dans le fichier de sortie sous forme de tableau */ 
      CreationFichierDeSortie(Datas);


      QUIT:

      /* Affichage du type de l'erreur (si erreur il y a) */
      if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        exit(EXIT_FAILURE);
      }


      /*[[LIBERATION MEMOIRE]]*/
      GRBfreemodel(model); /*Liberation du modèle (Gurobi)*/
      GRBfreeenv(env); /*Liberation de l'environnement (Gurobi)*/

      /*[Liberation des tableaux et variables]*/
      /*Variables Gurobi */
      free(sol);
      free(ind);
      free(val);
      free(obj);
      free(vtype);

      /*Liberation de la matrice matrice et de la structure Datas_t*/
      for (i = 0; i < Datas->nbBinome; i++)
      {
        free(matrice[i]); /*Liberation de chaque ligne de la matrice*/
        free(Datas->choix[i]);
        free(Datas->res[i]);
      }

      for (i = 0; i < Datas->nbProjet; i++)
      {
        free(Datas->correspondance[i]);
      }

      free(matrice);
      free(Datas->poids);
      free(Datas->choix);
      free(Datas->res);
      free(Datas->correspondance);
      free(Datas);

      /* Fermeture des fichier */
      fclose(fic1);
      fclose(fic2);
    }
  }

}

/*-------------------------------------------------------------------------------------------------------*/
/*Fonction appelée lorsqu'il faut générer un fichier d'entrée avant de le donner en entrée de Gurobi*/

void LancementGurobi2()
{

  /*Demande de nbBinome, nbProjet, nbChoix*/
  int nbBinome, nbProjet, nbChoix;
  printf("\n=======FICHIER D'ENTREE A GENERER ALEATOIREMENT=======\n");
  printf("Donnez le nombre de binome puis de projet et enfin de choix\n");

  scanf("%d %d %d", &nbBinome, &nbProjet, &nbChoix);


  GenerateurDonneesEntree(nbBinome, nbProjet, nbChoix);
  LancementGurobi("TestGen1.txt", "TestGen2.txt");
}

/*-------------------------------------------------------------------------------------------------------*/