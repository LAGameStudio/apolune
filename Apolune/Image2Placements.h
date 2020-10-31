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

#include "Vertex.h"
#include "Polygon.h"
#include "Model.h"

class Image2Placement : public ListItem {
public:
 string name;
 ImmediateModel model; // Color-sorted
 mPolygon points; // Consolidated
 Image2Placement( const char *filename ) : ListItem() {
  Load(filename);
  name=string(filename);
 }
 void Load( const char *filename ) {
  byte *pixels;
  int pixelSize,bytesSize;
  int width, height;

#if defined(USE_CORONA)
  corona::Image *image;
  corona::PixelFormat format;
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
  CPUImage *image;
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
  for ( int x=0; x<width; x++ ) for ( int y=0; y<height; y++ ) {
   byte *bgra=(byte *) &pixels[(x+y*width)*pixelSize];
   if ( bgra[0] == 255 && bgra[1] == 255 && bgra[2] == 255 ) continue;
   Vertex *v=new Vertex;
   v->rgba[0]=iratiof((int)bgra[2],255);
   v->rgba[1]=iratiof((int)bgra[1],255);
   v->rgba[2]=iratiof((int)bgra[0],255);
   v->rgba[3]=iratiof((int)bgra[3],255);
   v->x=iratiof(x,width);
   v->y=iratiof(y,height);
   v->z=0.0f;
   mPolygon *p=model.polygons.findFirstByColor( v->rgba );
   if ( !p ) {
    p=new mPolygon;
    p->draw_method=GL_POINTS;
    model.polygons.Append(p);
   }
   p->points.Append(v);
   points.points.Append(v->Copy());
  }
  Vertex least,greatest,difference;
  model.polygons.Leasts(&least);
  model.polygons.Greatests(&greatest);
  difference.Set(greatest.x-least.x,greatest.y-least.y,0.0f);
  EACH(points.points.first,Vertex,v) {
   if ( difference.x != 0.0f ) v->x=2.0f*((v->x-least.x)/difference.x)-1.0f;
   if ( difference.y != 0.0f ) v->y=2.0f*((v->y-least.y)/difference.y)-1.0f;
  }
  EACH(model.polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) {
   if ( difference.x != 0.0f ) v->x=2.0f*((v->x-least.x)/difference.x)-1.0f;
   if ( difference.y != 0.0f ) v->y=2.0f*((v->y-least.y)/difference.y)-1.0f;
  }
  delete image;
 }
};

class Image2Placements : public LinkedList {
public:
 CLEARLISTRESET(Image2Placement);
};