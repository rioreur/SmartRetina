#include "region.h"

//they don't use the idRegion, correct this

////////////////////////////
//--- Central symmetry ---//
////////////////////////////

// Returns the region with the given idRegion in a DonneesImageTab with the correct lenght.
DonneesImageTab* getShape(DonneesImageTab* tabRegion, IdRegion* idRegion);
// Returns the central symetric of the image
DonneesImageTab* getSymetricShape(DonneesImageTab* tabShape);
// Returns a global shape of the given tabShape using a tabSymetricShape
DonneesImageTab* createGlobalShape(DonneesImageTab* tabShape, DonneesImageTab* tabSymetricShape);
// Returns true if the pixel at the given coordinate in tabShape is at the border of a shape
bool isABorder(DonneesImageTab* tabShape, int x, int y);
// Draw the given shape into the given DonneesImageTab at the given coordinate
void drawShape(DonneesImageTab* tabImage, DonneesImageTab* tabShape, int x, int y);
// Give the area in pixels of the given shape
int getArea(DonneesImageTab* tabShape);

//////////////////////////
//--- Axial symmetry ---//
//////////////////////////

// Create a Hough transform from the given DonneesImageTab
//DonneesImageTab* createTabAxis(DonneesImageTab* tabImage, int sensibility);


