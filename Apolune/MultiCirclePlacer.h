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

#include "Trigger.h"
#include "ScreenPosition.h"
#include "VBO.h"

class MultiCirclePlace : public ListItem {
public:
 Zp<GLImage> image;
 Zdouble rads,degs;
 Circle circle;
 ScreenPosition screen;
 Crayon color;
 MultiCirclePlace() : ListItem() {
  color.Pick(alabaster);
  circle.precision=8;
 }
 bool within( double x, double y ) {
  return circle.within(x,y);
 }
 void Set( double x, double y, double R ) {
  circle.Set(x,y,R);
  screen.Set(x-R,y-R,R*2.0,R*2.0);
 }
 void Debug( double ofsx, double ofsy ) {
  circle.Debug2d(color,ofsx,ofsy);
 }
};

class MultiCirclePlaces : public LinkedList {
public:
 CLEARLISTRESET(MultiCirclePlace);
 MultiCirclePlace *Add( double radius, double x, double y, Crayon tint, GLImage *image ) {
  MultiCirclePlace *placement=new MultiCirclePlace;
  placement->Set(x,y,radius);
  placement->color.fromCrayon(tint);
  placement->image=image;
  Append(placement);
  return placement;
 }
 void Debug( double ofsx, double ofsy ) {
  FOREACH(MultiCirclePlace,p) p->Debug(ofsx,ofsy);
 }
};

class MultiCircleTier : public ListItem {
public:
 MultiCirclePlaces places;
 Crayon tint;
 Zint count;
 Zdouble scale;
 Zdouble bandLow,bandHigh,maxOverlap;
 Zp<GLImage> image;
 MultiCircleTier() {
  count=3;
  scale=1.0;
  bandLow=0.8;
  bandHigh=1.2;
  maxOverlap=0.2;
  tint.Pick(alabaster);
 }
 void Debug( double ofsx, double ofsy ) {
  places.Debug(ofsx,ofsy);
 }
};

class MultiCircleTiers : public LinkedList {
public:
 CLEARLISTRESET(MultiCirclePlaces);
 MultiCircleTier *Add( int count, double scale, double bandLow, double bandHigh, double maxOverlap ) {
  MultiCircleTier *m=new MultiCircleTier;
  m->count=count;
  m->scale=scale;
  m->bandLow=bandLow;
  m->bandHigh=bandHigh;
  Append(m);
  return m;
 }
 void ClearPlacements() {
  FOREACH(MultiCircleTier,t) t->places.Clear();
 }
 void Debug ( double ofsx, double ofsy ) {
  FOREACH(MultiCircleTier,t) t->Debug(ofsx,ofsy);
 }
};

class MultiCirclePlacer {
public:
 Zint maxAttempts;
 MultiCircleTiers tiers;
 void Clear() {
  tiers.Clear();
 }
 void ClearPlacements() {
  tiers.ClearPlacements();
 }
 MultiCirclePlacer() {
  maxAttempts=10;
 }
 void Place( double startRadius ) {
  MultiCirclePlace *active=null;
  MultiCircleTier *previous=null;
  EACH(tiers.first,MultiCircleTier,m) {
   if ( !m->prev ) { // the first tier
    for ( int i=0; i<m->count; i++ ) {
     if ( i==0 ) { // the first placement, place at 0,0
      active=m->places.Add(startRadius*m->scale,0.0,0.0,m->tint,m->image);
     } else {
      Circle c(active->circle.x,active->circle.y,
       double_range(startRadius*m->bandLow,startRadius*m->bandHigh)*m->scale);
      Vertexd center;
      bool found=true;
      int attempts=0;
      do {
       attempts++;
       c.RandomPointOn(&center);
       found=false;
       EACH(m->places.first,MultiCirclePlace,k) if ( k!=active && k->within(center.x,center.y) ) found=true;
      } while ( found && attempts < maxAttempts );
      if ( !found ) m->places.Add(startRadius*m->scale,center.x,center.y,m->tint,m->image);
     }
    }
   } else {
    previous=(MultiCircleTier *)m->prev;
    EACH(previous->places.first,MultiCirclePlace,p) {
     for ( int i=0; i<m->count; i++ ) {
      Circle c(p->circle.x,p->circle.y,double_range(startRadius*m->bandLow,startRadius*m->bandHigh)*previous->scale);
      Vertexd center;
      bool found=true;
      int attempts=0;
      do {
       attempts++;
       c.RandomPointOn(&center);
       found=false;
       EACH(tiers.first,MultiCircleTier,t)
        EACH(t->places.first,MultiCirclePlace,k)
         if ( k->within(center.x,center.y) ) found=true;
      } while ( found && attempts < maxAttempts );
      if ( !found ) m->places.Add(startRadius*m->scale,center.x,center.y,m->tint,m->image);
     }
    }
   }
  }
 }
 void Place( double startRadius, int seed ) {
  Random *set=randoms.GetSet(seed);
  double radius=startRadius;
  MultiCirclePlace *active=null;
  MultiCircleTier *previous=null;
  int iteration=seed;
  EACH(tiers.first,MultiCircleTier,m) {
   iteration++;
   if ( !m->prev ) { // the first tier
    for ( int i=0; i<m->count; i++ ) {
     iteration++;
     if ( i==0 ) { // the first placement, place at 0,0
      active=m->places.Add(radius,0.0,0.0,m->tint,m->image);
     } else {
      double R=active->circle.R*m->bandLow
              +set->auniqued(seed+iteration)
              *(startRadius*m->bandHigh-startRadius*m->bandLow);
      Circle c(active->circle.x,active->circle.y,R*m->scale);
      Vertexd center;
      bool found=true;
      int attempts=0;
      do {
       attempts++;
       c.RandomPointOn(&center,seed+attempts+iteration);
       found=false;
       EACH(m->places.first,MultiCirclePlace,k) if ( k!=active && k->within(center.x,center.y) ) found=true;
      } while ( found && attempts < maxAttempts );
      if ( !found ) m->places.Add(startRadius*m->scale,center.x,center.y,m->tint,m->image);
     }
    }
   } else {
    previous=(MultiCircleTier *)m->prev;
    EACH(previous->places.first,MultiCirclePlace,p) {
     iteration++;
     for ( int i=0; i<m->count; i++ ) {
      iteration++;
      double R=m->bandLow
              +set->auniqued(seed+iteration)
              *(m->bandHigh-m->bandLow);
      Circle c(p->circle.x,p->circle.y,R*previous->scale);
      Vertexd center;
      bool found=true;
      int attempts=0;
      do {
       attempts++;
       c.RandomPointOn(&center,seed+attempts+iteration);
       found=false;
       EACH(tiers.first,MultiCircleTier,t)
        EACH(t->places.first,MultiCirclePlace,k)
         if ( k->within(center.x,center.y) ) found=true;
      } while ( found && attempts < maxAttempts );
      if ( !found ) m->places.Add(startRadius*m->scale,center.x,center.y,m->tint,m->image);
     }
    }
   }
  }
 }
 VBOStaticVNTC *Upload() {
  VBOStaticVNTC *vbo=new VBOStaticVNTC;
  return vbo;
 }
};

