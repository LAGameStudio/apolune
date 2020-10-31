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

#include "string_functions.h"
#include "Vertex.h"
#include "Polygon.h"
#include "Model.h"

//#define USE_CORONA 1

#if defined(USE_CORONA)
#include <corona.h>
#else
#include "ImageLoader.h"
#endif

/*
 * Loads an image file and searches it for non-black dots, which it converts
 * to vertexes normalized to 1 and stores RGBA data, which can then be used
 * to make a vboVC or used as a scaffold, saved to a file, etc.
 */
enum Image2ModelModes {
 i2m_centered=0,
 i2m_top_left=1,
 i2m_bottom_left=2
};

class Image2Model : public ListItem {
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
 int width, height;
 ImmediateModel model;
 Image2ModelModes mode;
 Image2Model() : ListItem() {
  mode=i2m_centered;
  width=height=0;
  pixelSize=bytesSize=0;
  image=null;
  pixels=null;
  filename=string("");
 }
 Image2Model( const char *filename ) : ListItem() {
  mode=i2m_centered;
  Load(filename);
 }
 Image2Model( const char *filename, Image2ModelModes m ) {
  mode=m;
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
  width=image->getWidth();
  height=image->getHeight();
  pixels=image->getPixels();
  pixelSize=4;
  bytesSize=width*height*pixelSize;
#endif
  mPolygon *points=new mPolygon;
  points->draw_method=GL_POINTS;
  switch ( mode ) {
     case i2m_centered: ProcessCentered(points); break;
     case i2m_top_left: ProcessTopLeft(points); break;
  case i2m_bottom_left: ProcessBottomLeft(points); break;
  }
  model.polygons.Append(points);
  delete image;
  image=null;
  pixels=null;
 }
 void ProcessCentered( mPolygon *points ) {
  for ( int x=0; x<width; x++ ) for ( int y=0; y<height; y++ ) {
   byte *bgra=(byte *) &pixels[(x+y*width)*pixelSize];
   if ( bgra[0] == 0.0f && bgra[1] == 0.0f && bgra[2] == 0.0f ) continue;
   Vertex *v=new Vertex;
   v->rgba[0]=iratiof((int)bgra[2],255);
   v->rgba[1]=iratiof((int)bgra[1],255);
   v->rgba[2]=iratiof((int)bgra[0],255);
   v->rgba[3]=iratiof((int)bgra[3],255);
   v->x=iratiof(x,width)-0.5f;
   v->y=(1.0f-iratiof(y,height))-0.5f;
   points->points.Append(v);
  }
 }
 void ProcessTopLeft( mPolygon *points ) {
  for ( int x=0; x<width; x++ ) for ( int y=0; y<height; y++ ) {
   byte *bgra=(byte *) &pixels[(x+y*width)*pixelSize];
   if ( bgra[0] == 0.0f && bgra[1] == 0.0f && bgra[2] == 0.0f ) continue;
   Vertex *v=new Vertex;
   v->rgba[0]=iratiof((int)bgra[2],255);
   v->rgba[1]=iratiof((int)bgra[1],255);
   v->rgba[2]=iratiof((int)bgra[0],255);
   v->rgba[3]=iratiof((int)bgra[3],255);
   v->x=iratiof(x,width);
   v->y=1.0f-iratiof(y,height);
   points->points.Append(v);
  }
 }
 void ProcessBottomLeft( mPolygon *points ) {
  for ( int x=0; x<width; x++ ) for ( int y=0; y<height; y++ ) {
   byte *bgra=(byte *) &pixels[(x+y*width)*pixelSize];
   if ( bgra[0] == 0.0f && bgra[1] == 0.0f && bgra[2] == 0.0f ) continue;
   Vertex *v=new Vertex;
   v->rgba[0]=iratiof((int)bgra[2],255);
   v->rgba[1]=iratiof((int)bgra[1],255);
   v->rgba[2]=iratiof((int)bgra[0],255);
   v->rgba[3]=iratiof((int)bgra[3],255);
   v->x=iratiof(x,width);
   v->y=iratiof(y,height);
   points->points.Append(v);
  }
 }
 void Mangle( int mangle ) {
  Random *set=randoms.GetSet(mangle);
  mPolygon *points=(mPolygon *) model.polygons.first;
  int i=0;
  float dx=iratiof(1,width);
  float dy=iratiof(1,height);
  EACH(points->points.first,Vertex,v) {
   v->x+=set->unique(mangle+i,set->arnd(i),mangle+set->arnd(mangle+i))*dx;
   v->y+=set->unique(i,set->arnd(i),set->arnd(mangle+i))*dy;
   i++;
  }
 }
};

class Image2Models : public LinkedList {
public:
 Image2Model *Load( const char *filename ) {
  FOREACH(Image2Model,i) if ( contains(filename,i->filename.c_str()) ) return null;
  Image2Model *loaded;
  Append(loaded=new Image2Model(filename));
  return loaded;
 }
 Image2Model *Find( const char *filename ) {
  FOREACH(Image2Model,i) if ( !str_cmp(filename,i->filename.c_str()) ) return i;
  return null;
 }
 void Load( Strings *in ) {
  EACH(in->first,String,s) if ( !Find(s->s.c_str()) ) Append(new Image2Model(s->s.c_str()));
 }
 void Load( Strings *in, int mangle ) {
  EACH(in->first,String,s) if ( !Find(s->s.c_str()) ) {
   Image2Model *loaded;
   Append(loaded=new Image2Model(s->s.c_str()));
   loaded->Mangle(mangle);
  }
 }
 CLEARLISTRESET(Image2Model);
 ~Image2Models() {
  Clear();
 }
};

extern Image2Models i2mlibrary;