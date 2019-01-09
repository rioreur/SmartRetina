#include "region.h"

DonneesImageTab* initTabRegion(int width, int height, int value)
{
	// We initialize a DonneesImageTab using the initTab function
	DonneesImageTab* tabRegion = initTab(width, height);
	int i, j, cIndex;
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			for(cIndex = 0; cIndex < 3; cIndex++)
			{
				tabRegion->donneesTab[i][j][cIndex] = value;
			}
		}
	}
	// finaly we return it
	return tabRegion;
}

IdRegion* initIdRegion(int blue, int green, int red, int x, int y)
{
	IdRegion* idRegion = malloc(sizeof(IdRegion));
	idRegion->blue = blue;
	idRegion->green = green;
	idRegion->red = red;
	idRegion->x = x;
	idRegion->y = y;
	return idRegion;
}

IdRegions* initIdRegions(int size)
{
	IdRegions* idRegions = malloc(sizeof(IdRegions));
	idRegions->size = size;
	idRegions->regions = malloc(sizeof(IdRegion*)*size);
	int i;
	for (i = 0; i < size; i++)
	{
		idRegions->regions[i] = malloc(sizeof(IdRegion));
	}
	return idRegions;
}

void destructIdRegions(IdRegions** idRegions)
{
	if (idRegions != NULL)
	{
		if (*idRegions != NULL)
		{
			int i;
			for (i = 0; i < (*idRegions)->size; i++)
			{
				free((*idRegions)->regions[i]);
			}
			free((*idRegions)->regions);
			free(*idRegions);
			
			*idRegions = NULL;
		}
	}
}

IdRegion* findRegionBottomUp(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int x, int y, int sensibility)
{
	IdRegion* idRegion = NULL;
	// If the seed is in the image
	if (0 <= x && x <= tabImage->largeurImage && 
		0 <= y && y <= tabImage->largeurImage)
	{
		// We save the red, blue and green value of the seed
		idRegion = initIdRegion(
			tabImage->donneesTab[x][y][BLUE],
			tabImage->donneesTab[x][y][GREEN],
			tabImage->donneesTab[x][y][RED],
			x,
			y);
		int tempColor; // Use to calculate the distance (in colors) between a given pixel and the seed
		// We set the pixel at the seed's coordinate  in the region matrice to BORDER (-1) to say that it needs to be check
		tabRegion->donneesTab[x][y][BLUE] = BORDER;
		tabRegion->donneesTab[x][y][GREEN] = BORDER;
		tabRegion->donneesTab[x][y][RED] = BORDER;
		int borderFind; // Use to know if their is still pixels to be check (BORDER close enought to the seed color)
		int i, j;
		int k, l;
		do
		{
			borderFind = 0;
			// For each pixels
			for(i = 0; i < tabRegion->largeurImage; i++)
			{
				for(j = 0; j < tabRegion->hauteurImage; j++)
				{
					// If it needs to be check
					if (tabRegion->donneesTab[i][j][BLUE] == BORDER)
					{
						// We calculate the distance (in colors) between the current pixel and the seed
						tempColor = sqrt(
							pow(tabImage->donneesTab[i][j][BLUE] - idRegion->blue, 2) + 
							pow(tabImage->donneesTab[i][j][GREEN] - idRegion->green, 2) + 
							pow(tabImage->donneesTab[i][j][RED] - idRegion->red, 2));
						// If it is the a color close enought (distance inferior to the sensibility)
						if(tempColor < sensibility)
						{
							// We add it to the region matrice
							tabRegion->donneesTab[i][j][BLUE] = idRegion->blue;
							tabRegion->donneesTab[i][j][GREEN] = idRegion->green;
							tabRegion->donneesTab[i][j][RED] = idRegion->red;
							// We say that we found a new pixel to add to the region
							borderFind++;
							// Then, for each neighbours
							for(k = -1; k <= 1; k++)
							{
								for(l = -1; l <= 1; l++)
								{
									// If they have not been checked yet and if they are in the bounderies
									if (0 <= i + k && i + k < tabRegion->largeurImage && 
										0 <= j + l && j + l < tabRegion->hauteurImage &&
										tabRegion->donneesTab[i + k][j + l][BLUE] == UNCHECKED &&
										tabRegion->donneesTab[i + k][j + l][GREEN] == UNCHECKED &&
										tabRegion->donneesTab[i + k][j + l][RED] == UNCHECKED)
									{
										// Say that it's a border to check later (set it a BORDER value)
										tabRegion->donneesTab[i + k][j + l][BLUE] = BORDER;
										tabRegion->donneesTab[i + k][j + l][GREEN] = BORDER;
										tabRegion->donneesTab[i + k][j + l][RED] = BORDER;
									}
								}
							}
						}
					}
				}
			}
		// We ontinue until their is no point left to check
		}while (borderFind > 0);
		
		// For each pixels and colors, we set the remaining pixel with a BORDER value to an UNCHECKED value
		//(used if we need to add other region on this matrice)
		int cIndex;
		for(i = 0; i < tabRegion->largeurImage; i++)
		{
			for(j = 0; j < tabRegion->hauteurImage; j++)
			{
				for(cIndex = 0; cIndex < 3; cIndex++)
				{
					if (tabRegion->donneesTab[i][j][cIndex] == BORDER)
					{
						tabRegion->donneesTab[i][j][cIndex] = UNCHECKED;
					}
				}
			}
		}
	}
	return idRegion;
}

