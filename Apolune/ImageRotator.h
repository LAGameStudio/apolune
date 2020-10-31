/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

#include "GLImage.h"
#include "ImageProcess.h"

// This function effectively rotates an image, with bilinear filtering
// Source: SupercomputingBlog.com
// License: For any purpose
class ImageRotator : public ImageProcess
{
public:
 Crayon fill;
 Zbool nearest_neighbor;
 Zdouble angle;
 ImageRotator() : ImageProcess() {
  postfix=string(".rotated");
 }
 void Loop() {
  // Fill target image with "fill" value
  for ( int i=0; i<width; i++ ) for ( int j=0; j<height; j++ ) {
   Put(i,j,&fill);
  }
 }
 void Post() {
	 width = in->width;
  height = in->height;
 	double cX = (double)width/2.0f;
	 double cY = (double)height/2.0f;
  if ( nearest_neighbor ) { // Nearest Neighbor
 	 for (int i=0; i < height; i++)	{
 	 	double relY = cY-(double)i;		// get center y coordinate
 	 	for (int j=0; j < width; j++)	{
		 	 double relX = (double)j-cX;	// get center x coordinate
		 	 // do rotation transformation
		 	 double xPrime = relX*cos(angle) + relY*sin(angle);
		 	 double yPrime = -1.0 * relX*sin(angle) + relY*cos(angle);
		 	 // re-center pixel
		 	 xPrime += cX;
		 	 yPrime += cY;
		 	 int q11x = (int)floor(xPrime);
		 	 int q11y = (int)ceil(yPrime);
		 	 q11y = min(height-1, q11y);
		 	 q11y = max(0, q11y);
		 	 // We need to get the four nearest neighbooring pixels.
		 	 // Pixels which are past the border of the image are clamped to the border already.
     Crayon q11;
     Get(q11x,q11y,&q11);
     Put(j,height-i-1,&q11); // get rid of height-i-1?
		  }
   }
  } else { // Bilinear
 	 for (int i=0; i < height; i++)	{
 	 	double relY = cY-(double)i;		// get center y coordinate
 	 	for (int j=0; j < width; j++)	{
		 	 double relX = (double)j-cX;	// get center x coordinate
		 	 // do rotation transformation
		 	 double xPrime = relX*cos(angle) + relY*sin(angle);
		 	 double yPrime = -1.0 * relX*sin(angle) + relY*cos(angle);
		 	 // re-center pixel
		 	 xPrime += cX;
		 	 yPrime += cY;
		 	 // There are four nearest original pixels, q11, q12, q21, and q22
		 	 // While we may get away with using only four variables, this code
		 	 // seperates out the x and y of each point for clarity reasons.
		 	 // Most compilers should be capable of optimizing away the redundant
		 	 // steps here.
     
		 	 int q12x = (int)floor(xPrime);
		 	 int q12y = (int)floor(yPrime);
		 	 q12x = max(0, q12x);
		 	 q12y = max(0, q12y);
		 	 q12x = min(width-1, q12x);
		 	 q12y = min(height-1, q12y);
		 	 int q22x = (int)ceil(xPrime);
		 	 int q22y = q12y;
		 	 q22x = min(width-1, q22x);
		 	 q22x = max(0, q22x);
		 	 int q11x = q12x;
		 	 int q11y = (int)ceil(yPrime);
		 	 q11y = min(height-1, q11y);
		 	 q11y = max(0, q11y);
		 	 int q21x = q22x;
		 	 int q21y = q11y;
		 	 // We need to get the four nearest neighbooring pixels.
		 	 // Pixels which are past the border of the image are clamped to the border already.
     Crayon q11,q12,q21,q22;
		 	 double factor1;
		 	 double factor2;			 
		 	 if ( q21x == q11x ) // special case to avoid divide by zero
		 	 {
		 	 	factor1 = 1; // They're at the same X coordinate, so just force the calculatione to one point
		 	 	factor2 = 0;
		 	 }
		 	 else
		 	 {
		 	 	factor1 = (((double)q21x - (double)xPrime)/((double)q21x - (double)q11x));
		 	 	factor2 = (((double)xPrime - (double)q11x)/((double)q21x - (double)q11x));
		 	 }
		 	 double factor3;
		 	 double factor4;
		 	 if (q12y == q11y) // special case to avoid divide by zero
		 	 {
		 	 	factor3 = 1;
		 	 	factor4 = 0;
		 	 }
		 	 else
		 	 {
		 	 	factor3 = ((double) q12y - yPrime)/((double)q12y - (double)q11y);
		 	 	factor4 = (yPrime - (double)q11y)/((double)q12y - (double)q11y);
		 	 }
     Get(q11x,q11y,&q11);
     Get(q12x,q12y,&q12);
     Get(q21x,q21y,&q21);
     Get(q22x,q22y,&q22);
		 	 double R1r = factor1 * (double)q11.r + factor2*(double)q21.r;
		 	 double R1g = factor1 * (double)q11.g + factor2*(double)q21.g;
		 	 double R1b = factor1 * (double)q11.b + factor2*(double)q21.b;
		 	 double R1a = factor1 * (double)q11.a + factor2*(double)q21.a;
		 	 double R2r = factor1 * (double)q12.r + factor2*(double)q22.r;
		 	 double R2g = factor1 * (double)q12.g + factor2*(double)q22.g;
		 	 double R2b = factor1 * (double)q12.b + factor2*(double)q22.b;
		 	 double R2a = factor1 * (double)q12.a + factor2*(double)q22.a;
		 	 // Calculate the final unbounded RGB values
		 	 unsigned int finalR = (unsigned int)((factor3 * R1r) + (factor4*R2r));
		 	 unsigned int finalG = (unsigned int)((factor3 * R1g) + (factor4*R2g));
		 	 unsigned int finalB = (unsigned int)((factor3 * R1b) + (factor4*R2b));
		 	 unsigned int finalA = (unsigned int)((factor3 * R1a) + (factor4*R2a));
		 	 // Clamp the RBG values to a value between 0 and 255
		 	 finalR = min(255, finalR); finalR = max(0, finalR);
		 	 finalG = min(255, finalG); finalG = max(0, finalG);
		 	 finalB = min(255, finalB); finalB = max(0, finalB);
		 	 finalA = min(255, finalA); finalA = max(0, finalA);
     Crayon final;
     final.Int(finalR,finalG,finalB,finalA);
     Put(j,height-i-1,&final); // get rid of height-i-1?
		 	 // Note that we actually flip the image here to put it back into screen coordinates.
		  }
   }
  }
 }
};

extern ImageRotator ip_Rotator;