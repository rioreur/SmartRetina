#include "symetrie.h"

DonneesImageTab* getShape(DonneesImageTab* tabRegion,  IdRegion* idRegion)
{
	int i, j;
	DonneesImageTab* tabShape = NULL;
	bool isInRegion = false;;
	// Getting the height of the region
	int startY = tabRegion->hauteurImage; // the maximum Y coordinate wich contain the shape
	int endY = -1; // the minimum Y coordinate wich contain the shape
	for (i = 0; i < tabRegion->largeurImage; i++)
	{
		isInRegion = false;
		for (j = 0; j < tabRegion->hauteurImage; j++)
		{
			// If we are in the region
			if (tabRegion->donneesTab[i][j][BLUE] == idRegion->label && 
				tabRegion->donneesTab[i][j][GREEN] == idRegion->label &&
				tabRegion->donneesTab[i][j][RED] == idRegion->label)
			{
				// We activate the corresponding flag
				isInRegion = true;
				// And we update the startY only if it is lower than the previous one
				if (startY > j)
				{
					startY = j;
				}
			}
			// If we are not in the region and if we where previously
			if (tabRegion->donneesTab[i][j][BLUE] != idRegion->label && 
				tabRegion->donneesTab[i][j][GREEN] != idRegion->label &&
				tabRegion->donneesTab[i][j][RED] != idRegion->label && 
				isInRegion)
			{
				// We deactivate the corresponding flag
				isInRegion = false;
				// And we update the endY only if it is higher than the previous one
				if (endY < j)
				{
					endY = j;
				}
			}
		}
	}
	
	// Getting the width of the region
	int startX = tabRegion->largeurImage;
	int endX = -1;
	for (j = 0; j < tabRegion->hauteurImage; j++)
	{
		isInRegion = false;
		for (i = 0; i < tabRegion->largeurImage; i++)
		{
			// If we are in the region
			if (tabRegion->donneesTab[i][j][BLUE] == idRegion->label && 
				tabRegion->donneesTab[i][j][GREEN] == idRegion->label &&
				tabRegion->donneesTab[i][j][RED] == idRegion->label)
			{
				// We activate the corresponding flag
				isInRegion = true;
				// And we update the startX only if it is lower than the previous one
				if (startX > i)
				{
					startX = i;
				}
			}
			// If we are not in the region and if we where previously
			if (tabRegion->donneesTab[i][j][BLUE] != idRegion->label && 
				tabRegion->donneesTab[i][j][GREEN] != idRegion->label &&
				tabRegion->donneesTab[i][j][RED] != idRegion->label && 
				isInRegion)
			{
				// We deactivate the corresponding flag
				isInRegion = false;
				// And we update the endX only if it is higher than the previous one
				if (endX < i)
				{
					endX = i;
				}
			}
		}
	}

	// If we have found the width and the height of the region
	if (endX >= 0 && endY >= 0 && startX >= 0 && startY >= 0)
	{
		// We initialise the DonnesImageTab with the corect lenght
		tabShape = initTab(endX - startX, endY-startY);
		// And we fill the region in
		for (i = 0; i < tabShape->largeurImage; i++)
		{
			for (j = 0; j < tabShape->hauteurImage; j++)
			{
				if (tabRegion->donneesTab[i + startX][j + startY][BLUE] == idRegion->label && 
					tabRegion->donneesTab[i + startX][j + startY][GREEN] == idRegion->label &&
					tabRegion->donneesTab[i + startX][j + startY][RED] == idRegion->label)
				{
					tabShape->donneesTab[i][j][BLUE] = idRegion->blue;
					tabShape->donneesTab[i][j][GREEN] = idRegion->green;
					tabShape->donneesTab[i][j][RED] = idRegion->red;
				}
				else
				{
					tabShape->donneesTab[i][j][BLUE] = UNCHECKED;
					tabShape->donneesTab[i][j][GREEN] = UNCHECKED;
					tabShape->donneesTab[i][j][RED] = UNCHECKED;
				}
			}
		}
		
	}
	return tabShape;
}

DonneesImageTab* getSymetricShape(DonneesImageTab* tabShape)
{
	int i, j, cIndex;
	DonneesImageTab* tabSymetricShape = initTab(tabShape->largeurImage, tabShape->hauteurImage);
	for (i = 0; i < tabShape->largeurImage; i++)
	{
		for (j = 0; j < tabShape->hauteurImage; j++)
		{
			for (cIndex = 0; cIndex < 3; cIndex ++)
			{
				tabSymetricShape->donneesTab[tabShape->largeurImage - i - 1]
					[tabShape->hauteurImage - j - 1]
					[cIndex]
				 = tabShape->donneesTab[i][j][cIndex];
			}
		}
	}
	return tabSymetricShape;
}

