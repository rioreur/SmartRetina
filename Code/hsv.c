
#include "mes_fonctions.h"


void affiche_menu() {
	sautDeLigne();
    ecrisChaine("*************  traitement de l'image: flamants.bmp ******** \n");
    ecrisChaine("taper a	pour tester les fonctions  ecrisPixelOK et lisPixelOK \n");
    ecrisChaine("taper s	pour  UN SEULLAGE DE L'IMAGE COURANTE   : \n");
    ecrisChaine("taper m	pour la MAXIMISATION  DE L'IMAGE COURANTE    :\n");
    ecrisChaine("taper i	pour recharger  L'IMAGE COURANTE    :\n");
    ecrisChaine("taper q    pour  SORTIR de l'APPLICATION  : \n");
    sautDeLigne();
}
//------FONCTION HSV
//################################
float divide255(int color)
{
	return( (float)color / 255) ; 	
}

float maxColor(float r, float g, float b)
{
	return fmaxf(fmaxf(r,g),b);
}

float minColor(float r, float g, float b)
{
	return fminf(fminf(r,g),b);
}

float hue(float r, float g, float b, float delta)
{
	float max ; 
	// min ;
	max = maxColor(r,g,b);
	// min = minColor(r,g,b);
	
	if(delta == 0)
	{
		return 0 ;
	}
	else if(max == r)
	{
		return 60 * ( fmodf(((g - b ) / delta),6) ) ;
	}
	else if (max == g)
	{
		return 60 * (((b - r ) / delta) + 2 ) ; 
	}
	else if (max == b)
	{
		return 60 * (((r - g) / delta) + 4 ) ; 
	}
	return -1 ; 
}

float saturation(float cmax, float delta)
{
	if(cmax == 0)
	{
		return 0 ; 
	}
	else
	{
		return delta/cmax ; 
	}
}


couleurHSV RGBtoHSV(int R, int G, int B)
{
	float delta, Cmax, r2, g2, b2 ;
	couleurHSV hsv; 
	
	r2 = divide255(R);
	g2 = divide255(G);
	b2 = divide255(B);
	/*
	printf("r2 : %f\n", r2);
	printf("g2 : %f\n", g2);
	printf("b2 : %f\n", b2);
	*/
	Cmax = maxColor(r2,g2,b2);
	//printf("Cmax : %f\n", Cmax);
	delta = Cmax - minColor(r2,g2,b2) ; 
	//printf("delta : %f\n", delta);
	
	hsv.h = hue(r2, g2, b2, delta); 
	hsv.s = saturation(Cmax, delta);
	hsv.v = Cmax ; 
	/*
	printf("R : %d\n", R);
	printf("G : %d\n", G);
	printf("B : %d\n", B);
	printf("\n H : %f\n", hsv.h);
	printf("S : %f\n", hsv.s);
	printf("V : %f\n", hsv.v);
	*/
	return hsv;
}

couleurRVB HSVtoRGB(float H, float S, float V)
{
	float c, x, m;
	float newR, newV, newB, newH ; 
	float R, G, B;
	couleurRVB rvb ; 
		
	c = V * S ;
	newH = H / 60 ; 
	printf("c: %f\n", c);
	x = c * ( 1- (abs( fmod(newH, 2) - 1  ) ) );
	printf("x: %f\n", x); 
	m = V - c ; 
	printf("m: %f\n", m);
	if(0 <= newH && newH < 60)
	{
		newR = c; 
		newV = x;
		newB = 0;
	}
	else if(60 < newH && newH <= 120)
	{
		newR = x; 
		newV = c;
		newB = 0;
	}
	else if(120 < newH && newH <= 180)
	{
		newR = 0; 
		newV = c;
		newB = x;
	}
	else if(180 < newH && newH <= 240)
	{
		newR = 0; 
		newV = x;
		newB = c;
		printf("coucou\n");
		printf("r:%f, v:%f, b:%f\n", newR, newV, newB);
	}
	else if(240 < newH && newH <= 300)
	{
		newR = x; 
		newV = 0;
		newB = c;
	}
	else if(300 < newH && newH <= 360)
	{
		newR = c; 
		newV = 0;
		newB = x;
	}
	
	R = (newR + m) * 255 ;
	G = (newV + m) * 255 ; 
	B = (newB + m) * 255 ; 	
	
	rvb.r = R; 
	rvb.v = G;
	rvb.b = B;
	printf("r:%f, v:%f, b:%f\n", R, G, B);
	return rvb;
}

void afficheValeurCouleurRVB(matImageRVB* matRVB, int x, int y)
{
	printf("x:%d y:%d R:%d V:%d B:%d \n", x, y, matRVB->r[x][y], matRVB->v[x][y], matRVB->b[x][y]);
	return;
}

void afficheValeurCouleurHSV(matImageHSV* matHSV, int x, int y)
{
	printf("x:%d y:%d H:%f S:%f V:%f \n", x, y, matHSV->h[x][y], matHSV->s[x][y], matHSV->v[x][y]);
	return;
}

void testFonctionHSV(DonneesImageRGB *image)
{
	matImageRVB* matRVB ; 
	matImageHSV* matHSV ; 
	printf("matRVB \n");
	matRVB = creeMatImageRVB(image) ; 
	printf("matHSV \n");
	matHSV = creeMatImageHSV(matRVB);
	printf("affiche valeur \n");
	afficheValeurCouleurRVB(matRVB, 255,255);
	afficheValeurCouleurHSV(matHSV, 255,255);
	return ; 
}
//---FIN FONCTION HSV
//################################

//Permet de récupérer la valeur d'un pixel
int lisPixelOK(ImageGris image, int x, int y)
{
        int valeurRetour ;
        // Si les coordonnees sont valides
	if ( ( x >= 0) && (x < LargeurImageBMPLib) && (y >= 0) && (y < HauteurImageBMPLib) )
        {
	 // Lire la valeur du pixel concerne */
        valeurRetour = image[y][x];
        return (valeurRetour);
        }
	else return(-1);
}

