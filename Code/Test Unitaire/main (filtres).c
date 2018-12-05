
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
		
		// Test of the functions used to create histogram
		printf("Creating the histogram of the grey image\n");
		int* histogram = createHistogram(tabImage, BLUE);
		DonneesImageRGB* histogramImage = histogramToRGB(histogram);
		ecrisBMPRGB_Dans(histogramImage, "2 - histogram.bmp");
		
		// Test of the function used to create and apply a gaussian filter
		printf("Filtering the image with a gaussian filter\n");
		Filter* gaussianFilter = createGaussianFilter(21, 21);
		DonneesImageTab* gaussianTabImage = applyFilterOnTab(tabImage, gaussianFilter);
		printf(" Creating the filtered image\n");
		DonneesImageRGB* gaussianImage = tabToRGB(gaussianTabImage);
		ecrisBMPRGB_Dans(gaussianImage, "3 - gaussian.bmp");
		
		// Test of the function used to create and apply an avarage filter
		printf("Filtering the image with an avarage filter\n");
		Filter* avarageFilter = createAvarageFilter(21, 21);
		DonneesImageTab* avarageTabImage = applyFilterOnTab(tabImage, avarageFilter);
		printf(" Creating the filtered image\n");
		DonneesImageRGB* avarageImage = tabToRGB(avarageTabImage);
		ecrisBMPRGB_Dans(avarageImage, "4 - avarage.bmp");
		
		// Test of the function used to create and apply a median filter
		printf("Filtering the image with a median filter\n");
		DonneesImageTab* medianTabImage = applyMedianFilterOnTab(tabImage, 3, 3);
		printf(" Creating the filtered image\n");
		DonneesImageRGB* medianImage = tabToRGB(medianTabImage);
		ecrisBMPRGB_Dans(medianImage, "5 - median.bmp");
		
		// Test of the function used to create and apply a Roberts' filter
		printf("Filtering the median image with a Roberts' filter\n");
		DonneesImageTab* robertsTabImage = applyRobertsFilterOnTab(medianTabImage);
		printf(" Creating the filtered image\n");
		DonneesImageRGB* robertsImage = tabToRGB(robertsTabImage);
		ecrisBMPRGB_Dans(robertsImage, "6 - roberts.bmp");
		
		// Test of the function used to create and apply a gradiant filter
		printf("Filtering the median image with a gradiant filter\n");
		DonneesImageTab* gradiantTabImage = applyGradiantFilterOnTab(medianTabImage, PREWITT); //PREWITT ou SOBEL
		printf(" Creating the filtered image\n");
		DonneesImageRGB* gradiantImage = tabToRGB(gradiantTabImage);
		ecrisBMPRGB_Dans(gradiantImage, "7 - gradiant.bmp");
		
		// Test of the function used to create and apply a laplacian filter
		printf("Filtering the median image with a laplacian filter\n");
		Filter* laplacianFilter = createLaplacianFilter(HEIGHT_CX); //FOUR_CX ou HEIGHT_CX
		DonneesImageTab* laplacianTabImage = applyFilterOnTab(medianTabImage, laplacianFilter);
		printf(" Creating the filtered image\n");
		DonneesImageRGB* laplacianImage = tabToRGB(laplacianTabImage);
		ecrisBMPRGB_Dans(laplacianImage, "8 - laplacian.bmp");
		
		// Test of the function used to create a hough transform
		printf("Creating the Hough transformation of the gradiant image\n");
		DonneesImageTab* tabHough = createHough(gradiantTabImage, 200, 720);
		DonneesImageRGB* houghImage = houghToRGB(tabHough);
		ecrisBMPRGB_Dans(houghImage, "9 - hough.bmp");
		
		// Test of the function used to create a radon transform
		printf("Creating the Radon transformation of the gradiant image\n");
		DonneesImageCube* tabRadon = createRadon(gradiantTabImage, 200, 20, 59, 6);
		DonneesImageRGB* radonImage1 = radonToRGB(tabRadon, 50);
		ecrisBMPRGB_Dans(radonImage1, "10 - radon.bmp");
		DonneesImageRGB* radonImage2 = radonToRGB(tabRadon, 25);
		ecrisBMPRGB_Dans(radonImage2, "11 - radon.bmp");
		
		// Free of all the tests
		printf("Freeing the memory\n");
		libereDonneesImageRGB(&image);
		
		libereDonneesImageRGB(&greyImage);
		libereDonneesTab(&tabImage);
		
		free(histogram);
		libereDonneesImageRGB(&histogramImage);
		
		destructFilter(&gaussianFilter);
		libereDonneesTab(&gaussianTabImage);
		libereDonneesImageRGB(&gaussianImage);
		
		destructFilter(&avarageFilter);
		libereDonneesTab(&avarageTabImage);
		libereDonneesImageRGB(&avarageImage);
		
		libereDonneesTab(&medianTabImage);
		libereDonneesImageRGB(&medianImage);
		
		libereDonneesTab(&robertsTabImage);
		libereDonneesImageRGB(&robertsImage);
		
		libereDonneesTab(&gradiantTabImage);
		libereDonneesImageRGB(&gradiantImage);
		
		destructFilter(&laplacianFilter);
		libereDonneesTab(&laplacianTabImage);
		libereDonneesImageRGB(&laplacianImage);
		
		libereDonneesTab(&tabHough);
		libereDonneesImageRGB(&houghImage);
		
		libereDonneesCube(&tabRadon);
		libereDonneesImageRGB(&radonImage1);
		libereDonneesImageRGB(&radonImage1);
	}
	else
	{
		printf("Error : Can't read the image");
	}
	printf("---Done\n");
	return 0;
}
