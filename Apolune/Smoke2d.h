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
#include "Vertex.h"
#include "Art2d.h"
#include "Particle2d.h"
#include "Emitter2d.h"

class Puffd2d : public Particlef2d {
public:
 Zp<GLImage> gas;
 Crayon emission,vapor,tint;
 Cartesiand position,initialVelocity,velocity,actual;
 Zdouble sizeMin,sizeMax,sizeMaxActual,opacity,theta, age, lifespan,halflife, fade,mass,massGx,massGy;
 Zp<Cartesiand> gravity, wind, offset;
 Puffd2d() : Particlef2d() { Constructor(); }
 void Constructor() {
  sizeMin=0.0;
  sizeMax=1.0;
  theta=0.0;
  lifespan=1.0;
  halflife=0.5;
  fade=0.1;
  mass=1.0;
  emission.Pick(alabaster);
  vapor.Pick(gray);
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
  sizeMaxActual=sizeMax-sizeMin;
 }

 void Between() {
  if ( dead ) return;
  age+=1.0/FPS;
  if ( age >= lifespan ) dead=true;
  double complete,decay=(1.0-(complete=(age/lifespan)));
  double actualsize=complete*sizeMaxActual+sizeMin;
  velocity.x+=massGx;
  velocity.y+=massGy;
  position.x+=velocity.x+wind->x;
  position.y+=velocity.y+wind->y;
//  theta += (velocity.fx * velocity.Magnitude()) / 10.0f;
  actual.w=(int)(actualsize*(float)gas->width);
  actual.z=(int)(actualsize*(float)gas->height);
  actual.x=(int)(position.x)-actual.w/2;
  actual.y=(int)(position.y)-actual.z/2;
  tint.ColorInterp(emission,vapor,decay);
  tint.ScaleByAlphaf(opacity);
  if ( complete < 1.0/fade ) tint.ScaleByAlphaf(complete*fade);
  if ( complete > 1.0-(1.0/fade) ) tint.ScaleByAlphaf((1.0-complete)*fade);
 }

 void Render(GLWindow *surface) {
  art.Stretch(gas,tint,additive,offset->x+actual.x,offset->y+actual.y,actual.w,actual.z);
 }

 Puffd2d *Duplicate() {
  Puffd2d *n=new Puffd2d;
  n->gas=this->gas;
  n->emission.fromCrayon(emission);
  n->vapor.fromCrayon(vapor);
  n->tint.fromCrayon(tint);
  n->position=this->position;
  n->initialVelocity=this->initialVelocity;
  n->velocity=this->velocity;
  n->offset=offset;
  n->actual=actual;
  n->sizeMaxActual=sizeMaxActual;
  n->theta=theta;
  n->age=age;
  n->lifespan=lifespan;
  n->halflife=halflife;
  n->fade=fade;
  n->mass=mass;
  n->massGx=massGy;
  n->gravity=gravity;
  n->wind=wind;
  return n;
 }
};

PARTICLECOLLECTION(Puffsd2d,Puffd2d);