//ALLOUE MATRICE - Matrice 2D de type short int
short int ** alloueMatrice(int h, int l)
{
	short int **m ; 
	int i ;
	//Alloue tableau de pointeur de pointeur de short int de taille h(auteur)
	m = (short int **) malloc( sizeof(short int*) * h  );
	//Pour chaque case du tableau de taille h
	//on alloue un tableau de pointeur de short int de taille l(argeur)
	for(i=0;i<h;i++)
	{
		m[i] = (short int *)malloc(sizeof(short int)*l);
	}
	//on retourne la matrice 2D
	return m ; 
}

//ALLOUE MATRICE HSV - Matrice 2D de type float
float** alloueMatriceFloat(int h, int l)
{
	float **m ;
	int i ;
	
	m = (float **)malloc( sizeof(float*) * h);
	for(i=0 ; i<h ; i++)
	{
		m[i] = (float *)malloc(sizeof(float)*l);
	}
	//on retourne la matrice 2D
	return m ; 
}

//LIBERE MATRICE
void libereMatrice(short int **m, int h, int l )
{
	int i;
	//Pour toutes les cases du tableau  de pointeur de pointeur de short int de taille h(auteur)
	for(i=0;i<h;i++)
	{
		//on libère le tableau de pointeur de short int
		free(m[i]);
	}
	//On libère le tableau de pointeur de pointeur de short int
	free(m) ; 
	//On set le pointeur à NULL par sécurité (pour enlever une éventuel adresse attribuée)
	m = NULL;
}

//CREER 3 MATRICES R G B 
void cree3Matrices(DonneesImageRGB *image, short int ***bleu, short int ***vert, short int ***rouge)
{
	short int **b, **v, **r;
	int i, j, k=0 ; 
		//On alloue trois matrice rouge, vert, bleu
		r = alloueMatrice(image->hauteurImage, image->largeurImage);
		v = alloueMatrice(image->hauteurImage, image->largeurImage);
		b = alloueMatrice(image->hauteurImage, image->largeurImage);
		
		//Pour toutes les valeur de la matrice 2D
		for(i=0;i<image->hauteurImage; i++)
			for(j=0;j<image->largeurImage; j++)
			{
				//On prend la premiere case du tableau 1D RGB, on la met dans la matrice bleu
				b[i][j] = image->donneesRGB[k];
				k++;
				//On prend la deuxième case du tableau 1D RGB, on la met dans la matrice vert
				v[i][j] = image->donneesRGB[k];
				k++ ; 
				//On prend la troisième case du tableau 1D RGB, on la met dans la matrice rouge
				r[i][j] = image->donneesRGB[k];
				k++ ; 
				
				//...et ainsi de suite toutes les trois cases du tableau 1D RGB
			}
		//On passe toutes les matrice2D qu'on a alloué au matrice passé en paramètre
		*rouge = r;
		*vert = v;
		*bleu = b ; 
}

void cree3MatricesHSV(matImageRVB *image, float ***hue, float ***sat, float ***val)
{
	float **h ; 
	float **s ; 
	float **v ;  
	couleurHSV hsv ;
	int i, j;
	int taille = image->taille ; 
	
	h = alloueMatriceFloat(taille, taille);
	s = alloueMatriceFloat(taille, taille);
	v = alloueMatriceFloat(taille, taille);
	
	for(i=0 ; i<taille ; i++)
	{
		for(j=0 ; j<taille ; j++)
		{
			//Conversion d'une case couleur RGB en un case couleur HSV
			hsv = RGBtoHSV(image->r[i][j], image->v[i][j], image->b[i][j]);
			//Déplacement dans les matrices
			h[i][j] = hsv.h ; 
			s[i][j] = hsv.s ; 
			v[i][j] = hsv.v ; 
		}
	}
	//On passe toutes les matrice2D qu'on a alloué au matrice passé en paramètre
	*hue = h ;
	*sat = s ; 
	*val = v ;
	return ; 
}

//CREE IMAGE
DonneesImageRGB *creeImage(int h, int l, short int **r, short int **v, short int **b)
{
	DonneesImageRGB *image;
	int i, j, k=0 ; 
	//On créer est alloue la mémoire pour l'image RGB
	image = (DonneesImageRGB*)malloc(sizeof(DonneesImageRGB));
	image->largeurImage = l;
	image->hauteurImage = h ; 
	image->donneesRGB=(unsigned char *) malloc (sizeof(unsigned char)*l*h*3);
	
	//Pour toutes les cases des matrices 2D
	for(i=0;i<h;i++)
		for(j=0;j<l;j++)
		{
			//On prend la valeur des matrices 2D et on l'ajoute au tableau 1D RGB
			image->donneesRGB[k++]= b[i][j];
			image->donneesRGB[k++]= v[i][j];
			image->donneesRGB[k++]= r[i][j];
		}
		//On retourne l'image RGB
		return image ; 
		printf("creer image\n");
}

//Libère une structure matImage en mémoire
void libereMatImage(matImage* mat, int taille)
{
		libereMatrice(mat->m, taille, taille);
		mat->m = NULL ; 
		free(mat);
		mat = NULL ; 
}

//Libère une structure matImageRVB en mémoire
void libereMatImageRVB(matImageRVB* mat, int taille)
{
		libereMatrice(mat->r, taille, taille);
		libereMatrice(mat->v, taille, taille);
		libereMatrice(mat->b, taille, taille);
		mat->r = NULL ; 
		mat->v = NULL;
		mat->b = NULL;
		free(mat);
		mat = NULL ; 
}

//Affiche en console une matrice image 2D 
void afficheMatImage(matImage *imageMat)
{
	int i, j ; 
	//Pour toute les cases de la matrice 
	for(i=0 ; i<imageMat->taille ; i+=1)
	{
		for(j=0 ; j<imageMat->taille ; j+=1)
		{
			//On affiche sa valeur 
			printf("%d", imageMat->m[i][j]);
		}
		//On saute une ligne à chaque fin de ligne de la matrice 
		printf("\n");
	}
}