DonneesImageTab* createGlobalShape(DonneesImageTab* tabShape, DonneesImageTab* tabSymetricShape)
{
	int i, j;
	int startX = -1;
	int startY = -1;
	// This DonneesImageTab is used to create and store the border of the given shape
	DonneesImageTab* tabBorder = initTabRegion(tabShape->largeurImage, tabShape->hauteurImage);
	// This DonneesImageTab is used to create and store the gobalShape
	DonneesImageTab* tabGlobalShape = initTabRegion(tabShape->largeurImage*2, tabShape->hauteurImage*2);
	
	// We search for a starting point
	// For each pixel in the tabShape
	for (i = 0; i < tabShape->largeurImage; i++)
	{
		for (j = 0; j < tabShape->hauteurImage; j++)
		{
			// If we find a pixel which is a border
			if (isABorder(tabShape, i, j))
			{
				// We save the point
				startX = i;
				startY = j;
			}
		}
	}
	// If we have found a starting point
	if (startX >= 0 && startY >= 0)
	{
		// use to store the current pixel border. We make it start at the coordinate we previously found
		int currentX = startX;
		int currentY = startY;
		// Use to store the next pixel border
		int nextX;
		int nextY;
		// We start a loop
		do
		{
			// We say that the next pixel in the border isn't define yet
			nextX = -1;
			nextY = -1;
			// For each neighbours
			for (i = -1; i <= 1; i++)
			{
				for (j = -1; j <= 1; j++)
				{
					// If it is in the tabShape
					//and we still haven't found the next border pixel
					//and the neighbours is an empty pixel
					//and if it is a border pixel
					if (0 <= currentX + i && currentX + i < tabShape->largeurImage &&
						0 <= currentY + j && currentY + j < tabShape->hauteurImage &&
						nextX == -1 &&
						nextY == -1 &&
						tabBorder->donneesTab[currentX + i][currentY + j][BLUE] == UNCHECKED &&
						tabBorder->donneesTab[currentX + i][currentY + j][GREEN] == UNCHECKED &&
						tabBorder->donneesTab[currentX + i][currentY + j][RED] == UNCHECKED &&
						isABorder(tabShape, currentX + i, currentY + j))
					{
						// We save the next border pixel
						nextX = currentX + i;
						nextY = currentY + j;
					}
				}
			}
			// We save the current pixel in the tabBorder
			tabBorder->donneesTab[currentX][currentY][BLUE] = BORDER;
			tabBorder->donneesTab[currentX][currentY][GREEN] = BORDER;
			tabBorder->donneesTab[currentX][currentY][RED] = BORDER;
			// Then we update the current pixel with the next coordonate we found
			currentX = nextX;
			currentY = nextY;
		// We stop if we didn't found a new valid border or if we just get back to the starting point
		} while(nextX != -1 && nextY != -1 && (nextX != startX || nextY != startY));
	}
	
	// Then, for each pixels
	for (i = 0; i < tabShape->largeurImage; i++)
	{
		for (j = 0; j < tabShape->hauteurImage; j++)
		{
			// If it is a border pixel
			if (tabBorder->donneesTab[i][j][BLUE] == BORDER &&
				tabBorder->donneesTab[i][j][GREEN] == BORDER &&
				tabBorder->donneesTab[i][j][RED] == BORDER)
			{
				// We draw the shape at each pixel of the border
				drawShape(tabGlobalShape, tabSymetricShape, i, j);
			}
		}
	}
	// We free the DonnesImageTab used to store the border
	libereDonneesTab(&tabBorder);
	// Then we return the global shape
	return tabGlobalShape;
}