IdRegions* findAllRegionBottomUp(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int sensibility)
{
	// Use to store all IdRegion
	IdRegions* idRegions = initIdRegions(1);
	// Use to store the idRegions while realocating memories
	IdRegions* tempIdRegions = NULL;
	// Use to store the IdRegion fo the region found
	IdRegion* tempIdRegion = NULL;
	int i, j;
	int k;
	// Use to know the number of region found
	int nbrRegion = 0;
	
	// For each pixels
	for (i = 0; i < tabRegion->largeurImage; i++)
	{
		for (j = 0; j < tabRegion->hauteurImage; j++)
		{
			// If it is empty (valid starting point)
			if (tabRegion->donneesTab[i][j][BLUE] == UNCHECKED &&
				tabRegion->donneesTab[i][j][GREEN] == UNCHECKED &&
				tabRegion->donneesTab[i][j][RED] == UNCHECKED)
			{
				// We find the region which contain the starting point
				tempIdRegion = findRegionBottomUp(tabImage, tabRegion, i, j, sensibility);
				// We say that we find one more region
				nbrRegion++;
				
				// If we can save it in idRegions
				if (idRegions->size >= nbrRegion)
				{
					// We put it in the list of idRegion
					idRegions->regions[nbrRegion - 1] = tempIdRegion;
				}
				// If not
				else
				{
					// We realoc idRegions
					// First by initializing an IdRegions which will be use to transfer the value
					tempIdRegions = initIdRegions(nbrRegion);
					// We copy everything into the previously initialized IdRegion
					for(k = 0; k < idRegions->size; k++)
					{
						tempIdRegions->regions[k] = idRegions->regions[k];
						idRegions->regions[k] = NULL;
					}
					// We reInitialize idRegions
					destructIdRegions(&idRegions);
					idRegions = initIdRegions(nbrRegion);
					// Then, we put back all the value
					for(k = 0; k < idRegions->size; k++)
					{
						idRegions->regions[k] = tempIdRegions->regions[k];
						tempIdRegions->regions[k] = NULL;
					}
					// And we destruct the temporary IdRegions
					destructIdRegions(&tempIdRegions);
					
					// Finaly, we add the found region
					idRegions->regions[nbrRegion - 1] = tempIdRegion;
				}
			}
		}
	}
	// And we return the list of idRegion
	return idRegions;
}