//Créer un noeudBinaire (malloc)
noeudBinaire* creerNoeudBinaire(int x, int y)
{
	//On déclare le pointeur de noeud
	noeudBinaire* newNoeud ; 
	//On alloue la mémoire au pointeur de noeud
	newNoeud = (noeudBinaire*) malloc(sizeof(noeudBinaire));
	//On set la valeur de noeud avec une valeur par défaut
	newNoeud->val = -1;
	//On set les pointeurs noeud fils du noeud avec une valeur par défaut
	newNoeud->fils1 = NULL;
	newNoeud->fils2 = NULL;
	newNoeud->fils3 = NULL;
	newNoeud->fils4 = NULL;
	//On entre les coordonnées mis en paramètre dans le noeud
	newNoeud->x = x; 
	newNoeud->y = y; 
	//On retourne le pointeur de noeud
	return(newNoeud);
}

//Retourne la valeur associée à un carré (sous-matrice)
int caculCarreNB(int taille, matImage *image, int x, int y)
{
	int i, j, couleur, resu;
	resu = 0 ;  
	couleur = image->m[x][y] ;
	printf("\ncalculCarreNB\n");
	
	printf("couleur : %d\n", couleur);
	printf("\t x: %d y: %d taille:%d \n", x, y, taille);
	for(i=x ; i<taille+x; i+=1)
	{
		for(j=y ; j<taille+y; j+=1)
		{
			if(image->m[i][j] != couleur)
			{
				resu = -1 ; 
			}
		}
	}
	if(resu != -1)
	{
		resu = couleur ; 
	}
	printf("resu calculCarreNB : %d\n", resu);
	return(resu);				
}

void creeArbreNB(matImage *image, int taille, noeudBinaire *noeudNB)
{
	int i = 0 ;
	int resu ; 
	noeudBinaire* newNoeud ; 
	resu = caculCarreNB(taille, image, noeudNB->x, noeudNB->y);
	
	if(resu == NAC)
	{
		if(i == 0)
		{
			//printf("nac1\n");
			noeudNB->val = NAC ; 
			printf("\t noeudNB->val : %d\n", noeudNB->val );
			newNoeud = creerNoeudBinaire(noeudNB->x+ 0,noeudNB->y +0);
			noeudNB->fils1 = newNoeud ; 
			i+=1;
			//printf("\t taille/2 : %d\n", taille/2);
			creeArbreNB(image, taille/2, noeudNB->fils1) ;
		}
		//printf("i: %d, resu: %d, taille: %d\n", i, resu, taille);
		if(i == 1)
		{
			
			//printf("nac2\n");
			//noeudNB->val = nac ; 
			//printf("\t noeudNB->val : %d\n", noeudNB->val );
			newNoeud = creerNoeudBinaire(noeudNB->x + taille/2, noeudNB->y + 0);
			noeudNB->fils2 = newNoeud ; 
			i+=1;
			//printf("\t taille/2 : %d\n", taille/2);
			creeArbreNB(image, taille/2, noeudNB->fils2) ;
		}
		//printf("i: %d, resu: %d, taille: %d\n", i, resu, taille);
		if(i == 2)
		{
			
			//printf("nac3\n");
			//noeudNB->val = nac ;
			//printf("\t noeudNB->val : %d\n", noeudNB->val ); 
			newNoeud = creerNoeudBinaire(noeudNB->x + taille/2, noeudNB->y + taille/2);
			noeudNB->fils3 = newNoeud ; 
			i+=1;
			//printf("\t taille/2 : %d\n", taille/2);
			creeArbreNB(image, taille/2, noeudNB->fils3) ;
		}
		//printf("i: %d, resu: %d, taille: %d\n", i, resu, taille);
		if(i == 3)
		{			
			//printf("nac4\n");
			//noeudNB->val = nac ;
			//printf("\t noeudNB->val : %d\n", noeudNB->val ); 
			newNoeud = creerNoeudBinaire(noeudNB->x + 0, noeudNB->y + taille/2);
			noeudNB->fils4 = newNoeud ; 
			i+=1;
			//printf("\t taille/2 : %d\n", taille/2);
			creeArbreNB(image, taille/2, noeudNB->fils4) ;
		}		
	}
	else
	{
		//printf("else\n");
		noeudNB->val = resu;
		printf("\t noeudNB->val : %d\n", noeudNB->val );
		noeudNB->fils1 = NULL;
		noeudNB->fils2 = NULL ; 
		noeudNB->fils3 = NULL ; 
		noeudNB->fils4 = NULL ;
	}
		
}

void etiquetteFeuilleNB(noeudBinaire *racineNB, int *i)
{
	if(racineNB != NULL)
	{
		if(racineNB->val == NAC)
		{
			racineNB->etiquette = -1 ; 
			etiquetteFeuilleNB(racineNB->fils1, i);
			etiquetteFeuilleNB(racineNB->fils2, i);
			etiquetteFeuilleNB(racineNB->fils3, i);
			etiquetteFeuilleNB(racineNB->fils4, i);
		}
		else
		{
		racineNB->etiquette = *i = (*i + 20) % 256 ; 
		}
	}
	printf("val : %d etiquette : %d \n", racineNB->val, racineNB->etiquette);
}

void creeMatriceArbreNB(noeudBinaire *noeudNB, int taille, matImage *image, bool etiq)
{
	int i, j;
	noeudBinaire* tabFils[4] =  {noeudNB->fils1, noeudNB->fils2, noeudNB->fils3, noeudNB->fils4} ; 
	
	if(image->m == NULL)
	{
			image->m = alloueMatrice(taille, taille);
			image->taille = taille; 
	}
	
	if(noeudNB->val != NAC)
	{
		for(i=noeudNB->x ; i<taille+noeudNB->x ; i+=1)
		{
			for(j=noeudNB->y ; j<taille+noeudNB->y ; j+=1)
			{
				if(etiq == 1)
				{
					image->m[i][j] = noeudNB->etiquette ; 
				}
				else
				{
					image->m[i][j] = noeudNB->val ; 
				}
			}
		}
	}	
	
	for(i=0 ; i<4 ; i+=1)
	{
		if(tabFils[i] != NULL)
		{
			creeMatriceArbreNB(tabFils[i], taille/2, image, etiq);
		}
	}
	
}

