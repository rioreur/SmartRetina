// .h necessaires a l'application
#include "BmpLibSupport.h"
#include "GfxLib.h"
#include "ESLib.h"
#include "BmpLib.h"

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
typedef int ImageGris[HauteurImageBMPLib][LargeurImageBMPLib];
typedef int Tableau[MaxTableau];
typedef float Matrice[MaxMatrice][MaxMatrice];

//Structure définissant un noeud binaire (noir ou blanc)
typedef struct noeudBinaire{
	//Valeur du noeud (noir ou blanc)
	int val ; 
	//Position du pixel d'origine de la partie de l'image correspondant au noeud
	int x, y ; 
	//Numéro qui s'incrémente de 20 en 20 à partir de 0 le tout modulo 256
 	int etiquette;
 	//Noeuds lié au noeud
	struct noeudBinaire *fils1;
	struct noeudBinaire *fils2;
	struct noeudBinaire *fils3;
	struct noeudBinaire *fils4;
}noeudBinaire ;

//Structure définissant un noeud gris (niveau de gris)
typedef struct noeudGris{
	//Valeur du noeud (gris, un pixel a trois la même valeur)
	int val ; 
	int x, y ; 
	int etiquette; 
	struct noeudGris *fils1;
	struct noeudGris *fils2;
	struct noeudGris *fils3;
	struct noeudGris *fils4;
}noeudGris ;

//struct couleurRVB comportant les coordonnées du pixel
// dans les trois matrices R V B 
typedef struct couleurRVB {
	int x, y;
	int r, v, b;
}couleurRVB;

typedef struct couleurHSV {
	float h, s, v ;
}couleurHSV ; 

typedef struct noeudCouleur{
	couleurRVB val ; 
	int x, y ; 
	int etiquette; 
	struct noeudCouleur *fils1;
	struct noeudCouleur *fils2;
	struct noeudCouleur *fils3;
	struct noeudCouleur *fils4;
}noeudCouleur ;

//struct matImage comportant une matrice image 2D 
//(un pixel (RVB) a trois même valeurs (niveau de gris))
typedef struct matImage {
	int taille;
	short int **m ;
}matImage;

//struct matImageRVB comportant 3 matrice image 2D 
//(un pixel a trois valeurs pouvant être différente (couleurs))
typedef struct matImageRVB {
	int taille;
	short int **r;
	short int **v;
	short int **b;
}matImageRVB;

//struct matImageHSV comportant 3 matrice image 2D
//(un pixel a trois valeurs (h = hue = teinte, s = saturation, v = value = niveau (luminosité) )
typedef struct matImageHSV {
	int taille;
	float **h;
	float **s;
	float **v;
}matImageHSV;

/* Largeur et hauteur par defaut d'une fenetre correspondant a nos criteres */
#define LargeurFenetre LargeurImageBMPLib
#define HauteurFenetre HauteurImageBMPLib
#define Pixel_Min_Value 0
#define Pixel_Max_Value 255


/* Les prototypes des fonctions que nous aurons a ecrire */

//Permet d'afficher le menu, indique à l'utilisateur quoi taper pour faire quoi
void affiche_menu();
//Permet de changer la valeur d'un pixel
int ecrisPixelOK(ImageGris image, int x, int y, int pixel);
//Permet de récupérer la valeur d'un pixel
int lisPixelOK(ImageGris image, int x, int y);
//Permet de changer les couleurs en noir ou blanc selon si la valeur du pixel est inférieur ou supérieur au seuil
int seuille(ImageGris image, int seuil, ImageGris imageDestination);
//Donne la valeur maximal d'un pixel sur l'image
int maxImage(ImageGris image);
//Donne la valeur minimal d'un pixel sur l'image
int minImage(ImageGris image);
//Permet d’étendre la dynamique de l’image en niveaux de gris image de Pixel_Min_Value à Pixel_Max_Value
int maximise(ImageGris image);
//éclaircit ou l’assombrit en fonction du gamma
int Gamma(ImageGris image, float valeurGamma);
//Renvois la valeur moyenne des pixels en fonctions des pixels voisins
int moyennePixelVoisin(ImageGris image, int x, int y);
//valeur de chaque pixel sera donc la valeur moyenne du pixel et de tous les pixels dits 'premiersvoisins'
int floutage(ImageGris image, Matrice TABvaleursImage );

couleurRVB HSVtoRGB(float H, float S, float V);





//ALLOUE MATRICE
short int ** alloueMatrice(int h, int l );
float** alloueMatriceFloat(int h, int l) ; 

//LIBERE MATRICE
void libereMatrice(short int **m, int h, int l );

//CREER 3 MATRICES
void cree3Matrices(DonneesImageRGB *image, short int ***bleu, short int ***vert, short int ***rouge);

//CREE IMAGE
DonneesImageRGB *creeImage(int h, int l, short int **r, short int **v, short int **b);


