#include "retina.h"


/* @fonction		:	Renvoie une valeur aléatoire comprise entre deux valeurs
 *						passées en paramètre
 * 
 * @param			:
 *      valeurMin	: 	Valeur minimum atteinte aléatoirement
 *      valeurMax	: 	Valeur maximum atteinte aléatoirement
 * 
 * @retour		 	:	Valeur aléatoire comprise ente valeurMin et valeurMax
 */
float randomFloatBetween(float minValue, float maxValue)
{
    return ((float)rand()/(float)(RAND_MAX))*(maxValue - minValue) + minValue;
}


/* @fonction		:	Renvoie une valeur aléatoire comprise entre deux valeurs
 *						passées en paramètre
 * 
 * @param			:
 *      valeurMin	: 	Valeur minimum atteinte aléatoirement
 *      valeurMax	: 	Valeur maximum atteinte aléatoirement
 * 
 * @retour		 	:	Valeur aléatoire comprise ente valeurMin et valeurMax
 */
int randomIntBetween(int minValue, int maxValue)
{
    return randomFloatBetween(minValue, maxValue);
}


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
coneType getRandomConeType(void)
{
    int value = randomIntBetween(1, 100);
    if(value <= S_CONES_PERCENT)
        return SHORT;
    if(value <= S_CONES_PERCENT + M_CONES_PERCENT)
        return MEDIUM;
    return LONG;
}


/* @fonction
 *      Renvoie une structure Cone aléatoire
 *      en respectant la répartition LMS dans une fovéa
 * 
 * @param
 *      \
 * 
 * @retour
 *      Cone : structure Cone
 */
Cone getRandomCone(void)
{
    Cone newCone;
    newCone.type = getRandomConeType();
    
    if(newCone.type == LONG)
    {
        newCone.minHue = 360 - HSV_RANGE;
        newCone.maxHue = HSV_LONG + HSV_RANGE - 1;
    }
    else if(newCone.type == MEDIUM)
    {
        newCone.minHue = HSV_MEDIUM - HSV_RANGE;
        newCone.maxHue = HSV_MEDIUM + HSV_RANGE - 1;
    }
    else if(newCone.type == SHORT)
    {
        newCone.minHue = HSV_SHORT - HSV_RANGE;
        newCone.maxHue = HSV_SHORT + HSV_RANGE - 1;
    }

    newCone.valueThreshold = 10;

    return newCone;
}

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
DonneesImageTab* applyRetina(DonneesImageTab image)
{
    //Crée l'image à retourner
    DonneesImageTab *filteredImage = initTab(image.largeurImage, image.hauteurImage);

    int widthIndex, heightIndex;

    //Parcours le tableau
    for(widthIndex = 0; widthIndex < image.largeurImage; widthIndex++)
    {
        for(heightIndex = 0; heightIndex < image.hauteurImage; heightIndex++)
        {

            //Pour chaque point, convertis son RVB en HSV
            //Génère 9 photorécepteurs
            //Applique les au voisinage
            //Addition chelou
        }
    }

    return filteredImage;
}