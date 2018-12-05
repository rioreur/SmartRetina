#include "image.h"

#ifndef BORDER
	#define BORDER -1
#endif
#ifndef UNCHECKED
	#define UNCHECKED -2
#endif

// Returns a DonneesImageTab with all his value set to UNCHECKED
DonneesImageTab* initTabRegion(int width, int height);

// Returns a DonneesImageTab which contain the region which the pixel (x, y) is from. The region is in the color of the given pixel.
void findRegionTopDown(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int x, int y, int sensibility);

