#include __GENERE_H__
#define __GENERE_H__


/*[INCLUDES]*/
/*Librairies*/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




/* Structure de booleen */
typedef enum {
  false,
  true
} Bool;


/*-------------------------------------------------------------------------------------------------------*/

/*[PROTOTYPE]*/
Bool isNumber(char* p_str);
void GenerateurDonneesEntree(int nbBinome, int nbProjet, int nbChoix);


#endif