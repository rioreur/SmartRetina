#include "image.h"

// Tree identification
#ifndef T_NAC
	#define T_NAC -1
#endif
#ifndef T_BLACK
	#define T_BLACK 0
#endif
#ifndef T_WHITE
	#define T_WHITE 1
#endif

typedef struct Node
{
	int color;
	int label;
	int x1;
	int y1;
	int x2;
	int y2;
	struct Node* son1;
	struct Node* son2;
	struct Node* son3;
	struct Node* son4;
} Node;

typedef struct NodeRGB
{
	int colorR;
	int colorG;
	int colorB;
	int label;
	int x1;
	int y1;
	int x2;
	int y2;
	struct NodeRGB* son1;
	struct NodeRGB* son2;
	struct NodeRGB* son3;
	struct NodeRGB* son4;
} NodeRGB;

////////////////////
//--- QuadTree ---//
////////////////////

// Pour les couleurs, faire la médiane des couleurs. Pour ça, calculer la somme des distances (en couleur) d'un pixel avec tout les autres.
// Celui qui a la plus faible l'emporte.

// Initialisation functions
Node* initNode(int x1, int y1, int x2, int y2);
NodeRGB* initNodeRGB(int x1, int y1, int x2, int y2);
// Destruction functions
void destructNode(Node** node);
void destructNodeRGB(NodeRGB** node);
// Show the tree in the consol
void showTree(Node* node, int index);

// Calculates and returns the value of the given square. It can be T_WHITE, T_BLACK or T_NAC (if it isn't white nor black)
int calculCarreNB(DonneesImageTab* tabImage, int x1, int y1, int x2, int y2);
// Calculates and returns the mediane value of the given square. It can be between 0 and 255 or T_NAC if the pixels are not close enought
int calculCarreNG(DonneesImageTab* tabImage, int x1, int y1, int x2, int y2, int sensibility);
// set the mediane RGB value of the given node. It is T_NAC if the pixels are not close enought
void calculCarreRGB(DonneesImageTab* tabImage, NodeRGB* node, int sensibility);

// Create a tree and gives each node the corresponding "color" using the calculCarreNB function (black and white)
void creeArbreNB(DonneesImageTab* tabImage, Node* node);
// Create a tree and gives each node the corresponding "color" using the calculCarreNG function (grey level)
void creeArbreNG(DonneesImageTab* tabImage, Node* node, int sensibility);
// Create a tree and gives each node the corresponding "color" using the calculCarreRGB function (colors)
void creeArbreRGB(DonneesImageTab* tabImage, NodeRGB* node, int sensibility);

// Gives to each leafs of the tree a unique label between 0 and 256
int etiquetteFeuille(Node* node, int label);
int etiquetteFeuilleRGB(NodeRGB* node, int label);

// Create a DonneesImageTab from a black and white tree
void creeMatriceArbreNB(DonneesImageTab* tabImage, Node* node, bool etiq);
// Create a DonneesImageTab from a grey level tree
void creeMatriceArbreNG(DonneesImageTab* tabImage, Node* node, bool etiq);
// Create a DonneesImageTab from a grey level tree
void creeMatriceArbreRGB(DonneesImageTab* tabImage, NodeRGB* node, bool etiq);
