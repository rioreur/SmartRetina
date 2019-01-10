// Faire des putain de liste chainé pour les regions

#include "image.h"

#ifndef BORDER
	#define BORDER -1
#endif
#ifndef UNCHECKED
	#define UNCHECKED -2
#endif

//please, a chained list
typedef struct {
	// Color of the region
	int blue;
	int green;
	int red;
	// position of the corresponding seed
	int x;
	int y;
	// id of the region
	int label;
} IdRegion;

typedef struct {
	IdRegion** regions;
	int size;
} IdRegions;

// Returns a DonneesImageTab with all his value set to UNCHECKED
DonneesImageTab* initTabRegion(int width, int height);
// Initialisation function
IdRegion* initIdRegion(int blue, int green, int red, int x, int y, int label);
IdRegions* initIdRegions(int size);
// Destruction function
void destructIdRegions(IdRegions** idRegions);

// Put the region which the pixel (x, y) is from into tabRegion. The region is in the color of the given pixel.
//returns the color of the said region
IdRegion* findRegionBottomUp(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int x, int y, int label, int sensibility);
// Find all the regions in the image using the BottomUp algorithm and put them in tabRegion
//return all the idRegion of the different region
IdRegions* findAllRegionBottomUp(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int sensibility);

// Find all the regions in the image using the flow algorithm and put them in tabRegion
//return all the idRegion of the different region
IdRegions* findRegionFlow(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int sensibility);
// Give the color of a neighboor
IdRegion* whatIsNeighboorsColor(DonneesImageTab* tabRegion, int x, int y);

// Returns the line who appears the most in the given Hough transform region.
//Sensibility is the minimum number of pixel that must be on the region to be counted as a line
Line* getCenterLineFromRegion(DonneesImageTab* tabHough, DonneesImageTab* tabRegion, IdRegion* idRegion, int sensibility);

// Returns a list of all the center of gravity of all regions in tabRegion
Point** getAllGravityPoints(DonneesImageTab* tabRegion, IdRegions *allIds);
// Show the center of gravity on the given tabRegion with a red dot
void colorGravityPointRegions(DonneesImageTab* tabRegion, Point** tabPoints, int size);
// Free a list of Point from the memory
void destructTabPoints(Point*** tabPoints, int size);

DonneesImageTab* getColorTabRegions(DonneesImageTab* tabRegion, IdRegions* idRegions);

