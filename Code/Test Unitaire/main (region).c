
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
#include "region.h"

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
		
		// Test for the TopDown region
		printf(" Creating the image showing the region using top down method\n");
		DonneesImageTab* tabRegion = initTabRegion(gradiantTabImage->largeurImage, gradiantTabImage->hauteurImage);
		printf("Checking the arrow region\n");
		findRegionTopDown(gradiantTabImage, tabRegion, 50, 60, 200);
		printf("Checking the background zone\n");
		findRegionTopDown(gradiantTabImage, tabRegion, 0, 0, 200);
		DonneesImageRGB* newImage = tabToRGB(tabRegion);
		ecrisBMPRGB_Dans(newImage, "newImage.bmp");
		
		// Free of all the tests
		printf("Freeing the memory\n");
		libereDonneesImageRGB(&image);
		
		libereDonneesTab(&gradiantTabImage);

		libereDonneesTab(&tabRegion);
		libereDonneesImageRGB(&newImage);
	}
	else
	{
		printf("Error : Can't read the image\n");
	}
	printf("---Done\n");
	return 0;
}
