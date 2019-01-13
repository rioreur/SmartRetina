#include "hsv.h"

//------FONCTION HSV
//################################


/* @fonction		:	Return the value of a number divided by 255
 *						
 * 
 * @param			:
 * 		
 * 		number		: 	A number
 * 
 * @retour		 	:	The number divided by 255
 */
float divide255(int number)
{
	return( (float)number / 255) ; 	
}

/* @fonction		:	Return the maximum value from the three color of a pixel
 *						
 * 
 * @param			:
 * 		
 *		blue		: 	Value of the color blue from a pixel in blue green red
 *      green		: 	Value of the color green from a pixel in blue green red
 * 		red			: 	Value of the color red from a pixel in blue green red
 * 
 * @retour		 	:	The maximum value between the three number
 */
float maxColor(float blue, float green, float red)
{
	return fmaxf(fmaxf(red,green),blue);
}

/* @fonction		:	Return the minimum value from the three color of a pixel
 *						
 * 
 * @param			:
 * 		
 *		blue		: 	Value of the color blue from a pixel in blue green red
 *      green		: 	Value of the color green from a pixel in blue green red
 * 		red			: 	Value of the color red from a pixel in blue green red
 * 
 * @retour		 	:	The minimum value between the three number
 */
float minColor(float blue, float green, float red)
{
	return fminf(fminf(red,green),blue);
}

/* @fonction		:	Return the value of the hue from a color pixel blue, green, red
 *						
 * 
 * @param			:
 * 		blue		: 	Value of the color blue from a pixel in blue green red
 *      green		: 	Value of the color green from a pixel in blue green red
 * 		red			: 	Value of the color red from a pixel in blue green red
 * 		delta		: 	Value of the delta use in the BGR to HSV calculus conversion
 * 
 * @retour		 	:	Value of the hue
 */
float hue(float blue, float green, float red, float delta)
{
	float max, hue ; 
	max = maxColor(blue,green,red);
	hue = -1 ; 
	if(delta == 0)
	{
		hue = 0 ;
	}
	else if(max == red)
	{
		hue =  60 * (((green - blue ) / delta))  ;
	}
	else if (max == green)
	{
		hue = 60 * (((blue - red ) / delta) + 2 ) ; 
	}
	else if (max == blue)
	{
		hue = 60 * (((red - green) / delta) + 4 ) ; 
	}
	if(hue<0)
	{
		hue = hue + 360 ;
	}
	return hue ; 
}

/* @fonction		:	Return the value of the saturation use by the HSV to BGR color function
 *						
 * 
 * @param			:
 * 		colorMax	: 	Value of the maximum color between blue green red of a pixel
 * 		delta		: 	Value of the delta use in the BGR to HSV calculus conversion
 * 
 * @retour		 	:	Value of the saturation
 */
float saturation(float colorMax, float delta)
{
	if(colorMax == 0)
	{
		return 0 ; 
	}
	else
	{
		return delta/colorMax ; 
	}
}

/* @fonction		:	Return the HSV equivalent of a BGR color 
 *						
 * 
 * @param			:
 * 		blue		: 	Value of the color blue from a pixel in blue green red
 *      green		: 	Value of the color green from a pixel in blue green red
 * 		red			: 	Value of the color red from a pixel in blue green red
 * 		delta		: 	Value of the delta use in the BGR to HSV calculus conversion
 * 
 * @retour		 	:	Value of the hue
 */
colorHSV BGRtoHSV(int blue, int green, int red)
{
	float delta, colorMax, red255, green255, blue255 ;
	colorHSV hsv; 
	
	red255 = divide255(red);
	green255 = divide255(green);
	blue255 = divide255(blue);
	
	colorMax = maxColor(blue255,green255,red255);
	
	delta = colorMax - minColor(blue255,green255,red255) ; 
	
	hsv.hue = hue(blue255, green255, red255, delta); 
	hsv.saturation = saturation(colorMax, delta);
	hsv.value = colorMax ; 
	return hsv;
}

/* @fonction		:	Allocate a HSV matrix with its height and width
 *						
 * 
 * @param			:
 * 		blue		: 	Value of the color blue from a pixel in blue green red
 *      green		: 	Value of the color green from a pixel in blue green red
 * 		red			: 	Value of the color red from a pixel in blue green red
 * 		delta		: 	Value of the delta use in the BGR to HSV calculus conversion
 * 
 * @retour		 	:	Value of the hue
 */
colorHSV** allocateMatColorHSV(int height, int width)
{
	colorHSV** matHSV ; 
	int i ;
	matHSV = (colorHSV**)malloc(sizeof(colorHSV*) * width ) ; 
	for( i=0 ; i<height ; i++)
	{
		matHSV[i] = (colorHSV*)malloc(sizeof(colorHSV)*width) ;
	}
	//on retourne la matrice CouleurHsv
	return matHSV ; 
}

