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
#include "Art.h"
#include "Shape2d.h"
#include "Model.h"
#include "Image2Model.h"
#include "VBO.h"
#include "Trigger.h"

extern GLSetup gl;

class MultiPointSprite : public ListItem {
public:
 VBOStaticV vboV;
 Vertexd translation,rotation,scale;
 GLImage *sprite;
 float renderSize;
 float sizeMin,sizeMax;
 float quadratic[3];
 float fadeThreshold;
 bool useQuadratic,useFade,useMinMax;
 Blends blend;
 Crayon tint;
 MultiPointSprite(void) {
  sprite=null;
  sizeMin=gl.minPointSize;
  sizeMax=gl.maxPointSize;
  scale.Set(1.0,1.0,1.0);
  tint.Pick(alabaster);
  blend=transparency;
  quadratic[0]=1.0f; quadratic[1]=0.0f; quadratic[2]=0.01f; // near/middle/far scaling of sprites
  useQuadratic=false;
  useMinMax=false;
  useFade=false;
  fadeThreshold=60.0f;
  renderSize=gl.maxPointSize;
 }
 void SetSize( float max ) {
  if ( max > gl.maxPointSize ) max=gl.maxPointSize;
  sizeMax=max;
 }
 void SetSize( float min, float max ) {
  if ( min < gl.minPointSize ) min=gl.minPointSize;
  if ( max > gl.maxPointSize ) max=gl.maxPointSize;
  sizeMin=min;
  sizeMax=max;
 }
 void fromShape( Shape2d *shape ) {
  int size=shape->points.size();
  for ( int i=0; i<size; i++ ) vboV.PushVertex(&(shape->points[i].position));
  vboV.Create();
 }
 void fromShape( Shape2d *shape, bool create ) {
  int size=shape->points.size();
  for ( int i=0; i<size; i++ ) vboV.PushVertex(&(shape->points[i].position));
  if ( create ) vboV.Create();
 }
 void AddPoint( double x, double y, double z ) {
  Vertexd point;
  point.Set(x,y,z);
  vboV.PushVertex(&point);
 }
 virtual void Render() {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslated(translation.x,translation.y,translation.z);
  glRotated(rotation.x,1.0,0.0,0.0);
  glRotated(rotation.y,0.0,1.0,0.0);
  glRotated(rotation.z,0.0,0.0,1.0);
  glScaled(scale.x,scale.y,scale.z);
  glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
  if ( useQuadratic ) glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, quadratic );
  if ( useFade ) glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE, fadeThreshold );
  if ( useMinMax ) {
   glPointParameterf( GL_POINT_SIZE_MIN, sizeMin );
   glPointParameterf( GL_POINT_SIZE_MAX, sizeMax );
  } else glPointSize(renderSize);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, sprite->texture );
  Blending(blend);
  glColor4fv(tint.floats);
  glEnable(GL_POINT_SPRITE);
  vboV.Render();
  glDisable(GL_POINT_SPRITE);
  glPopMatrix();
 }
};

class MultiPointSprites : public LinkedList {
public:
 CLEARLISTRESET(MultiPointSprite);
};