bool isABorder(DonneesImageTab* tabShape, int x, int y)
{
	bool areEmptyNeighbours = false;
	// If the pixel to the right is empty
	if (x + 1 >= tabShape->largeurImage ||
		(tabShape->donneesTab[x + 1][y][BLUE] == UNCHECKED &&
		tabShape->donneesTab[x + 1][y][GREEN] == UNCHECKED &&
		tabShape->donneesTab[x + 1][y][RED] == UNCHECKED))
	{
		// We say that their is neighbours
		areEmptyNeighbours = true;
	}
	// If the pixel to the left is empty
	if (0 > x - 1 ||
		(tabShape->donneesTab[x - 1][y][BLUE] == UNCHECKED &&
		tabShape->donneesTab[x - 1][y][GREEN] == UNCHECKED &&
		tabShape->donneesTab[x - 1][y][RED] == UNCHECKED))
	{
		// We say that their is neighbours
		areEmptyNeighbours = true;
	}
	// If the pixel to the bottom is empty
	if (y + 1 >= tabShape->hauteurImage ||
		(tabShape->donneesTab[x][y + 1][BLUE] == UNCHECKED &&
		tabShape->donneesTab[x][y + 1][GREEN] == UNCHECKED &&
		tabShape->donneesTab[x][y + 1][RED] == UNCHECKED))
	{
		// We say that their is neighbours
		areEmptyNeighbours = true;
	}
	// If the pixel to the top is empty
	if (0 > y - 1 ||
		(tabShape->donneesTab[x][y - 1][BLUE] == UNCHECKED &&
		tabShape->donneesTab[x][y - 1][GREEN] == UNCHECKED &&
		tabShape->donneesTab[x][y - 1][RED] == UNCHECKED))
	{
		// We say that their is neighbours
		areEmptyNeighbours = true;
	}
	// If their is empty neighbours and if the pixel isn't empty
	if (tabShape->donneesTab[x][y][BLUE] != UNCHECKED &&
		tabShape->donneesTab[x][y][GREEN] != UNCHECKED &&
		tabShape->donneesTab[x][y][RED] != UNCHECKED &&
		areEmptyNeighbours)
	{
		// Then we are on a border
		return true;
	}
	// Otherwise, we're not
	return false;
}

void drawShape(DonneesImageTab* tabImage, DonneesImageTab* tabShape, int x, int y)
{
	int i, j;
	// For each pixel in the tabShape
	for (i = 0; i < tabShape->largeurImage; i++)
	{
		for (j = 0; j < tabShape->hauteurImage; j++)
		{
			// If it can be copied to the tabImage
			//and if it is not empty
			if (0 <= x + i && x + i < tabImage->largeurImage &&
				0 <= y + j && y + j < tabImage->hauteurImage &&
				tabShape->donneesTab[i][j][BLUE] != UNCHECKED &&
				tabShape->donneesTab[i][j][GREEN] != UNCHECKED &&
				tabShape->donneesTab[i][j][RED] != UNCHECKED)
			{
				// We copy it to the tab
				tabImage->donneesTab[x + i][y + j][BLUE] = tabShape->donneesTab[i][j][BLUE];
				tabImage->donneesTab[x + i][y + j][GREEN] = tabShape->donneesTab[i][j][GREEN];
				tabImage->donneesTab[x + i][y + j][RED] = tabShape->donneesTab[i][j][RED];
			}
		}
	}
}

int getArea(DonneesImageTab* tabShape)
{
	int i, j;
	// use to store the area of the shape
	int area = 0;
	// For each pixel in the tabShape
	for (i = 0; i < tabShape->largeurImage; i++)
	{
		for (j = 0; j < tabShape->hauteurImage; j++)
		{
			// If it is not an empty pixel
			if (tabShape->donneesTab[i][j][BLUE] != UNCHECKED &&
				tabShape->donneesTab[i][j][GREEN] != UNCHECKED &&
				tabShape->donneesTab[i][j][RED] != UNCHECKED)
			{
				// We increase the area
				area++;
			}
		}
	}
	return area;
}

void updateCoefGravityPoint(Point** gravityPoints, DonneesImageTab* tabRegion, IdRegions* allIds)
{
    int i, j;
    int pointIndex = -1;
    DonneesImageTab* tabShape = NULL;
    DonneesImageTab* tabSymetricShape = NULL;
    DonneesImageTab* tabGlobal = NULL;
    for(i = 1; i < allIds->size; i++)
    {
        // We get the shape of the current region
	    tabShape = getShape(tabRegion, allIds->regions[i]);
	    // We create his symetric
	    tabSymetricShape = getSymetricShape(tabShape);
	    // We create the global shape using the addition of Minkovski
	    tabGlobal = createGlobalShape(tabShape, tabSymetricShape);
	    for(j = 0; j < allIds->size-1; j++)
	    {
	        if (gravityPoints[j]->label == allIds->regions[i]->label)
	        {
	            pointIndex = j;
	        }
	    }
	    if (pointIndex != -1)
	    {
	        gravityPoints[pointIndex]->coef = ((float) getArea(tabShape))/((float) getArea(tabGlobal)/4);
	        pointIndex = -1;
	    }
	
	    libereDonneesTab(&tabShape);
	    libereDonneesTab(&tabSymetricShape);
	    libereDonneesTab(&tabGlobal);
	}
}

