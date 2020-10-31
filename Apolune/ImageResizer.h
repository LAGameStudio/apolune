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

class ImageResizer : public ImageProcess
{
public:
 Crayon fill;
 Zbool nearest_neighbor;
 Zint new_w,new_h;
 ImageResizer() : ImageProcess() {
  postfix=string(".resized");
 }
 GLImage *Resize( GLImage *in, int new_w, int new_h ) {
  this->new_w=new_w;
  this->new_h=new_h;
  return Process(in);
 }
 float lerp(float s, float e, float t){return s+(e-s)*t;}
 float blerp(float c00, float c10, float c01, float c11, float tx, float ty) {
  return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
}
 GLImage *Process( GLImage *in ) {
  if ( new_w == 0 || new_h == 0 ) {
   OUTPUT("ImageResizer:Process(`%s`): Cannot resize to a 0 dimension.  Returning null.\n", in->filename.c_str());
   return null;
  }
  this->in=in;
  width=in->width;
  height=in->height;
#if defined(USE_CORONA)
  format=in->image->getFormat();
  pixelSize=GetPixelSize(format); // corona::GetPixelSize()
  bytesSize=width*height*pixelSize;
#else
  pixelSize=4;
  bytesSize=width*height*pixelSize;
#endif
  string newname=string(in->filename)+postfix;
  out=new GLImage(newname.c_str(),new_w,new_h);
  out->filename=newname;
  pixels_in=(byte*)in->image->getPixels();
  pixels_out=(byte*)out->image->getPixels();
  if ( nearest_neighbor ) { // Nearest neighbor
   Crayon a, b, c, d;
   double x_ratio = iratiod(width-1,new_w);
   double y_ratio = iratiod(height-1,new_h);
   for (int i=0; i< new_h; i++) {
    for (int j=0; j< new_w; j++) {
     int x = (int)(x_ratio * (double)j) ;
     int y = (int)(y_ratio * (double)i) ;
     if ( Within(x,y) ) Get(x,y,&a); else a.fromCrayon(fill);
     byte *pixel=&pixels_out[(i+j*new_w)*pixelSize];
     pixel[0]=(byte)a.r;
     pixel[1]=(byte)a.g;
     pixel[2]=(byte)a.b;
     pixel[3]=(byte)a.a;
    }
   }
  } else { // Bilinear
   Crayon c;
   for ( int i=0; i<new_w; i++ ) for ( int j=0; j<new_h; j++ ) {
    float gx = i / (float)(new_w) * (width-1);
    float gy = j / (float)(new_h) * (height-1);
    int gxi = (int)gx;
    int gyi = (int)gy;
    Crayon c00,c10,c01,c11;
    Get(gxi,   gyi,   &c00);
    Get(gxi+1, gyi,   &c10);
    Get(gxi,   gyi+1, &c01);
    Get(gxi+1, gyi+1, &c11);
    c.Int(
     (int) blerp((float)c00.r,(float)c10.r,(float)c01.r,(float)c11.r, gx - gxi, gy -gyi),
     (int) blerp((float)c00.g,(float)c10.g,(float)c01.g,(float)c11.g, gx - gxi, gy -gyi),
     (int) blerp((float)c00.b,(float)c10.b,(float)c01.b,(float)c11.b, gx - gxi, gy -gyi),
     (int) blerp((float)c00.a,(float)c10.a,(float)c01.a,(float)c11.a, gx - gxi, gy -gyi)
    );
    byte *pixel=&pixels_out[(i+j*new_w)*pixelSize];
    pixel[0]=(byte)c.r;
    pixel[1]=(byte)c.g;
    pixel[2]=(byte)c.b;
    pixel[3]=(byte)c.a;
   }   
  }
  OUTPUT("ImageResizer created `%s`\n",newname.c_str());
  return out;
 }
};

extern ImageResizer ip_Resizer;

class ImageCropper : public ImageProcess
{
public:
 Crayon fill;
 Zint left,top,new_w,new_h;
 ImageCropper() : ImageProcess() {
  postfix=string(".cropped");
 }
 GLImage *Crop( GLImage *in, int left, int top, int new_w, int new_h ) {
  this->left=left;
  this->top=top;
  this->new_w=new_w;
  this->new_h=new_h;
  return Process(in);
 }
 GLImage *Process( GLImage *in ) {
  if ( new_w == 0 || new_h == 0 ) {
   OUTPUT("ImageCropper:Process(`%s`): Cannot resize to a 0 dimension.  Returning null.\n", in->filename.c_str());
   return null;
  }
  this->in=in;
  width=in->width;
  height=in->height;
#if defined(USE_CORONA)
  format=in->image->getFormat();
  pixelSize=GetPixelSize(format); // corona::GetPixelSize()
  bytesSize=width*height*pixelSize;
#else
  pixelSize=4;
  bytesSize=width*height*pixelSize;
#endif
  string newname=string(in->filename)+postfix;
  out=new GLImage(newname.c_str(),new_w,new_h);
  out->filename=newname;
  pixels_in=(byte*)in->image->getPixels();
  pixels_out=(byte*)out->image->getPixels();
  Crayon c;
  for ( int i=0; i<new_w; i++ ) for ( int j=0; j<new_h; j++ ) {
   int t_x=left+i;
   int t_y=top+j;
   if ( Within(t_x,t_y) ) {
    Get(t_x,t_y,&c);
   } else c.fromCrayon(fill);
   byte *pixel=&pixels_out[(i+j*new_w)*pixelSize];
   pixel[0]=(byte)c.r;
   pixel[1]=(byte)c.g;
   pixel[2]=(byte)c.b;
   pixel[3]=(byte)c.a;
  }
  OUTPUT("ImageCropper created `%s`\n",newname.c_str());
  return out;
 }
};

extern ImageCropper ip_Crop;