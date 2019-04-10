/*[INCLUDES]*/

/*Librairies*/
#include <stdlib.h> 
#include <stdio.h>

/*Fichiers*/
#include "../include/gurobi_c.h"
#include "../include/matrice.h"
#include "../include/GUROBI.h"
#include "../include/generer_entree.h"


/*-------------------------------------------------------------------------------------------------------*/
/*
* Quoiqu'il arrive, cdette 'LancementGurobi1()' est appelée, seul le fichier d'entrée change suivant si l'on veut un 
* fichier en entrée qui a été généré aléatoirement ou bien créé par l'utilisateur
*/


void LancementGurobi1(char *Fichier)
{
  /*Declarations*/
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
  FILE     *fic = NULL;
  Datas_t *Datas;


  fic = fopen(Fichier, "r"); /*Lecture du fichier d'entree 'Test_G.txt' ou 'Test_NonG.txt'*/
  if (fic == NULL) 
  {
    printf("erreur ouverture fichier");
    exit(EXIT_FAILURE);
  }
  else
  {
    Datas = FichierEnStructureDatas(fic); /*Affecter les données contenues dans le fichier d'entrée dans une structure*/
    matrice = StructureEnMatrice(Datas); /*Création de la matrice d'entrée (pour gurobi) à partir de la structure*/
    taille = (Datas->nbBinome)*(Datas->nbProjet);

    /*[Alocation des tableaux utiles pour gurobi]*/
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
      /*Pour gurobi, il est necessaire que la matrice (2D) soit en 1D*/
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

      error = GRBaddvars(model, taille, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
      if (error) goto QUIT;



      /*[[CONTRAINTES]]*/

      /* Ajoute les contraintes pour les variables nulles*/
      k = 0;
      for (i = 0; i < Datas->nbBinome; i++)
      {
        for (j = 0; j < Datas->nbProjet; j++)
        {
          if(matrice[i][j] == 0) /*Coefficients = 0*/
          {
            ind[0] = k;
            val[0] = 1;

            error = GRBaddconstr(model, 1, ind, val, GRB_EQUAL, 0.0, "contrainteVarNulle"); /*Création de la contrainte variable nulle*/
            if (error) goto QUIT;
          }

          k++;
        }
      }
      
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


      /* Optimize model */
      error = GRBoptimize(model);
      if (error) goto QUIT;

      /* Write model to 'ProjetZZ1.lp' */
      error = GRBwrite(model, "ProjetZZ1.lp");
      if (error) goto QUIT;

      /* Capture solution information */
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
        for (i = 0; i < taille; i++)
        {
          if (sol[i] == 1) /*Affichage du résultat*/
          {
            printf("le binome %d à recu le projet %d (choix °%d)\n",i/Datas->nbProjet + 1, i%Datas->nbProjet + 1, NumChoix_BinomeProjet(Datas, 
                                                          Datas->choix[i/Datas->nbProjet][0], i%Datas->nbProjet + 1));
            Datas->res[i/Datas->nbProjet] = malloc(sizeof(int)*2);
            Datas->res[i/Datas->nbProjet][0] = Datas->choix[i/Datas->nbProjet][0];
            Datas->res[i/Datas->nbProjet][1] = i%Datas->nbProjet + 1;
          }
        }
      }
      else if (optimstatus == GRB_INF_OR_UNBD) 
      {
        printf("Modele infaisable ou hors bornes\n");
      } 
      else 
      {
        printf("Optimization stoppée trop tôt\n");
      }

      /*Ecriture des affectations dans le fichier de sortie sous forme de tableau*/ 
      CreationFichierDeSortie(sol, Datas);


      QUIT:

      /*Affichage du type de l'erreur (si erreur il y a)*/
      if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        exit(EXIT_FAILURE);
      }


      /*[[LIBERATION MEMOIRE]]*/
      GRBfreemodel(model); /*Liberation du modèle*/
      GRBfreeenv(env); /*Liberation de l'environnement*/

      /*[Liberation des tableaux et variables]*/
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


      fclose(fic); /*Fermeture du fichier*/
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
  LancementGurobi1("Test_G.txt");
}

/*-------------------------------------------------------------------------------------------------------*/
