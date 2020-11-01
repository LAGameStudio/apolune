/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once
#include "GLImage.h"

class ImageProcess : public ListItem {
public:
 string postfix;
 GLImage *in;
 GLImage *out;
 byte *pixels_in;
 byte *pixels_out;
 byte *bgra_in,*bgra_out;
 int _x,_y,_pass,width,height;
 int pixelSize,bytesSize,passes;
#if defined(USE_CORONA)
 corona::PixelFormat format;
#endif
 ImageProcess() : ListItem() {
  postfix=string(".1");
  in=out=null;
  pixels_in=pixels_out=bgra_in=bgra_out=null;
  _x=_y=_pass=width=height=pixelSize=bytesSize=0;
  passes=1;
 }
 virtual GLImage *Process( GLImage *in ) {
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
  out=new GLImage(newname.c_str(),width,height);
  out->filename=newname;
  pixels_in=(byte*)in->image->getPixels();
  pixels_out=(byte*)out->image->getPixels();
  Pre();
  for ( _pass=0; _pass<passes; _pass++ ) for ( _x=0; _x<width; _x++ ) for ( _y=0; _y<height; _y++ ) {
   bgra_in=(byte *) &pixels_in[(_x+_y*width)*pixelSize];
   bgra_out=(byte *) &pixels_out[(_x+_y*width)*pixelSize];
   Loop();
  }
  Post();
  OUTPUT("ImageProcess created `%s`\n",newname.c_str());
  return out;
 }
 void Copy( int x, int y, Crayon *out ) {
  byte *p=Get(x,y);
  int r=(int)p[0];
  int g=(int)p[1];
  int b=(int)p[2];
  int a=(int)p[3];
  out->Int(r,g,b,a);
 }
 void Copy( int x, int y, byte *r, byte *g, byte *b, byte *a ) {
  byte *p=Get(x,y);
  *r=p[0];
  *g=p[1];
  *b=p[2];
  *a=p[3];
 }
 void CopyOut( int x, int y, Crayon *out ) {
  byte *p=GetOut(x,y);
  int r=(int)p[0];
  int g=(int)p[1];
  int b=(int)p[2];
  int a=(int)p[3];
  out->Int(r,g,b,a);
 }
 void Copy( int x, int y, int *r, int *g, int *b, int *a ) {
  byte *p=Get(x,y);
  *r=(int)p[0];
  *g=(int)p[1];
  *b=(int)p[2];
  *a=(int)p[3];
 }
 void CopyOut( int x, int y, byte *r, byte *g, byte *b, byte *a ) {
  byte *p=GetOut(x,y);
  *r=p[0];
  *g=p[1];
  *b=p[2];
  *a=p[3];
 }
 void CopyOut( int x, int y, int *r, int *g, int *b, int *a ) {
  byte *p=GetOut(x,y);
  *r=(int)p[0];
  *g=(int)p[1];
  *b=(int)p[2];
  *a=(int)p[3];
 }
 byte *Get( int x, int y ) {
   return (byte *) &pixels_in[(x+y*width)*pixelSize];
 }
 byte *GetOut( int x, int y ) {
   return (byte *) &pixels_out[(x+y*width)*pixelSize];
 }
 void Get( int x, int y, Crayon *out ) {
  Copy(x,y,out);
 }
 void GetOut( int x, int y, Crayon *out ) {
  CopyOut(x,y,out);
 }
 void Put( int x, int y, Crayon *c ) {
  byte *b=GetOut(x,y);
  b[0]=(byte)c->r;
  b[1]=(byte)c->g;
  b[2]=(byte)c->b;
  b[3]=(byte)c->a;
 }
 void Put( int x, int y, Crayon c ) {
  byte *b=GetOut(x,y);
  b[0]=(byte)c.r;
  b[1]=(byte)c.g;
  b[2]=(byte)c.b;
  b[3]=(byte)c.a;
 }
 bool Within( int x, int y ) {
  return WITHIN(x,y,0,0,width-1,height-1);
 }
 virtual void Loop() {}
 virtual void Pre() {}
 virtual void Post() {}
};

class BlackBackgroundToTransparent : public ImageProcess {
public:
 BlackBackgroundToTransparent() : ImageProcess() { postfix=string(".b2t"); }
 void Loop(){
  bgra_out[0]=bgra_in[0];
  bgra_out[1]=bgra_in[1];
  bgra_out[2]=bgra_in[2];
  bgra_out[3]=
   (GLubyte)(((double)bgra_in[0]+
              (double)bgra_in[1]+
              (double)bgra_in[2])
              /3.0
            );
 }
};

extern BlackBackgroundToTransparent ip_BlackBackgroundToTransparent;

