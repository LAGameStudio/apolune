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
#include "GLWindow.h"
#include "Art.h"
#include "Vertex.h"
#include "Film.h"

/*
 * Use wrapping textures (loaded with GL_WRAP).
 */

class ForeverWrappingQuadLayer : public ListItem {
public:
 Zp<GLImage> layer;
 Blends blend;
 Crayon tint;
 Cartesiand tiles;
 ForeverWrappingQuadLayer( GLImage *i, Blends b ) : ListItem() {
  layer=i;
  blend=b;
  tint.Pick(alabaster);
  tiles.Set(1.0,1.0);
 }
 ForeverWrappingQuadLayer( Crayon c, GLImage *i, Blends b ) : ListItem() {
  layer=i;
  blend=b;
  tint.fromCrayon(c);
  tiles.Set(1.0,1.0);
 }
 void Render( Cartesiand *tc, ScreenPosition *screen ) {
  Blending(blend);
  glBindTexture(GL_TEXTURE_2D, layer->texture);
  glColor4fv(tint.floats); 
  glBegin(GL_QUADS);  
  glTexCoord2d(tc->x,  tc->y);                   glVertex2i(screen->x,  screen->y);
  glTexCoord2d(tc->x2*tiles.x, tc->y);           glVertex2i(screen->x2, screen->y);
  glTexCoord2d(tc->x2*tiles.x, tc->y2*tiles.y);  glVertex2i(screen->x2, screen->y2);
  glTexCoord2d(tc->x,  tc->y2*tiles.y);          glVertex2i(screen->x,  screen->y2);
  glEnd();
 } 
 void RenderUpsideDown( Cartesiand *tc, ScreenPosition *screen ) {
  Blending(blend);
  glBindTexture(GL_TEXTURE_2D, layer->texture);
  glColor4fv(tint.floats); 
  glBegin(GL_QUADS);  
  glTexCoord2d(tc->x,  tc->y);                   glVertex2i(screen->x,  screen->y);
  glTexCoord2d(tc->x2*tiles.x, tc->y);           glVertex2i(screen->x2, screen->y);
  glTexCoord2d(tc->x2*tiles.x, tc->y2*tiles.y);  glVertex2i(screen->x2, screen->y2);
  glTexCoord2d(tc->x,  tc->y2*tiles.y);          glVertex2i(screen->x,  screen->y2);
  glEnd();
 }
};

class ForeverWrappingQuadLayers : public LinkedList {
public:
 Cartesiand position,tc;
 Cartesian tile;
 ScreenPosition screen;
 ForeverWrappingQuadLayers() : LinkedList() {
  screen.Set(0.0,0.0,display.wd,display.hd);
 }
 //PROGRAMMABLE(ForeverWrappingLayeredQuads,RenderPtr,render,Render);
 ForeverWrappingQuadLayer *Add( GLImage *i, Blends b ) {
  ForeverWrappingQuadLayer *f=new ForeverWrappingQuadLayer(i,b);
  Append(f);
  return f;
 }
 ForeverWrappingQuadLayer *Add( GLImage *i, Blends b, Crayon c ) {
  ForeverWrappingQuadLayer *f=new ForeverWrappingQuadLayer(c,i,b);
  Append(f);
  return f;
 }
 void MoveTo( double dx, double dy ) {
  position.x=dx;
//  while ( position.x > (double) screen.w ) position.x-=(double) screen.w;
//  while ( position.x < (double) -screen.w ) position.x+=(double) screen.w;
  position.y=dy;
//  while ( position.y > (double) screen.h ) position.y-=(double) screen.h;
//  while ( position.y < (double) -screen.h ) position.y+=(double) screen.h;
  position.x2=position.x+(double) tile.w;
  position.y2=position.y+(double) tile.h;
  UpdateTexCoords();
 }
 void MoveBy( double dx, double dy ) {
  position.x+=dx;
//  while ( position.x > (double) screen.w ) position.x-=(double) screen.w;
//  while ( position.x < (double) -screen.w ) position.x+=(double) screen.w;
  position.y+=dy;
//  while ( position.y > (double) screen.h ) position.y-=(double) screen.h;
//  while ( position.y < (double) -screen.h ) position.y+=(double) screen.h;
  position.x2=position.x+(double) tile.w;
  position.y2=position.y+(double) tile.h;
  UpdateTexCoords();
 }
 void UpdateTexCoords() {
  tc.x=(position.x/(double) screen.w);
  tc.y=(position.y/(double) screen.h);
  tc.x2=tc.x+1.0;
  tc.y2=tc.y+1.0;
 }
 virtual void Between() {
 }
 void Render() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  FOREACH(ForeverWrappingQuadLayer,f) f->Render( &tc, &screen );
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glPopAttrib();
 }
 void RenderUpsideDown() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  FOREACH(ForeverWrappingQuadLayer,f) f->RenderUpsideDown( &tc, &screen );
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glPopAttrib();
 }
 CLEARLISTRESET(ForeverWrappingQuadLayer);
 ~ForeverWrappingQuadLayers() { Clear(); }
};

