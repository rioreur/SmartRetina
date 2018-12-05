
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
#include "image.h"

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
		
		// Test of the function used to create a hough transform
		printf("Creating the Hough transformation of the gradiant image\n");
		DonneesImageTab* tabHough = createHough(gradiantTabImage, 200, 720);
		DonneesImageRGB* houghImage = houghToRGB(tabHough);
		ecrisBMPRGB_Dans(houghImage, "9 - hough.bmp");
		
		// Visualizing the line
		printf("Drawing the line on a new image\n");
		Line* line = getMaxLine(tabHough);
		DonneesImageTab* newTabImage = traceLineOnImage(gradiantTabImage, line, 255, 0, 0);
		DonneesImageRGB* newImage = tabToRGB(newTabImage);
		ecrisBMPRGB_Dans(newImage, "newImage.bmp");
		
		// Little problems, the lineGradiant1 get a ratio of 0.30 whereas every other as a 0.90 ratio
		// Probably a problem with the precision...
		getRatioLine(gradiantTabImage, line, 200);
		
		// Free of all the tests
		printf("Freeing the memory\n");
		libereDonneesImageRGB(&image);
		
		libereDonneesTab(&gradiantTabImage);
		
		libereDonneesTab(&tabHough);
		libereDonneesImageRGB(&houghImage);
		
		free(line);
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
