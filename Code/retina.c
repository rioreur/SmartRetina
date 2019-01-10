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
float getConeActivationValue(couleurHSV color, coneType currentCone)
{
    int pixelHue = color.h;
    int coneHue = currentCone;

    int distanceHues = (coneHue - pixelHue + 360)%360;

    return distanceHues * color.v * color.s;
}


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
DonneesImageTab* applyRetina(DonneesImageTab *image, int sideSize)
{
    //Indexs de tableau
    int widthIndex, heightIndex, neighbourWidth, neighbourHeight;

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
            float newPixelHue = 0;
            float sumOfActivationValues = 0;
            int range = sideSize / 2;

            //Parcours les 8 voisins et le point lui même
            for(neighbourWidth = -range; neighbourWidth <= range; neighbourWidth++)
            {
                for(neighbourHeight = -range; neighbourHeight <= range; neighbourHeight++)
                {
                    //Coordonnées du point courant
                    int currentWidth = widthIndex + neighbourWidth;
                    int currentHeight = heightIndex + neighbourHeight;

                    if( (currentWidth >= 0) && (currentWidth < imageHSV->largeur) 
                            && (currentHeight >= 0) && (currentHeight < imageHSV->hauteur) )
                    {
                        //Génère un cône aléatoire
                        coneType cone = getRandomConeType();

                        //Applique le photorécepteur au point et récupère sa valeur d'activation
                        float activationValue = getConeActivationValue(imageHSV->tabHsv[currentWidth][currentHeight], cone );
                        
                        //Calcul du numérateur de la moyenne
                        newPixelHue += cone * activationValue; 

                        //Calcul du dénominateur de la moyenne
                        sumOfActivationValues += activationValue;
                    }
                }
            }

            //Calcul de la nouvelle Hue du pixel
            if(sumOfActivationValues != 0)
                newPixelHue = newPixelHue / sumOfActivationValues;
            
            newPixelHue += imageHSV->tabHsv[widthIndex][heightIndex].h;

            //Enregistre le nouveau pixel
            couleurHSV newPixel;
            newPixel.h = newPixelHue;
            newPixel.s = imageHSV->tabHsv[widthIndex][heightIndex].s;
            newPixel.v = imageHSV->tabHsv[widthIndex][heightIndex].v;

            tmpTabHSV->tabHsv[widthIndex][heightIndex] = newPixel;
        }
    }

    return tabHsvToTabBgr(tmpTabHSV);

}





