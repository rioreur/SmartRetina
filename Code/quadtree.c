#include "quadtree.h"

Node* initNode(int x1, int y1, int x2, int y2)
{
	Node* node = malloc(sizeof(Node));
	node->color = T_NAC;
	node->label = -1;
	node->x1 = x1;
	node->y1 = y1;
	node->x2 = x2;
	node->y2 = y2;
	node->son1 = NULL;
	node->son2 = NULL;
	node->son3 = NULL;
	node->son4 = NULL;
	return node;
}

NodeRGB* initNodeRGB(int x1, int y1, int x2, int y2)
{
	NodeRGB* node = malloc(sizeof(NodeRGB));
	node->colorR = T_NAC;
	node->colorG = T_NAC;
	node->colorB = T_NAC;
	node->label = -1;
	node->x1 = x1;
	node->y1 = y1;
	node->x2 = x2;
	node->y2 = y2;
	node->son1 = NULL;
	node->son2 = NULL;
	node->son3 = NULL;
	node->son4 = NULL;
	return node;
}

void destructNode(Node** node)
{
	if (node != NULL)
	{
		if(*node != NULL)
		{
			destructNode(&(*node)->son1);
			destructNode(&(*node)->son2);
			destructNode(&(*node)->son3);
			destructNode(&(*node)->son4);
			free(*node);
			*node = NULL;
		}
	}
}
void destructNodeRGB(NodeRGB** node)
{
	if (node != NULL)
	{
		if(*node != NULL)
		{
			destructNodeRGB(&(*node)->son1);
			destructNodeRGB(&(*node)->son2);
			destructNodeRGB(&(*node)->son3);
			destructNodeRGB(&(*node)->son4);
			free(*node);
			*node = NULL;
		}
	}
}

void showTree(Node* node, int index)
{
	int i;
	if (node != NULL)
	{
		for (i = 0; i < index; i++)
		{
			printf("  ");
		}
		printf("%d, %d : (%d, %d) (%d, %d), c = %d\n",
			index,
			node->label,
			node->x1,
			node->y1,
			node->x2,
			node->y2,
			node->color);
		showTree(node->son1, index+1);
		showTree(node->son2, index+1);
		showTree(node->son3, index+1);
		showTree(node->son4, index+1);
	}
}

int calculCarreNB(DonneesImageTab* tabImage, int x1, int y1, int x2, int y2)
{
	int colorRef = T_NAC;
	if (x2 > x1 && y2 > y1)
	{
		int i, j;
		bool end = false;
		if (0 <= x1 && x1 < tabImage->largeurImage &&
			0 <= y1 && y1 < tabImage->hauteurImage)
		{
			colorRef = tabImage->donneesTab[x1][y1][BLUE];
		}
		i = x1;
		while(i <= x2 && end == false)
		{
			j = y1;
			while(j <= y2 && end == false)
			{
				if (0 <= i && i < tabImage->largeurImage &&
					0 <= j && j < tabImage->hauteurImage)
				{
					if (tabImage->donneesTab[i][j][BLUE] != colorRef)
					{
						colorRef = T_NAC;
						end = true;
					}
				}
				j++;
			}
			i++;
		}
	}
	switch(colorRef)
	{
		case 0: // Black
			colorRef = T_BLACK;
			break;
		case 255: // White
			colorRef = T_WHITE;
			break;
		default:
			colorRef = T_NAC;
			break;
	}
	return colorRef;
}

