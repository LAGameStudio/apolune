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

#include "Crayon.h"
#include "Cartesian.h"
#include "GLWindow.h"
#include "GLImage.h"
#include "Art2d.h"
#include "Vertex.h"
#include "Particle2d.h"
#include "Emitter2d.h"

class Emberd2d : public Particlef2d {
public:
 Zp<GLImage> ember;
 Crayon spark,flame,ash,tint;
 Cartesiand position,initialVelocity,velocity,actual;
 Zdouble size, theta, age,lifespan,halflife, fade,mass,massGx,massGy;
 Zp<Cartesiand> gravity,offset;
 Emberd2d() : Particlef2d() { Constructor(); }
 void Constructor() {
  ember=null;
  age=0.0;
  gravity=null;
  offset=null;
  size=1.0;
  theta=0.0;
  lifespan=1.0;
  halflife=0.5;
  fade=0.1;
  mass=1.0;
  spark.Pick(alabaster);
  flame.Pick(rufous);
  ash.Pick(charcoal);
  massGx=0.0;
  massGy=1.0;
  dead=false;
 }
 
 void Init() {
  velocity.x=initialVelocity.x;
  velocity.y=initialVelocity.y;
  position.x=0;
  position.y=0;
  massGx=mass*gravity->x;
  massGy=mass*gravity->y;
  age=0.0;
  if ( fade < 1.0 ) fade=1.0;
 }

 void Between() {
  if ( dead ) return;
  age+=1.0f/FPS;
  if ( age >= lifespan ) dead=true;
  double decay=(1.0-age/lifespan);
  velocity.x+=massGx;
  velocity.y+=massGy;
  position.x+=velocity.x;
  position.y+=velocity.y;
//  theta += (velocity.fx * velocity.Magnitude()) / 10.0f;
  actual.w=(int)(size*(double)ember->width);
  actual.z=(int)(size*(double)ember->height);
  actual.x=(int)(position.x)-actual.w/2;
  actual.y=(int)(position.y)-actual.z/2;
  if ( age < halflife )
       tint.ColorInterp(spark,flame,1.0f-(decay-0.5f*2.0f));
  else tint.ColorInterp(flame,ash,1.0f-decay*2.0f);  
  if ( decay < 1.0f/fade ) tint.ScaleByAlphaf(decay*fade);
  size*=2.0f*(decay/2);
 }

 virtual void Render(GLWindow *surface) {
  // Disable depth test?
  art.Stretch(ember,tint,additive,offset->x+actual.x,offset->y+actual.y,actual.w,actual.z);
 }

 Emberd2d *Duplicate() {
  Emberd2d *n=new Emberd2d;
  n->ember=ember;
  n->spark.fromCrayon(spark);
  n->flame.fromCrayon(flame);
  n->ash.fromCrayon(ash);
  n->tint.fromCrayon(tint);
  n->position=position;
  n->initialVelocity=initialVelocity;
  n->velocity=velocity;
  n->offset=offset;
  n->actual=actual;
  n->size=size;
  n->theta=theta;
  n->age=age;
  n->lifespan=lifespan;
  n->halflife=halflife;
  n->fade=fade;
  n->mass=mass;
  n->massGx=massGy;
  n->gravity=gravity;
  return n;
 }

};

PARTICLECOLLECTION(Emberd2ds,Emberd2d);

class Sparkd2d : public Emitter2d {
public:
 Emberd2ds embers;
 Cartesiand gravity,offset;

 void Create_MonocolorImage(
  int countLow, int countHigh,
  double massLow, double massHigh,
  double sizeMin, double sizeMax,
  double lifespanMin, double lifespanMax,
  double fadeMin, double fadeMax,
  double maxVelocity,
  int positionJitter,
  int cx, int cy,
  Crayon spark,
  Crayon flame,
  Crayon ash,
  GLImage *ember
 ) {
  int count=number_range(countLow,countHigh);
  int i;
  for ( i=0; i< count; i++ ) {
   Emberd2d *e=new Emberd2d;
   e->mass=double_range( massLow, massHigh );
   e->size=double_range( sizeMin, sizeMax );
   e->lifespan=double_range( lifespanMin, lifespanMax );
   e->halflife=e->lifespan/2.0;
   e->fade=double_range( fadeMin, fadeMax );
   e->spark.fromCrayon(spark);
   e->flame.fromCrayon(flame);
   e->ash.fromCrayon(ash);
   e->gravity=&gravity;
   e->ember=ember;
   e->initialVelocity.x=double_range(0.0,maxVelocity)-(maxVelocity/2);
   e->initialVelocity.y=double_range(0.0,maxVelocity)-(maxVelocity/2);
   e->offset=&offset;
   offset.x=cx;
   offset.y=cy;
   embers.Append(e);
  }
 }

 void Create_MultiColorImage(
  int countLow, int countHigh,
  double massLow, double massHigh,
  double sizeMin, double sizeMax,
  double lifespanMin, double lifespanMax,
  double fadeMin, double fadeMax,
  double maxVelocity,
  int positionJitter,
  int cx, int cy,
  Crayons *sparks,
  Crayons *flames,
  Crayons *ashes,
  GLImage *ember
 ) {
  int count=number_range(countLow,countHigh);
  int i;
  for ( i=0; i< count; i++ ) {
   Emberd2d *e=new Emberd2d;
   e->mass=double_range( massLow, massHigh );
   e->size=double_range( sizeMin, sizeMax );
   e->lifespan=double_range( lifespanMin, lifespanMax );
   e->halflife=e->lifespan/2.0;
   e->fade=double_range( fadeMin, fadeMax );
   e->spark.fromCrayon(sparks->random());
   e->flame.fromCrayon(flames->random());
   e->ash.fromCrayon(ashes->random());
   e->gravity=&gravity;
   e->ember=ember;
   e->initialVelocity.x=double_range(0.0,maxVelocity)-(maxVelocity/2);
   e->initialVelocity.y=double_range(0.0,maxVelocity)-(maxVelocity/2);   
   e->offset=&offset;
   offset.x=cx;
   offset.y=cy;
   embers.Append(e);
  }
 }

