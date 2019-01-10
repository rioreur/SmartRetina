#include "hsv.h"
#include <time.h>
#include <stdbool.h>

/* @type
 *      Type de cône
 */
typedef enum {LONG = 0, MEDIUM = 120, SHORT = 240}coneType;

/* @type
 *      Paramétrage du filtre médian
 */
typedef enum {SORT_BY_HUE, SORT_BY_SATURATION, SORT_BY_VALUE}medianSortBy;


/* @definition
 *      Valeurs de R (255, 0, 0), G et B en HSV selon 
 *      la convention LMS des cônes
 */
#define HSV_LONG 0      //R
#define HSV_MEDIUM 120  //G
#define HSV_SHORT 240   //B

/* @definition
 *      Portée de l'intervalle d'une couleur
 *      Une valeur comprise entre HSV_LONG +- HSV_RANGE
 *      modulo 360 sera considérée comme rouge
 */
#define HSV_RANGE 60


/* @definition
 *      Pourcentages de répartition des cônes dans la fovéa
 */
#define L_CONES_PERCENT 40
#define M_CONES_PERCENT 40
#define S_CONES_PERCENT 20


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
 *      Renvoie une coneType aléatoire
 *      en respectant la répartition LMS dans une fovéa
 * 
 * @param
 *      \
 * 
 * @retour
 *      coneType : type du cône
 */
coneType getRandomConeType(void);


/* @fonction
 *      Renvoie la valeur d'activation d'un cône
 *      en prenant en compte la couleur HSV à
 *      laquelle il est exposé
 * 
 * @param
 *      couleurHSV color : couleur exposée
 *      Cone currentCone : cone à tester
 * 
 * @retour
 *      float : valeur d'activation du cône
 */
float getConeActivationValue(couleurHSV color, coneType currentCone);


/* @fonction
 *      Crée une matrice DonneesImageTab contenant le résultat
 *      de l'image filtrée par la rétine
 * 
 * @param
 *      DonneesImageTab image   :   pointeur sur l'image à filtrer
 *      int sideSize            :   taille du carré du voisinnage à prendre en compte
 * 
 * @retour
 *      DonneesImageTab* : pointeur vers la matrice crée
 */
DonneesImageTab* applyRetina(DonneesImageTab *image, int sideSize);