int calculCarreNG(DonneesImageTab* tabImage, int x1, int y1, int x2, int y2, int sensibility)
{
	int medianColor = T_NAC;
	if (x2 > x1 && y2 > y1)
	{
		int i, j;
		// We calulate the median of all the pixels in 
		int deadPixel = 0;
		int filter[(x2-x1 + 1)*(y2-y1 + 1)];
		for(i = x1; i <= x2; i++)
		{
			for(j = y1; j <= y2; j++)
			{
				if(0 <= i && i < tabImage->largeurImage &&
					0 <= j && j < tabImage->hauteurImage)
				{
					filter[(i-x1)*(y2-y1 + 1) + (j-y1) - deadPixel] = tabImage->donneesTab[i][j][BLUE];
				}
				else
				{
					deadPixel++;
				}
			}
		}
		
		qsort(filter, (x2-x1 + 1)*(y2-y1 + 1) - deadPixel, sizeof(int), pixelComp);
		medianColor = filter[(int) floor(((x2-x1 + 1)*(y2-y1 + 1) - deadPixel)/2)];
		
		// We check for each pixels of the defined square
		bool end = false;
		i = x1;
		while(i <= x2 && end == false)
		{
			j = y1;
			while(j <= y2 && end == false)
			{
				if (0 <= i && i < tabImage->largeurImage &&
					0 <= j && j < tabImage->hauteurImage)
				{
					// If it is in a certain range of our median color
					if (absValue(tabImage->donneesTab[i][j][BLUE] - medianColor) >= sensibility)
					{
						medianColor = T_NAC;
						end = true;
					}
				}
				j++;
			}
			i++;
		}
	}
	return medianColor;
}

void calculCarreRGB(DonneesImageTab* tabImage, NodeRGB* node, int sensibility)
{
	int x1 = node->x1;
	int x2 = node->x2;
	int y1 = node->y1;
	int y2 = node->y2;
	if (x2 > x1 && y2 > y1 &&
		0 <= x1 && x1 < tabImage->largeurImage &&
		0 <= y1 && y1 < tabImage->hauteurImage &&
		0 <= x2 && x2 < tabImage->largeurImage &&
		0 <= y2 && y2 < tabImage->hauteurImage)
	{
		int i, j, k, l;
		// We calulate the median of all the pixels in 
		int bestDistance = 200000; //theorical maximum is around 195 075 but it is not really important
		int currentDistance;
		int b = T_NAC;
		int g = T_NAC;
		int r = T_NAC;
		for(i = x1; i <= x2; i++)
		{
			for(j = y1; j <= y2; j++)
			{
				currentDistance = 0;
				for(k = x1; k <= x2; k++)
				{
					for(l = y1; l <= y2; l++)
					{
						currentDistance += 
							(pow(tabImage->donneesTab[i][j][BLUE] - tabImage->donneesTab[k][l][BLUE], 2) + 
							pow(tabImage->donneesTab[i][j][GREEN] - tabImage->donneesTab[k][l][GREEN], 2) +
							pow(tabImage->donneesTab[i][j][RED] - tabImage->donneesTab[k][l][RED], 2));
					}
				}
				if (currentDistance < bestDistance)
				{
					bestDistance = currentDistance;
					b = tabImage->donneesTab[i][j][BLUE];
					g = tabImage->donneesTab[i][j][GREEN];
					r = tabImage->donneesTab[i][j][RED];
				}
			}
		}
		
		node->colorB = b;
		node->colorG = g;
		node->colorR = r;
		// We check for each pixels of the defined square
		bool end = false;
		i = x1;
		while(i <= x2 && end == false)
		{
			j = y1;
			while(j <= y2 && end == false)
			{
				if (0 <= i && i < tabImage->largeurImage &&
					0 <= j && j < tabImage->hauteurImage)
				{
					currentDistance = sqrt(
						pow(tabImage->donneesTab[i][j][BLUE] - b, 2) + 
						pow(tabImage->donneesTab[i][j][GREEN] - g, 2) +
						pow(tabImage->donneesTab[i][j][RED] - r, 2));
					// If it is in a certain range of our median color
					if (currentDistance >= sensibility)
					{
						node->colorB = T_NAC;
						node->colorG = T_NAC;
						node->colorR = T_NAC;
						end = true;
					}
				}
				j++;
			}
			i++;
		}
	}
}

