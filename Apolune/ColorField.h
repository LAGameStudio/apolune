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
#include "Crayon.h"
#include "Trigger.h"

class ColorFieldPoint : public ListItem {
public:
 Crayon color;
 Zdouble x,y;
 Zdouble mixRate;
 Zdouble dist;
};

class ColorField : public LinkedList {
public:
 Zbool wrap,useDiagonal;
 Box box;
 Zdouble greatestD,diagonalD;
 CLEARLISTRESET(ColorFieldPoint);
 ColorField() : LinkedList() {
  wrap=false;
  useDiagonal=false;
 }
 Zdouble testX,testY;
 void DistanceSort( double x, double y ) {
  testX=x;
  testY=y;
  SortDistance();
 }
 SORTING(ColorFieldPoint,SortDistance,{
  FOREACH(ColorFieldPoint,cfp) cfp->dist=ddistance(testX,testY,cfp->x,cfp->y);
 },{ if ( A->dist < B->dist ) result=-1; },{});
 void Add( Crayon c, double x, double y ) {
  ColorFieldPoint *cfp=new ColorFieldPoint;
  cfp->x=x;
  cfp->y=y;
  cfp->color.fromCrayon(c);
  Append(cfp);
  UpdateBox();
 }
 void UpdateBox() {
  Zdouble minx=10000000.0,maxx,miny=10000000.0,maxy;
  Zdouble d;
  FOREACH(ColorFieldPoint,p) {
   FOREACH(ColorFieldPoint,q) {
    double dist=ddistance(p->x,p->y,q->x,q->y);
    if ( dist > d ) d=dist;
   }
   if ( p->x < minx ) minx=p->x;
   if ( p->y < miny ) miny=p->y;
   if ( p->x > maxx ) maxx=p->x;
   if ( p->y > maxy ) maxy=p->y;
  }
  greatestD=d;
  box.Set(minx,miny,maxx,maxy);
  diagonalD=ddistance(minx,miny,maxx,maxy);
 }
 Crayon RequestSortedDistance( double x, double y, bool colorAmp ) {
  if ( count == 0 ) return crayons.Pick(black);
  if ( count == 1 ) return ((ColorFieldPoint *) first)->color;
  DistanceSort(x,y);
  double F=((ColorFieldPoint *) last)->dist;
  double N=((ColorFieldPoint *) first)->dist;
  Crayon mixed;
  FOREACH(ColorFieldPoint,p) {
   float mix=(float)(p->dist/F);
   mixed.rf+=p->color.rf*mix;
   mixed.gf+=p->color.gf*mix;
   mixed.bf+=p->color.bf*mix;
   mixed.af+=p->color.af*mix;
  }
  mixed.rf/=(float)count;
  mixed.gf/=(float)count;
  mixed.bf/=(float)count;
  mixed.af/=(float)count;
  if ( colorAmp ) {
   if ( mixed.rf > mixed.gf && mixed.rf > mixed.bf ) {
    float diff=1.0f-mixed.rf;
    mixed.gf*=1.0f+diff;
    mixed.bf*=1.0f+diff;
    mixed.rf=1.0f;
   } else if ( mixed.gf > mixed.rf && mixed.gf > mixed.bf ) {
    float diff=1.0f-mixed.gf;
    mixed.rf*=1.0f+diff;
    mixed.bf*=1.0f+diff;
    mixed.gf=1.0f;
   } else {
    float diff=1.0f-mixed.bf;
    mixed.rf*=1.0f+diff;
    mixed.gf*=1.0f+diff;
    mixed.bf=1.0f;
   }
  }
  return mixed;
 }
 Crayon Request( double x, double y ) {
  if ( count == 0 ) return crayons.Pick(clear);
  if ( count == 1 ) return ((ColorFieldPoint *)first)->color;
  if ( wrap ) {
   if ( x < box.x ) {
    while ( x < box.x ) x+=box.w;
   } else if ( x > box.x2 ) {
    while ( x > box.x2 ) x-=box.w;
   }
   if ( y < box.y ) {
    while ( y < box.y ) y+=box.h;
   } else if ( y > box.y2 ) {
    while ( y > box.y2 ) y-=box.h;
   }
  }
  Zdouble maxMix;
  if ( useDiagonal ) {
   FOREACH(ColorFieldPoint,p) {
    double tempD=ddistance(x,y,p->x,p->y);
    p->mixRate=UMAX(0.0,1.0-(tempD/diagonalD));
    if ( p->mixRate > maxMix ) maxMix=p->mixRate;
   }
  } else {
   FOREACH(ColorFieldPoint,p) {
    double tempD=ddistance(x,y,p->x,p->y);
    p->mixRate=UMAX(0.0,1.0-(tempD/greatestD));
    if ( p->mixRate > maxMix ) maxMix=p->mixRate;
   }
  }
  Crayon mixer(black);
  FOREACH(ColorFieldPoint,p) {
   p->mixRate=p->mixRate/maxMix;
   mixer.rf+=p->color.rf*(float)p->mixRate;
   mixer.gf+=p->color.gf*(float)p->mixRate;
   mixer.bf+=p->color.bf*(float)p->mixRate;
   mixer.af+=p->color.af*(float)p->mixRate;
  }
  mixer.rf/=(float)(count);
  mixer.gf/=(float)(count);
  mixer.bf/=(float)(count);
  mixer.af/=(float)(count);
  mixer.Float(mixer.rf,mixer.gf,mixer.bf,mixer.af);
  return mixer;
 }
 Crayon InterpNearestFarthest( double x, double y ) {
  Crayon cnear=nearest(x,y);
  Crayon cfar=farthest(x,y);
 }
 Crayon nearest( double x, double y ) {
  if ( count == 0 ) return crayons.Pick(clear);
  if ( count == 1 ) return ((ColorFieldPoint *)first)->color;
  ColorFieldPoint *candidate=null;
  Zdouble dist;
  FOREACH(ColorFieldPoint,p) {
   if ( !candidate ) {
    dist=ddistance(x,y,p->x,p->y);
    candidate=p;
   } else {
    Zdouble td=ddistance(x,y,p->x,p->y);
    if ( td < dist ) {
     dist=td;
     candidate=p;
    }
   }
  }
  return candidate->color;
 }
 Crayon farthest( double x, double y ) {
  if ( count == 0 ) return crayons.Pick(clear);
  ColorFieldPoint *candidate=null;
  Zdouble dist;
  FOREACH(ColorFieldPoint,p) {
   if ( !candidate ) {
    dist=ddistance(x,y,p->x,p->y);
    candidate=p;
   } else {
    Zdouble td=ddistance(x,y,p->x,p->y);
    if ( td > dist ) {
     dist=td;
     candidate=p;
    }
   }
  }
  return candidate->color;
 }
};