/* @fonction		:	Conversion of an array of BGR values to an array of HSV values
 *						
 * 
 * @param			:
 * 		arrayBGR	: 	Array of BGR values
 * 
 * 
 * @retour		 	:	Array of HSV values
 */
arrayColorHSV* arrayBGRToArrayHSV(DonneesImageTab* arrayBGR)
{
	colorHSV hsv ;
	arrayColorHSV* arrayStructHSV ; 
	arrayStructHSV = (arrayColorHSV*)malloc(sizeof(arrayColorHSV)) ;
	arrayStructHSV->width = arrayBGR->largeurImage ; 
	arrayStructHSV->height = arrayBGR->hauteurImage ; 
	arrayStructHSV->arrayHSV = allocateMatColorHSV(arrayStructHSV->width, arrayStructHSV->height) ; 
	int column, line ; 
	
	for(column=0 ; column < arrayStructHSV->width ; column+=1)
	{
		for(line=0 ; line < arrayStructHSV->height ; line+=1)
		{
			hsv = BGRtoHSV( arrayBGR->donneesTab[column][line][BLUE], arrayBGR->donneesTab[column][line][GREEN], arrayBGR->donneesTab[column][line][RED] ) ;  
			arrayStructHSV->arrayHSV[column][line] = hsv ; 
		}
	}
	return arrayStructHSV ; 
}

//Take a HSV matrix and return a BGR matrix
/* @fonction		:	Conversion of an array of HSV values to an array of BGR values
 *						
 * 
 * @param			:
 *  arrayStructHSV	: 	Struct with an array of HSV values
 * 
 * @retour		 	:	Struct with an array of BGR values 
 */
DonneesImageTab* arrayHSVToArrayBGR(arrayColorHSV* arrayStructHSV)
{
	colorBGR bgr ;
	DonneesImageTab* arrayImage ;  
	int column, line ; 
	int width = arrayStructHSV->width ; 
	int height = arrayStructHSV->height ; 
	arrayImage = initTab(width, height) ;
	
	for(column=0 ; column<width ; column+=1)
	{
		for(line=0 ; line<height; line+=1)
		{
			bgr = HSVtoBGR(arrayStructHSV->arrayHSV[column][line].hue, arrayStructHSV->arrayHSV[column][line].saturation, arrayStructHSV->arrayHSV[column][line].value) ; 
			arrayImage->donneesTab[column][line][BLUE] = bgr.blue ; 
			arrayImage->donneesTab[column][line][GREEN] = bgr.green ; 
			arrayImage->donneesTab[column][line][RED] = bgr.red ; 
		}
	}
	return arrayImage ; 
}

/* @fonction		:	Return the BGR color equivalent of a HSV color 
 *						
 * 
 * @param			:
 * 		hue			: 	Hue of a HSV color
 *      saturation	: 	Saturation of a HSV color
 * 		value		: 	Value of a HSV color
 * 
 * @retour		 	:	Struct with the values of a BGR color
 */
colorBGR HSVtoBGR(float hue, float saturation, float value)
{
	float c, x, m;
	float newRed, newGreen, newBlue, newHue ; 
	float red, green, blue;
	colorBGR bgr ; 
		
	c = value * saturation ;
	newHue = hue / 60 ; 
	
	x = c * ( 1- (fabs( fmod(newHue, 2) - 1  ) ) );
	
	m = value - c ; 
	
	if(0 <= hue && hue < 60)
	{
		//printf("if 1 \n");
		newRed = c; 
		newGreen = x;
		newBlue = 0;
	}
	else if(60 <= hue && hue < 120)
	{
		//printf("if 2 \n");
		newRed = x; 
		newGreen = c;
		newBlue = 0;
	}
	else if(120 <= hue && hue < 180)
	{
		//printf("if 3 \n");
		newRed = 0; 
		newGreen = c;
		newBlue = x;
	}
	else if(180 <= hue && hue < 240)
	{
		//printf("if 4 \n");
		newRed = 0; 
		newGreen = x;
		newBlue = c;
		//printf("coucou\n");
		//printf("r:%f, v:%f, b:%f\n", newR, newV, newB);
	}
	else if(240 <= hue && hue < 300)
	{
		newRed = x; 
		newGreen = 0;
		newBlue = c;
	}
	else if(300 <= hue && hue < 360)
	{
		newRed = c; 
		newGreen = 0;
		newBlue = x;
	}
	else
	{
		newRed = 0; 
		newGreen = 0;
		newBlue = 0;
	}
	
	red = (newRed + m) ;
	green = (newGreen + m) ; 
	blue = (newBlue + m) ; 	
	
	red = (red * 255) ;
	green = (green * 255) ;
	blue = (blue * 255) ; 
	
	bgr.red = red; 
	bgr.green = green;
	bgr.blue = blue;
	//printf("r:%f, v:%f, b:%f\n", R, G, B);
	return bgr;
}




