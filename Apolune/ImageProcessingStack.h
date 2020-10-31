/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2015 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once
#include <math.h>
#include "LinkedList.h"
#include "Indexed.h"
#include "Crayon.h"
#include "GLImage.h"
#include "TexFont.h"
#include "GLTransform.h"

enum ic_Blends { 
 ic_Add=0,
 ic_Sub=1,
 ic_Transparency=2,
 ic_Multiply=3,
 ic_Overwrite=4,
 ic_Count=5 // Just a "max" value
};

/*
Take region of input and fit to region of output, with blend op, invertable.
Draw texfont scaled to region
*/
class ImageProcessLayer : public ListItem {
public:
 Zp<GLImage> in;
 byte *pixels_in;
 Zint pixelSize;
 GLTransform transform;
 ic_Blends blend;
 Vertexd canvasImageScale;
 ImageProcessLayer() : ListItem() {
  blend = ic_Overwrite;
  pixelSize = 4;
 }
 void GetPixel(int x, int y, Crayon * clr) {
  byte *p = (byte *) &pixels_in[(x+y*in->width)*pixelSize];
  int r=(int)p[0]; int g=(int)p[1]; int b=(int)p[2]; int a=(int)p[3];
  clr->Int(r,g,b,a);
 }
 float lerp(float s, float e, float t){return s+(e-s)*t;}
 float blerp(float c00, float c10, float c01, float c11, float tx, float ty) {
  return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
 }
 virtual bool Process(CPUImage * out, bool nearestNeighbor = false) {
  int bytesSize=out->getWidth()*out->getHeight()*pixelSize;
  byte *pixels_out;
  pixels_in=(byte*)in->image->getPixels();
  pixels_out=(byte*)out->getPixels();
  Vertex centerPoint(out->getWidth()*transform.position.x,out->getHeight()*transform.position.y);
  double imageWidth = in->width*canvasImageScale.x*transform.scale.x;
  double imageHeight = in->height*canvasImageScale.y*transform.scale.y;
  //Get where the top left corner of image goes on the CPUImage
  Cartesian TL((int)(centerPoint.x-imageWidth/2),(int)(centerPoint.y-imageHeight/2));
  //Get where the bottom right corner of image goes on the CPUImage
  Cartesian BR((int)(centerPoint.x+imageWidth/2),(int)(centerPoint.y+imageHeight/2));
  float s =(float) sin(deg2rad(transform.rotation.z));
  float c =(float) cos(deg2rad(transform.rotation.z));
  Crayon cr;
  if(nearestNeighbor) {
   for( int j = TL.y; j<BR.y; j++) {
    for(int i = TL.x; i<BR.x; i++) {
     double percentInX = ((double)(i-TL.x))/(double)((BR.x-TL.x));
     double percentInY = ((double)(j-TL.y))/(double)((BR.y-TL.y));
     float newX = (float)i;
     float newY = (float)j;
     //newX -= centerPoint.x; newY -= centerPoint.y;
     float xnew = newX * c - newY * s;
     float ynew = newX * s + newY * c; 
     //newX = xnew + centerPoint.x; newY = ynew + centerPoint.y;
     int pixelXToTake = (int)((double)in->width * percentInX);
     int pixelYToTake = (int)((double)in->height * percentInY);
     GetPixel(pixelXToTake,pixelYToTake,&cr);
     int finalX = (int)rint(newX);
     int finalY = (int)rint(newY);
     byte *pixel=&pixels_out[(finalX+finalY*out->getWidth())*pixelSize];
     pixel[0]=(byte)cr.r; pixel[1]=(byte)cr.g; pixel[2]=(byte)cr.b; pixel[3]=(byte)cr.a;
    }
   }
  }
  else {
   for( int j = TL.y; j<BR.y; j++) {
    for(int i = TL.x; i<BR.x; i++) {
     double percentInX = ((double)(i-TL.x))/(double)((BR.x-TL.x));
     double percentInY = ((double)(j-TL.y))/(double)((BR.y-TL.y));
     float s = (float) sin(deg2rad(transform.rotation.z));
     float c = (float) cos(deg2rad(transform.rotation.z));
     float newX = (float)i;
     float newY = (float)j;
     newX -= centerPoint.x; newY -= centerPoint.y;
     float xnew = newX * c - newY * s;
     float ynew = newX * s + newY * c; 
     newX = xnew + centerPoint.x; newY = ynew + centerPoint.y;
     int pixelXToTake = (int)((double)in->width * percentInX);
     int pixelYToTake = (int)((double)in->height * percentInY);
     Crayon c00,c10,c01,c11;
     GetPixel(pixelXToTake,   pixelYToTake,   &c00);
     GetPixel(pixelXToTake+1, pixelYToTake,   &c10);
     GetPixel(pixelXToTake,   pixelYToTake+1, &c01);
     GetPixel(pixelXToTake+1, pixelYToTake+1, &c11);
     cr.Int(
     (int) blerp((float)c00.r,(float)c10.r,(float)c01.r,(float)c11.r, 0,0),
     (int) blerp((float)c00.g,(float)c10.g,(float)c01.g,(float)c11.g, 0,0),
     (int) blerp((float)c00.b,(float)c10.b,(float)c01.b,(float)c11.b, 0,0),
     (int) blerp((float)c00.a,(float)c10.a,(float)c01.a,(float)c11.a, 0,0)
     );
     byte *pixel=&pixels_out[((int)(newX)+(int)(newY)*out->getWidth())*pixelSize];
     pixel[0]=(byte)cr.r; pixel[1]=(byte)cr.g; pixel[2]=(byte)cr.b; pixel[3]=(byte)cr.a;
    }
   }
  }
  return true;
 }
};

class ImageProcessingStack : public LinkedList {
public:
 Zp<CPUImage> out;
 Zbool nearestNeighbor;
 Cartesian outputSize;
 Vertexd canvasToImageScale;
 ImageProcessingStack() {
  outputSize.Set(512,512);
 }
 void Add(GLImage * img, double xTranslation, double yTranslation, double angle, double scaleX, double scaleY) {
  ImageProcessLayer * ipl = new ImageProcessLayer;
  ipl->in = img; 
  ipl->transform.position(xTranslation,yTranslation,0.0);
  ipl->transform.rotation(0.0,0.0,angle);
  ipl->transform.scale(scaleX,scaleY,0.0);
  ipl->canvasImageScale.Set(canvasToImageScale.x,canvasToImageScale.y);
  Append(ipl);
 }
 GLImage * Process( ) {
  out = new CPUImage(outputSize.x,outputSize.y);
  FOREACH(ImageProcessLayer,i) {
   i->Process(out,true);
  }
  GLImage * result = new GLImage;
  result->image = out;
  result->width = out->getWidth();
  result->height = out->getHeight();
  result->pixels = out->getPixels();
  result->deleteAfterUpload = false;
  result->Upload(&gl,GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
  return result;
 }
 CLEARLISTRESET(ImageProcessLayer);
};