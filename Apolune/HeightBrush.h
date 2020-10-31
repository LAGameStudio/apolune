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
#include "ListItem.h"
#include "ImageLoader.h"
#include "ImageLoaders.h"
#include "UglyFont.h"

class HeightBrush : public ListItem {
public:
 string filename;
 byte *pixels;
#if defined(USE_CORONA)
 corona::Image *image;
 corona::PixelFormat format;
#else
 CPUImage *image;
#endif
 int pixelSize,bytesSize;
 unsigned int width, height;
 matrix2d<double> brush;
 double operator () ( unsigned int x, unsigned int y ) { return this->brush(x,y); }
 HeightBrush( const char *filename ) : ListItem() {
  Load(filename);
 }
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
  width=(unsigned int) image->getWidth();
  height=(unsigned int) image->getHeight();
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
 void ProcessCentered() {
  for ( unsigned int x=0; x<width; x++ ) for ( unsigned int y=0; y<height; y++ ) {
   byte *bgra=(byte *) &pixels[(x+y*width)*pixelSize];
   if  ( bgra[0] == bgra[1] == bgra[2] ) brush(x,y)=iratiod((int)bgra[0],255);
   else {
    if ( bgra[0] == 0 && bgra[1] == 0 ) brush(x,y)=iratiod((int)bgra[2],255)/3.0; // red
    else
    if ( bgra[1] == 0 && bgra[3] == 0 ) brush(x,y)=1.0/3.0+iratiod((int)bgra[1],255)/3.0; // green
    else
    if ( bgra[2] == 0 && bgra[3] == 0 ) brush(x,y)=2.0/3.0+iratiod((int)bgra[0],255)/3.0; // blue
    else
    brush(x,y)=iratiod((int)bgra[0],255)+iratiod((int)bgra[1],255)+iratiod((int)bgra[2],255)/3.0;
   }
  }
 }
};

class HeightBrushes : public LinkedList {
public:
 HeightBrush *Load( const char *filename ) {
  if ( !file_exists(filename) ) return null;
  FOREACH(HeightBrush,b) if ( contains(filename,b->filename.c_str()) ) return b;
  HeightBrush *loaded;
  Append(loaded=new HeightBrush(filename));
  OUTPUT("HeightBrushes: loaded `%s`\n",filename);
  return loaded;
 }
 void LoadPath( const char *path ) {
  Strings *files=ls(path,true,true,true);
  EACH(files->first,String,s) Load((char *) s->s.c_str());
  delete files;
 }
 HeightBrush *randomBySize( int w, int h ) {
  int total=0;
  FOREACH(HeightBrush,b) if ( b->width==(unsigned int) w && (unsigned int) b->height==h ) total++;
  int candidate=1+upto(total);
  FOREACH(HeightBrush,b) if ( b->width==(unsigned int) w && (unsigned int) b->height==h ) {
   candidate--;
   if ( candidate == 0 ) return b;
  }
  return null;
 }
 HeightBrush *randomBySize( int w, int h, int seed ) {
  int total=0;
  FOREACH(HeightBrush,b) if ( b->width==(unsigned int) w && b->height==(unsigned int) h ) total++;
  int candidate=1+upto(total,seed);
  FOREACH(HeightBrush,b) if ( b->width==(unsigned int) w && b->height==(unsigned int) h ) {
   candidate--;
   if ( candidate == 0 ) return b;
  }
  return null;
 }
 CLEARLISTRESET(HeightBrush);
};

extern HeightBrushes heightbrushes;

void InitHeightBrushes();