class MultiColorMultiPointSprite : public ListItem {
public:
 VBOStaticVC vboVC;
 Vertexd translation,rotation,scale;
 Zp<GLImage> sprite;
 float renderSize;
 float sizeMin,sizeMax;
 float quadratic[3];
 Zfloat fadeThreshold;
 Zbool useQuadratic,useFade,useMinMax;
 Blends blend;
 Zdisposable<Image2Model> buffer;
 ImmediateModel model;
 Zint width,height;
 MultiColorMultiPointSprite(void) {
  sprite=null;
  sizeMin=gl.minPointSize;
  sizeMax=gl.maxPointSize;
  scale.Set(1.0,1.0,1.0);
  blend=transparency;
  quadratic[0]=1.0f; quadratic[1]=0.0f; quadratic[2]=0.01f; // near/middle/far scaling of sprites
  useQuadratic=false;
  useMinMax=false;
  useFade=false;
  fadeThreshold=60.0f;
  renderSize=gl.maxPointSize;
 }
 void SetSize( float max ) {
  if ( max > gl.maxPointSize ) max=gl.maxPointSize;
  sizeMax=max;
 }
 void SetSize( float min, float max ) {
  if ( min < gl.minPointSize ) min=gl.minPointSize;
  if ( max > gl.maxPointSize ) max=gl.maxPointSize;
  sizeMin=min;
  sizeMax=max;
 }
 void fromShape( Shape2d *shape ) {
  int size=shape->points.size();
  for ( int i=0; i<size; i++ ) vboVC.PushVertexColor(&(shape->points[i].position),shape->points[i].color);
  vboVC.Create();
 }
 void fromShape( Shape2d *shape, bool create ) {
  int size=shape->points.size();
  for ( int i=0; i<size; i++ ) vboVC.PushVertexColor(&(shape->points[i].position),shape->points[i].color);
  if ( create ) vboVC.Create();
 }
 void fromShape( Shape2d *shape, Crayon color, bool create ) { 
  int size=shape->points.size();
  for ( int i=0; i<size; i++ ) vboVC.PushVertexColor(&(shape->points[i].position),color);
  if ( create ) vboVC.Create();
 }
 void AddPoint( double x, double y, double z, Crayon color ) {
  Vertexd point;
  point.Set(x,y,z);
  vboVC.PushVertexColor(&point,color);
 }
 void fromModel( ImmediateModel *model ) {
  EACH(model->polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) vboVC.PushVertex(v);
 }
 void fromModel( ImmediateModel *m, bool copy ) {
  model.Duplicate(m);
  EACH(model.polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) vboVC.PushVertex(v);  
 }
 void fromModel( ImmediateModel *m, bool copy, int mangle, float dx, float dy ) {
  model.Duplicate(m,mangle,dx,dy);
  EACH(model.polygons.first,mPolygon,p) EACH(p->points.first,Vertex,v) vboVC.PushVertex(v);  
 }
 void fromImage( const char *filename ) {
  buffer.Recycle(new Image2Model(filename));
  fromModel(&buffer->model);
  delete buffer;
 }
 void fromImage( const char *filename, int mangle ) {
  Random *set=randoms.GetSet(mangle);
  buffer.Recycle(new Image2Model(filename));
  buffer->Mangle(mangle);
  fromModel(&buffer->model);
 }
 void RemoveWithin( double x, double y, double R ) {
  if ( model.polygons.count > 0 ) RemoveWithinModel(x,y,R);
  else if ( buffer.pointer ) RemoveWithinBuffer(x,y,R);
 }
 void FindWithin( double x, double y, double R, VertexHandles *out ) {
  if ( model.polygons.count > 0 ) FindWithinModel(x,y,R,out);
  else if ( buffer.pointer ) FindWithinBuffer(x,y,R,out);
 }
 void FindWithinModel( double x, double y, double R, VertexHandles *out ) {
  mPolygon *points=(mPolygon *) model.polygons.first;
  Circle circle(x,y,R,8);
  EACH(points->points.first,Vertex,v) if ( circle.within(v->x,v->y,v->z) ) out->Append(v);
 }
 void RemoveWithinModel( double x, double y, double R ) {
  mPolygon *points=(mPolygon *) model.polygons.first;
  Circle circle(x,y,R,8);
  EACHN(points->points.first,Vertex,v,{
   if ( circle.within(v->x,v->y,v->z) ) {
    points->points.Remove(v);
    delete v;
   }
  });
  vboVC.Clear();
  fromModel(&model);
 }
 void FindWithinBuffer( double x, double y, double R, VertexHandles *out ) {
  mPolygon *points=(mPolygon *) buffer->model.polygons.first;
  Circle circle(x,y,R,8);
  EACH(points->points.first,Vertex,v) if ( circle.within(v->x,v->y,v->z) ) out->Append(v);
 }
 void RemoveWithinBuffer( double x, double y, double R ) {
  mPolygon *points=(mPolygon *) buffer->model.polygons.first;
  Circle circle(x,y,R,8);
  EACHN(points->points.first,Vertex,v,{
   if ( circle.within(v->x,v->y,v->z) ) {
    points->points.Remove(v);
    delete v;
   }
  });
  vboVC.Clear();
  fromModel(&buffer->model);
 }
 virtual void Render() {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslated(translation.x,translation.y,translation.z);
  glRotated(rotation.x,1.0,0.0,0.0);
  glRotated(rotation.y,0.0,1.0,0.0);
  glRotated(rotation.z,0.0,0.0,1.0);
  glScaled(scale.x,scale.y,scale.z);
  glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
  if ( useQuadratic ) glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, quadratic );
  if ( useFade ) glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE, fadeThreshold );
  if ( useMinMax ) {
   glPointParameterf( GL_POINT_SIZE_MIN, sizeMin );
   glPointParameterf( GL_POINT_SIZE_MAX, sizeMax );
  } else glPointSize(renderSize);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, sprite->texture );
  Blending(blend);
  glEnable(GL_POINT_SPRITE);
  vboVC.Render();
  glDisable(GL_POINT_SPRITE);
  glPopMatrix();
 }
};

