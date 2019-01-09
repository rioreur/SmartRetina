#include "hsv.h"

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

/*
DonneesImageTab tabBgrToTabHsv(DonneesImageTab tabBgr)
{
	couleurHSV hsv ; 
	DonneesImageTab tabHsv ; 
	tabHsv = initTab(tabBgr->largeurImage, tabBgr->hauteurImage) ;
	int i1, i2, i3 ; 
	int blue, green, red ; 
	
	for(i1=0 ; i1<largeurImage ; i1+=1)
	{
		for(i2 ; i2<hauteurImage ; i2+=1)
		{
			hsv = RGBtoHSV( tabBgr[i1][i2][BLUE], tabBgr[i1][i2][GREEN], tabBgr[i1][i2][RED] ) ; 
			(tabHsv[i1][i2])->tabGhsv.h ;
			tabHsv[i1][i2][hsv.s] ; 
			tabHsv[i1][i2][hsv.v] ;  
		}
	}
	return tabHsv ; 
}
*/ 
couleurHSV** alloueMatCouleurHsv(int hauteur, int largeur)
{
	couleurHSV** matHsv ; 
	int i ;
	matHsv = (couleurHSV**)malloc(sizeof(couleurHSV*) * largeur ) ; 
	for(i=0;i<hauteur;i++)
	{
		matHsv[i] = (couleurHSV*)malloc(sizeof(couleurHSV)*largeur);
	}
	//on retourne la matrice CouleurHsv
	return matHsv ; 
}

tabCouleurHSV* tabBgrToTabHsv(DonneesImageTab* tabBgr)
{
	couleurHSV hsv ;
	tabCouleurHSV* tabStructHsv ;
	int largeurImage = tabBgr->largeurImage ; 
	int hauteurImage = tabBgr->hauteurImage ; 
	tabStructHsv = 	(tabCouleurHSV*)malloc(sizeof(tabCouleurHSV)) ; 
	tabStructHsv->tabHsv = alloueMatCouleurHsv(largeurImage, hauteurImage) ; 
	int i1, i2, i3 ; 
	int blue, green, red ; 
	
	for(i1=0 ; i1<largeurImage ; i1+=1)
	{
		for(i2 ; i2<hauteurImage ; i2+=1)
		{
			hsv = RGBtoHSV( tabBgr->donneesTab[i1][i2][BLUE], tabBgr->donneesTab[i1][i2][GREEN], tabBgr->donneesTab[i1][i2][RED] ) ; 
			tabStructHsv->tabHsv[i1][i2] = hsv ; 
		}
	}
	return tabStructHsv ; 
}

couleurRVB HSVtoRGB(float H, float S, float V)
{
	float c, x, m;
	float newR, newV, newB, newH ; 
	float R, G, B;
	couleurRVB rvb ; 
		
	c = V * S ;
	newH = H / 60 ; 
	printf("newH = %f", newH);
	printf("H = %f", H);
	printf("c: %f\n", c);
	x = c * ( 1- (fabs( fmod(newH, 2) - 1  ) ) );
	printf("x: %f\n", x); 
	m = V - c ; 
	printf("m: %f\n", m);
	if(0 <= H && H < 60)
	{
		printf("if 1 \n");
		newR = c; 
		newV = x;
		newB = 0;
	}
	else if(60 <= H && H < 120)
	{
		printf("if 2 \n");
		newR = x; 
		newV = c;
		newB = 0;
	}
	else if(120 <= H && H < 180)
	{
		printf("if 3 \n");
		newR = 0; 
		newV = c;
		newB = x;
	}
	else if(180 <= H && H < 240)
	{
		printf("if 4 \n");
		newR = 0; 
		newV = x;
		newB = c;
		printf("coucou\n");
		printf("r:%f, v:%f, b:%f\n", newR, newV, newB);
	}
	else if(240 <= H && H < 300)
	{
		printf("if 5 \n");
		newR = x; 
		newV = 0;
		newB = c;
	}
	else if(300 <= H && H < 360)
	{
		printf("if 6 \n");
		newR = c; 
		newV = 0;
		newB = x;
	}
	else
	{
		newR = 0; 
		newV = 0;
		newB = 0;
	}
	
	R = (newR + m) ;
	G = (newV + m) ; 
	B = (newB + m) ; 	
	
	if(R > 255)
	{
		R=255;
	}
	if(V > 255)
	{
		V=255;
	}
	if(B > 255 )
	{
		B = 255 ; 
	}
	if(R < 0 )
	{
		R = 0 ;
	}
	if( G < 0 )
	{
		G = 0 ; 
	}
	if( B < 0 )
	{
		B = 0 ; 
	}
	
	R = (R * 255)/100 ;
	G = (G * 255)/100 ;
	B = (B * 255)/100 ; 
	
	rvb.r = R; 
	rvb.v = G;
	rvb.b = B;
	printf("r:%f, v:%f, b:%f\n", R, G, B);
	return rvb;
}

void afficheValeurCouleurHSV(matImageHSV* matHSV, int x, int y)
{
	printf("x:%d y:%d H:%f S:%f V:%f \n", x, y, matHSV->h[x][y], matHSV->s[x][y], matHSV->v[x][y]);
	return;
}

/*
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
* */
//---FIN FONCTION HSV
//################################


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

/*
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
*/


//HSV

//faire division en 3 matrice h s et v
//faire fonction qui met dans une struct
/*
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
*/



