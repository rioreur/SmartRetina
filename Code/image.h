// à faire
//modifier la creation de l'histogramme pour qu'elle se fasse pour toutes les couleurs

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdbool.h>
#include <math.h>

#include "BmpLib.h"

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

	// Color ID
#ifndef BLUE
	#define BLUE 0
#endif
#ifndef GREEN
	#define GREEN 1
#endif
#ifndef RED
	#define RED 2
#endif

	// Gradian filter
#ifndef PREWITT
	#define PREWITT 1
#endif
#ifndef SOBEL
	#define SOBEL 2
#endif

	// Laplacian filter
#ifndef FOUR_CX
	#define FOUR_CX 0
#endif
#ifndef HEIGHT_CX
	#define HEIGHT_CX 1
#endif

// Used to store an image in a matrice.
//First dimension is X, then Y, then the color (BLUE, GREEN or RED)
typedef struct
{
	int largeurImage;
	int hauteurImage;
	int ***donneesTab; /* Donnees sauvees dans l'ordre bleu-vert-rouge */
} DonneesImageTab;

// Used to store an image in a 3D matrice.
//First dimension is X, then Y, then Z, then the color (BLUE, GREEN or RED)
typedef struct
{
	int largeurImage;
	int hauteurImage;
	int profondeurImage;
	short int ****donneesCube; /* Donnees sauvees dans l'ordre bleu-vert-rouge */
	int minProfondeur;
	int maxProfondeur;
} DonneesImageCube;

// Used for image processing
typedef struct
{
	int filterWidth;
	int filterHeight;
	float** filterTab;
} Filter;

// Used to store the information of a line in a Hough transform
typedef struct
{
	int rIndex;
	int angularIndex;
	int maxRIndex;
	int maxAngularIndex;
	
	int startX;
	int startY;
	int endX;
	int endY;
	
	float lenght;
	float lenghtRatio;
} Line;

// Use to store histogram information
typedef struct {
	int* values;
	int size;
} Histogram;

//Use to store a point
typedef struct {
	int x;
	int y;
	int label;
	int coef;
} Point;


///////////////////////////
//--- Maths functions ---//
///////////////////////////

// Map a given value, which vary between vmin and vmax, to a value between min and max
float nmap(float value, float vmin, float vmax, float min, float max);
// Give the gaussian value at a given point for a gaussian function centered on (0, 0)
float gaussian(float x, float y, int filterSize);
// Compare two pixel value, used in sorting functions
int pixelComp(void const *p1, void const *p2);
// Compare two value and returns the greater one
float max(float a, float b);
// Compare all the values in the given list and returns the maximum one
int getMax(int* list, int size);
// Compare two value and returns the lower one
float min(float a, float b);
// Compare all the values in the given list and returns the minimum one
int getMin(int* list, int size);
// Return the absolute value of the given number
float absValue(float v);
// Return the sign of the given value
int sign(float v);

/////////////////////////////
//--- Init and Destruct ---//
/////////////////////////////

//Initialisation function
DonneesImageRGB* initImage(int width, int height);
DonneesImageTab* initTab(int width, int height);
DonneesImageCube* initCube(int width, int height, int depth, int minRadius, int maxRadius);
Filter* initFilter(int width, int height);
Histogram* initHistogram(int size);
Line* initLine(int maxRIndex, int maxAngularIndex);
//Destruction function
void libereDonneesTab(DonneesImageTab** tabImage);
void libereDonneesCube(DonneesImageCube** cubeImage);
void destructFilter(Filter** filter);
void destructHistogram(Histogram** histogram);

/////////////////////////
//--- Image editing ---//
/////////////////////////

// Functions to convert DonneesImageRGB to DonneesImageTab and the other way around
DonneesImageTab* RGBToTab(DonneesImageRGB* image);
DonneesImageRGB* tabToRGB(DonneesImageTab* tabImage);
// Function that returns a copy of the given DonneesImageTab
DonneesImageTab* cpyTab(DonneesImageTab* tabImage);

// Transform the given DonneesImageTab to a grey level equivalant
void makeGreyLevel(DonneesImageTab* tabImage);
// every value above max become 255 and every value below min become 0
void cutBetweenLevel(DonneesImageTab* tabImage, int min, int max);

/////////////////////
//--- Histogram ---//
/////////////////////

// Create an histogram from the given DonneesImageTab for the given color (BLUE, RED or GREEN)
Histogram* createHistogram(DonneesImageTab* tabImage, int color);
// Convert the histogram to a DonneesImageRGB
DonneesImageRGB* histogramToRGB(Histogram* histogram);

//////////////////////////
//--- Line detection ---//
//////////////////////////

// Create a Hough transform from the given DonneesImageTab
DonneesImageTab* createHough(DonneesImageTab* tabImage, int sensibility, int nbrAngularSteps);
// Convert the Hough transform to a DonneesImageRGB
DonneesImageRGB* houghToRGB(DonneesImageTab* tabHough);
// Returns the line who appears the most in the given Hough transform
Line* getMaxLine(DonneesImageTab* tabHough);
// Returns the ratio of the given line that realy exists on the image and set the coordinates and the ratio of the Line structure
void updateLineInfo(DonneesImageTab* tabImage, Line* line, int sensibility);
// trace the given line on the given DonneesImageTab with the given color
void traceLineOnImage(DonneesImageTab* tabImage, Line* line, int r, int g, int b);

////////////////////////////
//--- Circle detection ---//
////////////////////////////

// Create a Radon transform from the given DonneesImageTab, BE CAREFUL WITH THE VALUE, not too high
DonneesImageCube* createRadon(DonneesImageTab* tabImage, int sensibility, int minRadius, int maxRadius, int nbrRadius);
// Convert the Radon transform to a DonneesImageRGB
DonneesImageRGB* radonToRGB(DonneesImageCube* tabRadon, int radius);

//////////////////
//--- Filter ---//
//////////////////

// Different filter's creation function
Filter* createAvarageFilter(int width, int height);
Filter* createGaussianFilter(int width, int height);
Filter* createLaplacianFilter(int type); //FOUR_CX or HEIGHT_CX

// Apply a specific filter to DonneesImageTab on all colors independently
DonneesImageTab* applyFilterOnTab(DonneesImageTab* tabImage, Filter* filter);
DonneesImageTab* applyMedianFilterOnTab(DonneesImageTab* tabImage, const int filterWidth, const int filterHeight);
DonneesImageTab* applyRobertsFilterOnTab(DonneesImageTab* tabImage);
DonneesImageTab* applyGradiantFilterOnTab(DonneesImageTab* tabImage, int type); //PREWITT or SOBEL
void applyDillatationFilter(DonneesImageTab* tabImage, int whiteLevel);
bool areNeighboursWhite(DonneesImageTab* tabImage, int whiteLevel, int x, int y);

