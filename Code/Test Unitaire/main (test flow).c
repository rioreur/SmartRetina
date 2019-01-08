
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
	printf("%s, %d\n", argv[1], argc);
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
		// Test of the functions used to create a grey image
		printf("Creating the grey image\n");
		DonneesImageTab* tabImage = RGBToTab(image);
		makeGreyLevel(tabImage);
		DonneesImageRGB* greyImage = tabToRGB(tabImage);
		ecrisBMPRGB_Dans(greyImage, "1 - grey.bmp");
		
		// Test of the function used to create and apply a median filter
		printf("Filtering the image with a median filter\n");
		DonneesImageTab* medianTabImage = applyMedianFilterOnTab(tabImage, 3, 3);
		printf(" Creating the filtered image\n");
		DonneesImageRGB* medianImage = tabToRGB(medianTabImage);
		ecrisBMPRGB_Dans(medianImage, "5 - median.bmp");
		
		// Test of the function used to create and apply a gradiant filter
		printf("Filtering the median image with a gradiant filter\n");
		DonneesImageTab* gradiantTabImage = applyGradiantFilterOnTab(medianTabImage, PREWITT); //PREWITT ou SOBEL
		printf(" Creating the filtered image\n");
		DonneesImageRGB* gradiantImage = tabToRGB(gradiantTabImage);
		ecrisBMPRGB_Dans(gradiantImage, "7 - gradiant.bmp");
		
		// Test of the function used to create a hough transform
		printf("Creating the Hough transformation of the gradiant image\n");
		DonneesImageTab* tabHough = createHough(gradiantTabImage, 200, 720);
		DonneesImageRGB* houghImage = houghToRGB(tabHough);
		ecrisBMPRGB_Dans(houghImage, "9 - hough.bmp");
		
		// Test of the function used to find region by flooding the image
		/*printf("Creating the region using a flow method\n");
		DonneesImageTab* tabRegion = initTabRegion(tabHough->largeurImage, tabHough->hauteurImage);
		IdRegions* idRegions = findRegionFlow(tabHough, tabRegion);
		printf("idRegions : %d\n", idRegions->size);
		DonneesImageRGB* regionImage = tabToRGB(tabRegion);
		ecrisBMPRGB_Dans(regionImage, "region.bmp");*/

		// Free of all the tests
		printf("Freeing the memory\n");
		libereDonneesImageRGB(&image);
		
		libereDonneesImageRGB(&greyImage);
		libereDonneesTab(&tabImage);
		
		libereDonneesTab(&medianTabImage);
		libereDonneesImageRGB(&medianImage);
		
		libereDonneesTab(&gradiantTabImage);
		libereDonneesImageRGB(&gradiantImage);
		
		libereDonneesTab(&tabHough);
		libereDonneesImageRGB(&houghImage);
		
		/*libereDonneesTab(&tabRegion);
		libereDonneesImageRGB(&regionImage);
		destructIdRegions(&idRegions);*/
	}
	else
	{
		printf("Error : Can't read the image");
	}
	printf("---Done\n");
	return 0;
}