IdRegions* findRegionFlow(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int sensibility)
{
	int i, j;
	int k;
	// We find the max color norm value in the tabImage
	int max = 0;
	int colorNorm;
	for (i = 0; i < tabImage->largeurImage; i++)
	{
		for (j = 0; j < tabImage->hauteurImage; j++)
		{
			colorNorm = pow(tabImage->donneesTab[i][j][BLUE], 2) + 
				pow(tabImage->donneesTab[i][j][GREEN], 2) +
				pow(tabImage->donneesTab[i][j][RED], 2);
			if (colorNorm > max)
			{
				max = colorNorm;
			}
		}
	}
	
	// We initialize the flowLevel 
	int flowLevel = sqrt(max);
	// The label wich will be use to set the regions
	int label = 0;
	// use to save all the idRegion
	IdRegions* idRegions = initIdRegions(1);
	IdRegions* tempIdRegions = NULL;
	IdRegion* tempIdRegion = NULL;
	int nbrRegion = 0;
	// We start a first loop that will last until we flooded everything
	while(flowLevel >= 0)
	{
		// For each pixels
		for (i = 0; i < tabRegion->largeurImage; i++)
		{
			for (j = 0; j < tabRegion->hauteurImage; j++)
			{
				// We calculate the norm of the pixel
				colorNorm = sqrt(pow(tabImage->donneesTab[i][j][BLUE], 2) + 
					pow(tabImage->donneesTab[i][j][GREEN], 2) +
					pow(tabImage->donneesTab[i][j][RED], 2));
				// If we are in en empty pixel and if it is at the flow level
				if (tabRegion->donneesTab[i][j][BLUE] == UNCHECKED &&
					tabRegion->donneesTab[i][j][GREEN] == UNCHECKED &&
					tabRegion->donneesTab[i][j][RED] == UNCHECKED &&
					absValue(flowLevel - colorNorm) < sensibility)
				{
					// We check the color of the neighbours
					tempIdRegion = whatIsNeighboorsColor(tabRegion, i, j);
					// If didn't found any neighbours
					if(tempIdRegion == NULL)
					{
						// We color the pixel with the label of a new region.
						tabRegion->donneesTab[i][j][BLUE] = label;
						tabRegion->donneesTab[i][j][GREEN] = label;
						tabRegion->donneesTab[i][j][RED] = label;
						// We initialize an idRegion if it as not been yet
						if (tempIdRegion == NULL)
						{
							tempIdRegion = initIdRegion(
								label,
								label,
								label,
								-1,
								-1);
						}
						// We increase the total number of region found
						nbrRegion++;
						// If we can save it in idRegions
						if (idRegions->size >= nbrRegion)
						{
							// We put it in the list of idRegion
							idRegions->regions[nbrRegion - 1] = tempIdRegion;
						}
						// If not
						else
						{
							// We realoc idRegions
							// First by initializing an IdRegions which will be use to transfer the value
							tempIdRegions = initIdRegions(nbrRegion);
							// We copy everything into the previously initialized IdRegion
							for(k = 0; k < idRegions->size; k++)
							{
								tempIdRegions->regions[k] = idRegions->regions[k];
								idRegions->regions[k] = NULL;
							}
							// We reInitialize idRegions
							destructIdRegions(&idRegions);
							idRegions = initIdRegions(nbrRegion);
							// Then, we put back all the value
							for(k = 0; k < idRegions->size; k++)
							{
								idRegions->regions[k] = tempIdRegions->regions[k];
								tempIdRegions->regions[k] = NULL;
							}
							// And we destruct the temporary IdRegions
							destructIdRegions(&tempIdRegions);
							
							// Finaly, we add the found region
							idRegions->regions[nbrRegion - 1] = tempIdRegion;
						}
						// Then we update the label for the next region
						label = (label + 20)%255;
						// And we say that the idRegion can be reuse
						tempIdRegion = NULL;
					}
					// If we found a neighbours
					else
					{
						// We color the pixel with the same label
						tabRegion->donneesTab[i][j][BLUE] = tempIdRegion->blue;
						tabRegion->donneesTab[i][j][GREEN] = tempIdRegion->green;
						tabRegion->donneesTab[i][j][RED] = tempIdRegion->red;
					}
				}
			}
		}
		// each loop, we lower the flood level
		flowLevel--;
	}
	// In the end, we return the list of idRegion
	return idRegions;
}

IdRegion* whatIsNeighboorsColor(DonneesImageTab* tabRegion, int x, int y)
{
	int i, j;
	IdRegion* idRegion = NULL;
	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			if (0 <= x + i && x + i < tabRegion->largeurImage &&
				0 <= y + j && y + j < tabRegion->hauteurImage &&
				tabRegion->donneesTab[x + i][y + j][BLUE] != UNCHECKED &&
				tabRegion->donneesTab[x + i][y + j][GREEN] != UNCHECKED &&
				tabRegion->donneesTab[x + i][y + j][RED] != UNCHECKED &&
				idRegion == NULL)
			{
				idRegion = malloc(sizeof(IdRegion));
				idRegion->blue = tabRegion->donneesTab[x + i][y + j][BLUE];
				idRegion->green = tabRegion->donneesTab[x + i][y + j][GREEN];
				idRegion->red = tabRegion->donneesTab[x + i][y + j][RED];
				idRegion->x = x + i;
				idRegion->y = y + j;
			}
		}
	}
	return idRegion;
}

//====================================================================================




//====================================================================================