class ParallaxFWQL : public ListItem {
public:
 Zdouble delta;
 ForeverWrappingQuadLayers fwql;
};

class ParallaxFWQLs : public LinkedList {
public:
 Cartesian tile,screen;
 Cartesian tween;
 ParallaxFWQL *Add( double delta ) {
  ParallaxFWQL *create=new ParallaxFWQL;
  create->delta=delta;
  create->fwql.tile.Set(&tile);
  create->fwql.screen.Set(&screen);
  Append(create);
  return create;
 }
 virtual void MoveBy( double dx, double dy ) {
  FOREACH(ParallaxFWQL,p) p->fwql.MoveBy(dx*p->delta,dy*p->delta);
 }
 virtual void Render() {
  FOREACH(ParallaxFWQL,p) p->fwql.Render();
 }
 virtual void Between() {
  FOREACH(ParallaxFWQL,p) p->fwql.Between();
 }
 CLEARLISTRESET(ParallaxFWQL);
 ~ParallaxFWQLs() { Clear(); }
};

class ForeverWrappingTiledFilm : public ListItem {
public:
 Zp<Film> layer;
 Blends blend;
 Crayon tint;
 Cartesiand tiles;
 Zfloat speed;
 ForeverWrappingTiledFilm( Film *f, Blends b ) : ListItem() {
  layer=f;
  blend=b;
  tint.Pick(alabaster);
  tiles.Set(0.1,0.1);
  speed=1.0f;
 }
 ForeverWrappingTiledFilm( Crayon c, Film *f, Blends b ) : ListItem() {
  layer=f;
  blend=b;
  tint.fromCrayon(c);
  tiles.Set(0.1,0.1);
  speed=1.0f;
 }
 void Render( Cartesiand *tc, ScreenPosition *screen, Zfloat expired ) {
  GLImage *frame=layer->frame(expired*speed);
  Blending(blend);
  glBindTexture(GL_TEXTURE_2D, frame->texture);
  glColor4fv(tint.floats); 
  glBegin(GL_QUADS);  
  glTexCoord2d(tc->x,  tc->y);                   glVertex2i(screen->x,  screen->y);
  glTexCoord2d(tc->x2*tiles.x, tc->y);           glVertex2i(screen->x2, screen->y);
  glTexCoord2d(tc->x2*tiles.x, tc->y2*tiles.y);  glVertex2i(screen->x2, screen->y2);
  glTexCoord2d(tc->x,  tc->y2*tiles.y);          glVertex2i(screen->x,  screen->y2);
  glEnd();
 } 
 void RenderUpsideDown( Cartesiand *tc, ScreenPosition *screen, Zfloat expired ) {
  GLImage *frame=layer->frame(expired*speed);
  Blending(blend);
  glBindTexture(GL_TEXTURE_2D, frame->texture);
  glColor4fv(tint.floats); 
  glBegin(GL_QUADS);  
  glTexCoord2d(tc->x,  tc->y);                   glVertex2i(screen->x,  screen->y);
  glTexCoord2d(tc->x2*tiles.x, tc->y);           glVertex2i(screen->x2, screen->y);
  glTexCoord2d(tc->x2*tiles.x, tc->y2*tiles.y);  glVertex2i(screen->x2, screen->y2);
  glTexCoord2d(tc->x,  tc->y2*tiles.y);          glVertex2i(screen->x,  screen->y2);
  glEnd();
 }
};

