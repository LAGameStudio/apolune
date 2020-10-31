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

#include "GLSetup.h"

#include <iostream>
#include <math.h>

#if defined(USE_CORONA)
#include <corona.h>
#else
#include "ImageLoader.h"
#endif

#include "string_functions.h"
#include "Vertex.h"
#include "Polygon.h"
#include "Model.h"

#include "UglyFont.h"

// Can be mapped to anything
enum BitBrushMode {
 bb_blank=0,      // black
 bb_fortyfive=1,  // red 255
 bb_opaque=2,     // any other color
 bb_transparent=3,// blue 255
 bb_outside=4,    // green 255
 bb_inside=5,     // white
 bb_solid=6
};

class BitBrush : public ListItem {
public:
 Zstring filename;
 byte *pixels;
#if defined(USE_CORONA)
 corona::Image *image;
 corona::PixelFormat format;
#else
 Zp<CPUImage> image;
#endif
 Zint pixelSize,bytesSize;
 Zint width, height;
 matrix2d<BitBrushMode> brush;
 BitBrush( const char *filename ) : ListItem() {
  pixels=null;
  Load(filename);
 }
 BitBrushMode operator () ( unsigned int x, unsigned int y ) { return this->brush(x,y); }
 void Load( const char *filename ) {
  this->filename=string(filename);

#if defined(USE_CORONA)
  try {
   image=corona::OpenImage(filename, corona::PF_B8G8R8A8);
  } catch (bad_alloc& ba) {
   OUTPUT("GLImage::GLImage / corona::CreateImage: bad_alloc caught: %s\n", ba.what());
  }
  if ( !image ) { OUTPUT("Image file '%s' missing\n",filename); return; }
  width=image->getWidth();
  height=image->getHeight();
  pixels=(byte*)image->getPixels();
  format=image->getFormat();
  pixelSize=GetPixelSize(format); // corona::GetPixelSize()
  bytesSize=width*height*pixelSize;
#else
  image=imageLoader.Load(filename);
  if ( !image ) {
   OUTPUT("Image file '%s' missing\n",filename);
  } else if ( image->error ) {
   OUTPUT("Image file '%s' was corrupt, or an unsupported format.\n");
  }
  if ( ( pixels=image->getPixels() ) == null ) {
   OUTPUT("Image was blank (null pixels)\n");
  }
  width=image->getWidth();
  height=image->getHeight();
  pixels=image->getPixels();
  pixelSize=4;
  bytesSize=width*height*pixelSize;
#endif
  brush.resize(width,height);
  ProcessCentered();
  delete image;
  image=null;
  pixels=null;
 }
 void Debug( double dx, double dy ) {
  char buf[2];
  buf[0]=' ';
  buf[1]='\0';
  glColor3d(1.0,1.0,1.0);
  Text(this->filename.c_str(),dx,dy-20.0,10.0,10.0,false);
  for ( int x=0; x<width; x++ ) for ( int y=0; y<height; y++ ) {
   switch ( brush(x,y) ) {
    case bb_blank: buf[0]='0'; break;
    case bb_fortyfive: buf[0]='1'; break;
    case bb_opaque: buf[0]='2'; break;
    case bb_transparent: buf[0]='3'; break;
    case bb_outside: buf[0]='4'; break;
    case bb_inside: buf[0]='5'; break;
    case bb_solid: buf[0]='6'; break;
   }
   Text(buf,(double)x*10.0+dx,(double)y*10.0+dy,8.0,6.0,true);
  }
 }
 void ProcessCentered() {
  for ( int x=0; x<width; x++ ) for ( int y=0; y<height; y++ ) {
   byte *bgra=(byte *) &pixels[(x+y*width)*pixelSize];
   if ( bgra[3]==0 ) {
    brush(x,y)=bb_blank;
   } else if ( bgra[0] == 0 && bgra[1] == 0 && bgra[2] == 0 && bgra[3] == 255 ) {
    brush(x,y)=bb_solid;
   } else if ( bgra[0] == 255 && bgra[1] == 255 && bgra[2] == 255 ) {
    brush(x,y)=bb_inside;
   } else if ( bgra[0] == 255 && bgra[1] == 0 && bgra[2] == 0 ) {
    brush(x,y)=bb_transparent;
   } else if ( bgra[0] == 0 && bgra[1] == 0 && bgra[2] == 255 ) {
    brush(x,y)=bb_fortyfive;
   } else if ( bgra[0] == 0 && bgra[1] == 255 && bgra[2] == 0 ) {
    brush(x,y)=bb_outside;
   } else {
    brush(x,y)=bb_opaque;
   }
  }
 }
};

class BitBrushes : public LinkedList {
public:
 BitBrush *Load( const char *filename ) {
  if ( !file_exists(filename) ) return null;
  FOREACH(BitBrush,b) if ( contains(filename,b->filename.c_str()) ) return b;
  BitBrush *loaded;
  Append(loaded=new BitBrush(filename));
  OUTPUT("BitBrushes: loaded `%s`\n",filename);
  return loaded;
 }
 void LoadPath( const char *path ) {
  Strings *files=ls(path,true,true,true);
  EACH(files->first,String,s) Load((char *) s->s.c_str());
  delete files;
 }
 BitBrush *randomBySize( int w, int h ) {
  int total=0;
  FOREACH(BitBrush,b) if ( b->width==w && b->height==h ) total++;
  int candidate=1+upto(total);
  FOREACH(BitBrush,b) if ( b->width==w && b->height==h ) {
   candidate--;
   if ( candidate == 0 ) return b;
  }
  return null;
 }
 BitBrush *randomBySize( int w, int h, int seed ) {
  int total=0;
  FOREACH(BitBrush,b) if ( b->width==w && b->height==h ) total++;
  int candidate=1+upto(total,seed);
  FOREACH(BitBrush,b) if ( b->width==w && b->height==h ) {
   candidate--;
   if ( candidate == 0 ) return b;
  }
  return null;
 }
 CLEARLISTRESET(BitBrush);
};

extern BitBrushes bitbrushes;

void InitBitBrushes();