class MultiColorMultiPointSprites : public LinkedList {
public:
 CLEARLISTRESET(MultiColorMultiPointSprite);
};

#include "LookAtPerspective.h"
#include "TextureLibrary.h"

class MultiPointSpriteTest : public GLWindow {
public:
 LookAtPerspective lap;
 MultiPointSprite mps;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  for ( int i=0; i<1000; i++ ) {
   mps.AddPoint(5.0*(0.5-uniform()),5.0*(0.5-uniform()),0.0*(0.5-uniform()));
  }
  lap.Far=20.0;
  lap.eye.Set(0.0,0.0,10);
  mps.vboV.Create();
  mps.renderSize=12.0f;
//  mps.SetSize(1.0f,3.0f);
//  mps.useMinMax=true;
  mps.useQuadratic=true;
  mps.blend=transparency;
  mps.sprite=library.Load("data/images/flares/tinygrass.png");
  mps.tint.Pick(green);
 }
 void Between() {
  int mx,my;
  MousePosition(&mx,&my);
  float dx,dy;
  dx=iratiof(mx,display->w)*360.0f;
  dy=iratiof(my,display->h)*360.0f;
  mps.rotation.x=dx;
  mps.rotation.y=dy;
 }
 void Render() {
  glPush();
  lap.Apply();
  mps.Render();
  glPop();
 }
};

class MultiColorMultiPointSpriteTest : public GLWindow {
public:
 LookAtPerspective lap;
 MultiColorMultiPointSprite mcmps;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(clear);
  for ( int i=0; i<720000; i++ ) { // could be 100000 though
   mcmps.AddPoint(100.0*(0.5-uniform()),5.0*(0.5-uniform()),5.0*(0.5-uniform()),crayons.Any());
  }
  lap.Far=200.0;
  lap.eye.Set(0.0,0.0,80);
  mcmps.vboVC.Create();
  mcmps.renderSize=64.0f;
//  mps.SetSize(1.0f,3.0f);
//  mps.useMinMax=true;
  mcmps.useQuadratic=true;
  mcmps.blend=additive;
  mcmps.sprite=library.Load("data/images/flares/blue2.png");
 }
 void Between() {
  int mx,my;
  MousePosition(&mx,&my);
  float dx,dy;
  dx=iratiof(mx,display->w)*360.0f;
  dy=iratiof(my,display->h)*360.0f;
  mcmps.rotation.x=dx;
  mcmps.rotation.y=dy;
 }
 void Render() {
  glPush();
  lap.Apply();
  mcmps.Render();
  glPop();
 }
};

#include "Film.h"


class MultiColorMultiPointFilmTest : public GLWindow {
public:
 LookAtPerspective lap;
 MultiColorMultiPointSprite mcmps;
 Film *f;
 float expired,duration;
 void OnLoad() {
  Anchor(0.0f,0.0f,0.0f,0.0f);
  background.Pick(indigo);
  for ( int i=0; i<100; i++ ) { // could be 100000 though
   mcmps.AddPoint(100.0*(0.5-uniform()),5.0*(0.5-uniform()),5.0*(0.5-uniform()),crayons.Brights(i));
  }
  lap.Far=200.0;
  lap.eye.Set(0.0,0.0,10);
  mcmps.vboVC.Create();
  mcmps.renderSize=64.0f;
//  mps.SetSize(1.0f,3.0f);
//  mps.useMinMax=true;
  mcmps.useQuadratic=true;
  mcmps.blend=additive;
  expired=0.0f;
  duration=2.0f;

  f=vault.find("Sparks","Red Sparks");
  mcmps.sprite=f->frame(expired/duration);
 }
 void Between() {
  int mx,my;
  MousePosition(&mx,&my);
  float dx,dy;
  dx=iratiof(mx,display->w)*360.0f;
  dy=iratiof(my,display->h)*360.0f;
  mcmps.rotation.x=dx;
  mcmps.rotation.y=dy;
  expired+=FRAMETIME;
  if ( expired>=duration ) expired=0.0f;
  mcmps.sprite=f->frame(expired/duration);
 }
 void Render() {
  glPush();
  lap.Apply();
  mcmps.Render();
  glPop();
 }
};