//Créer une image BMP à partir d'un imageMat
DonneesImageRGB* matABMP(matImage * imageMat)
{
	int i, j, k, taille;
	taille = imageMat->taille;
	DonneesImageRGB *imageDest ;
	imageDest = (DonneesImageRGB*)malloc(sizeof(DonneesImageRGB));
	imageDest->largeurImage = taille;
	imageDest->hauteurImage = taille; 
	imageDest->donneesRGB = (unsigned char *) malloc (sizeof(unsigned char)*taille*taille*3);
	
	k=0 ; 
	for(j=taille-1; j>=0 ; j-=1)
	//for(j=0 ; j<taille ; j+=1)
	{
		for(i=0 ; i<taille;i+=1)
		{
			imageDest->donneesRGB[k] = imageMat->m[i][j] ; 
			k+=1;
			imageDest->donneesRGB[k] = imageMat->m[i][j] ;
			k+=1;
			imageDest->donneesRGB[k] = imageMat->m[i][j] ;
			k+=1;
		}
	}
	return imageDest ; 
}


//FONCTION PARTIE 4

void afficheTab1D(int taille, int tab[])
{
	int i;
	printf("affiche tableau  \n");
	for(i=0;i<taille;i++)
	{
		printf("%d|", tab[i]);
	}
	printf("\n");
}

int* matATab1D(matImage *imageMat, int taille, int x, int y)
{
	int i, j, k;
	int* tab1D;
	tab1D = (int*)malloc(sizeof(int) * taille * taille ) ;
	//printf("matATabD afficheMatImage\n");
	//afficheMatImage(imageMat) ; 
	k=0;
	for(i=x ; i<taille+x ; i+=1)
	{
		for(j=y ; j<taille+y ; j+=1)
		{
			tab1D[k++] = imageMat->m[i][j] ; 
		}
	}
	//printf("k = %d\n", k);
	//printf("affiche tableau matATab1D\n");
	//afficheTab1D(taille*taille, tab1D);
	return(tab1D) ; 
}
			
//Créer un noeudGris(malloc)
noeudGris* creerNoeudGris(int x, int y)
{
	noeudGris* newNoeud ; 
	newNoeud = (noeudGris*) malloc(sizeof(noeudGris));
	newNoeud->val = -1;
	newNoeud->fils1 = NULL;
	newNoeud->fils2 = NULL;
	newNoeud->fils3 = NULL;
	newNoeud->fils4 = NULL;
	newNoeud->x = x; 
	newNoeud->y = y; 
	return(newNoeud);
}

void libereNoeudGris(noeudGris* noeud)
{
	free(noeud->fils1);
	free(noeud->fils2);
	free(noeud->fils3);
	free(noeud->fils4);
	noeud->fils1 = NULL;
	noeud->fils2 = NULL;
	noeud->fils3 = NULL;
	noeud->fils4 = NULL;
	free(noeud);
	noeud = NULL ; 
}

int caculCarreNG(int taille, matImage *image, int x, int y, int e)
{
	int i, j, resu, mc, ng;
	int* tab1D ;
	resu = 0 ;  
	ng = image->m[x][y];
	
	printf("entrer dans caculCarreNG\n");
	printf("création du tableau 1D\n");
	
	tab1D = matATab1D(image, taille, x, y) ; 
	
	printf("triTableauCroissant\n");
	mc = triTableauCroissant(taille*taille, tab1D) ; 
	printf("fin triTableau\n");
	
	printf("\n entrer dans boucle machin calculCarreNG\n");
	for(i=x ; i<taille+x ; i+=1)
	{
		for(j=y ; j<taille+y; j+=1)
		{
			if( abs(ng-mc) >= e ) 
			{
				resu = -1 ; 
			}
			ng = image->m[i][j] ;
		}
	}
	printf("entrer dans boucle machcine caculCarreNG\n");
	printf("resu1 : %d\n", resu);
	printf("mc : %d\n", mc);
	printf("fin boucle machine\n");
	
	if(resu != -1)
	{
		resu = mc ; 
	}
	printf("resu2 : %d", resu);
	return(resu);		
}

void creeArbreNG(matImage *image, int taille, noeudGris *noeudNG, int e)
{
	int i = 0 ;
	int resu ; 
	noeudGris* newNoeud ; 
	
	resu = caculCarreNG(taille, image, noeudNG->x, noeudNG->y, e);
	
	if(resu == -1)
	{
		//printf("i: %d, resu: %d, taille: %d\n", i, resu, taille);
		if(i == 0)
		{
			printf("nac1\n");
			noeudNG->val = -1 ; 
			printf("\t noeudNB->val : %d\n", noeudNG->val );
			newNoeud = creerNoeudGris(noeudNG->x+ 0,noeudNG->y +0);
			noeudNG->fils1 = newNoeud ; 
			i+=1;
			//printf("\t taille/2 : %d\n", taille/2);
			creeArbreNG(image, taille/2, noeudNG->fils1, e) ;
		}
		//printf("i: %d, resu: %d, taille: %d\n", i, resu, taille);
		if(i == 1)
		{
			
			//printf("nac2\n");
			//noeudNB->val = nac ; 
			//printf("\t noeudNB->val : %d\n", noeudNB->val );
			newNoeud = creerNoeudGris(noeudNG->x + taille/2, noeudNG->y + 0);
			noeudNG->fils2 = newNoeud ; 
			i+=1;
			//printf("\t taille/2 : %d\n", taille/2);
			creeArbreNG(image, taille/2, noeudNG->fils2, e) ;
		}
		//printf("i: %d, resu: %d, taille: %d\n", i, resu, taille);
		if(i == 2)
		{
			
			//printf("nac3\n");
			//noeudNB->val = nac ;
			//printf("\t noeudNB->val : %d\n", noeudNB->val ); 
			newNoeud = creerNoeudGris(noeudNG->x + taille/2, noeudNG->y + taille/2);
			noeudNG->fils3 = newNoeud ; 
			i+=1;
			//printf("\t taille/2 : %d\n", taille/2);
			creeArbreNG(image, taille/2, noeudNG->fils3, e) ;
		}
		//printf("i: %d, resu: %d, taille: %d\n", i, resu, taille);
		if(i == 3)
		{			
			//printf("nac4\n");
			//noeudNB->val = nac ;
			//printf("\t noeudNB->val : %d\n", noeudNB->val ); 
			newNoeud = creerNoeudGris(noeudNG->x + 0, noeudNG->y + taille/2);
			noeudNG->fils4 = newNoeud ; 
			i+=1;
			//printf("\t taille/2 : %d\n", taille/2);
			creeArbreNG(image, taille/2, noeudNG->fils4, e) ;
		}		
	}
	else
	{
		//printf("else\n");
		noeudNG->val = resu;
		printf("\t noeudNB->val : %d\n", noeudNG->val );
		noeudNG->fils1 = NULL;
		noeudNG->fils2 = NULL ; 
		noeudNG->fils3 = NULL ; 
		noeudNG->fils4 = NULL ;
	}
		
}

