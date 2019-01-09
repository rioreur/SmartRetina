
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
		DonneesImageTab* tabHough = createTabAxis(gradiantTabImage, 200);
		printf(" Creating the image of the hough transform\n");
		DonneesImageRGB* houghImage = houghToRGB(tabHough);
		ecrisBMPRGB_Dans(houghImage, "2 - hough.bmp");
		/*DonneesImageTab* tabHough = createHough(gradiantTabImage, 200, 720);
		printf(" Edit it so it is easier to use\n");
        //  Making every point black or white
		cutBetweenLevel(tabHough, 80, 255); // need to find what to put for the min (histogram ?)
		cutBetweenLevel(tabHough, 0, 80);
		//  Making sure to have a blob around each point to find the regions easily
		applyDillatationFilter(tabHough, 200);
		printf(" Creating the image of the hough transform\n");
		DonneesImageRGB* houghImage = houghToRGB(tabHough);
		ecrisBMPRGB_Dans(houghImage, "2 - hough.bmp");
		
		// Test of the function used to find region by flooding the image
		printf("Creating a region for each lines on the hough transform \n");
		DonneesImageTab* tabRegion = initTabRegion(tabHough->largeurImage, tabHough->hauteurImage);
		IdRegions* idRegions = findAllRegionBottomUp(tabHough, tabRegion, 200);
		//IdRegions* idRegions = findRegionFlow(tabHough, tabRegion, 240); // need to find the sensibility
		printf(" Regions found : %d\n", idRegions->size);
		DonneesImageRGB* regionImage = tabToRGB(tabRegion);
		printf(" Creating the image of the hough regions\n");
		ecrisBMPRGB_Dans(regionImage, "3 - region.bmp");
		
		printf("Finding the lines on each regions\n");
		// Test of the functions used to get the line in the hough transform
		DonneesImageTab* newTabImage = cpyTab(tabImage);
		int i;
		for(i = 1; i < idRegions->size; i++) // the first one is the background so we skip it
		{
		    printf(" Finding line %d...\n", i);
		    Line* line = getCenterLineFromRegion(tabHough, tabRegion, idRegions->regions[i]);
		    updateLineInfo(gradiantTabImage, line, 200);
		    traceLineOnImage(newTabImage, line, 255, 0, 0);
		    free(line);
		    line = NULL;
		}
		printf(" Done.\n");
		printf(" Creating the image to show the found lines in red\n");
		DonneesImageRGB* newImage = tabToRGB(newTabImage);
		ecrisBMPRGB_Dans(newImage, "4 - newImage.bmp");*/

		// Free of all the tests
		printf("Freeing the memory\n");
		libereDonneesImageRGB(&image);
		
		libereDonneesTab(&tabImage);
		
		libereDonneesTab(&medianTabImage);
		
		libereDonneesTab(&gradiantTabImage);
		
		libereDonneesTab(&tabHough);
		libereDonneesImageRGB(&houghImage);
		
		/*libereDonneesTab(&tabRegion);
		libereDonneesImageRGB(&regionImage);
		destructIdRegions(&idRegions);
		
		libereDonneesTab(&newTabImage);
		libereDonneesImageRGB(&newImage);*/
	}
	else
	{
		printf("Error : Can't read the image");
	}
	printf("---Done\n");
	return 0;
}
