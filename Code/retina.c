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
 *      Renvoie la valeur d'activation d'un cône
 *      en prenant en compte la couleur HSV à
 *      laquelle il est exposé
 * 
 * @param
 *      couleurHSV color : couleur exposée
 *      Cone currentCone : cone à tester
 * 
 * @retour
 *      couleurHSV : valeur d'activation du cône
 */
couleurHSV getConeActivationValue(couleurHSV color, Cone currentCone)
{
    // Noir pas défaut
    couleurHSV activationValue;
    activationValue.h = -1;
    activationValue.s = 0;
    activationValue.v = 0;

    if(color.v < currentCone.valueThreshold)
    {     
        // Si c'est un cône rouge, l'intervalle de Hue a un traitement spécial
        if(currentCone.type == LONG)
        {
            if( (color.h >= currentCone.minHue && color.h < 360)
                || (color.h >= 0 && color.h <= currentCone.maxHue) ) 
            {
                activationValue.h = color.h;
                activationValue.s = color.s;
                activationValue.v = color.v;
            }
        }
        else
        {
            if(color.h >= currentCone.minHue && color.h <= currentCone.maxHue)
            {
                activationValue.h = color.h;
                activationValue.s = color.s;
                activationValue.v = color.v;
            }
        }
    }

    return activationValue;
}


/* @fonction
 *      Crée une matrice DonneesImageTab contenant le résultat
 *      de l'image filtrée par la rétine
 * 
 * @param
 *      DonneesImageTab image   :   pointeursur l'image à filtrer
 * 
 * @retour
 *      DonneesImageTab* : pointeur vers la matrice crée
 */
DonneesImageTab* applyRetina(DonneesImageTab *image)
{
    //Crée l'image à retourner
    DonneesImageTab *filteredImage = initTab(image->largeurImage, image->hauteurImage);

    //Indexs de tableau
    int widthIndex, heightIndex, neighbourWidth, neighbourHeight, coneIndex;

    //Valeurs d'activations d'un groupe de 9 cônes
    couleurHSV *conesActivationValues = (couleurHSV*)malloc(9 * sizeof(couleurHSV));

    //Convertis l'image RVB en HSV
    tabCouleurHSV* imageHSV = tabBgrToTabHsv(image);

    //Matrice HSV temporaire
    tabCouleurHSV* tmpTabHSV = (tabCouleurHSV*)malloc(sizeof(tabCouleurHSV)) ;
    tmpTabHSV->largeur = imageHSV->largeur; 
    tmpTabHSV->hauteur = imageHSV->hauteur; 
    tmpTabHSV->tabHsv = alloueMatCouleurHsv(tmpTabHSV->largeur, tmpTabHSV->hauteur) ;


    //Parcours le tableau
    for(widthIndex = 0; widthIndex < image->largeurImage; widthIndex++)
    {
        for(heightIndex = 0; heightIndex < image->hauteurImage; heightIndex++)
        {
            //Parcours les 8 voisins et le point lui même
            for(neighbourWidth = -1; neighbourWidth <= 1; neighbourWidth++)
            {
                coneIndex = 0;

                for(neighbourHeight = -1; neighbourHeight <= 1; neighbourHeight++)
                {
                    //Coordonnées du point courant
                    int currentWidth = widthIndex + neighbourWidth;
                    int currentHeight = heightIndex + neighbourHeight;

                    //Génère un photorécepteur
                    Cone currentCone = getRandomCone();

                    if( (currentWidth >= 0) && (currentWidth < imageHSV->largeur) 
                            && (currentHeight >= 0) && (currentHeight < imageHSV->hauteur) )
                    {
                        //Applique le photorécepteur au point et récupère sa valeur d'activation
                        conesActivationValues[coneIndex] = getConeActivationValue(imageHSV->tabHsv[currentWidth][currentHeight] ,currentCone);
                    }
                }

                //Addition chelou (médian)
                tmpTabHSV->tabHsv[widthIndex][heightIndex] = medianHSV(conesActivationValues, 9, SORT_BY_HUE);
            }
        }
    }

    return filteredImage;
}



/* @fonction
 *      Calcule et renvoie la médiane d'un tableau
 * 		1D de couleurHSV
 * 
 * @param
 * 		couleurHSV *table :   Tableau dont on cherche la médiane
 * 		int size  :   Taille de ce tableau
 *      medianSortBy attribute  :   attribut H, S ou V à trier
 * 
 * @retour
 *      couleurHSV  :   Médiane du tableau
 * */
couleurHSV medianHSV(couleurHSV *table, int size, medianSortBy attribute)
{ 
	int i, j;
	
	for(i = size - 1; i > 0 ; i--)
	{
		for(j = 0; j < i - 1; j++)
		{
            if(attribute == SORT_BY_VALUE)
            {
                if(table[j+1].v < table[j].v)
                {
                    couleurHSV tmp = table[j+1];
                    table[j+1] = table[j];
                    table[j] = tmp;
                }
            }
            else if(attribute == SORT_BY_SATURATION)
            {
                if(table[j+1].s < table[j].s)
                {
                    couleurHSV tmp = table[j+1];
                    table[j+1] = table[j];
                    table[j] = tmp;
                }
            }
            else
            {
                if(table[j+1].h < table[j].h)
                {
                    couleurHSV tmp = table[j+1];
                    table[j+1] = table[j];
                    table[j] = tmp;
                }
            }

			
		}	
	}
	
	return table[size/2];
}