//Libere de la mémoire un arbre de noeudGris
void libereArbreNG(noeudGris *noeud)
{
	if(noeud == NULL)
	{
		return;
	}
	libereArbreNG(noeud->fils4);
	libereArbreNG(noeud->fils3);
	libereArbreNG(noeud->fils2);
	libereArbreNG(noeud->fils1);
	free(noeud);
}

void libereArbreBN(noeudBinaire *noeud)
{
	if(noeud == NULL)
	{
		return;
	}
	libereArbreBN(noeud->fils4);
	libereArbreBN(noeud->fils3);
	libereArbreBN(noeud->fils2);
	libereArbreBN(noeud->fils1);
	free(noeud);
}

void libereArbreRVB(noeudCouleur*noeud)
{
	if(noeud == NULL)
	{
		return;
	}
	libereArbreRVB(noeud->fils4);
	libereArbreRVB(noeud->fils3);
	libereArbreRVB(noeud->fils2);
	libereArbreRVB(noeud->fils1);
	free(noeud);
}

void etiquetteFeuilleNG(noeudGris *racineNG, int *i)
{
	if(racineNG != NULL)
	{
		if(racineNG->val == -1)
		{
			racineNG->etiquette = -1 ; 
			etiquetteFeuilleNG(racineNG->fils1, i);
			etiquetteFeuilleNG(racineNG->fils2, i);
			etiquetteFeuilleNG(racineNG->fils3, i);
			etiquetteFeuilleNG(racineNG->fils4, i);
		}
		else
		{
		racineNG->etiquette = *i = (*i + 20) % 256 ; 
		}
	}
	printf("val : %d etiquette : %d \n", racineNG->val, racineNG->etiquette);
}

void creeMatriceArbreNG(matImage *image, int taille, noeudGris *noeudNG, bool etiq)
{
	int i, j;
	noeudGris* tabFils[4] =  {noeudNG->fils1, noeudNG->fils2, noeudNG->fils3, noeudNG->fils4} ; 
	
	if(image->m == NULL)
	{
			image->m = alloueMatrice(taille, taille);
			image->taille = taille; 
	}
	
	if(noeudNG->val != -1)
	{
		for(i=noeudNG->x ; i<taille+noeudNG->x ; i+=1)
		{
			for(j=noeudNG->y ; j<taille+noeudNG->y ; j+=1)
			{
				if(etiq == 1)
				{
					image->m[i][j] = noeudNG->etiquette ; 
				}
				else
				{
					image->m[i][j] = noeudNG->val ; 
				}
			}
		}
	}	
	
	for(i=0 ; i<4 ; i+=1)
	{
		if(tabFils[i] != NULL)
		{
			creeMatriceArbreNG(image, taille/2, tabFils[i], etiq);
		}
	}
		
}

//FONCTIONS DE TRIE
int estTrie(int taille, int tab[])
{
	int i;
	bool boolean ; 
	boolean = 1 ; 
	for(i=1 ; i<taille ; i++)
	{
		if(tab[i]<tab[i-1])
		{
			boolean = 0 ; 
		}
	}
	return(boolean);		 
}

void echangeValeur(int *valeur1, int *valeur2)
{
		int sauv;
		sauv = *valeur1;
		*valeur1 = *valeur2;
		*valeur2 = sauv;
}


int triTableauCroissant(int taille, int tab[])
{
	int i ;
	bool boolean;
	boolean = 0 ; 
	//afficheTab1D(taille, tab);
	while(boolean == 0)
	{
		//i commence à 1 car on fait i-1 ensuite
		for(i=1; i<taille ;i+=1)
		{
			if(tab[i]<tab[i-1])
			{
				echangeValeur(&tab[i],&tab[i-1]); 
			}
			//printf("i:%d", i);
		}
		boolean = estTrie(taille, tab);
	}
	//printf("affiche tableau triTableauCroissant : \n");
	//afficheTab1D(taille, tab);
	return(tab[taille/2]);
}

//PARTIE 5 

matImageRVB* creeMatImageRVB(DonneesImageRGB *image)
{
	int taille = image->largeurImage;
	short int **rouge;
	short int **vert;
	short int **bleu;
	matImageRVB *imageRVB ; 
	imageRVB = (matImageRVB*)malloc(sizeof(matImageRVB));
	imageRVB->taille = taille;
	
	rouge = alloueMatrice(taille, taille);
	vert  = alloueMatrice(taille, taille);
	bleu  = alloueMatrice(taille, taille);
	cree3Matrices(image, &bleu, &vert, &rouge);
	
	imageRVB->r = rouge;
	imageRVB->v = vert;
	imageRVB->b = bleu;
	
	return imageRVB ; 
}
 

//HSV

//faire division en 3 matrice h s et v
//faire fonction qui met dans une struct
matImageHSV* creeMatImageHSV(matImageRVB *image)
{
	float **h;
	float **s;
	float **v;
	//struct 3 matrices de valeur h s v 
	matImageHSV *matHSV; 
	matHSV = (matImageHSV*)malloc(sizeof(matImageHSV));
	matHSV->taille = image->taille ; 
	//Allocation mémoire aux matrics h s v 
	h = alloueMatriceFloat(image->taille, image->taille);
	s = alloueMatriceFloat(image->taille, image->taille);
	v = alloueMatriceFloat(image->taille, image->taille);
	
	cree3MatricesHSV(image, &h, &s, &v) ; 
	matHSV->h = h;
	matHSV->s = s;
	matHSV->v = v;
	
	return matHSV ; 
}