//Fonction qui retourne une image de type DonneesImageRGB
//Cette image représente les regions de l'image en type DonneesImageRGB passée en paramètre
DonneesImageRGB* creerImageRegion(DonneesImageRGB *image, tableauRegion tableau)
{			
	//Déclaration des variables utiles aux traitements
	int hauteur = image->hauteurImage;
	int largeur = image->largeurImage;
	germe *unGerme = NULL;
	int hauteurGerme;
	int largeurGerme;
	int compteurRegion = 0;
		
	//Déclaration de notre image de traitement
	DonneesImageTab *imageTraitement = RGBToTab(image);
	
	//Déclaration de notre image de retour
	DonneesImageTab *retour = initTabRegion(largeur, hauteur, 255);
	
	//On cherche une germe et on la récupère
	unGerme = chercheGermeImagePixel(retour, imageTraitement, 0, 0);
		
	//Tant qu'il y a une nouvelle germe
	while(unGerme->set != false)
	{		
		//On colore la région
		coloriseVoisinsRegionImagePixel(retour, imageTraitement, unGerme->hauteurGerme, unGerme->largeurGerme, unGerme->rouge, unGerme->vert, unGerme->bleu,
										compteurRegion, tableau);
		
		//On cherche une nouvelle région
		hauteurGerme = unGerme->hauteurGerme;
		largeurGerme = unGerme->largeurGerme;		
		free(unGerme);
		unGerme = chercheGermeImagePixel(retour, imageTraitement, hauteurGerme, largeurGerme);
		
		compteurRegion++;
	}
	
	//On interprete les régions sur l'image résultante
	interpreteRegionImagePixel(retour);
		
	//Libération de mémoire
	libereDonneesTab(&imageTraitement);
		
	//On retourne notre image avec les regions en type DonneesImageRGB
	return(tabToRGB(retour));	
}


//Fonction qui cherche une germe dans une image en type imagePixel différente de la dernière germe
germe* chercheGermeImagePixel(DonneesImageTab *imageRetour, DonneesImageTab *imageTraitement, int hauteur, int largeur)
{
	//Déclaration des variables utiles pour les boucles while
	int i = 0;
	int j = 0;
	bool premiereLigne = true;
	
	//Déclaration de notre germe de retour
	germe *retour = (germe*)malloc(sizeof(germe));
	
	i = hauteur;
	//On cherche le premier point différent de blanc(255, 255, 255)
	while(i < imageRetour->hauteurImage)
	{
		if(premiereLigne == true)
		{
			j = largeur;
			premiereLigne = false;
		}
		else
		{
			j = 0;
		}
		while(j < imageRetour->largeurImage)
		{
			if( (imageRetour->donneesTab[i][j][RED] >= 0) && (imageRetour->donneesTab[i][j][GREEN] >= 0) && (imageRetour->donneesTab[i][j][BLUE] >= 0))
			{				
				//On initialise nos coordonnées du germe
				retour->hauteurGerme = i;
				retour->largeurGerme = j;
				retour->set = true;
				retour->rouge = imageTraitement->donneesTab[i][j][RED];
				retour->vert = imageTraitement->donneesTab[i][j][GREEN];
				retour->bleu = imageTraitement->donneesTab[i][j][BLUE];
				
				return(retour);
			}
			j++;
		}
		i++;
	}
	
	//Si on n'a pas de nouvelle germe, on l'indique via "set"
	if( (i >= imageRetour->hauteurImage) && (j >= imageRetour->largeurImage) )
	{
		retour->hauteurGerme = -1;
		retour->largeurGerme = -1;
		retour->set = false;
		return(retour);
	}
	
	return(NULL);
}