class ForeverWrappingTiledFilms : public LinkedList {
public:
 Cartesiand position,tc;
 Cartesian tile;
 ScreenPosition screen;
 Zfloat expired;
 ForeverWrappingTiledFilms() : LinkedList() {
  screen.Set(0.0,0.0,display.wd,display.hd);
 }
 //PROGRAMMABLE(ForeverWrappingLayeredQuads,RenderPtr,render,Render);
 ForeverWrappingTiledFilm *Add( Film *film, Blends b ) {
  ForeverWrappingTiledFilm *f=new ForeverWrappingTiledFilm(film,b);
  Append(f);
  return f;
 }
 ForeverWrappingTiledFilm *Add( Film *film, Blends b, Crayon c ) {
  ForeverWrappingTiledFilm *f=new ForeverWrappingTiledFilm(c,film,b);
  Append(f);
  return f;
 }
 void MoveTo( double dx, double dy ) {
  position.x=dx;
//  while ( position.x > (double) screen.w ) position.x-=(double) screen.w;
//  while ( position.x < (double) -screen.w ) position.x+=(double) screen.w;
  position.y=dy;
//  while ( position.y > (double) screen.h ) position.y-=(double) screen.h;
//  while ( position.y < (double) -screen.h ) position.y+=(double) screen.h;
  position.x2=position.x+(double) tile.w;
  position.y2=position.y+(double) tile.h;
  UpdateTexCoords();
 }
 void MoveBy( double dx, double dy ) {
  position.x+=dx;
//  while ( position.x > (double) screen.w ) position.x-=(double) screen.w;
//  while ( position.x < (double) -screen.w ) position.x+=(double) screen.w;
  position.y+=dy;
//  while ( position.y > (double) screen.h ) position.y-=(double) screen.h;
//  while ( position.y < (double) -screen.h ) position.y+=(double) screen.h;
  position.x2=position.x+(double) tile.w;
  position.y2=position.y+(double) tile.h;
  UpdateTexCoords();
 }
 void UpdateTexCoords() {
  tc.x=(position.x/(double) screen.w);
  tc.y=(position.y/(double) screen.h);
  tc.x2=tc.x+1.0;
  tc.y2=tc.y+1.0;
 }
 virtual void Between() {
  expired+=FRAMETIME;
 }
 void Render() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  FOREACH(ForeverWrappingTiledFilm,f) f->Render( &tc, &screen, expired );
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glPopAttrib();
 }
 void RenderUpsideDown() {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  FOREACH(ForeverWrappingTiledFilm,f) f->RenderUpsideDown( &tc, &screen, expired );
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glPopAttrib();
 }
 CLEARLISTRESET(ForeverWrappingQuadLayer);
 ~ForeverWrappingTiledFilms() { Clear(); }
};

class ParallaxFWTF : public ListItem {
public:
 Zdouble delta;
 ForeverWrappingTiledFilms fwtf;
};

class ParallaxFWTFs : public LinkedList {
public:
 Cartesian tile,screen;
 Cartesian tween;
 CLEARLISTRESET(ParallaxFWTF);
 ~ParallaxFWTFs() { Clear(); }
 ParallaxFWTF *Add( double delta ) {
  ParallaxFWTF *create=new ParallaxFWTF;
  create->delta=delta;
  create->fwtf.tile.Set(&tile);
  create->fwtf.screen.Set(&screen);
  Append(create);
  return create;
 }
 virtual void MoveBy( double dx, double dy ) {
  FOREACH(ParallaxFWTF,p) p->fwtf.MoveBy(dx*p->delta,dy*p->delta);
 }
 virtual void Render() {
  FOREACH(ParallaxFWTF,p) p->fwtf.Render();
 }
 virtual void Between() {
  FOREACH(ParallaxFWTF,p) p->fwtf.Between();
 }
};