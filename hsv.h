// .h necessaires a l'application
#include "symetrie.h"

#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/** @struct colorBGR
 *  @brief This structure represent a BGR color 
 *  
 *	@Members : 
 * 		blue : contain the blue value of the color
 * 		green: contain the green value of the color
 * 		red  : contain the red value of the color
 */
typedef struct colorBGR {
	int blue, green, red ; 
}colorBGR ;

/** @struct colorHSV
 *  @brief This structure represent a HSV color 
 *  
 *	@Members : 
 * 		hue 	   : contain the hue value of the color
 * 		saturation : contain the saturantion value of the color
 * 		value  	   : contain the value of the color
 */
typedef struct colorHSV {
	float hue, saturation, value ;
}colorHSV ; 

/** @struct arrayColorHSV
 *  @brief This structure represent an array of color HSV
 *  
 *	@Members : 
 * 		width    : contain the value of the image's width
 * 		height 	 : contain the value of the image's height
 * 		arrayHSV : contain the 2D array of colorHSV
 */
typedef struct arrayColorHSV {
	int width ; 
	int height ;
	colorHSV** arrayHSV; 
} arrayColorHSV; 


//Fonction HSV
/* @fonction		:	Return the value of a number divided by 255
 *						
 * 
 * @param			:
 * 		
 * 		number		: 	A number
 * 
 * @retour		 	:	The number divided by 255
 */
float divide255(int color);

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
float maxColor(float red, float green, float blue);

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
float minColor(float red, float green, float blue);

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
float hue(float red, float green, float blue, float delta);

/* @fonction		:	Return the value of the saturation use by the HSV to BGR color function
 *						
 * 
 * @param			:
 * 		colorMax	: 	Value of the maximum color between blue green red of a pixel
 * 		delta		: 	Value of the delta use in the BGR to HSV calculus conversion
 * 
 * @retour		 	:	Value of the saturation
 */
float saturation(float colorMax, float delta);

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
colorHSV BGRtoHSV(int blue, int green, int red) ;


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
colorBGR HSVtoBGR(float hue, float saturation, float value) ;

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
colorHSV** allocateMatColorHSV(int height, int width) ;

/* @fonction		:	Conversion of an array of BGR values to an array of HSV values
 *						
 * 
 * @param			:
 * 		arrayBGR	: 	Array of BGR values
 * 
 * 
 * @retour		 	:	Array of HSV values
 */
arrayColorHSV* arrayBGRToArrayHSV(DonneesImageTab* arrayBGR) ;

/* @fonction		:	Conversion of an array of HSV values to an array of BGR values
 *						
 * 
 * @param			:
 *  arrayStructHSV	: 	Struct with an array of HSV values
 * 
 * @retour		 	:	Struct with an array of BGR values 
 */
DonneesImageTab* arrayHSVToArrayBGR(arrayColorHSV* arrayHSV) ;