//Fonction qui colorise les voisins pour une région
//Et l'image de retour et l'image de donnees
void coloriseVoisinsRegionImagePixel(DonneesImageTab *retour, DonneesImageTab *image, int hauteurGerme, int largeurGerme, short int rouge, short int vert, short int bleu, 
									int compteur, tableauRegion tableau)
{
	//Déclaration des variables pour les boucles for
	int i = 0;
	int j = 0;
	
	//Déclaration des variables utiles au traitement
	int hauteurImage = retour->hauteurImage;
	int largeurImage = retour->largeurImage;
	float normeCouleurRegion = sqrt( pow(rouge, 2) + pow(vert, 2) + pow(bleu, 2) );
	float normeCouleurImage;
	
	//Si on a les bonnes conditions
	if( (hauteurGerme >= 0) && (hauteurGerme < hauteurImage) && (largeurGerme >= 0) && (largeurGerme < largeurImage) )
	{
		if ( (retour->donneesTab[hauteurGerme][largeurGerme][RED] != (-rouge)-1)
			&& (retour->donneesTab[hauteurGerme][largeurGerme][GREEN] != (-vert)-1)
			&& (retour->donneesTab[hauteurGerme][largeurGerme][BLUE] != (-bleu)-1) )
		{
			normeCouleurImage = sqrt( pow(image->donneesTab[hauteurGerme][largeurGerme][RED], 2) 
										+ pow(image->donneesTab[hauteurGerme][largeurGerme][GREEN], 2) 
										+ pow(image->donneesTab[hauteurGerme][largeurGerme][BLUE], 2) );
			
			//Si on est toujours dans la région
			if( (normeCouleurImage < normeCouleurRegion + ACUITE)
				&& (normeCouleurImage > normeCouleurRegion - ACUITE) )
			{
				
				//Traitement
				//On colore le germe
				retour->donneesTab[hauteurGerme][largeurGerme][RED] = (-rouge)-1;
				retour->donneesTab[hauteurGerme][largeurGerme][GREEN] = (-vert)-1;
				retour->donneesTab[hauteurGerme][largeurGerme][BLUE] = (-bleu)-1;
				
				//On colore l'image de la region
				tableau[compteur].donneesTab[hauteurGerme][largeurGerme][RED] = rouge;
				tableau[compteur].donneesTab[hauteurGerme][largeurGerme][GREEN] = vert;
				tableau[compteur].donneesTab[hauteurGerme][largeurGerme][BLUE] = bleu;
				
				//On colore les voisins
				for(i = hauteurGerme-1; i <= hauteurGerme+1; i++)
				{
					for(j = largeurGerme-1; j <= largeurGerme+1; j++)
					{
						coloriseVoisinsRegionImagePixel(retour, image, i, j, rouge, vert, bleu, compteur, tableau);
					}
				}
			}
		}
	}
	else
	{
		return;
	}
}

//Fonction qui interprete les regions sur une image de type image Pixel
//car durant le traitement les regions sont en valeurs négatives
void interpreteRegionImagePixel(DonneesImageTab *retour)
{
	//Déclaration des variables pour les boucles for
	int i;
	int j;
	
	//Traitement
	for(i = 0; i < retour->hauteurImage; i++)
	{
		for(j = 0; j < retour->largeurImage; j++)
		{
			retour->donneesTab[i][j][RED] = (-retour->donneesTab[i][j][RED])-1;
			retour->donneesTab[i][j][GREEN] = (-retour->donneesTab[i][j][GREEN])-1;
			retour->donneesTab[i][j][BLUE] = (-retour->donneesTab[i][j][BLUE])-1;
		}
	}	
}

//Fonction qui créer et initialise un tableauRegion
void initTableauRegion(tableauRegion tableau, int hauteur, int largeur, int taille)
{	
	//Déclaration des variables utiles aux traitements
	int i;

	//On remplie notre tableau d'image blanche 400x400
	for(i = 0; i < taille; i++)
	{
		tableau[i] = *initTabRegion(largeur, hauteur, 255);
	}
}

//Fonction qui écrit dans le répertoire courant les images regions séparément
//Renvoie le nombre de régions
unsigned int ecrisRegions(tableauRegion regions, char *nom, int taille)
{
	int i;
	char bufferNom[50];
	char bufferNum[5];
	unsigned int nbRegion = 0;
	
	for(i = 0; i < taille; i++)
	{
		if(evalueRegionsVide(regions, i) != true)
		{
			nbRegion++;
			
			sprintf(bufferNum, "%d", i);
			strcpy(bufferNom, "regions/");
			strcat(bufferNom, bufferNum);
			strcat(bufferNom, nom);
			ecrisBMPRGB_Dans(tabToRGB(&regions[i]), bufferNom);
		}
	}
	
	return(nbRegion);
}

//Fonction qui regarde dans le tableau de régions, si à l'index donné, l'image est vide (255,255,255)
bool evalueRegionsVide(tableauRegion regions, int index)
{
	int i;
	int j;
	int compteur = 0;
	
	for(i = 0; i < regions[index].hauteurImage; i++)
	{
		for(j = 0; j < regions[index].largeurImage; j++)
		{
			if(regions[index].donneesTab[i][j][RED] == 255)
			{
				compteur++;
			}
		}
	}
	
	if(compteur == (regions[index].hauteurImage * regions[index].largeurImage) )
	{
		return(true);
	}
	else
	{
		return(false);
	}
}	