class Smoked2d : public Emitter2d {
public:
 Smoked2d() : Emitter2d() {
  billowing=true;
 }
 Cartesiand offset;
 Puffsd2d puffs;
 Cartesiand gravity,wind;
 Zdouble billowChance;
 Zint thickness, limit;
 Zdouble massLow,massHigh,sizeMin,sizeMax,lifespanMin,lifespanMax,fadeMin,fadeMax,maxVelocity,opacity;
 Zint positionJitter;
 Images gas;
 Crayons emissions,vapors;
 void Set( 
   Cartesiand gravity, Cartesiand wind,
   double billowChance, int thickness, int limit,
   double massLow, double massHigh,
   double sizeMin, double sizeMax,
   double lifespanMin, double lifespanMax,
   double fadeMin, double fadeMax,
   double maxVelocity, double opacity,
   int positionJitter,
   Crayon emission, Crayon vapor,
   GLImage *gas,
   int cx, int cy ) {
    this->gravity.x=gravity.x;
    this->gravity.y=gravity.y;
    this->wind.x=wind.x;
    this->wind.y=wind.y;
    this->billowChance=billowChance;
    this->thickness=thickness;
    this->limit=limit;
    this->massLow=massLow;
    this->massHigh=massHigh;
    this->sizeMin=sizeMin;
    this->sizeMax=sizeMax;
    this->lifespanMin=lifespanMin;
    this->lifespanMax=lifespanMax;
    this->fadeMin=fadeMin;
    this->fadeMax=fadeMax;
    this->maxVelocity=maxVelocity;
    this->positionJitter=positionJitter;
    this->opacity=opacity;
    offset.x=cx;
    offset.y=cy;
    this->emissions.Hold(emission);
    this->vapors.Hold(vapor);
    this->gas.addIfUnique(new ImageHandle(gas));
 }
 void Set( 
   Cartesiand gravity, Cartesiand wind,
   double billowChance, int thickness, int limit,
   double massLow, double massHigh,
   double sizeMin, double sizeMax,
   double lifespanMin, double lifespanMax,
   double fadeMin, double fadeMax,
   double maxVelocity, double opacity,
   int positionJitter,
   Crayon emission, Crayon vapor,
   Images *gasses,
   int cx, int cy ) {
    this->gravity.x=gravity.x;
    this->gravity.y=gravity.y;
    this->wind.x=wind.x;
    this->wind.y=wind.y;
    this->billowChance=billowChance;
    this->thickness=thickness;
    this->limit=limit;
    this->massLow=massLow;
    this->massHigh=massHigh;
    this->sizeMin=sizeMin;
    this->sizeMax=sizeMax;
    this->lifespanMin=lifespanMin;
    this->lifespanMax=lifespanMax;
    this->fadeMin=fadeMin;
    this->fadeMax=fadeMax;
    this->maxVelocity=maxVelocity;
    this->positionJitter=positionJitter;
    this->opacity=opacity;
    offset.x=cx;
    offset.y=cy;
    this->emissions.Hold(emission);
    this->vapors.Hold(vapor);
    EACH(gasses->first,ImageHandle,ih) this->gas.addIfUnique(ih->i);
 }
 void Set( 
   Cartesiand gravity, Cartesiand wind,
   double billowChance, int thickness, int limit,
   double massLow, double massHigh,
   double sizeMin, double sizeMax,
   double lifespanMin, double lifespanMax,
   double fadeMin, double fadeMax,
   double maxVelocity, double opacity,
   int positionJitter,
   Crayons *emissions, Crayons *vapors,
   Images *gasses,
   int cx, int cy ) {
    this->gravity.x=gravity.x;
    this->gravity.y=gravity.y;
    this->wind.x=wind.x;
    this->wind.y=wind.y;
    this->billowChance=billowChance;
    this->thickness=thickness;
    this->limit=limit;
    this->massLow=massLow;
    this->massHigh=massHigh;
    this->sizeMin=sizeMin;
    this->sizeMax=sizeMax;
    this->lifespanMin=lifespanMin;
    this->lifespanMax=lifespanMax;
    this->fadeMin=fadeMin;
    this->fadeMax=fadeMax;
    this->maxVelocity=maxVelocity;
    this->positionJitter=positionJitter;
    this->opacity=opacity;
    offset.x=cx;
    offset.y=cy;
    this->emissions.Hold(emissions);
    this->vapors.Hold(vapors);
    EACH(gasses->first,ImageHandle,ih) this->gas.addIfUnique(ih->i);
 }

 void Set( 
   Cartesiand gravity, Cartesiand wind,
   double billowChance, int thickness, int limit,
   double massLow, double massHigh,
   double sizeMin, double sizeMax,
   double lifespanMin, double lifespanMax,
   double fadeMin, double fadeMax,
   double maxVelocity, double opacity,
   int positionJitter,
   Crayons *emissions, Crayons *vapors,
   GLImage *gas,
   int cx, int cy ) {
    this->gravity.x=gravity.x;
    this->gravity.y=gravity.y;
    this->wind.x=wind.x;
    this->wind.y=wind.y;
    this->billowChance=billowChance;
    this->thickness=thickness;
    this->limit=limit;
    this->massLow=massLow;
    this->massHigh=massHigh;
    this->sizeMin=sizeMin;
    this->sizeMax=sizeMax;
    this->lifespanMin=lifespanMin;
    this->lifespanMax=lifespanMax;
    this->fadeMin=fadeMin;
    this->fadeMax=fadeMax;
    this->maxVelocity=maxVelocity;
    this->positionJitter=positionJitter;
    this->opacity=opacity;
    offset.x=cx;
    offset.y=cy;
    this->emissions.Hold(emissions);
    this->vapors.Hold(vapors);
    this->gas.addIfUnique(new ImageHandle(gas));
 }
 void Create() {
  int count=number_range(0,thickness);
  int i;
  for ( i=0; i< count; i++ ) {
   Puffd2d *p=new Puffd2d;
   p->mass=double_range( massLow, massHigh );
   p->sizeMin=sizeMin;
   p->sizeMax=sizeMax;
   p->lifespan=double_range( lifespanMin, lifespanMax );
   p->halflife=p->lifespan/2.0f;
   p->fade=double_range( fadeMin, fadeMax );
   p->emission.fromCrayon(emissions.random());
   p->vapor.fromCrayon(vapors.random());
   p->gravity=&gravity;
   p->wind=&wind;
   p->gas=gas.random();
   p->initialVelocity.x=double_range(0.0f,maxVelocity)-(maxVelocity/2);
   p->initialVelocity.y=double_range(0.0f,maxVelocity)-(maxVelocity/2);
   p->opacity=opacity;
   p->offset=&offset;
   p->Init();
   puffs.Append(p);
  }
 }