//###############################################
//PARTIE 3 --- NIVEAU BINAIRE
//Retourne la valeur associée à un carré (sous-matrice)
int caculCarreNB(int taille, matImage *image, int x, int y);
//Créer un noeudBinaire (malloc)
noeudBinaire* creerNoeudBinaire(int x, int y);
//Créer arbre à partir de matrice image binaire
void creeArbreNB(matImage *image, int taille, noeudBinaire *noeudNB);
//Créer matImage Binaire
matImage* creeMatImageNB(DonneesImageRGB *image);
//Créer matImage Niveau Gris
matImage* creeMatImageNG(DonneesImageRGB *image);
//Libère une structure matImage en mémoire
void libereMatImage(matImage* mat, int taille);
//Affiche en console une matrice binaire
void afficheMatImage(matImage *imageMat); 
//Attache une étiquette à chaque noeud de l'arbre
void etiquetteFeuilleNB(noeudBinaire *racineNB, int *i);
//Créer une matrice binaire à partire d'un arbre
void creeMatriceArbreNB(noeudBinaire *noeudNB, int taille, matImage *image, bool etiq);
//Créer une image BMP à partir d'un imageMat
DonneesImageRGB* matABMP(matImage * imageMAT);

//###############################################
//PARTIE 4 --- NIVEAU GRIS 
//Renvois la valeur médiane du carré si ce carré en niveaux de gris est homogène
int caculCarreNG(int taille, matImage *image, int x, int y, int e);

//Créer un noeudBinaire (malloc)
noeudGris* creerNoeudGris(int x, int y);
//Libère un noeudGris
void libereNoeudGris(noeudGris* noeud);

//Créer arbre à partir de matrice image gris
void creeArbreNG(matImage *image, int taille, noeudGris *noeudNG, int e);
//Libère un arbre à partir de sa racine
void libereArbreNG(noeudGris *noeud);

//Transforme une matrice matImage en tableau 1D (pour pouvoir faire le trie facilement)
int* matATab1D(matImage *imageMat, int taille, int x, int y);

//Tri dans l'ordre croissant un tableau 1D et renvoie la valeur du milieu
int triTableauCroissant(int taille, int tab[]);

//Indique par un boolean si le tableau est trié
int estTrie(int taille, int tab[]);

//Attache une étiquette à chaque noeud de l'arbre
void etiquetteFeuilleNG(noeudGris *racineNG, int *i);

//Créer une matrice binaire à partir d'un arbre
void creeMatriceArbreNG(matImage *image, int taille, noeudGris *noeudNG, bool etiq);

//##############################################
//PARTIE 5 --- IMAGE RVB

//Créer une structre matImageRVB (trois matrices couleurs R V B + taille de l'image)
matImageRVB* creeMatImageRVB(DonneesImageRGB *image);

//Fait le calcul de distance entre deux couleurs
int calculDistance(matImageRVB *image, int xc1, int yc1, int xc2, int yc2);
//Cacule la distance entre deux couleurs ; 
int calculSommeDistance(matImageRVB *image, int x, int y, int taille, int xc1, int yc1);
//Calcule la couleur médiane d'une image ; 
couleurRVB calculCouleurMediane(matImageRVB *image, int x, int y, int taille);
//Renvois la valeur médiane du carré si ce carré en niveaux de gris est homogène
couleurRVB caculCarreRVB(matImageRVB *image, int taille, int x, int y, int e);
//Créer un arbre quarternaire à partir d'une image RVB (composé de trois matrices d'une couleur)
void creeArbreRVB(matImageRVB *image, int taille, noeudCouleur *noeudRVB, int e);
//Créer un noeudCouleur (allocation mémoire)
noeudCouleur* creerNoeudRVB(int x, int y);
//Attache une étiquette à chaque noeud de l'arbre
void etiquetteFeuilleRVB(noeudCouleur *racineRVB, int *i);
//Créer une matrice binaire à partir d'un arbre
void creeMatriceArbreRVB(matImageRVB *image, int taille, noeudCouleur *noeudRVB, bool etiq);
//Prend une imageRVB et renvois un BMP
DonneesImageRGB* rvbABMP(matImageRVB *imageRVB);

/*
//TRI TABLEAU POUR FONCTION MATRICE MEDIANE
int triTableauCroissant(int taille, short int tab[]);

//CHERCHE VALEUR POUR MATRICE MEDIANE
int chercheMediane(int l, int k, short int *ptrMatrice)

//CREER MATRICE MEDIANE (FONCTION FLOU IMAGE)
void creeMatriceMediane(short int **ptrMatriceMediane, matrice **ptrMatriceReference);
*/

//##################################################
//Fonction HSV
float divide255(int color);
float maxColor(float r, float g, float b);
float minColor(float r, float g, float b);
float hue(float r, float g, float b, float delta);
float saturation(float cmax, float delta);
couleurHSV RGBtoHSV(int R, int G, int B);
void afficheValeurCouleurRVB(matImageRVB* matRVB, int x, int y);
void afficheValeurCouleurHSV(matImageHSV* matHSV, int x, int y);
void testFonctionHSV(DonneesImageRGB *image);
void cree3MatricesHSV(matImageRVB *image, float ***hue, float ***sat, float ***val);
matImageHSV* creeMatImageHSV(matImageRVB *image) ; 



