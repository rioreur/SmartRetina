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
 *      colorHSV color : couleur exposée
 *      Cone currentCone : cone à tester
 * 
 * @retour
 *      float : valeur d'activation du cône
 */
float getConeActivationValue(colorHSV color, coneType currentCone)
{
    int pixelHue = color.hue;
    int coneHue = currentCone;


    int distanceOneWay = (coneHue - pixelHue + 360)%360;
    int distanceAnotherWay = (pixelHue - coneHue + 360)%360;
    int distanceHues;

    if(distanceOneWay >= distanceAnotherWay)
    {
        distanceHues = distanceAnotherWay;
    }
    else
    {
        distanceHues = distanceOneWay;
    }

    return ((360 - distanceHues)/360) * color.value * color.saturation;
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

    //Convertis l'image BGR en HSV
    arrayColorHSV* imageHSV = arrayBGRToArrayHSV(image);

    //Matrice HSV temporaire
    arrayColorHSV* tmpTabHSV = (arrayColorHSV*)malloc(sizeof(arrayColorHSV)) ;
    tmpTabHSV->width = imageHSV->width; 
    tmpTabHSV->height = imageHSV->height; 
    tmpTabHSV->arrayHSV = allocateMatColorHSV(tmpTabHSV->width, tmpTabHSV->height) ;


    //Parcours le tableau
    for(widthIndex = 0; widthIndex < image->largeurImage; widthIndex++)
    {
        for(heightIndex = 0; heightIndex < image->hauteurImage; heightIndex++)
        {
            int newPixelHue = 0;
            int sumOfActivationValues = 0;
            int range = sideSize / 2;

            //Parcours les 8 voisins et le point lui même
            for(neighbourWidth = -range; neighbourWidth <= range; neighbourWidth++)
            {
                for(neighbourHeight = -range; neighbourHeight <= range; neighbourHeight++)
                {
                    //Coordonnées du point courant
                    int currentWidth = widthIndex + neighbourWidth;
                    int currentHeight = heightIndex + neighbourHeight;

                    if( (currentWidth >= 0) && (currentWidth < imageHSV->width) 
                            && (currentHeight >= 0) && (currentHeight < imageHSV->height) )
                    {
                        //Génère un cône aléatoire
                        coneType cone = getRandomConeType();

                        //Applique le photorécepteur au point et récupère sa valeur d'activation
                        float activationValue = getConeActivationValue(imageHSV->arrayHSV[currentWidth][currentHeight], cone );
                        
                        //Calcul du numérateur de la moyenne
                        newPixelHue += (imageHSV->arrayHSV[widthIndex][heightIndex].hue - cone) * activationValue; 

                        //Calcul du dénominateur de la moyenne1
                        sumOfActivationValues += activationValue;
                    }
                }
            }

            //Calcul de la nouvelle Hue du pixel
            if(sumOfActivationValues != 0)
                newPixelHue = newPixelHue / sumOfActivationValues;
            
            newPixelHue = (newPixelHue + (int)imageHSV->arrayHSV[widthIndex][heightIndex].hue + 360)%360;

            //Enregistre le nouveau pixel
            colorHSV newPixel;
            newPixel.hue = newPixelHue;
            newPixel.saturation = imageHSV->arrayHSV[widthIndex][heightIndex].saturation ;
            newPixel.value = imageHSV->arrayHSV[widthIndex][heightIndex].value ;

            tmpTabHSV->arrayHSV[widthIndex][heightIndex] = newPixel;
        }
    }

    return arrayHSVToArrayBGR(tmpTabHSV);

}





