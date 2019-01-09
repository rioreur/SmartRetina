#include "symetrie.h"
#include <time.h>

/* @fonction		:	Renvoie une valeur aléatoire comprise entre deux valeurs
 *						passées en paramètre
 * 
 * @param			:
 *      valeurMin	: 	Valeur minimum atteinte aléatoirement
 *      valeurMax	: 	Valeur maximum atteinte aléatoirement
 * 
 * @retour		 	:	Valeur aléatoire comprise ente valeurMin et valeurMax
 */
float randomFloatBetween(float minValue, float maxValue);


/* @fonction		:	Renvoie une valeur aléatoire comprise entre deux valeurs
 *						passées en paramètre
 * 
 * @param			:
 *      valeurMin	: 	Valeur minimum atteinte aléatoirement
 *      valeurMax	: 	Valeur maximum atteinte aléatoirement
 * 
 * @retour		 	:	Valeur aléatoire comprise ente valeurMin et valeurMax
 */
int randomIntBetween(int minValue, int maxValue);


/* @fonction
 *      Crée une matrice DonneesImageTab contenant le résultat
 *      de l'image filtrée par la rétine
 * 
 * @param
 *      DonneesImageTab image   :   image à filtrer
 * 
 * @retour
 *      DonneesImageTab* : pointeur vers la matrice crée
 */
DonneesImageTab* applyRetina(DonneesImageTab image);


/* @fonction
 *      
 * 
 * 
 */