void creeArbreNB(DonneesImageTab* tabImage, Node* node)
{
	node->color = calculCarreNB(tabImage, node->x1, node->y1, node->x2, node->y2);
	if (node->color == T_NAC)
	{
		int width = floor((node->x1 + node->x2)/2);
		int height = floor((node->y1 + node->y2)/2);
		node->son1 = initNode(node->x1, node->y1, width, height);
		node->son2 = initNode(width+1, node->y1, node->x2, height);
		node->son3 = initNode(node->x1, height+1, width, node->y2);
		node->son4 = initNode(width+1, height+1, node->x2, node->y2);
		creeArbreNB(tabImage, node->son1);
		creeArbreNB(tabImage, node->son2);
		creeArbreNB(tabImage, node->son3);
		creeArbreNB(tabImage, node->son4);
	}
}

void creeArbreNG(DonneesImageTab* tabImage, Node* node, int sensibility)
{
	node->color = calculCarreNG(tabImage, node->x1, node->y1, node->x2, node->y2, sensibility);
	if (node->color == T_NAC)
	{
		int width = floor((node->x1 + node->x2)/2);
		int height = floor((node->y1 + node->y2)/2);
		node->son1 = initNode(node->x1, node->y1, width, height);
		node->son2 = initNode(width+1, node->y1, node->x2, height);
		node->son3 = initNode(node->x1, height+1, width, node->y2);
		node->son4 = initNode(width+1, height+1, node->x2, node->y2);
		creeArbreNG(tabImage, node->son1, sensibility);
		creeArbreNG(tabImage, node->son2, sensibility);
		creeArbreNG(tabImage, node->son3, sensibility);
		creeArbreNG(tabImage, node->son4, sensibility);
	}
}

void creeArbreRGB(DonneesImageTab* tabImage, NodeRGB* node, int sensibility)
{
	calculCarreRGB(tabImage, node, sensibility);
	if (node->colorB == T_NAC || node->colorG == T_NAC || node->colorR == T_NAC)
	{
		int width = floor((node->x1 + node->x2)/2);
		int height = floor((node->y1 + node->y2)/2);
		node->son1 = initNodeRGB(node->x1, node->y1, width, height);
		node->son2 = initNodeRGB(width+1, node->y1, node->x2, height);
		node->son3 = initNodeRGB(node->x1, height+1, width, node->y2);
		node->son4 = initNodeRGB(width+1, height+1, node->x2, node->y2);
		creeArbreRGB(tabImage, node->son1, sensibility);
		creeArbreRGB(tabImage, node->son2, sensibility);
		creeArbreRGB(tabImage, node->son3, sensibility);
		creeArbreRGB(tabImage, node->son4, sensibility);
	}
}

int etiquetteFeuille(Node* node, int label)
{
	if (node != NULL)
	{
		label = etiquetteFeuille(node->son1, label);
		label = etiquetteFeuille(node->son2, label);
		label = etiquetteFeuille(node->son3, label);
		label = etiquetteFeuille(node->son4, label);
		if (node->son1 == NULL && node->son2 == NULL && node->son3 == NULL && node->son4 == NULL)
		{
			node->label = label;
			return (label + 20)%256;
		}
		else
		{
			node->label = -1;
			return label;
		}
	}
	return label;
}

int etiquetteFeuilleRGB(NodeRGB* node, int label)
{
	if (node != NULL)
	{
		label = etiquetteFeuilleRGB(node->son1, label);
		label = etiquetteFeuilleRGB(node->son2, label);
		label = etiquetteFeuilleRGB(node->son3, label);
		label = etiquetteFeuilleRGB(node->son4, label);
		if (node->son1 == NULL && node->son2 == NULL && node->son3 == NULL && node->son4 == NULL)
		{
			node->label = label;
			return (label + 20)%256;
		}
		else
		{
			node->label = -1;
			return label;
		}
	}
	return label;
}

