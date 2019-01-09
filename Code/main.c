//General header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//More specific headers files
#include <time.h>
#include <math.h>
#include <stdbool.h>

//Header file
#include "symetrie.h"

int main(int argc, char** argv)
{
   //Déclaration de variables utiles ===================================
   
	unsigned int nombreRegion;
	
   //Déclaration de touts nos pointeurs ================================
	
	//Pointeur vers un tableau de region
	tableauRegion regions;
	
	//Pointeur vers l'image en couleur
	DonneesImageRGB *image = NULL;
	
	//Pointeur vers une image région
	DonneesImageRGB *imageRegions = NULL;
	
   //===================================================================
	
	// We get the image we will use for the test
	printf("Reading the image\n");
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
		//On l'initialise dans la foulé le tableauRegion
		initTableauRegion(regions, image->hauteurImage, image->largeurImage, MAX_REGIONS); 
	
		//On crée notre image représentant les regions
		imageRegions = creerImageRegion(image, regions);
				
		//On l'écrit dans le répertoire courant
		ecrisBMPRGB_Dans(imageRegions, "regions_e.bmp");
	
		//On l'écrit dans le répertoire "regions" courant les images regions séparément
		nombreRegion = ecrisRegions(regions, "_region_e.bmp", MAX_REGIONS);
		printf("Nombre de région : %u \n", nombreRegion);
		
		//Libération de mémoire
		libereDonneesImageRGB(&image);
		libereDonneesImageRGB(&imageRegions);
	}
	else
	{
		printf("Error : Can't read the image\n");
	}
	printf("---Done\n");
	return 0;
}