 void Create_MonocolorImages(
  int countLow, int countHigh,
  double massLow, double massHigh,
  double sizeMin, double sizeMax,
  double lifespanMin, double lifespanMax,
  double fadeMin, double fadeMax,
  double maxVelocity,
  int positionJitter,
  int cx, int cy,
  Crayon spark,
  Crayon flame,
  Crayon ash,
  Images *images
 ) {
  int count=number_range(countLow,countHigh);
  int i;
  for ( i=0; i< count; i++ ) {
   Emberd2d *e=new Emberd2d;
   e->mass=double_range( massLow, massHigh );
   e->size=double_range( sizeMin, sizeMax );
   e->lifespan=double_range( lifespanMin, lifespanMax );
   e->halflife=e->lifespan/2.0;
   e->fade=double_range( fadeMin, fadeMax );
   e->spark.fromCrayon(spark);
   e->flame.fromCrayon(flame);
   e->ash.fromCrayon(ash);
   e->gravity=&gravity;
   e->ember=images->random();
   e->initialVelocity.x=double_range(0.0,maxVelocity)-(maxVelocity/2);
   e->initialVelocity.y=double_range(0.0,maxVelocity)-(maxVelocity/2);
   e->offset=&offset;
   offset.x=cx;
   offset.y=cy;
   embers.Append(e);
  }
 }

 void Create_ManyColorImages(
  int countLow, int countHigh,
  double massLow, double massHigh,
  double sizeMin, double sizeMax,
  double lifespanMin, double lifespanMax,
  double fadeMin, double fadeMax,
  double maxVelocity,
  int positionJitter,
  int cx, int cy,
  Crayons *sparks,
  Crayons *flames,
  Crayons *ashes,
  Images *images
 ) {
  int count=number_range(countLow,countHigh);
  int i;
  for ( i=0; i< count; i++ ) {
   Emberd2d *e=new Emberd2d;
   e->mass=double_range( massLow, massHigh );
   e->size=double_range( sizeMin, sizeMax );
   e->lifespan=double_range( lifespanMin, lifespanMax );
   e->halflife=e->lifespan/2.0;
   e->fade=double_range( fadeMin, fadeMax );
   e->spark.fromCrayon(sparks->random());
   e->flame.fromCrayon(flames->random());
   e->ash.fromCrayon(ashes->random());
   e->gravity=&gravity;
   e->ember=images->random();
   e->initialVelocity.x=double_range(0.0,maxVelocity)-(maxVelocity/2);
   e->initialVelocity.y=double_range(0.0,maxVelocity)-(maxVelocity/2);
   e->offset=&offset;
   offset.x=cx;
   offset.y=cy;
   embers.Append(e);
  }
 }

 void MoveTo( int x, int y ) { offset.x=x;  offset.y=y;  }
 void MoveBy( int x, int y ) { offset.x+=x; offset.y+=y; }

 void Init() { EACH(embers.first,Emberd2d,f) f->Init(); }

 void Between() {
  Emberd2d *t,*n;
  for ( t=(Emberd2d *)embers.first; t; t=n ) {
   n=(Emberd2d *) t->next;
   if ( t->dead ) {
    embers.Remove(t);
    delete t;
   } else t->Between();
  }
 }
 void Render( GLWindow *surface ) {
  EACH(embers.first,Emberd2d,f) f->Render(surface);
 } 

};

DYINGCPUPARTICLESYSTEMLIST(Sparksd2d,Sparkd2d,embers); 

class PrecomputedSparkd2d : public ListItem {
public: PRECOMPUTEDPARTICLESYSTEMMOVIE(Emberd2dsHandle,Emberd2dsCollection,Emberd2ds,Emberd2d,spark,spark.embers);
 Zdouble massLow, massHigh, sizeMin, sizeMax, lifespanMin, lifespanMax, fadeMin, fadeMax, maxVelocity;
 Zint countLow, countHigh, positionJitter;
 Crayons sparks;
 Crayons flames;
 Crayons ashes;
 Images images;
 Sparkd2d spark;
 void Set( double massLow, double massHigh, double sizeMin, double sizeMax, double lifespanMin, double lifespanMax,
           double fadeMin, double fadeMax, double maxVelocity, int countLow, int countHigh, int positionJitter ) {
  this->massLow=massLow;
  this->massHigh=massHigh;
  this->sizeMin=sizeMin;
  this->sizeMax=sizeMax;
  this->lifespanMin=lifespanMin;
  this->lifespanMax=lifespanMax;
  this->fadeMin=fadeMin;
  this->fadeMax=fadeMax;
  this->maxVelocity=maxVelocity;
  this->countLow=countLow;
  this->countHigh=countHigh;
  this->positionJitter=positionJitter;
 }
 void Precompute( ) {
  frames=(int)(lifespanMax*FPS)+1;
  spark.Create_ManyColorImages(
    countLow, countHigh, 
    massLow, massHigh, 
    sizeMin, sizeMax, 
    lifespanMin, lifespanMax, 
    fadeMin, fadeMax, maxVelocity, positionJitter, -1000, -1000,
    &sparks, &flames, &ashes, &images );
  spark.Init();
  Prerender();
 }
};

PRECOMPUTEDPARTICLESYSTEMCOLLECTION(PrecomputedSparksd2d,PrecomputedSparkd2d);