void creeMatriceArbreNB(DonneesImageTab* tabImage, Node* node, bool etiq)
{
	if (node != NULL)
	{
		if (node->label == -1)
		{
			creeMatriceArbreNB(tabImage, node->son1, etiq);
			creeMatriceArbreNB(tabImage, node->son2, etiq);
			creeMatriceArbreNB(tabImage, node->son3, etiq);
			creeMatriceArbreNB(tabImage, node->son4, etiq);
		}
		else
		{
			int i, j;
			for(i = node->x1; i <= node->x2; i++)
			{
				for(j = node->y1; j <= node->y2; j++)
				{
					if (0 <= i && i < tabImage->largeurImage &&
						0 <= j && j < tabImage->hauteurImage)
					{
						if (etiq)
						{
							tabImage->donneesTab[i][j][BLUE] = node->label;
							tabImage->donneesTab[i][j][GREEN] = node->label;
							tabImage->donneesTab[i][j][RED] = node->label;
						}
						else
						{
							switch(node->color)
							{
								case T_BLACK:
									tabImage->donneesTab[i][j][BLUE] = 0;
									tabImage->donneesTab[i][j][GREEN] = 0;
									tabImage->donneesTab[i][j][RED] = 0;
									break;
								case T_WHITE:
									tabImage->donneesTab[i][j][BLUE] = 255;
									tabImage->donneesTab[i][j][GREEN] = 255;
									tabImage->donneesTab[i][j][RED] = 255;
									break;
								default:
									tabImage->donneesTab[i][j][BLUE] = 127;
									tabImage->donneesTab[i][j][GREEN] = 127;
									tabImage->donneesTab[i][j][RED] = 127;
									break;
							}
						}
					}
				}
			}
		}
	}
}

void creeMatriceArbreNG(DonneesImageTab* tabImage, Node* node, bool etiq)
{
	if (node != NULL)
	{
		if (node->label == -1)
		{
			creeMatriceArbreNG(tabImage, node->son1, etiq);
			creeMatriceArbreNG(tabImage, node->son2, etiq);
			creeMatriceArbreNG(tabImage, node->son3, etiq);
			creeMatriceArbreNG(tabImage, node->son4, etiq);
		}
		else
		{
			int i, j;
			for(i = node->x1; i <= node->x2; i++)
			{
				for(j = node->y1; j <= node->y2; j++)
				{
					if (0 <= i && i < tabImage->largeurImage &&
						0 <= j && j < tabImage->hauteurImage)
					{
						if (etiq)
						{
							tabImage->donneesTab[i][j][BLUE] = node->label;
							tabImage->donneesTab[i][j][GREEN] = node->label;
							tabImage->donneesTab[i][j][RED] = node->label;
						}
						else
						{
							tabImage->donneesTab[i][j][BLUE] = node->color;
							tabImage->donneesTab[i][j][GREEN] = node->color;
							tabImage->donneesTab[i][j][RED] = node->color;
						}
					}
				}
			}
		}
	}
}

void creeMatriceArbreRGB(DonneesImageTab* tabImage, NodeRGB* node, bool etiq)
{
	if (node != NULL)
	{
		if (node->label == -1)
		{
			creeMatriceArbreRGB(tabImage, node->son1, etiq);
			creeMatriceArbreRGB(tabImage, node->son2, etiq);
			creeMatriceArbreRGB(tabImage, node->son3, etiq);
			creeMatriceArbreRGB(tabImage, node->son4, etiq);
		}
		else
		{
			int i, j;
			for(i = node->x1; i <= node->x2; i++)
			{
				for(j = node->y1; j <= node->y2; j++)
				{
					if (0 <= i && i < tabImage->largeurImage &&
						0 <= j && j < tabImage->hauteurImage)
					{
						if (etiq)
						{
							tabImage->donneesTab[i][j][BLUE] = node->label;
							tabImage->donneesTab[i][j][GREEN] = node->label;
							tabImage->donneesTab[i][j][RED] = node->label;
						}
						else
						{
							tabImage->donneesTab[i][j][BLUE] = node->colorB;
							tabImage->donneesTab[i][j][GREEN] = node->colorG;
							tabImage->donneesTab[i][j][RED] = node->colorR;
						}
					}
				}
			}
		}
	}
}