DonneesImageTab* createTabAxis(DonneesImageTab* tabImage, int sensibility, int step)
{
    // Use in the for loop
	int x1, y1, x2, y2, cIndex;
	// Use to identify a line. "r" is the distance from the line to the origin in pixels and "a" his angle in degree
	int r, a;
	// use for the equation of the axis (y = ax+p)
	float m;
	float p;
	// We calculate the maximum r value possible (distance to the origine)
	float maxR = sqrt(pow(tabImage->largeurImage, 2) + pow(tabImage->hauteurImage, 2));
	// Use to calculate "a", temporary variable
	float angle;
	// Coordinate on the axis wich will be calculated each time
	float x, y;
	// We init the tab that will contain the Hough transform. We use maxR*2 to save the negative values
	DonneesImageTab* tabAxis = initTab(720, maxR*2);
	
	// For each pixel on our image
	for(x1 = 0; x1 < tabImage->largeurImage; x1+=step)
	{
		for(y1 = 0; y1 < tabImage->hauteurImage; y1+=step)
		{
			// And for each color
			for (cIndex = 0; cIndex < 3; cIndex++)
			{
				// If the pixel is concidered usefull (superior to the given sensibility)
				if (tabImage->donneesTab[x1][y1][cIndex] > sensibility)
				{
					// And for each second pixel on our image
	                for(x2 = 0; x2 < tabImage->largeurImage; x2+=step)
	                {
		                for(y2 = 0; y2 < tabImage->hauteurImage; y2+=step)
		                {
		                    // We check for future division by 0
		                    if (y1 != y2 && x1 != x2)
		                    {
		                        ////////////////////////////
		                        //--- c'est dégeulasse ---//
		                        ////////////////////////////
		                        
		                        // We find a point (x, y) wich is on the perpendicular bisector (our symmetrical axis)
		                        x = ((float) (x1 + x2))/2;
		                        y = ((float) (y1 + y2))/2;
		                        // We find the y = mx + p equation of this line
		                        m = ((float) (x1 - x2)) / ((float) (y2 - y1));
		                        p = x / (m * y);
		                        // We calculate the distance from the line to the center
		                        r = (int) (p/sqrt(1 + pow(m, 2)));
		                        if (r >= maxR)
		                        {
		                            r = maxR-1;
		                        }
		                        if (r <= -maxR)
		                        {
		                            r = -maxR+1;
		                        }
		                        // Then we calculate the angle to the center
		                        angle = atan2(y2 - y1, x2 - x1);
		                        if (angle < 0)
		                        {
		                            angle += M_PI;
		                        }
		                        a = (int) nmap(angle, 0, M_PI, 0, 720);
						    }
						    else if (y1 == y2)
						    {
		                        // We calculate the distance from the line to the center
		                        r = ((float) (x1 + x2))/2;
		                        // Then we calculate the angle to the center
		                        a = 0;
						    }
						    else
						    {
						        // We calculate the distance from the line to the center
						        r = ((float) (y1 + y2))/2;
						        // Then we calculate the angle to the center
						        a = 360; // this one is 90° because we have a maximum of 720 différent angle
						    }
						    //printf("%d, %d\n", a, r);
						    // We increase the corresponding value in our Hough table
						    tabAxis->donneesTab[a][(int) (r + maxR)][cIndex]++;
		                }
		            }
				}
			}
		}
	}
	// We return the DonneesImageTab wich contain all the information on the most probable axis
	return tabAxis;
}

bool isARegionBorder(DonneesImageTab* tabRegion, IdRegion* idRegion, int x, int y)
{
	bool areEmptyNeighbours = false;
	// If the pixel to the right is empty
	if (x + 1 >= tabRegion->largeurImage ||
		(tabRegion->donneesTab[x + 1][y][BLUE] != idRegion->label &&
		tabRegion->donneesTab[x + 1][y][GREEN] != idRegion->label &&
		tabRegion->donneesTab[x + 1][y][RED] != idRegion->label))
	{
		// We say that their is neighbours
		areEmptyNeighbours = true;
	}
	// If the pixel to the left is empty
	if (0 > x - 1 ||
		(tabRegion->donneesTab[x - 1][y][BLUE] != idRegion->label &&
		tabRegion->donneesTab[x - 1][y][GREEN] != idRegion->label &&
		tabRegion->donneesTab[x - 1][y][RED] != idRegion->label))
	{
		// We say that their is neighbours
		areEmptyNeighbours = true;
	}
	// If the pixel to the bottom is empty
	if (y + 1 >= tabRegion->hauteurImage ||
		(tabRegion->donneesTab[x][y + 1][BLUE] != idRegion->label &&
		tabRegion->donneesTab[x][y + 1][GREEN] != idRegion->label &&
		tabRegion->donneesTab[x][y + 1][RED] != idRegion->label))
	{
		// We say that their is neighbours
		areEmptyNeighbours = true;
	}
	// If the pixel to the top is empty
	if (0 > y - 1 ||
		(tabRegion->donneesTab[x][y - 1][BLUE] != idRegion->label &&
		tabRegion->donneesTab[x][y - 1][GREEN] != idRegion->label &&
		tabRegion->donneesTab[x][y - 1][RED] != idRegion->label))
	{
		// We say that their is neighbours
		areEmptyNeighbours = true;
	}
	// If their is empty neighbours and if the pixel isn't empty
	if (tabRegion->donneesTab[x][y][BLUE] == idRegion->label &&
		tabRegion->donneesTab[x][y][GREEN] == idRegion->label &&
		tabRegion->donneesTab[x][y][RED] == idRegion->label &&
		areEmptyNeighbours)
	{
		// Then we are on a border
		return true;
	}
	// Otherwise, we're not
	return false;
}

