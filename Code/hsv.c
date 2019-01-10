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
	float max, h ; 
	// min ;
	max = maxColor(r,g,b);
	// min = minColor(r,g,b);
	h = -1 ; 
	if(delta == 0)
	{
		h = 0 ;
	}
	else if(max == r)
	{
		h =  60 * (((g - b ) / delta))  ;
	}
	else if (max == g)
	{
		h = 60 * (((b - r ) / delta) + 2 ) ; 
	}
	else if (max == b)
	{
		h = 60 * (((r - g) / delta) + 4 ) ; 
	}
	if(h<0)
	{
		h = h + 360 ;
	}
	return h ; 
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
	tabStructHsv = 	(tabCouleurHSV*)malloc(sizeof(tabCouleurHSV)) ;
	tabStructHsv->largeur = tabBgr->largeurImage ; 
	tabStructHsv->hauteur = tabBgr->hauteurImage ; 
	tabStructHsv->tabHsv = alloueMatCouleurHsv(tabStructHsv->largeur, tabStructHsv->hauteur) ; 
	int i1, i2 ; 
	
	for(i1=0 ; i1<tabStructHsv->largeur ; i1+=1)
	{
		for(i2=0 ; i2<tabStructHsv->hauteur ; i2+=1)
		{
			hsv = RGBtoHSV(tabBgr->donneesTab[i1][i2][RED], tabBgr->donneesTab[i1][i2][GREEN], tabBgr->donneesTab[i1][i2][BLUE]) ;  
			tabStructHsv->tabHsv[i1][i2] = hsv ; 
		}
	}
	return tabStructHsv ; 
}

//Take a HSV matrix and return a BGR matrix
DonneesImageTab* tabHsvToTabBgr(tabCouleurHSV* tabStructHsv)
{
	couleurRVB rvb ;
	DonneesImageTab* imageTab ;  
	int i1, i2 ; 
	int largeur = tabStructHsv->largeur ; 
	int hauteur = tabStructHsv->hauteur ; 
	imageTab = initTab(largeur, hauteur) ;
	for(i1=0 ; i1<largeur ; i1+=1)
	{
		for(i2=0 ; i2<hauteur ; i2+=1)
		{
			rvb = HSVtoRGB(tabStructHsv->tabHsv[i1][i2].h, tabStructHsv->tabHsv[i1][i2].s, tabStructHsv->tabHsv[i1][i2].v) ; 
			imageTab->donneesTab[i1][i2][BLUE] = rvb.b ; 
			imageTab->donneesTab[i1][i2][GREEN] = rvb.v ; 
			imageTab->donneesTab[i1][i2][RED] = rvb.r ; 
		}
	}
	return imageTab ; 
}

couleurRVB HSVtoRGB(float H, float S, float V)
{
	float c, x, m;
	float newR, newV, newB, newH ; 
	float R, G, B;
	couleurRVB rvb ; 
		
	c = V * S ;
	newH = H / 60 ; 
	//printf("newH = %f", newH);
	//printf("H = %f", H);
	//printf("c: %f\n", c);
	x = c * ( 1- (fabs( fmod(newH, 2) - 1  ) ) );
	//printf("x: %f\n", x); 
	m = V - c ; 
	//printf("m: %f\n", m);
	if(0 <= H && H < 60)
	{
		//printf("if 1 \n");
		newR = c; 
		newV = x;
		newB = 0;
	}
	else if(60 <= H && H < 120)
	{
		//printf("if 2 \n");
		newR = x; 
		newV = c;
		newB = 0;
	}
	else if(120 <= H && H < 180)
	{
		//printf("if 3 \n");
		newR = 0; 
		newV = c;
		newB = x;
	}
	else if(180 <= H && H < 240)
	{
		//printf("if 4 \n");
		newR = 0; 
		newV = x;
		newB = c;
		//printf("coucou\n");
		//printf("r:%f, v:%f, b:%f\n", newR, newV, newB);
	}
	else if(240 <= H && H < 300)
	{
		newR = x; 
		newV = 0;
		newB = c;
	}
	else if(300 <= H && H < 360)
	{
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
	
	R = (R * 255) ;
	G = (G * 255) ;
	B = (B * 255) ; 
	
	rvb.r = R; 
	rvb.v = G;
	rvb.b = B;
	//printf("r:%f, v:%f, b:%f\n", R, G, B);
	return rvb;
}

void afficheValeurCouleurHSV(matImageHSV* matHSV, int x, int y)
{
	printf("x:%d y:%d H:%f S:%f V:%f \n", x, y, matHSV->h[x][y], matHSV->s[x][y], matHSV->v[x][y]);
	return;
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



