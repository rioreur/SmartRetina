
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
	    /////////////////////
	    //--- parameter ---// by default, work for Images/shapes.bmp
	    /////////////////////
	    
	    // the sensibility to find regions. The higher it is, the less we find regions
	    int regionSensibility = 100; //between 0 and 255
	    // the sensibility to create the hough transform. The higher it is, the more we'll put lines in the hough transform
	    //it is use to check the edge of the shapes on the image. It will chekc more edge if this number is lower
	    int houghSensibility = 100; //between 0 and 255
	    // the median value threshhold for the hough transform. the higher it is, the less we find potential lines.
	    //everything below this value on the houghTransform will become black, and everything above will become white
	    int houghMedianValue = 70; //between 0 and 255
	    // the sensibility to find regions on the final hough transform. The higher it is, the less we find potential lines
	    int regionHoughSensibility = 100; //between 0 and 255
	    // the minimum size a region on the hough transform must have to be counted as a line
	    int regionHoughSize = 0; //minimum 0
	    
	    /////////////////////////
	    //--- preprocessing ---//
	    /////////////////////////
	    
		// Create the tab from the image
		printf("Creating the matrice from the image\n");
		DonneesImageTab* tabImage = RGBToTab(image);
		
		// Create all the different regions
		printf("Creating the region using bottom up method\n");
		DonneesImageTab* tabRegion = initTabRegion(tabImage->largeurImage, tabImage->hauteurImage);
		// We find all regions in the image using a bottom up method
		IdRegions* allIds = findAllRegionBottomUp(tabImage, tabRegion, regionSensibility);
		printf(" Number of region found : %d\n", allIds->size);
		// We create a tabRegionColor wich have all the regions shown with their own color
		DonneesImageTab* tabRegionColor = getColorTabRegions(tabRegion, allIds);
		printf(" Creating the image of the regions\n");
		DonneesImageRGB* imageRegion = tabToRGB(tabRegionColor);
		ecrisBMPRGB_Dans(imageRegion, "1 - regions.bmp");
		
		// We put a grey level on the original image and we use a median filter on it to lower the noise
		printf("Changing the image so it become a grey level one\n");
		makeGreyLevel(tabImage);
		printf("Filtering the image with a median filter\n");
		DonneesImageTab* medianTabImage = applyMedianFilterOnTab(tabImage, 3, 3);
		
		// Then we use a gradiant filter to get the edges
		printf("Filtering the median image with a gradiant filter\n");
		DonneesImageTab* gradiantTabImage = applyGradiantFilterOnTab(medianTabImage, PREWITT); //PREWITT ou SOBEL
		printf(" Creating the filtered image\n");
		DonneesImageRGB* gradiantImage = tabToRGB(gradiantTabImage);
		ecrisBMPRGB_Dans(gradiantImage, "2 - gradiant.bmp");
		
		//////////////////////////
	    //--- line detection ---//
	    //////////////////////////
		
		// We create the hough transform of the image
		printf("Creating the Hough transformation of the gradiant image\n");
		DonneesImageTab* tabHough = createHough(gradiantTabImage, houghSensibility, 720);
		printf(" Edit it so it is easier to use\n");
		//Creating the histogram of the hough transform
		/*Histogram* histogram = createHistogram(tabHough, BLUE);       //marche pas
		houghMedianValue = getMedianHistogram(histogram);*/
        //Making every point black or white
		cutBetweenLevel(tabHough, houghMedianValue, 255);
		cutBetweenLevel(tabHough, 0, houghMedianValue);
		//Making sure to have a blob around each point to find the regions easily with a dillatation filter
		applyDillatationFilter(tabHough, 200);
		printf(" Creating the image of the hough transform\n");
		DonneesImageRGB* houghImage = houghToRGB(tabHough);
		ecrisBMPRGB_Dans(houghImage, "3 - hough.bmp");
		
		// We find each point on the hough transform by finding regions using the bottom up method
		printf("Creating a region for each lines on the hough transform \n");
		DonneesImageTab* tabRegionHough = initTabRegion(tabHough->largeurImage, tabHough->hauteurImage);
		IdRegions* idRegionsHough = findAllRegionBottomUp(tabHough, tabRegionHough, regionHoughSensibility);
		printf(" line founds : %d\n", idRegionsHough->size-1);
		
		printf("Finding the lines on each regions\n");
		int i;
		for(i = 1; i < idRegionsHough->size; i++) // the first one is the background so we skip it
		{
		    printf(" Finding line %d...\n", i);
		    Line* line = getCenterLineFromRegion(tabHough, tabRegionHough, idRegionsHough->regions[i], regionHoughSize);
		    if (line != NULL)
		    {
		        updateLineInfo(gradiantTabImage, line, houghSensibility);
		        traceLineOnImage(tabRegionColor, line, 255, 0, 0);
		        free(line);
		        line = NULL;
		    }
		}
		printf(" Done.\n");
		libereDonneesImageRGB(&imageRegion);
		imageRegion = tabToRGB(tabRegionColor);
		ecrisBMPRGB_Dans(imageRegion, "4 - lineDetection.bmp");
		
		////////////////////////////
	    //--- central symmetry ---//
	    ////////////////////////////
		
		// Find all central symmetry for each shape (regions)
		printf("Finding all central symetry\n");
		//Get the center of gravity of all the regions
		Point** gravityPoints = getAllGravityPoints(tabRegion, allIds);
		//Update their coeficient of Minkovski
		updateCoefGravityPoint(gravityPoints, tabRegion, allIds);
		//We show them on the tabRegion
		colorGravityPointRegions(tabRegionColor, gravityPoints, allIds->size-1); // We need to put them in the color tabRegion
		printf(" Creating the image of the regions with the central dots\n");
		libereDonneesImageRGB(&imageRegion);
		imageRegion = tabToRGB(tabRegionColor);
		ecrisBMPRGB_Dans(imageRegion, "5 - centralSymmetry.bmp");
		
		/////////////////////////////
	    //--- memory liberation ---//
	    /////////////////////////////
	    
		printf("Freeing the memory\n");
		libereDonneesImageRGB(&image);
		
		libereDonneesTab(&tabImage);
		
		libereDonneesTab(&medianTabImage);
		libereDonneesTab(&gradiantTabImage);
		
		destructTabPoints(&gravityPoints, allIds->size-1);
    
        libereDonneesTab(&tabHough);
		libereDonneesImageRGB(&houghImage);
		//destructHistogram(&histogram);
		
		libereDonneesTab(&tabRegionHough);
		destructIdRegions(&idRegionsHough);
    
        destructIdRegions(&allIds);
        
		libereDonneesTab(&tabRegion);
		libereDonneesImageRGB(&imageRegion);
	}
	else
	{
		printf("Error : Can't read the image\n");
	}
	printf("---Done\n");
	return 0;
}