DonneesImageTab* getShapeEdge(DonneesImageTab* tabRegion, IdRegion* idRegion)
{
	int i, j;
	int startX = idRegion->x;
	int startY = idRegion->y;
	// This DonneesImageTab is used to create and store the border of the given shape
	DonneesImageTab* tabBorder = initTabRegion(tabRegion->largeurImage, tabRegion->hauteurImage);
	
	// We search for a starting point
	// For each pixel in the tabShape
	for (i = 0; i < tabRegion->largeurImage; i++)
	{
		for (j = 0; j < tabRegion->hauteurImage; j++)
		{
			// If we find a pixel which is a border
			if (isARegionBorder(tabRegion, idRegion, i, j))
			{
				// We save the point
				startX = i;
				startY = j;
			}
		}
	}
	// If we have found a starting point
	if (startX >= 0 && startY >= 0)
	{
		// use to store the current pixel border. We make it start at the coordinate we previously found
		int currentX = startX;
		int currentY = startY;
		// Use to store the next pixel border
		int nextX;
		int nextY;
		// We start a loop
		do
		{
			// We say that the next pixel in the border isn't define yet
			nextX = -1;
			nextY = -1;
			// For each neighbours
			for (i = -1; i <= 1; i++)
			{
				for (j = -1; j <= 1; j++)
				{
					// If it is in the tabShape
					//and we still haven't found the next border pixel
					//and the neighbours is an empty pixel
					//and if it is a border pixel
					if (0 <= currentX + i && currentX + i < tabRegion->largeurImage &&
						0 <= currentY + j && currentY + j < tabRegion->hauteurImage &&
						nextX == -1 &&
						nextY == -1 &&
						tabBorder->donneesTab[currentX + i][currentY + j][BLUE] == UNCHECKED &&
						tabBorder->donneesTab[currentX + i][currentY + j][GREEN] == UNCHECKED &&
						tabBorder->donneesTab[currentX + i][currentY + j][RED] == UNCHECKED &&
						isARegionBorder(tabRegion, idRegion, currentX + i, currentY + j))
					{
						// We save the next border pixel
						nextX = currentX + i;
						nextY = currentY + j;
					}
				}
			}
			// We save the current pixel in the tabBorder
			tabBorder->donneesTab[currentX][currentY][BLUE] = BORDER;
			tabBorder->donneesTab[currentX][currentY][GREEN] = BORDER;
			tabBorder->donneesTab[currentX][currentY][RED] = BORDER;
			// Then we update the current pixel with the next coordonate we found
			currentX = nextX;
			currentY = nextY;
		// We stop if we didn't found a new valid border or if we just get back to the starting point
		} while(nextX != -1 && nextY != -1 && (nextX != startX || nextY != startY));
	}
	
	// Then, for each pixels
	for (i = 0; i < tabRegion->largeurImage; i++)
	{
		for (j = 0; j < tabRegion->hauteurImage; j++)
		{
			// If it is a border pixel
			if (tabBorder->donneesTab[i][j][BLUE] == BORDER &&
				tabBorder->donneesTab[i][j][GREEN] == BORDER &&
				tabBorder->donneesTab[i][j][RED] == BORDER)
			{
				// We draw the shape at each pixel of the border
				tabBorder->donneesTab[i][j][BLUE] = 255;
				// We draw the shape at each pixel of the border
				tabBorder->donneesTab[i][j][GREEN] = 255;
				// We draw the shape at each pisxel of the border
				tabBorder->donneesTab[i][j][RED] = 255;
			}
		}
	}
	// Then we return the global shape
	return tabBorder;
}
