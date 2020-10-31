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
 ***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#pragma once
#include <Windows.h>
#include <math.h>

#include "Crayon.h"
#include "Cartesian.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "Vertex.h"
#include "Particle2d.h"
#include "Emitter2d.h"

#include "Randoms.h"
/*
class Multiparticle : public Particlef2d {
public:
 Cartesian *offset;
 Cartesian actual,position;
 Cartesian *wind,*gravity,movement,*bounds;
 GLImage *flake;
 Crayon color,tint;
 float mass, sizeMin,sizeMax,deltaSize,actualSize,lifespan;
 float waiting;
 float *stagnancy,*fadeLength;
 bool dead;

 void Init() {
  dead=false;
  actualSize=sizeMax;
  lifespan=0;
  tint.fromCrayon(color);
  deltaSize=float_range(sizeMin/10,sizeMin/2);
  Wait();
 }
 void Wait() {
  int elNombre=upto(60000);
  waiting=xyzs[elNombre]*(*stagnancy);
  if ( elNombre % 2 == 1 ) waiting=-waiting;
  deltaSize=-deltaSize;
 }
 void Between() {
  if ( position.y < bounds->y2 ) {
   actualSize=URANGE(sizeMin,actualSize+deltaSize,sizeMax);
   position.x+=(int) ((movement.fx*waiting)+gravity->fx*mass+wind->fx*actualSize);
   position.y+=(int) ((movement.fy*waiting)+gravity->fy*mass+wind->fy*actualSize);
  } else {
   if ( actualSize > 0.0f ) actualSize-=abs(deltaSize);
   lifespan+=1.0f/FPS;
   if ( lifespan >= (*fadeLength) ) dead=true;
   else tint.ColorInterp(color,crayons.Pick(black),lifespan/(*fadeLength));
  }
  actual.w=(int)(actualSize*flake->width);
  actual.h=(int)(actualSize*flake->height);
  actual.x=offset->x+position.x-(actual.w/2);
  actual.y=offset->y+position.y-(actual.h/2);
  waiting-=(1.0f/FPS);
  if ( waiting <= 0 ) Wait();
 }
 void Render(GLWindow *surface) {
  if ( dead ) return;
  StretchiNoAlpha(surface,flake,tint,additive,actual.x,actual.y,actual.w,actual.h);
 }
 Flakef2d *Duplicate() {
  Flakef2d *n=new Flakef2d;
  n->offset=offset;
  n->actual=actual;
  n->wind=wind;
  n->position.x=position.x;
  n->position.y=position.y;
  n->gravity=gravity;
  n->movement=movement;
  n->bounds=bounds;
  n->flake=flake;
  n->tint.fromCrayon(tint);
  n->sizeMin=sizeMin;
  n->sizeMax=sizeMax;
  n->waiting=waiting;
  n->stagnancy=stagnancy;
  n->dead=dead;
 }
};

PARTICLECOLLECTION(Flakesf2d,Flakef2d);

class Snowf2d : public ListItem {
public:
 Cartesian bounds;
 Cartesian wind,gravity,movement,offset;
 Flakesf2d flakes;
 Images images;
 Crayons colors;
 int density,jitter,limit;
 float stagnancy, fadeLength;
 float sizeMin,sizeMax;
 double intensity;
 Snowf2d() { Zero(); }
 Snowf2d( int density, int jitter, int limit, float stagnancy, double intensity, float fadeLength, float sizeMin, float sizeMax ) {
  Zero();
  Create(density,jitter,limit,stagnancy,intensity,fadeLength,sizeMin,sizeMax);
 }
 void Create( int density, int jitter, int limit, float stagnancy, double intensity, float fadeLength, float sizeMin, float sizeMax ) {
  this->density=density;
  this->jitter=jitter;
  this->stagnancy=stagnancy;
  this->intensity=intensity;
  this->limit=limit;
  this->fadeLength=fadeLength;
  this->sizeMin=sizeMin;
  this->sizeMax=sizeMax;
 }
 void Emit() {
  int creating=upto(density);
  for ( int i=0; i<creating; i++ ) {
   Flakef2d *f=new Flakef2d;
   f->gravity=&gravity;
   f->movement.fx=movement.fx;
   f->movement.fy=movement.fy;
   f->wind=&wind;
   f->offset=&offset;
   f->stagnancy=&stagnancy;
   f->bounds=&bounds;
   f->fadeLength=&fadeLength;
   f->flake=images.random();
   f->mass=float_range(0.1f,2.0f);
   f->color.fromCrayon(colors.random());
   f->sizeMin=sizeMin+float_range(0.0f,sizeMin/2);
   f->sizeMax=sizeMax+float_range(0.0f,sizeMax/2);
   f->position.x=number_range(bounds.x,bounds.x2);
   f->position.y=number_range(0,jitter)+bounds.y-f->flake->height;
   f->Init();   
   flakes.Append(f);
  }
 }
 void Init() {
  Emit();
 }
 void Between() {
  if ( flakes.count < limit && uniform() < intensity ) Emit();
  Flakef2d *t,*n;
  for ( t=(Flakef2d *)flakes.first; t; t=n ) {
   n=(Flakef2d *) t->next;
   if ( t->dead ) {
    flakes.Remove(t);
    delete t;
   } else t->Between();
  }
 }
 void Render(GLWindow *surface) {
  EACH(flakes.first,Flakef2d,f) f->Render(surface);
 }
 void MoveTo( int x, int y ) {
  offset.x=x;
  offset.y=y;
 }
};

class Snowf2d2 : public ListItem {
public:
 Cartesian bounds;
 Cartesian wind,gravity,movement,offset;
 Flakesf2d flakes;
 Images images;
 Crayons colors;
 int density,jitter,limit;
 float stagnancy, fadeLength;
 float sizeMin,sizeMax;
 double intensity;
 Snowf2d2() { Zero(); }
 Snowf2d2( int density, int jitter, int limit, float stagnancy, double intensity, float fadeLength, float sizeMin, float sizeMax ) {
  Zero();
  Create(density,jitter,limit,stagnancy,intensity,fadeLength,sizeMin,sizeMax);
 }
 void Create( int density, int jitter, int limit, float stagnancy, double intensity, float fadeLength, float sizeMin, float sizeMax ) {
  this->density=density;
  this->jitter=jitter;
  this->stagnancy=stagnancy;
  this->intensity=intensity;
  this->limit=limit;
  this->fadeLength=fadeLength;
  this->sizeMin=sizeMin;
  this->sizeMax=sizeMax;
 }
 void Emit() {
  int creating=upto(density);
  for ( int i=0; i<creating; i++ ) {
   Flakef2d *f=new Flakef2d;
   f->gravity=&gravity;
   f->movement.fx=movement.fx;
   f->movement.fy=movement.fy;
   f->wind=&wind;
   f->offset=&offset;
   f->stagnancy=&stagnancy;
   f->bounds=&bounds;
   f->fadeLength=&fadeLength;
   f->flake=images.random();
   f->mass=float_range(0.1f,2.0f);
   f->color.fromCrayon(colors.random());
   f->sizeMin=sizeMin+float_range(0.0f,sizeMin/2);
   f->sizeMax=sizeMax+float_range(0.0f,sizeMax/2);
   f->position.x=number_range(bounds.x,bounds.x2);
   f->position.y=number_range(0,jitter)+bounds.y-f->flake->height;
   f->Init();   
   flakes.Append(f);
  }
 }
 void Init() {
  Emit();
 }
 void Between() {
  if ( flakes.count < limit && uniform() < intensity ) Emit();
  Flakef2d *t,*n;
  for ( t=(Flakef2d *)flakes.first; t; t=n ) {
   n=(Flakef2d *) t->next;
   if ( t->dead ) {
    flakes.Remove(t);
    delete t;
   } else t->Between();
  }
 }
 void Render(GLWindow *surface) {
  EACH(flakes.first,Flakef2d,f) f->Render(surface);
 }
 void MoveBy( int dx, int dy ) {
  Flakef2d *t,*n;
  for ( t=(Flakef2d *)flakes.first; t; t=n ) {
   t->position.x+=dx;
   if ( t->position.x > bounds.x2 ) t->position.x-=bounds.w;
   t->position.y+=dy;
   if ( t->position.y > bounds.y2 ) t->position.y-=bounds.h;
  }
 }
};

class Snowf2ds : public LinkedList {
public:
 Snowf2ds() {
  Zero();
 }
 Snowf2d *Add( int density, int jitter, int limit, float stagnancy, double intensity, float fadeLength, float sizeMin, float sizeMax ) {
  Snowf2d *s;
  Append(s=new Snowf2d( density, jitter, limit, stagnancy, intensity, fadeLength, sizeMin, sizeMax ) );
  return s;
 }
 CLEARLISTRESET(Snowf2d);
 ~Snowf2ds() { Clear(); }
};

//CPUPARTICLESYSTEMLIST(Snowf2d,Flakef2d,flakes); 
*/