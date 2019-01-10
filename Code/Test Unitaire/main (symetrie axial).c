
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
		
		// Test of the function used to find region by flooding the image
		printf("Creating regions\n");
		DonneesImageTab* tabRegion = initTabRegion(medianTabImage->largeurImage, medianTabImage->hauteurImage);
		IdRegions* idRegions = findAllRegionBottomUp(medianTabImage, tabRegion, 200);
		printf(" Regions found : %d\n", idRegions->size);
		DonneesImageRGB* regionImage = tabToRGB(tabRegion);
		printf(" Creating the image of the regions\n");
		ecrisBMPRGB_Dans(regionImage, "2 - region.bmp");
		
		// Test of the function to get the borders
		printf("Creating the edge of the region\n");
		DonneesImageTab* tabEdge = getShapeEdge(tabRegion, idRegions->regions[1]);
		printf(" Creating the image of the edge\n");
		DonneesImageRGB* edgeImage = tabToRGB(tabEdge);
		ecrisBMPRGB_Dans(edgeImage, "3 - edge.bmp");
		
		// Test of the function used to aproximate the axis
		printf("Creating the Hough transformation of the gradiant image\n");
		DonneesImageTab* tabHough = createTabAxis(tabEdge, 100, 1);
		printf(" Edit it so it is easier to use\n");
        //  Making every point black or white
		/*cutBetweenLevel(tabHough, 80, 255); // need to find what to put for the min (histogram ?)
		cutBetweenLevel(tabHough, 0, 80);*/
		//  Making sure to have a blob around each point to find the regions easily
		applyDillatationFilter(tabHough, 200);
		printf(" Creating the image of the hough transform\n");
		DonneesImageRGB* houghImage = houghToRGB(tabHough);
		ecrisBMPRGB_Dans(houghImage, "4 - hough.bmp");
		
		// Test of the functions used to get the line in the hough transform
		DonneesImageTab* newTabImage = cpyTab(tabImage);
	    printf(" Finding line\n");
	    Line* line = getMaxLine(tabHough);
	    updateLineInfo(tabEdge, line, 200);
	    traceLineOnImage(newTabImage, line, 255, 0, 0);
	    printf(" %d, %f\n", line->rIndex, nmap(line->angularIndex, 0, line->maxAngularIndex, 0, 180));
	        free(line);
		printf(" Creating the image to show the found lines in red\n");
		DonneesImageRGB* newImage = tabToRGB(newTabImage);
		ecrisBMPRGB_Dans(newImage, "4 - newImage.bmp");

		// Free of all the tests
		printf("Freeing the memory\n");
		libereDonneesImageRGB(&image);
		
		libereDonneesTab(&tabImage);
		
		libereDonneesTab(&medianTabImage);
		
		libereDonneesTab(&tabRegion);
		libereDonneesImageRGB(&regionImage);
		destructIdRegions(&idRegions);
		
		libereDonneesTab(&tabEdge);
		libereDonneesImageRGB(&edgeImage);
		
		libereDonneesTab(&tabHough);
		libereDonneesImageRGB(&houghImage);
		
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
