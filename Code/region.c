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

void findRegionTopDown(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int x, int y, int sensibility)
{
	// If the seed is in the image
	if (0 <= x && x <= tabImage->largeurImage && 
		0 <= y && y <= tabImage->largeurImage)
	{
		// We save the red, blue and green value of the seed
		int b = tabImage->donneesTab[x][y][BLUE];
		int g = tabImage->donneesTab[x][y][GREEN];
		int r = tabImage->donneesTab[x][y][RED];
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
							pow(tabImage->donneesTab[i][j][BLUE] - b, 2) + 
							pow(tabImage->donneesTab[i][j][GREEN] - g, 2) + 
							pow(tabImage->donneesTab[i][j][RED] - r, 2));
						// If it is the a color close enought (distance inferior to the sensibility)
						if(tempColor < sensibility)
						{
							// We add it to the region matrice
							tabRegion->donneesTab[i][j][BLUE] = b;
							tabRegion->donneesTab[i][j][GREEN] = g;
							tabRegion->donneesTab[i][j][RED] = r;
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
}
