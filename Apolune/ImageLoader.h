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
#include "ImageLoaders.h"

#include "Crayon.h"

// Multi-format image loading class.
// Format 4-byte BGRA

class FBO;

class CPUImage {
 int w,h;
public:
 Indexed<GLubyte> pixels; 
 bool error;
 CPUImage( string filename, ImageFormats format ) {
  w=h=0;
  error=false;
  switch ( format ) {
    case PNG: error=LoadPNG( filename, &pixels, &w, &h ); break;
   case JPEG: error=LoadJPG( filename, &pixels, &w, &h ); break;
//    case GIF: error=LoadGIF(  filename, &pixels, &w, &h ); break;
//    case PCX: error=LoadPCX(  filename, &pixels, &w, &h ); break;
//   case TIFF: error=LoadTIFF( filename, &pixels, &w, &h ); break;
//    case RAW: error=LoadRAW(  filename, &pixels, &w, &h ); break;
    default:
     OUTPUT("CPUImage(`%s`) :: Unsupported format (%d).\n", filename.c_str(), (int)format );
     error=true;
    break;
  }
 }
 CPUImage( int w, int h ) {
  this->w=w;
  this->h=h;
  error=false;
  pixels.Size(w*h*4);
  ZeroMemory(pixels.list,sizeof(GLubyte)*w*h*4);
 }
 CPUImage( FBO *f, int fx, int fy, int fw, int fh );
 int getWidth() { return w; }
 int getHeight() { return h; }
 GLubyte *getPixels() { return pixels.list; }
 void Copy( CPUImage *in ) {
  Crayon pix;
  for ( int i=0; i<w; i++ ) {
   for ( int j=0; j<h; j++ ) {
    in->GetPixel(i,j,&pix);
    SetPixel(i,j,&pix);
   }
  }
 }
 CPUImage *FlipV() {
  CPUImage *out=new CPUImage(w,h);
  if ( h == 1 ) {
   out->Copy(this);
   return out;
  }
  Crayon pix;
  for ( int i=0; i<w; i++ ) {
   for ( int j=0; j<h; j++ ) {
    GetPixel(i,j,&pix);
    out->SetPixel(i,h-j-1,&pix);
   }
  }
  return out;
 }
 CPUImage *FlipH() {
  CPUImage *out=new CPUImage(w,h);
  if ( w == 1 ) {
   out->Copy(this);
   return out;
  }
  Crayon pix;
  for ( int i=0; i<w; i++ ) {
   for ( int j=0; j<h; j++ ) {
    GetPixel(i,j,&pix);
    out->SetPixel(w-i-1,j,&pix);
   }
  }
  return out;
 }
 CPUImage *HeightToNormalMap();
 CPUImage *Crop( int startX, int startY, int newW, int newH ) {
  Crayon pix;
  CPUImage *out=new CPUImage(newW,newH);
  for ( int i=0; i<newW; i++ ) {
   for ( int j=0; j<newH; j++ ) {
    int px=startX+i;
    int py=startY+j;
    if ( WITHIN(px,py,0,0,w,h) ) GetPixel(px,py,&pix);
    else pix.Pick(clear);
    out->SetPixel(i,j,&pix);
   }
  }
  return out;
 }
 bool SetPixel( unsigned int x, unsigned int y, Crayon *color ) {
  if ( WITHIN(x,y,0,0,(unsigned int)w,(unsigned int)h) ) {
   pixels[(x+y*w)*4+0]=(GLubyte)color->r;
   pixels[(x+y*w)*4+1]=(GLubyte)color->g;
   pixels[(x+y*w)*4+2]=(GLubyte)color->b;
   pixels[(x+y*w)*4+3]=(GLubyte)color->a;
   return true;
  }
  return false;
 }
 void GetPixel( unsigned int x, unsigned int y, Crayon *color ) {
  color->Int(
   (int) pixels[(x+y*w)*4+0],
   (int) pixels[(x+y*w)*4+1],
   (int) pixels[(x+y*w)*4+2],
   (int) pixels[(x+y*w)*4+3]
  );
 }
 GLubyte *GetPixel( unsigned int x, unsigned int y ) {
  return &pixels[(x+y*w)*4];
 }
 void Save( const char *pngname ) {
  SavePNG(pngname,w,h,&pixels);
 }
 void Size( int nw, int nh ) {
  this->w=nw;
  this->h=nh;
  error=false;
  pixels.Size(nw*nh*4);
  ZeroMemory(pixels.list,sizeof(GLubyte)*nw*nh*4);
 }
 ~CPUImage( ) {
 }
};

class ImageLoader {
public:
 ImageLoader(void) {
 }
 ~ImageLoader(void) {
 }
 CPUImage *Load( char *filename ) {
  if ( !filename ) {
   OUTPUT("Error: filename was null\n");
   return null;
  }
  string fn=string(filename);
  return Load(fn);
 }
 CPUImage *Load( string filename ) {
  if ( !str_infix(filename.c_str(),".png") ) {
   return Load(filename,PNG);
  } else
  if ( !str_infix(filename.c_str(),".jpg") || !str_infix(filename.c_str(),".jpeg") ) {
   return Load(filename,JPEG);
  } else
  if ( !str_infix(filename.c_str(),".gif") ) {
   return Load(filename,GIF);
  } else
  if ( !str_infix(filename.c_str(),".tif") ) {
   return Load(filename,TIFF);
  } else
  if ( !str_infix(filename.c_str(),".pcx") ) {
   return Load(filename,PCX);
  }
  OUTPUT("ImageLoader: Unsupported format for file `%s`\n", filename.c_str());
  return null;
 }
 CPUImage *Load( char *filename, ImageFormats format ) {
  return Load(string(filename),format);
 }
 CPUImage *Load( string filename, ImageFormats format ) {
  if ( !file_exists((char*)filename.c_str()) ) return null;
  return new CPUImage(filename,format);
 }
 CPUImage *fromFBO( FBO *f, int fx, int fy, int fw, int fh ) {
  return new CPUImage( f,fx,fy,fw,fh );
 }
};

extern ImageLoader imageLoader;