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

/*
 * Fills the target image based on the first provided size of the "in" image.  If the images
 * are not the same size, no copy occurs for the second image that is beyond scope.
 */
class ImageCombiner : public ImageProcess
{
public:
 Zp<GLImage> B;
 ImageCombiner() : ImageProcess() {
  postfix=string(".combined");
 }
 GLImage *Combine( GLImage *in, GLImage *b ) {
  B=b;
  return Process(in);
 }
 GLImage *Process( GLImage *in ) {
  if ( !in || !B ) {
   OUTPUT("ImageCombiner:Process cannot proceed because one or more images was not defined.\n");
   return null;
  }
  this->in=in;
  width=in->width;
  height=in->height;
  int width_b=B->width;
  int height_b=B->height;
#if defined(USE_CORONA)
  format=in->image->getFormat();
  pixelSize=GetPixelSize(format); // corona::GetPixelSize()
  bytesSize=width*height*pixelSize;
#else
  pixelSize=4;
  bytesSize=width*height*pixelSize;
#endif
  string newname=string(in->filename)+postfix;
  out=new GLImage(newname.c_str(),width,height);
  out->filename=newname;
  pixels_in=(byte*)in->image->getPixels();
  byte *pixels_in_b=(byte*)B->image->getPixels();
  pixels_out=(byte*)out->image->getPixels();
  for ( int i=0; i<width; i++ ) for ( int j=0; j<height; j++ ) {
   Crayon a,b;
   Get(i,j,&a);
   if ( WITHIN(i,j,0,0,width_b-1,height_b-1) ) {
    byte *p=&pixels_in_b[(i+j*width_b)*pixelSize];
    b.Int((int)p[0],(int)p[1],(int)p[2],(int)p[3]);
    Crayon c;
    if ( b.af != 1.0f ) { // Respects transparency
     c.ColorAddTransparency(a,b);
     Put(i,j,&c);
    } else {
     Put(i,j,&b);
    }
   } else {
    Put(i,j,&a);
   }
  }
  OUTPUT("ImageProcess created `%s`\n",newname.c_str());
  return out;
 }
};

extern ImageCombiner ip_Combiner;