//Cacule la distance entre deux couleurs ; 
int calculDistance(matImageRVB *image, int xc1, int yc1, int xc2, int yc2)
{
	//printf("entrer dans calculDistance \n");
	//xc et xy: coords x et y de la couleur
	int resu;
	//On effectue le calcule de la distance entre deux couleurs 
	resu = 
	sqrt(pow(image->r[xc1][yc1] - image->r[xc2][yc2],2) + 
		 pow(image->v[xc1][yc1] - image->v[xc2][yc2],2) +
		 pow(image->b[xc1][yc1] - image->b[xc2][yc2],2)) ;
	return(resu);
}

//Addition de toute les distances par rapport à une couleur donnée
int calculSommeDistance(matImageRVB *image, int x, int y, int taille, int xc1, int yc1)
{
	int i, j, resu;
	resu = 0;
	//printf("entrer dans calculSommeDistance\n");
	//Pour toutes les pixels d'une zone image...
	for(i=x ; i<taille+x ; i+=1)
	{
		for(j=y ; j<taille+y ; j+=1)
		{
			//... sauf le pixel de la couleur donnée : 
			if(i != xc1 && j != yc1)
			{
				//On calcule la distance entre le pixel et le pixel de la couleur donnée
				//... qu'on additionne au résultat précédent
				resu = resu + calculDistance(image, xc1, yc1, i, j);
			}
		}
	}
	return(resu);
}

//Retourne la couleur médiane d'une matrice (en calculant la somme de distance minimum)
couleurRVB calculCouleurMediane(matImageRVB *image, int x, int y, int taille)
{
	int i, j, sommeCouleur, nouvelleSomme;
	nouvelleSomme = 0;
	sommeCouleur = 0;
	couleurRVB cm;
	
	printf("entrer dans calculCouleurMediane\n");
	//Pour tous le pixel d'une zone image donnée
	for(i = x ; i<taille+x ; i+=1)
	{
		for(j = y ; j<taille+y ; j+=1)
		{
			
			if(nouvelleSomme == 0 && sommeCouleur == 0)
			{
				nouvelleSomme = calculSommeDistance(image, x, y, taille, i, j);
				sommeCouleur = nouvelleSomme ; 
				cm.x = i;
				cm.y = j;
				cm.r = image->r[i][j] ; 
				cm.v = image->v[i][j] ; 
				cm.b = image->b[i][j] ; 
			}
			//On calcule la somme des distances 
			//Si la somme des distances est inférieur à celle caculé 
			else
			{
				nouvelleSomme = calculSommeDistance(image, x, y, taille, i, j);
				if(nouvelleSomme < sommeCouleur)
				{
					//printf("nouvelle somme : %d\n", nouvelleSomme);
					//On prend cette somme comme somme minimum temporaire
					sommeCouleur = nouvelleSomme ;  
					//On sauvegarde les coordonnées de la couleur de la somme minimum
					cm.x = i;
					cm.y = j;
					cm.r = image->r[i][j] ; 
					cm.v = image->v[i][j] ; 
					cm.b = image->b[i][j] ; 
				}
			}
		}
	}
	printf("fin calculCouleurMédiane\n");
	//On renvoie la couleur médiane
	return cm;
}
	
//Renvois la couleur médiane si carré homogène 
couleurRVB caculCarreRVB(matImageRVB *image, int taille, int x, int y, int e)
{
	int i, j, xcm, ycm, resu; 
	couleurRVB cm; 
	
	resu = 0 ; 
	//printf("entrer dans calculCarreRVB\n");
	//On prend la couleur médiane 
	cm = calculCouleurMediane(image, x, y, taille);
	xcm = cm.x ; 
	ycm = cm.y ; 
	//printf("CalculCarreRVB cm.x : %d\n", cm.x);
	//printf("CalculCarreRVB cm.y : %d\n", cm.y);
	//Pour toutes les pixels de la zone image
	for(i=x ; i<taille+x ; i+=1)
	{
		for(j=y ; j<taille+y; j+=1)
		{
			//Si la distance est supérieur au seuil
			if( calculDistance(image, xcm, ycm, i, j)  >= e ) 
			{
				//le carré n'est pas homogène
				resu = -1 ; 
			}
		}
	}

	//Si la distance n'est jamais supérieur au seuil 
	if(resu == -1)
	{
		//On renvois la valeur -1
		cm.x = -1;
		cm.y = -1; 
	}
	return cm;		
}

//Créer un noeudGris(malloc)
noeudCouleur* creerNoeudRVB(int x, int y)
{
	noeudCouleur* newNoeud ; 
	newNoeud = (noeudCouleur*) malloc(sizeof(noeudCouleur));
	newNoeud->val.x = -1;
	newNoeud->val.y = -1;
	newNoeud->fils1 = NULL;
	newNoeud->fils2 = NULL;
	newNoeud->fils3 = NULL;
	newNoeud->fils4 = NULL;
	newNoeud->x = x; 
	newNoeud->y = y; 
	return(newNoeud);
}


