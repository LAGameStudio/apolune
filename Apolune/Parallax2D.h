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
#include "GLImage.h"
#include "GLWindow.h"
#include "Proce55or.h"
#include "Presentation.h"

#include "CompositeImage.h"
#include "Film.h"
#include "TileMap2d.h"

class ParallaxPresentationLayer2d : public ListItem {
public:
 Presentation *p;
 float factor;
 ParallaxPresentationLayer2d() : ListItem() {
  factor=1.0f;
  p=null;
 }
 void MoveLayerBy( int x, int y ) {
  p->MoveBy( (int) ((float) x*factor), (int) ((float) y*factor) );
 }
};

class ParallaxPresentationLayers2d : public LinkedList {
public:
 void MoveBy( int x, int y ) {
  EACH(first,ParallaxPresentationLayer2d,p) p->MoveLayerBy(x,y);
 }
 CLEARLISTRESET(ParallaxPresentationLayer2d);
 ~ParallaxPresentationLayers2d() {
  Clear();
 }
};

class ParallaxPresentation2d : public ListItem
{
public:
 Presentation *background;
 ParallaxPresentationLayers2d layers;
 void SetBackground( Presentation *p ) {
  background=p;
 }
 void Add( Presentation *p, float factor ) {
  ParallaxPresentationLayer2d *pl=new ParallaxPresentationLayer2d;
  pl->p=p;
  pl->factor=factor;
  layers.Append(pl);
 }
 void MoveBy( float x, float y ) {
  EACH(layers.first,ParallaxPresentationLayer2d,l) l->MoveLayerBy( (int) x, (int) y );
 }
};

/*
 * A version that wraps, does not use the presentation / slide format,
 * does simple animations.
 */
class ParallaxLayer2di : public ListItem {
public:
 GLWindow *surface;
 CompositeImages images;
 CompositeFilms films;
 TileMaps2d tiles;
 float factor;
 int x,y;
 ParallaxLayer2di() : ListItem() {
  surface=null;
  factor=1.0f;
  x=y=0;
 }
 void MoveByStored() {
  images.MoveBy(x,y);
  films.MoveBy(x,y);
  tiles.MoveBy(x,y);
 }
 void MoveBy( int X, int Y ) {
  x=(int) (factor*(float)X);
  y=(int) (factor*(float)Y);
  images.MoveBy(x,y);
  films.MoveBy(x,y);
  tiles.MoveBy(x,y);
 }
 void RenderTo( GLWindow *surface ) {
  images.RenderTo(surface);
  films.RenderTo(surface);
  films.Rewind();
  tiles.RenderTo(surface);
 }
 void Between() {
  films.Between();
 }
 void Render() {
  images.Render();
  films.Render();
  tiles.Render();
 }
};

class ParallaxLayers2di : public LinkedList {
public:
 void MoveBy( int x, int y ) { FOREACH(ParallaxLayer2di,p) p->MoveBy( x, y ); }
 void Render() { FOREACH(ParallaxLayer2di,p) p->Render(); }
 void Between() { FOREACH(ParallaxLayer2di,p) p->Between(); }
 void RenderTo( GLWindow *surface ) { FOREACH(ParallaxLayer2di,p) p->RenderTo(surface);}
 CLEARLISTRESET(ParallaxLayer2di);
 ~ParallaxLayers2di() { Clear(); }
};

class Parallax2di : public ListItem {
public:
 ParallaxLayer2di background;
 ParallaxLayers2di layers;
 Parallax2di() : ListItem() { background.factor=0.0f; }
 ParallaxLayer2di *AddLayer( float factor ) {
  ParallaxLayer2di *d=new ParallaxLayer2di;
  d->factor=factor;
  layers.Append(d);
  return d;
 }
 void MoveBy( int x, int y ) {
  layers.MoveBy(x,y);
 }
 void Between() {
  background.Between();
  layers.Between();
 }
 void RenderTo(GLWindow *surface) {
  background.RenderTo(surface);
  layers.RenderTo(surface);
 }
 void Render() {
  background.Render();
  layers.Render();
 }
};


class MassiveTileMapParallaxLayer2d : public ListItem {
public:
 MassiveTileMap2d map;
 double factor;
 ScreenPosition position;
 void MoveBy( double x, double y ) {
  position.MoveBy(x,y);
 }
 void MoveBy( int x, int y ) {
  position.MoveBy(x,y);
 }
};

class MassiveTileMapParallaxLayer2dd : public ListItem {
public:
 MassiveTileMap2dd map;
 Zdouble factor;
 ScreenPosition position;
 void MoveBy( double x, double y ) {
  position.MoveBy(x,y);
 }
};

