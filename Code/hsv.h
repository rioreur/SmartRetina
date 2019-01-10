// .h necessaires a l'application
#include "symetrie.h"

#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define MaxTableau 500
#define MaxMatrice 500
#define BLANC 255
#define NOIR 0
#define NAC -1

//definition des types

typedef int Tableau[MaxTableau];
typedef float Matrice[MaxMatrice][MaxMatrice];




//struct couleurRVB comportant les coordonnées du pixel
// dans les trois matrices R V B 
typedef struct couleurRVB {
	int x, y;
	int r, v, b;
}couleurRVB;

typedef struct couleurHSV {
	float h, s, v ;
}couleurHSV ; 

typedef struct tabCouleurHSV {
	int largeur ; 
	int hauteur ;
	couleurHSV** tabHsv ; 
} tabCouleurHSV; 


//struct matImageHSV comportant 3 matrice image 2D
//(un pixel a trois valeurs (h = hue = teinte, s = saturation, v = value = niveau (luminosité) )
typedef struct matImageHSV {
	int taille;
	float **h;
	float **s;
	float **v;
} matImageHSV ; 

/* Largeur et hauteur par defaut d'une fenetre correspondant a nos criteres */
#define LargeurFenetre LargeurImageBMPLib
#define HauteurFenetre HauteurImageBMPLib
#define Pixel_Min_Value 0
#define Pixel_Max_Value 255


/* Les prototypes des fonctions que nous aurons a ecrire */






//ALLOUE MATRICE
short int ** alloueMatrice(int h, int l );
float** alloueMatriceFloat(int h, int l) ; 



//CREER 3 MATRICES
void cree3Matrices(DonneesImageRGB *image, short int ***bleu, short int ***vert, short int ***rouge);

//CREE IMAGE
DonneesImageRGB *creeImage(int h, int l, short int **r, short int **v, short int **b);

//##################################################
//Fonction HSV
float divide255(int color);
float maxColor(float r, float g, float b);
float minColor(float r, float g, float b);
float hue(float r, float g, float b, float delta);
float saturation(float cmax, float delta);
couleurHSV RGBtoHSV(int R, int G, int B);
void afficheValeurCouleurHSV(matImageHSV* matHSV, int x, int y);
void testFonctionHSV(DonneesImageRGB *image);
couleurRVB HSVtoRGB(float H, float S, float V);
couleurHSV** alloueMatCouleurHsv(int hauteur, int largeur) ;
tabCouleurHSV* tabBgrToTabHsv(DonneesImageTab* tabBgr);
DonneesImageTab* tabHsvToTabBgr(tabCouleurHSV* tabHsv);