class RoughColorIndexer : public ImageProcess {
public:
 Crayons bucket;
 RoughColorIndexer() : ImageProcess() {
  postfix=string(".b2t");
  bucket.nearness=12.5f/255.0f;
 }
 void Loop(){
  bucket.Hold(
   crayons.jami(0,
    (int)bgra_out[0],
    (int)bgra_out[1],
    (int)bgra_out[2],
    (int)bgra_out[3]
   )
  );
  bgra_out[0]=bgra_in[0];
  bgra_out[1]=bgra_in[1];
  bgra_out[2]=bgra_in[2];
  bgra_out[3]=bgra_in[3];
 }
};

extern RoughColorIndexer ip_RoughColorIndexer;

class YBottomProbe : public ImageProcess {
public:
 Zint probePoint;
 Crayon background;
 YBottomProbe() : ImageProcess() {
  probePoint=-1;
  postfix=string(".ybp");
  background.Int(0,0,0,0);
 }
 YBottomProbe( int probePoint ) : ImageProcess() {
  this->probePoint=probePoint;
  postfix=string(".ybp");
 }
 void Pre() {
  if ( probePoint == -1 ) {
   probePoint = in->width/2;
  }
 }
 void Loop(){
  if ( _x == probePoint ) {
   Crayon color;
   color.Int(
    (int)bgra_in[0],
    (int)bgra_in[1],
    (int)bgra_in[2],
    (int)bgra_in[3]
   );
   if ( !background.Equals(&color) ) {
    in->yBottom=_y;
   }
  }
  bgra_out[0]=bgra_in[0];
  bgra_out[1]=bgra_in[1];
  bgra_out[2]=bgra_in[2];
  bgra_out[3]=bgra_in[3];
 }
};

extern YBottomProbe ip_YBottomProbe;


class LuminanceImageProcess : public ImageProcess {
public:
 LuminanceImageProcess() : ImageProcess() {
  postfix=string(".lum");
 }
 void Loop(){
  unsigned int lum=bgra_in[0]+bgra_in[1]+bgra_in[2];
  lum/=3;
  bgra_out[0]=(byte) lum;
  bgra_out[1]=(byte) lum;
  bgra_out[2]=(byte) lum;
  bgra_out[3]=bgra_in[3];
 }
};

extern LuminanceImageProcess ip_Luminance;

class ColorInvert : public ImageProcess {
public:
 ColorInvert() : ImageProcess() {
  postfix=string(".invert");
 }
 void Loop(){
  // Acquire pixel
  Crayon pixel;
  Get(_x,_y,&pixel);
  // Save in output pixel buffer
  bgra_out[0]=(byte)(255-pixel.r);
  bgra_out[1]=(byte)(255-pixel.g);
  bgra_out[2]=(byte)(255-pixel.b);
  bgra_out[3]=(byte)(255-pixel.a);
 }
};

extern ColorInvert ip_Invert;

class YInvert : public ImageProcess {
public:
 YInvert() : ImageProcess() {
  postfix=string(".invert");
 }
 void Loop(){
  // Acquire pixel
  Crayon pixel;
  Get(width-1-_x,height-1-_y,&pixel);
  // Save in output pixel buffer
  bgra_out[0]=(byte)(255-pixel.r);
  bgra_out[1]=(byte)(255-pixel.g);
  bgra_out[2]=(byte)(255-pixel.b);
  bgra_out[3]=(byte)(255-pixel.a);
 }
};

extern YInvert ip_YInvert;

class ProceduralImage : public ListItem {
public:
 string postfix;
 GLImage *out;
 GLubyte *pixels_out;
 GLubyte *bgra_out;
 int x,y,width,height;
 int pixelSize,bytesSize;
#if defined(USE_CORONA)
 corona::PixelFormat format;
#endif
 ProceduralImage() : ListItem() {
  postfix=string(".1");
  out=null;
  pixels_out=bgra_out=null;
  x=y=width=height=pixelSize=bytesSize=0;
 }
 GLImage *Process( const char *filename, int w, int h, bool save ) {
  width=w;
  height=h;
#if defined(USE_CORONA)
  format=corona::PF_B8G8R8A8;
  pixelSize=GetPixelSize(format); // corona::GetPixelSize()
  bytesSize=width*height*pixelSize;
#else
  pixelSize=4;
  bytesSize=width*height*pixelSize;
#endif
  string newname=string(filename)+postfix;
#pragma warning( push ) 
#pragma warning( disable : 4101 )
  NEW(out,GLImage(newname.c_str(),width,height));
#pragma warning( pop )
  out->filename=newname;
  pixels_out=out->image->getPixels();
  for ( y=height-1; y>=0; y-- ) for ( x=0; x<width; x++ ) {
   bgra_out=&pixels_out[(x+y*width)*pixelSize];
   Loop();
  }
  if ( save ) {
#if defined(USE_CORONA)
   corona::SaveImage(filename,corona::FF_PNG,out->image);
#else
   out->image->Save(filename);
#endif
  }
  return out;
 }
 virtual void Loop() {
 }
};