 void MoveTo( int x, int y ) { offset.x=x;  offset.y=y;  }
 void MoveBy( int x, int y ) { offset.x+=x; offset.y+=y; }
 void MoveBy( double x, double y ) { offset.x+=x; offset.y+=y; }

 void Init() { EACH(puffs.first,Puffd2d,f) f->Init(); }

 Zbool billowing;
 void Between() {
  Puffd2d *t,*n;
  if ( billowing && puffs.count < limit && (/*puffs.count==0 ||*/ uniform() <= billowChance)  ) {
   Create();
  }
  for ( t=(Puffd2d *)puffs.first; t; t=n ) {
   n=(Puffd2d *) t->next;
   if ( t->dead ) {
    puffs.Remove(t);
    delete t;
   } else t->Between();
  }
 }
 void Render( GLWindow *surface=null ) {
  EACH(puffs.first,Puffd2d,f) f->Render(surface);
 } 

};

CPUPARTICLESYSTEMLIST(Smokesd2d,Smoked2d,puffs); 

class PrecomputedSmoked2d : public ListItem {
public: PRECOMPUTEDPARTICLESYSTEMMOVIE(Puffsd2dsHandle,Puffsd2dsCollection,Puffsd2d,Puffd2d,smoke,smoke.puffs);
 Crayons emissions;
 Crayons vapors;
 Cartesiand gravity,wind;
 Images images;
 Smoked2d smoke;
 Zdouble opacity;
 Zdouble massLow, massHigh, sizeMin, sizeMax, lifespanMin, lifespanMax,
           fadeMin, fadeMax, maxVelocity;
 int thickness, limit, positionJitter;
 void Set( double massLow, double massHigh, double sizeMin, double sizeMax, double lifespanMin, double lifespanMax,
           double fadeMin, double fadeMax, double maxVelocity, int thickness, int limit, int positionJitter ) {
  this->massLow=massLow;
  this->massHigh=massHigh;
  this->sizeMin=sizeMin;
  this->sizeMax=sizeMax;
  this->lifespanMin=lifespanMin;
  this->lifespanMax=lifespanMax;
  this->fadeMin=fadeMin;
  this->fadeMax=fadeMax;
  this->maxVelocity=maxVelocity;
  this->thickness=thickness;
  this->limit=limit;
  this->positionJitter=positionJitter;
 }
 void Precompute( ) {
  frames=(int)(lifespanMax*FPS)+1;
  smoke.Set( gravity, wind, 0.0, 
   (int)thickness, (int)limit, (double)massLow, 
   (double)massHigh, (double)sizeMin, (double)sizeMax, 
   (double)lifespanMin, (double)lifespanMax, (double)fadeMin, (double)fadeMax,
   (double)maxVelocity, (double)opacity, (int)positionJitter, &emissions, &vapors, &images, -1000, -1000
   );
  smoke.Create();
  smoke.Init();
  Prerender();
 }
};

PRECOMPUTEDPARTICLESYSTEMCOLLECTION(PrecomputedSmokesd2d,PrecomputedSmoked2d);

//----------------------------------------------------------------------------------------------------------

#include "Proce55or.h"
#include "Delay.h"

class Smoke2d : public Proce55or {
public:
 Smoked2d vapor;
 Cartesiand gravity;
 Cartesiand wind;
 Cartesiand drift;
 Delay billow;
 Smoke2d() : Proce55or() {
  billow.Set(1.0);
  billow.Reset();
  drift.Set(1.0,-1.0);
  wind.x=(float)drift.x;
  wind.y=(float)drift.y;
  vapor.Set( gravity, wind, 
   0.95, 1, 75, 0.1, 10.0, 0.25, 0.5, 
   1.0, 5.0, 
   0.5, 2.0, 1.0, 0.25, 
   15,
   crayons.Pick(green), crayons.Pick(charcoal),
   library.Load("data/images/flares/smoke2.png"),
   x,y );
 }
 void MovedBy( double x, double y ) { vapor.MoveBy(x,y); }
 void OnSetExtents() {
  vapor.offset.x=x;
  vapor.offset.y=y;
 }
 void Between() {
  billow.Between();
  if ( !billow ) vapor.billowing=false;
  vapor.Between();
 }
 void Render() {
  vapor.Render();
  if ( !billow && vapor.puffs.count == 0 ) {
   this->dropMe=true;
  }
 }
};