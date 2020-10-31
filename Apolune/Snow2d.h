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
#include <Windows.h>
#include <math.h>

#include "Randoms.h"
#include "Crayon.h"
#include "Cartesian.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "Vertex.h"
#include "Art2d.h"
#include "Particle2d.h"
#include "Emitter2d.h"


class Flaked2d : public Particlef2d {
public:
 Zp<Cartesiand> offset;
 Cartesiand actual,position,movement;
 Zp<Cartesiand> wind,gravity,bounds;
 Zp<GLImage> flake;
 Crayon color,tint;
 Zdouble mass, sizeMin,sizeMax,deltaSize,actualSize,lifespan;
 Zdouble waiting;
 Zp<double> stagnancy,fadeLength;
 bool dead;

 void Init() {
  dead=false;
  actualSize=sizeMax;
  lifespan=0;
  tint.fromCrayon(color);
  deltaSize=double_range(sizeMin/10,sizeMin/2);
  Wait();
 }
 void Wait() {
  int elNombre=upto(60000);
  waiting=xyzs[elNombre]*(*stagnancy.pointer);
  if ( elNombre % 2 == 1 ) waiting=-waiting;
  deltaSize=-deltaSize;
 }
 void Between() {
  if ( position.y < bounds->y2 ) {
   actualSize=URANGE(sizeMin,actualSize+deltaSize,sizeMax);
   position.x+=(int) ((movement.x*waiting)+gravity->x*mass+wind->x*actualSize);
   position.y+=(int) ((movement.y*waiting)+gravity->y*mass+wind->y*actualSize);
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
  art.Stretch(flake,tint,additive,actual.x,actual.y,actual.w,actual.h);
 }
 Flaked2d *Duplicate() {
  Flaked2d *n=new Flaked2d;
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

PARTICLECOLLECTION(Flakesd2d,Flaked2d);

class Snowd2d : public ListItem {
public:
 Cartesiand bounds;
 Cartesiand wind,gravity,movement,offset;
 Flakesd2d flakes;
 Images images;
 Crayons colors;
 Zint density,jitter,limit;
 Zdouble stagnancy, fadeLength;
 Zdouble sizeMin,sizeMax;
 Zdouble intensity;
 Snowd2d() : ListItem() {
  density=jitter=limit=0;
  stagnancy=fadeLength=sizeMin=sizeMax=0.0;
  intensity=0.0;
 }
 Snowd2d( int density, int jitter, int limit, double stagnancy, double intensity, double fadeLength, double sizeMin, double sizeMax ) : ListItem() {
  Create(density,jitter,limit,stagnancy,intensity,fadeLength,sizeMin,sizeMax);
 }
 void Create( int density, int jitter, int limit, double stagnancy, double intensity, double fadeLength, double sizeMin, double sizeMax ) {
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
   Flaked2d *f=new Flaked2d;
   f->gravity=&gravity;
   f->movement.x=movement.x;
   f->movement.y=movement.y;
   f->wind=&wind;
   f->offset=&offset;
   f->stagnancy=&stagnancy.value;
   f->bounds=&bounds;
   f->fadeLength=&fadeLength.value;
   f->flake=images.random();
   f->mass=double_range(0.1,2.0);
   f->color.fromCrayon(colors.random());
   f->sizeMin=sizeMin+double_range(0.0,sizeMin/2);
   f->sizeMax=sizeMax+double_range(0.0,sizeMax/2);
   f->position.x=double_range(bounds.x,bounds.x2);
   f->position.y=double_range(0,jitter)+bounds.y-f->flake->height;
   f->Init();   
   flakes.Append(f);
  }
 }
 void Init() {
  Emit();
 }
 void Between() {
  if ( flakes.count < limit && uniform() < intensity ) Emit();
  Flaked2d *t,*n;
  for ( t=(Flaked2d *)flakes.first; t; t=n ) {
   n=(Flaked2d *) t->next;
   if ( t->dead ) {
    flakes.Remove(t);
    delete t;
   } else t->Between();
  }
 }
 void Render(GLWindow *surface) {
  EACH(flakes.first,Flaked2d,f) f->Render(surface);
 }
 void MoveTo( int x, int y ) {
  offset.x=x;
  offset.y=y;
 }
};

class Snowf2d2 : public ListItem {
public:
 Cartesiand bounds;
 Cartesiand wind,gravity,movement,offset;
 Flakesd2d flakes;
 Images images;
 Crayons colors;
 Zint density,jitter,limit;
 Zdouble stagnancy, fadeLength,sizeMin,sizeMax,intensity;
 Snowf2d2() : ListItem() {
  density=jitter=limit=0;
  stagnancy=fadeLength=sizeMin=sizeMax=0.0;
  intensity=0.0;
 }
 Snowf2d2( int density, int jitter, int limit, double stagnancy, double intensity, double fadeLength, double sizeMin, double sizeMax ) : ListItem() {
  Create(density,jitter,limit,stagnancy,intensity,fadeLength,sizeMin,sizeMax);
 }
 void Create( int density, int jitter, int limit, double stagnancy, double intensity, double fadeLength, double sizeMin, double sizeMax ) {
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
   Flaked2d *f=new Flaked2d;
   f->gravity=&gravity;
   f->movement.x=movement.x;
   f->movement.y=movement.y;
   f->wind=&wind;
   f->offset=&offset;
   f->stagnancy=&stagnancy.value;
   f->bounds=&bounds;
   f->fadeLength=&fadeLength.value;
   f->flake=images.random();
   f->mass=double_range(0.1,2.0);
   f->color.fromCrayon(colors.random());
   f->sizeMin=sizeMin+double_range(0.0,sizeMin/2);
   f->sizeMax=sizeMax+double_range(0.0,sizeMax/2);
   f->position.x=double_range(bounds.x,bounds.x2);
   f->position.y=double_range(bounds.y,bounds.y2);
   f->Init();   
   flakes.Append(f);
  }
 }
 void Init() {
  Emit();
 }
 void Between() {
  if ( flakes.count < limit && uniform() < intensity ) Emit();
  EACHN(flakes.first,Flaked2d,t,{
   if ( t->dead ) {
    flakes.Remove(t);
    delete t;
   } else t->Between();
  });
 }
 void Render(GLWindow *surface) {
  EACH(flakes.first,Flaked2d,f) f->Render(surface);
 }
 void MoveBy( int dx, int dy ) {
  EACHN(flakes.first,Flaked2d,t,{
   t->position.x+=dx;
   if ( t->position.x > bounds.x2 ) t->position.x-=bounds.w;
   t->position.y+=dy;
   if ( t->position.y > bounds.y2 ) t->position.y-=bounds.h;
  });
 }
};

class Snowd2ds : public LinkedList {
public:
 Snowd2ds() : LinkedList() {
 }
 Snowd2d *Add( int density, int jitter, int limit, double stagnancy, double intensity, double fadeLength, double sizeMin, double sizeMax ) {
  Snowd2d *s;
  Append(s=new Snowd2d( density, jitter, limit, stagnancy, intensity, fadeLength, sizeMin, sizeMax ) );
  return s;
 }
 CLEARLISTRESET(Snowd2d);
 ~Snowd2ds() { Clear(); }
};

//CPUPARTICLESYSTEMLIST(Snowf2d,Flakef2d,flakes); 
