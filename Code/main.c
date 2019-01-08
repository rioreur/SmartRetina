
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
#include "symetrie.h"

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
		
		// Test for the BottomUp region
		printf(" Creating the image showing the region using top down method\n");
		DonneesImageTab* tabRegion = initTabRegion(gradiantTabImage->largeurImage, gradiantTabImage->hauteurImage);
		
		printf("Checking the circle region\n");
		IdRegions* allIds = findAllRegionBottomUp(gradiantTabImage, tabRegion, 200);
		printf(" Number of region found : %d\n", allIds->size);
		
		DonneesImageRGB* newImage = tabToRGB(tabRegion);
		ecrisBMPRGB_Dans(newImage, "newImage.bmp");
		
		//test for the Symetries
		printf("Getting the shape of the circle zone\n");
		/*DonneesImageTab* tabShape = getShape(tabRegion, allIds->regions[0]);
		DonneesImageTab* tabSymetricShape = getSymetricShape(tabShape);
		DonneesImageTab* tabGlobal = createGlobalShape(tabShape, tabSymetricShape);
		DonneesImageRGB* shapeImage = tabToRGB(tabSymetricShape);
		ecrisBMPRGB_Dans(shapeImage, "shapeImage.bmp");
		DonneesImageRGB* borderImage = tabToRGB(tabGlobal);
		ecrisBMPRGB_Dans(borderImage, "borderImage.bmp");
		printf(" Symetric ratio : %f\n", ((float) getArea(tabShape))/((float) getArea(tabGlobal)/4));*/
		
		// Free of all the tests
		printf("Freeing the memory\n");
		libereDonneesImageRGB(&image);
		
		libereDonneesTab(&gradiantTabImage);
		destructIdRegions(&allIds);

		/*libereDonneesTab(&tabRegion);
		libereDonneesImageRGB(&newImage);
		libereDonneesTab(&tabShape);
		libereDonneesTab(&tabSymetricShape);
		libereDonneesImageRGB(&shapeImage);
		libereDonneesTab(&tabGlobal);
		libereDonneesImageRGB(&borderImage);*/
	}
	else
	{
		printf("Error : Can't read the image\n");
	}
	printf("---Done\n");
	return 0;
}