void creeArbreRVB(matImageRVB *image, int taille, noeudCouleur *noeudRVB, int e)
{
	int i = 0 ;
	couleurRVB resu ; 
	noeudCouleur* newNoeud ; 
	
	resu = caculCarreRVB(image, taille, noeudRVB->x, noeudRVB->y, e);
	
	if(resu.x == -1)
	{
		//printf("i: %d, resu: %d, taille: %d\n", i, resu, taille);
		if(i == 0)
		{
			printf("nac1\n");
			noeudRVB->val.x = -1 ; 
			noeudRVB->val.y = -1 ; 
			printf("\t noeudNB->val.x : %d val.y : %d \n", noeudRVB->val.x, noeudRVB->val.y );
			newNoeud = creerNoeudRVB(noeudRVB->x+ 0,noeudRVB->y +0);
			noeudRVB->fils1 = newNoeud ; 
			i+=1;
			//printf("\t taille/2 : %d\n", taille/2);
			creeArbreRVB(image, taille/2, noeudRVB->fils1, e) ;
		}
		//printf("i: %d, resu: %d, taille: %d\n", i, resu, taille);
		if(i == 1)
		{
			
			//printf("nac2\n");
			//noeudNB->val = nac ; 
			//printf("\t noeudNB->val : %d\n", noeudNB->val );
			newNoeud = creerNoeudRVB(noeudRVB->x + taille/2, noeudRVB->y + 0);
			noeudRVB->fils2 = newNoeud ; 
			i+=1;
			//printf("\t taille/2 : %d\n", taille/2);
			creeArbreRVB(image, taille/2, noeudRVB->fils2, e) ;
		}
		//printf("i: %d, resu: %d, taille: %d\n", i, resu, taille);
		if(i == 2)
		{
			
			//printf("nac3\n");
			//noeudNB->val = nac ;
			//printf("\t noeudNB->val : %d\n", noeudNB->val ); 
			newNoeud = creerNoeudRVB(noeudRVB->x + taille/2, noeudRVB->y + taille/2);
			noeudRVB->fils3 = newNoeud ; 
			i+=1;
			//printf("\t taille/2 : %d\n", taille/2);
			creeArbreRVB(image, taille/2, noeudRVB->fils3, e) ;
		}
		//printf("i: %d, resu: %d, taille: %d\n", i, resu, taille);
		if(i == 3)
		{			
			//printf("nac4\n");
			//noeudNB->val = nac ;
			//printf("\t noeudNB->val : %d\n", noeudNB->val ); 
			newNoeud = creerNoeudRVB(noeudRVB->x + 0, noeudRVB->y + taille/2);
			noeudRVB->fils4 = newNoeud ; 
			i+=1;
			//printf("\t taille/2 : %d\n", taille/2);
			creeArbreRVB(image, taille/2, noeudRVB->fils4, e) ;
		}		
	}
	else
	{
		//printf("else\n");
		noeudRVB->val = resu;
		printf("\t noeudNB->val.x : %d  val.y : %d \n", noeudRVB->val.x, noeudRVB->val.y );
		noeudRVB->fils1 = NULL;
		noeudRVB->fils2 = NULL ; 
		noeudRVB->fils3 = NULL ; 
		noeudRVB->fils4 = NULL ;
	}
		
}


void etiquetteFeuilleRVB(noeudCouleur *racineRVB, int *i)
{
	if(racineRVB != NULL)
	{
		if(racineRVB->val.x == -1)
		{
			racineRVB->etiquette = -1 ; 
			etiquetteFeuilleRVB(racineRVB->fils1, i);
			etiquetteFeuilleRVB(racineRVB->fils2, i);
			etiquetteFeuilleRVB(racineRVB->fils3, i);
			etiquetteFeuilleRVB(racineRVB->fils4, i);
		}
		else
		{
		racineRVB->etiquette = *i = (*i + 1) % 256 ; 
		}
	}
	printf("val.x : %d, val.y : %d etiquette : %d \n", racineRVB->val.x, racineRVB->val.y, racineRVB->etiquette);
}

