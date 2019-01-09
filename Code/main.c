
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
		
		// Test of the function used to create and apply a median filter
		printf("Filtering the image with a median filter\n");
		DonneesImageTab* medianTabImage = applyMedianFilterOnTab(tabImage, 3, 3);
		
		// Test of the function used to create and apply a gradiant filter
		printf("Filtering the median image with a gradiant filter\n");
		DonneesImageTab* gradiantTabImage = applyGradiantFilterOnTab(medianTabImage, PREWITT); //PREWITT ou SOBEL
		printf(" Creating the filtered image\n");
		DonneesImageRGB* gradiantImage = tabToRGB(gradiantTabImage);
		ecrisBMPRGB_Dans(gradiantImage, "1 - gradiant.bmp");
		
		// Test of the function used to create a hough transform
		printf("Creating the Hough transformation of the gradiant image\n");
		DonneesImageTab* tabHough = createHough(gradiantTabImage, 200, 720);
		cutBetweenLevel(tabHough, 110, 255); // need to find what to put for the min (histogram ?)
		cutBetweenLevel(tabHough, 0, 100);// after that, every point is blanck
		// need to widen the points
		// then region 
		// then get the average of each point of each region to find the center
		DonneesImageRGB* houghImage = houghToRGB(tabHough);
		ecrisBMPRGB_Dans(houghImage, "2 - hough.bmp");
		
		// Test of the function used to find region by flooding the image
		printf("Creating the region using a flow method\n");
		DonneesImageTab* tabRegion = initTabRegion(tabHough->largeurImage, tabHough->hauteurImage);
		IdRegions* idRegions = findRegionFlow(tabHough, tabRegion, 240); // need to find the sensibility
		printf(" Regions found : %d\n", idRegions->size);
		DonneesImageRGB* regionImage = tabToRGB(tabRegion);
		ecrisBMPRGB_Dans(regionImage, "3 - region.bmp");
		// Test of the functions used to get the line in the hough transform
		Line* line = getMaxLineFromRegion(tabHough, tabRegion, idRegions->regions[0]);
		updateLineInfo(gradiantTabImage, line, 200);
		printf("x : %d, y : %d,\nxe : %d, ye : %d\n", line->startX, line->startY, line->endX, line->endY);
		DonneesImageTab* newTabImage = traceLineOnImage(tabImage, line, 255, 0, 0);
		DonneesImageRGB* newImage = tabToRGB(newTabImage);
		ecrisBMPRGB_Dans(newImage, "4 - newImage.bmp");

		// Free of all the tests
		printf("Freeing the memory\n");
		libereDonneesImageRGB(&image);
		
		libereDonneesTab(&tabImage);
		
		libereDonneesTab(&medianTabImage);
		
		libereDonneesTab(&gradiantTabImage);
		
		libereDonneesTab(&tabHough);
		libereDonneesImageRGB(&houghImage);
		
		libereDonneesTab(&tabRegion);
		libereDonneesImageRGB(&regionImage);
		destructIdRegions(&idRegions);
		
		free(line);
		libereDonneesTab(&newTabImage);
		libereDonneesImageRGB(&newImage);
	}
	else
	{
		printf("Error : Can't read the image");
	}
	printf("---Done\n");
	return 0;
}
