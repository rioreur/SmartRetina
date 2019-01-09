#include "region.h"

DonneesImageTab* initTabRegion(int width, int height)
{
	// We initialize a DonneesImageTab using the initTab function
	DonneesImageTab* tabRegion = initTab(width, height);
	// Then we put all the value in it at UNCHECKED (-2)
	int i, j, cIndex;
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			for(cIndex = 0; cIndex < 3; cIndex++)
			{
				tabRegion->donneesTab[i][j][cIndex] = UNCHECKED;
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

Line* getMaxLineFromRegion(DonneesImageTab* tabHough, DonneesImageTab* tabRegion, IdRegion* idRegion)
{
    int i, j;
	Line* maxLine = malloc(sizeof(Line));
	maxLine->angularIndex = -1;
	maxLine->rIndex = -1;
	maxLine->maxRIndex = tabHough->hauteurImage;
	maxLine->maxAngularIndex = tabHough->largeurImage;
	maxLine->startX = -1;
	maxLine->startY = -1;
	maxLine->endX = -1;
	maxLine->endY = -1;
	maxLine->lenght = 0;
	maxLine->lenghtRatio = 0;
	int max = 0;
	// For each pixel in the Hough matrice
	for(i = 0; i < tabHough->largeurImage; i++)
	{
		for(j = 0; j < tabHough->hauteurImage; j++)
		{
			if (tabHough->donneesTab[i][j][BLUE] +
			    tabHough->donneesTab[i][j][GREEN] +
			    tabHough->donneesTab[i][j][RED] > max &&
			    tabRegion->donneesTab[i][j][BLUE] == idRegion->blue &&
			    tabRegion->donneesTab[i][j][GREEN] == idRegion->green &&
			    tabRegion->donneesTab[i][j][RED] == idRegion->red)
			{
				max = tabHough->donneesTab[i][j][BLUE] +
				    tabHough->donneesTab[i][j][GREEN] +
				    tabHough->donneesTab[i][j][RED];
				maxLine->angularIndex = i;
				maxLine->rIndex = j;
			}
		}
	}
	if (max == 0)
	{
	    return NULL;
	}
    return maxLine;
}