void creeMatriceArbreRVB(matImageRVB *image, int taille, noeudCouleur *noeudRVB, bool etiq)
{
	int i, j;
	
	const unsigned char maPALETTE[256][3] = {
    {0, 0, 0},
    {128, 0, 0},
    {0, 128, 0},
    {128, 128, 0},
    {0, 0, 128},
    {128, 0, 128},
    {0, 128, 128},
    {192, 192, 192},
    {128, 128, 128},
    {0, 255, 0},
    {255, 0, 0},
    {255, 255, 0},
    {0, 0, 255},
    {255, 0, 255},
    {0, 255, 255},
    {255, 255, 255},
    {0, 0, 95},
    {0, 0, 135},
    {0, 0, 175},
    {0, 0, 215},
    {0, 0, 255},
    {0, 95, 0},
    {0, 95, 95},
    {0, 95, 135},
    {0, 95, 175},
    {0, 95, 215},
    {0, 95, 255},
    {0, 135, 0},
    {0, 135, 95},
    {0, 135, 135},
    {0, 135, 175},
    {0, 135, 215},
    {0, 135, 255},
    {0, 175, 0},
    {0, 175, 95},
    {0, 175, 135},
    {0, 175, 175},
    {0, 175, 215},
    {0, 175, 255},
    {0, 215, 0},
    {0, 215, 95},
    {0, 215, 135},
    {0, 215, 175},
    {0, 215, 215},
    {0, 215, 255},
    {0, 255, 0},
    {0, 255, 95},
    {0, 255, 135},
    {0, 255, 175},
    {0, 255, 215},
    {0, 255, 255},
    {95, 0, 0},
    {95, 0, 95},
    {95, 0, 135},
    {95, 0, 175},
    {95, 0, 215},
    {95, 0, 255},
    {95, 95, 0},
    {95, 95, 95},
    {95, 95, 135},
    {95, 95, 175},
    {95, 95, 215},
    {95, 95, 255},
    {95, 135, 0},
    {95, 135, 95},
    {95, 135, 135},
    {95, 135, 175},
    {95, 135, 215},
    {95, 135, 255},
    {95, 175, 0},
    {95, 175, 95},
    {95, 175, 135},
    {95, 175, 175},
    {95, 175, 215},
    {95, 175, 255},
    {95, 215, 0},
    {95, 215, 95},
    {95, 215, 135},
    {95, 215, 175},
    {95, 215, 215},
    {95, 215, 255},
    {95, 255, 0},
    {95, 255, 95},
    {95, 255, 135},
    {95, 255, 175},
    {95, 255, 215},
    {95, 255, 255},
    {135, 0, 0},
    {135, 0, 95},
    {135, 0, 135},
    {135, 0, 175},
    {135, 0, 215},
    {135, 0, 255},
    {135, 95, 0},
    {135, 95, 95},
    {135, 95, 135},
    {135, 95, 175},
    {135, 95, 215},
    {135, 95, 255},
    {135, 135, 0},
    {135, 135, 95},
    {135, 135, 135},
    {135, 135, 175},
    {135, 135, 215},
    {135, 135, 255},
    {135, 175, 0},
    {135, 175, 95},
    {135, 175, 135},
    {135, 175, 175},
    {135, 175, 215},
    {135, 175, 255},
    {135, 215, 0},
    {135, 215, 95},
    {135, 215, 135},
    {135, 215, 175},
    {135, 215, 215},
    {135, 215, 255},
    {135, 255, 0},
    {135, 255, 95},
    {135, 255, 135},
    {135, 255, 175},
    {135, 255, 215},
    {135, 255, 255},
    {175, 0, 0},
    {175, 0, 95},
    {175, 0, 135},
    {175, 0, 175},
    {175, 0, 215},
    {175, 0, 255},
    {175, 95, 0},
    {175, 95, 95},
    {175, 95, 135},
    {175, 95, 175},
    {175, 95, 215},
    {175, 95, 255},
    {175, 135, 0},
    {175, 135, 95},
    {175, 135, 135},
    {175, 135, 175},
    {175, 135, 215},
    {175, 135, 255},
    {175, 175, 0},
    {175, 175, 95},
    {175, 175, 135},
    {175, 175, 175},
    {175, 175, 215},
    {175, 175, 255},
    {175, 215, 0},
    {175, 215, 95},
    {175, 215, 135},
    {175, 215, 175},
    {175, 215, 215},
    {175, 215, 255},
    {175, 255, 0},
    {175, 255, 95},
    {175, 255, 135},
    {175, 255, 175},
    {175, 255, 215},
    {175, 255, 255},
    {215, 0, 0},
    {215, 0, 95},
    {215, 0, 135},
    {215, 0, 175},
    {215, 0, 215},
    {215, 0, 255},
    {215, 95, 0},
    {215, 95, 95},
    {215, 95, 135},
    {215, 95, 175},
    {215, 95, 215},
    {215, 95, 255},
    {215, 135, 0},
    {215, 135, 95},
    {215, 135, 135},
    {215, 135, 175},
    {215, 135, 215},
    {215, 135, 255},
    {215, 175, 0},
    {215, 175, 95},
    {215, 175, 135},
    {215, 175, 175},
    {215, 175, 215},
    {215, 175, 255},
    {215, 215, 0},
    {215, 215, 95},
    {215, 215, 135},
    {215, 215, 175},
    {215, 215, 215},
    {215, 215, 255},
    {215, 255, 0},
    {215, 255, 95},
    {215, 255, 135},
    {215, 255, 175},
    {215, 255, 215},
    {215, 255, 255},
    {255, 0, 0},
    {255, 0, 95},
    {255, 0, 135},
    {255, 0, 175},
    {255, 0, 215},
    {255, 0, 255},
    {255, 95, 0},
    {255, 95, 95},
    {255, 95, 135},
    {255, 95, 175},
    {255, 95, 215},
    {255, 95, 255},
    {255, 135, 0},
    {255, 135, 95},
    {255, 135, 135},
    {255, 135, 175},
    {255, 135, 215},
    {255, 135, 255},
    {255, 175, 0},
    {255, 175, 95},
    {255, 175, 135},
    {255, 175, 175},
    {255, 175, 215},
    {255, 175, 255},
    {255, 215, 0},
    {255, 215, 95},
    {255, 215, 135},
    {255, 215, 175},
    {255, 215, 215},
    {255, 215, 255},
    {255, 255, 0},
    {255, 255, 95},
    {255, 255, 135},
    {255, 255, 175},
    {255, 255, 215},
    {255, 255, 255},
    {8, 8, 8},
    {18, 18, 18},
    {28, 28, 28},
    {38, 38, 38},
    {48, 48, 48},
    {58, 58, 58},
    {68, 68, 68},
    {78, 78, 78},
    {88, 88, 88},
    {98, 98, 98},
    {108, 108, 108},
    {118, 118, 118},
    {128, 128, 128},
    {138, 138, 138},
    {148, 148, 148},
    {158, 158, 158},
    {168, 168, 168},
    {178, 178, 178},
    {188, 188, 188},
    {198, 198, 198},
    {208, 208, 208},
    {218, 218, 218},
    {228, 228, 228},
    {238, 238, 238},
    {248, 248, 248},
};
	
	noeudCouleur* tabFils[4] =  {noeudRVB->fils1, noeudRVB->fils2, noeudRVB->fils3, noeudRVB->fils4} ; 
		
	if(image->r == NULL)
	{
			image->r = alloueMatrice(taille, taille);
			image->taille = taille; 
	}
	if(image->v == NULL)
	{
			image->b = alloueMatrice(taille, taille);
			image->taille = taille; 
	}
	if(image->b == NULL)
	{
			image->b = alloueMatrice(taille, taille);
			image->taille = taille; 
	}
	
	if(noeudRVB->val.x != -1)
	{
		for(i=noeudRVB->x ; i<taille+noeudRVB->x ; i+=1)
		{
			for(j=noeudRVB->y ; j<taille+noeudRVB->y ; j+=1)
			{
				if(etiq == 1)
				{
					image->r[i][j] = maPALETTE[noeudRVB->etiquette][0] ;
					image->v[i][j] = maPALETTE[noeudRVB->etiquette][1] ;
					image->b[i][j] = maPALETTE[noeudRVB->etiquette][2] ;
				}
				else
				{
					image->r[i][j] = noeudRVB->val.r ;
					image->v[i][j] = noeudRVB->val.v ;
					image->b[i][j] = noeudRVB->val.b ;
				}
			}
		}
	}	
	//4 est le nombre de fils
	for(i=0 ; i<4 ; i+=1)
	{
		if(tabFils[i] != NULL)
		{
			creeMatriceArbreRVB(image, taille/2, tabFils[i], etiq);
		}
	}
		
}

//Créer une image BMP à partir d'un imageMatRVB
DonneesImageRGB* rvbABMP(matImageRVB * imageRVB)
{
	int i, j, k, taille;
	taille = imageRVB->taille;
	DonneesImageRGB *imageDest ;
	imageDest = (DonneesImageRGB*)malloc(sizeof(DonneesImageRGB));
	imageDest->largeurImage = taille;
	imageDest->hauteurImage = taille; 
	imageDest->donneesRGB = (unsigned char *) malloc (sizeof(unsigned char)*taille*taille*3);
	
	k=0 ; 
	//for(j=taille-1; j>=0 ; j-=1)
	for(i=0 ; i<taille ; i+=1)
	{
		for(j=0 ; j<taille;j+=1)
		{
			imageDest->donneesRGB[k] = imageRVB->r[i][j] ; 
			k+=1;
			imageDest->donneesRGB[k] = imageRVB->v[i][j] ;
			k+=1;
			imageDest->donneesRGB[k] = imageRVB->b[i][j] ;
			k+=1;
		}
	}
	return imageDest ; 
}




