#include <string.h>
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
} IdRegion;

typedef struct {
	IdRegion** regions;
	int size;
} IdRegions;

// Returns a DonneesImageTab with all his value set to UNCHECKED
DonneesImageTab* initTabRegion(int width, int height, int value);
// Initialisation function
IdRegion* initIdRegion(int blue, int green, int red, int x, int y);
IdRegions* initIdRegions(int size);
// Destruction function
void destructIdRegions(IdRegions** idRegions);

// Put the region which the pixel (x, y) is from into tabRegion. The region is in the color of the given pixel.
//returns the color of the said region
IdRegion* findRegionBottomUp(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int x, int y, int sensibility);
// Find all the regions in the image using the BottomUp algorithm and put them in tabRegion
//return all the idRegion of the different region
IdRegions* findAllRegionBottomUp(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int sensibility);

// Find all the regions in the image using the flow algorithm and put them in tabRegion
//return all the idRegion of the different region
//Core dumped et parche pas T_T
IdRegions* findRegionFlow(DonneesImageTab* tabImage, DonneesImageTab* tabRegion, int sensibility);
// Give the color of a neighboor
IdRegion* whatIsNeighboorsColor(DonneesImageTab* tabRegion, int x, int y);

//=============================================================================================================================


//Déclaration du type Germe
typedef struct germe{
	int hauteurGerme;
	int largeurGerme;
	bool set;
	int rouge;
	int bleu;
	int vert;
}germe;

//Définition du maximum de regions
#define MAX_REGIONS (10)

//Définition de l'acuité pour la segmentation de regions
#define ACUITE (10)

//Déclaration du type tableau de regions
typedef DonneesImageTab tableauRegion[MAX_REGIONS];

//Cette image représente les regions de l'image en type DonneesImageRGB passée en paramètre
DonneesImageRGB* creerImageRegion(DonneesImageRGB *image, tableauRegion tableau);

//Fonction qui cherche une germe dans une image en type imagePixel différente de la dernière germe
germe* chercheGermeImagePixel(DonneesImageTab *imageRetour, DonneesImageTab *imageTraitement, int hauteur, int largeur);


//Fonction qui colorise les voisins pour une région
//Et l'image de retour et l'image de donnees
void coloriseVoisinsRegionImagePixel(DonneesImageTab *retour, DonneesImageTab *image, int hauteurGerme, int largeurGerme, short int rouge, short int vert, short int bleu, 
									int compteur, tableauRegion tableau);

//Fonction qui interprete les regions sur une image de type image Pixel
//car durant le traitement les regions sont à -1
void interpreteRegionImagePixel(DonneesImageTab *retour);

//Fonction qui créer et initialise un tableauRegion
void initTableauRegion(tableauRegion tableau, int hauteur, int largeur, int taille);

//Fonction qui écrit dans le répertoire courant les images regions séparément
//Renvoie le nombre de régions
unsigned int ecrisRegions(tableauRegion regions, char *nom, int taille);

//Fonction qui regarde dans le tableau de régions, si à l'index donné, l'image est vide (255,255,255)
bool evalueRegionsVide(tableauRegion regions, int index);

