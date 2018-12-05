
//General header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//More specific headers files
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

//Header file
#include "quadtree.h"

int main(int argc, char** argv)
{
	// We get the image we will use for the test
	printf("Reading the image\n");
	DonneesImageRGB* image = NULL;
	if (argc != 2)
	{
		image = lisBMPRGB("test.bmp");
	}
	else
	{
		image = lisBMPRGB(argv[1]);
	}
	if (image != NULL)
	{
		// Create the tab from the image
		printf("Creating the matrice from the image\n");
		DonneesImageTab* gradiantTabImage = RGBToTab(image);
		
		// Test for tree
		printf("Creating the tree\n");
		NodeRGB* startingNode = initNodeRGB(0, 0, gradiantTabImage->largeurImage-1, gradiantTabImage->hauteurImage-1);
		creeArbreRGB(gradiantTabImage, startingNode, 50);
		etiquetteFeuilleRGB(startingNode, 0);
		//showTree(startingNode, 0);
		
		printf(" Creating the image from the tree\n");
		DonneesImageTab* newTabImage = initTab(gradiantTabImage->largeurImage, gradiantTabImage->hauteurImage);
		creeMatriceArbreRGB(newTabImage, startingNode, false);
		DonneesImageRGB* newImage = tabToRGB(newTabImage);
		ecrisBMPRGB_Dans(newImage, "newImage.bmp");
		
		// Free of all the tests
		printf("Freeing the memory\n");
		libereDonneesImageRGB(&image);
		
		libereDonneesTab(&gradiantTabImage);
		
		destructNodeRGB(&startingNode);

		libereDonneesTab(&newTabImage);
		libereDonneesImageRGB(&newImage);
	}
	else
	{
		printf("Error : Can't read the image\n");
	}
	printf("---Done\n");
	return 0;
}