class MassiveTileMapParallaxLayers2d : public LinkedList {
public:
 void Reposition() { FOREACH(MassiveTileMapParallaxLayer2d,p) p->map.Reposition(); }
 void Resized() { FOREACH(MassiveTileMapParallaxLayer2d,p) p->map.Resized(); }
 void Render() { FOREACH(MassiveTileMapParallaxLayer2d,p) p->map.Render(); }
 void Zoom( float w, float h ) { FOREACH(MassiveTileMapParallaxLayer2d,p) p->map.Zoom(w,h); }
 void RenderTo( GLWindow *surface ) { FOREACH(MassiveTileMapParallaxLayer2d,p) p->map.RenderTo(surface);}
 void MoveByd( double x, double y ) {
  FOREACH(MassiveTileMapParallaxLayer2d,p) {
   p->position.MoveBy((x*p->factor),(y*p->factor));
  }
 }
 void MoveBy( int x, int y ) {
  double dx=(double)x;
  double dy=(double)y;
  FOREACH(MassiveTileMapParallaxLayer2d,p) {
   p->position.MoveBy((int)(dx*p->factor),(int)(dy*p->factor));
  }
 }
 CLEARLISTRESET(MassiveTileMapParallaxLayer2d);
 ~MassiveTileMapParallaxLayers2d() { Clear(); }
};

class MassiveTileMapParallaxLayers2dd : public LinkedList {
public:
 void Reposition() { FOREACH(MassiveTileMapParallaxLayer2dd,p) p->map.Reposition(); }
 void Resized() { FOREACH(MassiveTileMapParallaxLayer2dd,p) p->map.Resized(); }
 void Render() { FOREACH(MassiveTileMapParallaxLayer2dd,p) p->map.Render(); }
 void Zoom( float w, float h ) { FOREACH(MassiveTileMapParallaxLayer2dd,p) p->map.Zoom(w,h); }
 void RenderTo( GLWindow *surface ) { FOREACH(MassiveTileMapParallaxLayer2dd,p) p->map.RenderTo(surface);}
 void MoveBy( double x, double y ) {
  FOREACH(MassiveTileMapParallaxLayer2dd,p) {
   p->position.MoveBy((x*p->factor),(y*p->factor));
  }
 }
 CLEARLISTRESET(MassiveTileMapParallaxLayer2dd);
 ~MassiveTileMapParallaxLayers2dd() { Clear(); }
};

class MassiveTileMapParallax2d : public ListItem {
public:
 MassiveTileMapParallaxLayers2d layers;
 MassiveTileMapParallax2d() : ListItem() { }
 MassiveTileMapParallaxLayer2d *AddLayer(
  TilemapRenderModes mode, TilemapDrawModes draw,
  float factor, GLWindow *T, TileSet2d *t2d, Strings *map,
  int w, int h, int tw, int th, int cL
 ) {
  MassiveTileMapParallaxLayer2d *d=new MassiveTileMapParallaxLayer2d;
  d->factor=factor;
  d->map.Init( mode, draw, T, &(d->position.x.value),&(d->position.y.value), t2d, map, w, h, tw, th, cL ); 
  layers.Append(d);
  return d;
 } 
 Vertexd excess;
 void MoveBy( double fx, double fy ) {
  int ix,iy;
  fx+=excess.x;
  fy+=excess.y;
  ix=(int) fx;
  iy=(int) fy;
  excess.Set( fx - (double) ix, fy - (double) iy );
  MoveBy(ix,iy);
 }
 void MoveBy( int x, int y ) {
  layers.MoveBy(x,y);
  layers.Reposition();
 }
 void RenderTo(GLWindow *surface) {
  layers.RenderTo(surface);
 }
 void Render() {
  layers.Render();
 }
};

class MassiveTileMapParallax2dd : public ListItem {
public:
 MassiveTileMapParallaxLayers2dd layers;
 MassiveTileMapParallax2dd() : ListItem() { }
 MassiveTileMapParallaxLayer2dd *AddLayer(
  TilemapRenderModes mode, TilemapDrawModes draw,
  float factor, GLWindow *T, TileSet2d *t2d, Strings *map,
  int w, int h, int tw, int th, int cL
 ) {
  MassiveTileMapParallaxLayer2dd *d=new MassiveTileMapParallaxLayer2dd;
  d->factor=factor;
  d->map.Init( mode, draw, T, &(d->position.dx.value),&(d->position.dy.value), t2d, map, w, h, tw, th, cL ); 
  layers.Append(d);
  return d;
 } 
 void MoveBy( double x, double y ) {
  layers.MoveBy(x,y);
  layers.Reposition();
 }
 void RenderTo(GLWindow *surface) {
  layers.RenderTo(surface);
 }
 void Render() {
  layers.Render();
 }
};