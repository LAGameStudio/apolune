
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include <iostream>
#include <string>
using namespace std;

#include <Windows.h>
#include <math.h>

#include "GLWindow.h"
#include "GLImage.h"

#include "ListItem.h"
#include "LinkedList.h"

#include "Vertex.h"
#include "Quaternion.h"
#include "Matrix16.h"
#include "Polygon.h"


mPolygon *mPolygon::Copy(void) {
 mPolygon *q = new mPolygon;
 EACH(points.first,Vertex,v) q->points.Append(v->Copy());
 q->material=material;
 q->area=area;
 q->group=group;
 if ( bary )      q->bary     = bary->Copy();
 if ( rotation)   q->rotation = rotation->Copy();
 if ( normal)     q->normal   = normal->Copy();
 return q;
}



mPolygon *mPolygon::Copy( int mangle, float dx, float dy ) {
 Random *set=randoms.GetSet(mangle);
 mPolygon *q = new mPolygon;
 Vertex *v;
 int i=0;
 for ( v=(Vertex *) (points.first); v; v=(Vertex *) (v->next) ) {
  Vertex *c=v->Copy();
  c->x+=set->unique(mangle+i,set->arnd(i),mangle+set->arnd(mangle+i))*dx;
  c->y+=set->unique(i,set->arnd(i),set->arnd(mangle+i))*dy;
  q->points.Append(c);
  i++;
 }
 q->material=material;
 q->area=area;
 q->group=group;
 if ( bary )      q->bary     = bary->Copy();
 if ( rotation)   q->rotation = rotation->Copy();
 if ( normal)     q->normal   = normal->Copy();
 return q;
}

void mPolygon::remVertex( Vertex *v ) { 
// Vertex *prev;
 if ( !v ) return;
 points.Remove(v);
 delete v;
}

void mPolygon::addVertex( Vertex *v ) { 
 if ( !v) return; 
 points.Append(v); 
}

void mPolygon::addVertex( Vertex *v, Crayon c ) { 
 v->Color(c);
 if ( !v) return; 
 points.Append(v); 
}

Vertex * mPolygon::addVertex( Vertexd *vd ) {
 Vertex *v=new Vertex((float)vd->x,(float)vd->y,(float)vd->z);
 points.Append(v);
 return v;
}

Vertex * mPolygon::addVertex( Vertexd *vd, Crayon c ) {
 Vertex *v=new Vertex((float)vd->x,(float)vd->y,(float)vd->z);
 v->Color(c);
 points.Append(v);
 return v;
}

void mPolygon::Divide( Vertex *v ) {
 points.Divide(v);
}

void mPolygon::Color( Crayon c ) {
 points.Color(c);
}

void mPolygon::ColorMultiply( Crayon c ) {
 points.ColorMultiply(c);
}

 // The averaged center of N points
Vertex *mPolygon::barycenter(void) {
  Vertex *r = new Vertex;
  Vertex *v;
  r->x=r->y=r->z=0; 
  for (  v=(Vertex *) (points.first); v; v=(Vertex *) (v->next) ) { 
   r->x+= v->x;
   r->y+= v->y;
   r->z+= v->z;
  }
  r->x/=(float)points.count;
  r->y/=(float)points.count;
  r->z/=(float)points.count;
  return r;
 }

 void mPolygon::GrahamScan(void) {
 }

 // is the point in the poly?
bool mPolygon::inPoly( int xz_or_zy_or_xy, float xt, float yt ) {
 float xnew=0.0f,ynew=0.0f;
 float xold=0.0f,yold=0.0f;
 float x1=0.0f,yy1=0.0f;
 float x2=0.0f,y2=0.0f;
 Vertex *i_last=(Vertex *) points.last;
 bool inside=false;
 if (points.count < 3) return false;
 if ( i_last )
 switch( xz_or_zy_or_xy ) {
  case 0: xold=i_last->x; yold=i_last->z; break; 
  case 1: xold=i_last->x; yold=i_last->y; break;
  case 2: xold=i_last->z; yold=i_last->y; break; 
 }
 EACH(points.first,Vertex,i) {
  switch( xz_or_zy_or_xy ) {
   case 0: xnew=i->x; ynew=i->z; break; 
   case 1: xnew=i->x; ynew=i->y; break;
   case 2: xnew=i->z; ynew=i->y; break; 
  }
  if (xnew > xold) { x1=xold; x2=xnew; yy1=yold; y2=ynew; } 
              else { x1=xnew; x2=xold; yy1=ynew; y2=yold; }
  if ( (xnew < xt) == (xt <= xold)         /* edge "open" at left end */
      && (yt-yy1)*(x2-x1)
       < (y2-yy1)*(xt-x1) ) inside=!inside;
  xold=xnew;
  yold=ynew;
 }
 return(inside);
}

bool mPolygon::vertexInPoly( Vertex *v ) { 
 Vertex *t; 
 for ( t=(Vertex *) (points.first); t; t=(Vertex *) (t->next) )
  if ( t == v ) return true; 
 return false; 
}

bool mPolygon::vertexInPolyOrColocated( Vertex *v ) { 
 Vertex *t; 
 for (  t=(Vertex *) (points.first); t; t=(Vertex *) (t->next) )
  if ( t == v || (v->x==t->x && v->y==t->y && v->z==t->z) ) return true;
 return false;
}

// Finds the model's maximum bounding box, set to 0,0->w,h
void mPolygon::boundingBox( mPolygon *p, float *w, float *h ) { }

void mPolygon::setEqual( mPolygon *p ) { 
 Vertex *w,*v; 
 for ( v=(Vertex *) (points.first); v; v=(Vertex *) (v->next) ) 
  for ( w=(Vertex *) (p->points.first); w; w=(Vertex *) (w->next)  )
   v->setPosition(w);
}

void mPolygon::ScaleX( float factor ) {
 Vertex *v;
 for ( v=(Vertex *) (points.first); v; v=(Vertex *) (v->next) )
  v->x*=factor;
}

void mPolygon::ScaleY( float factor ) {
 Vertex *v;
 for ( v=(Vertex *) (points.first); v; v=(Vertex *) (v->next) )
  v->y*=factor; 
}

void mPolygon::ScaleZ( float factor ) {
 Vertex *v;
 for ( v=(Vertex *) (points.first); v; v=(Vertex *) (v->next) )
  v->z*=factor;
}

void mPolygon::UnscaleX( float factor ) {
 Vertex *v; 
 for ( v=(Vertex *) (points.first); v; v=(Vertex *) (v->next) )
  v->x/=factor; 
}

void mPolygon::UnscaleY( float factor ) {
 Vertex *v;
 for ( v=(Vertex *) (points.first); v; v=(Vertex *) (v->next) )
  v->y/=factor;
}

void mPolygon::UnscaleZ( float factor ) {
 Vertex *v;
 for ( v=(Vertex *) (points.first); v; v=(Vertex *) (v->next) )
  v->z/=factor;
}

//mPolygon *mPolygon::tesselate(void) { }

// Polygons class

mPolygon *Polygons::pointInPolysTest( int viewing, float xt, float yt ) {
 mPolygon *p; 
 for ( p=(mPolygon *) first; p; p=(mPolygon *) (p->next) ) 
  if ( p->inPoly(viewing,xt,yt) ) return p; 
 return null;
}

void Polygons::ScaleX( float factor ) { 
 mPolygon *p;
 for ( p=(mPolygon *) first; p; p=(mPolygon *) (p->next) ) { 
  Vertex *v; 
  for ( v=(Vertex *) (p->points.first); v; v=(Vertex *) v->next ) v->x*=factor;
 } 
}

void Polygons::ScaleY( float factor ) { 
 mPolygon *p; 
 for ( p=(mPolygon *) first; p; p=(mPolygon *) (p->next) ) { 
  Vertex *v; 
  for ( v=(Vertex *) (p->points.first); v; v=(Vertex *) v->next ) v->y*=factor; 
 } 
}

void Polygons::ScaleZ( float factor ) {
 mPolygon *p;
 for ( p=(mPolygon *) first; p; p=(mPolygon *) (p->next) ) {
  Vertex *v; 
  for ( v=(Vertex *) (p->points.first); v; v=(Vertex *) v->next ) v->z*=factor;
 }
}

void Polygons::UnscaleX( float factor ) {
 mPolygon *p;
 for ( p=(mPolygon *) first; p; p=(mPolygon *) (p->next) ) { 
  Vertex *v; 
  for ( v=(Vertex *) (p->points.first); v; v=(Vertex *) v->next ) v->x/=factor; 
 } 
}

void Polygons::UnscaleY( float factor ) {
 mPolygon *p;
 for ( p=(mPolygon *) first; p; p=(mPolygon *) (p->next) ) { 
  Vertex *v; 
  for ( v=(Vertex *) (p->points.first); v; v=(Vertex *) v->next ) v->y/=factor; 
 }
}

void Polygons::UnscaleZ( float factor ) { 
 mPolygon *p;
 for ( p=(mPolygon *) first; p; p=(mPolygon *) (p->next) ) { 
  Vertex *v;
  for ( v=(Vertex *) (p->points.first); v; v=(Vertex *) v->next ) v->z/=factor;
 }
}

bool PolygonList::inList( mPolygon *p ) {
 PolygonHandle *pL; 
 if ( !p ) return false; 
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) ) 
  if ( pL->p == p ) return true; 
 return false;
}

void PolygonList::addIfUnique( mPolygon *p ) {
 if ( !p ) return;
 if ( inList(p) ) return;
 Push( p );
}

void PolygonList::freeListandPolygons(void) { 
 PolygonHandle *pL,*pL_next;
 for ( pL=(PolygonHandle *)first; pL; ) {
  pL_next=(PolygonHandle *) pL->next;
  delete pL->p;
  delete pL;
  pL=pL_next;
 }
 count=0;
}

PolygonList *PolygonList::copy(void) { 
 PolygonList *new_list=new PolygonList;
 PolygonHandle *pL;
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) )
  new_list->Push(pL->p);
 return new_list;
}

PolygonList *PolygonList::Duplicate(void) {
 PolygonList *new_list=new PolygonList;
 PolygonHandle *pL;
// mPolygon *p;
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) ) 
  new_list->Push(pL->p->Copy()); 
 return new_list;
}

void PolygonList::setEqual( PolygonList *qL ) {
 PolygonHandle *pL,*q;
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) )
  for ( q=(PolygonHandle *) (qL->first); q; q=(PolygonHandle *) (q->next) )
   pL->p->setEqual(q->p);
}

void PolygonList::move( float x, float y, float z ) {
 PolygonHandle *pL;
 Vertex *v;
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) )
  for ( v=(Vertex *) (pL->p->points.first); v; v=(Vertex *)(v->next) ) {
   v->x+=x;
   v->y+=y;
   v->z+=z;
  }
}

void PolygonList::ScaleX( float factor ) {
 PolygonHandle *pL;
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) ) 
  pL->p->ScaleX(factor);
}

void PolygonList::ScaleY( float factor ) {
 PolygonHandle *pL;
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) )
  pL->p->ScaleY(factor);
}

void PolygonList::ScaleZ( float factor ) {
 PolygonHandle *pL;
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) )
  pL->p->ScaleZ(factor); 
}

void PolygonList::UnscaleX( float factor ) {
 PolygonHandle *pL;
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) )
  pL->p->UnscaleX(factor);
}

void PolygonList::UnscaleY( float factor ) {
 PolygonHandle *pL;
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) ) 
  pL->p->UnscaleY(factor);
}

void PolygonList::UnscaleZ( float factor ) {
 PolygonHandle *pL;
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) )
  pL->p->UnscaleZ(factor);
}

VertexList *PolygonList::toVertexList() {
 PolygonHandle *pL; 
 Vertex *v; 
 VertexList *vList=new VertexList; 
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) ) 
  for ( v=(Vertex *) (pL->p->points.first); v; v=(Vertex *)(v->next) )
   vList->addIfUnique(v);
 return vList;
}

 // Find polygon groups and return polygon_list
 // Group -1 is defined as no grouping
PolygonList *PolygonList::findGroup( mPolygon *p ) {
 PolygonList *pList=new PolygonList;
 PolygonHandle *q;
 if ( p->group == -1 ) { this->Push(p); return this; }
 for ( q=(PolygonHandle *)first; q; q=(PolygonHandle *) (q->next) )
  if ( q->p!=p && q->p->group == p->group ) pList->Push(q->p);
 pList->Push(p);
 return pList;
}

void PolygonList::Push( mPolygon *p ) {
 if ( p ) {
  PolygonHandle *pL=new PolygonHandle;
  pL->p=p;
  Append(pL);
 }
}

PolygonHandle *PolygonList::find( mPolygon *p ) {
 PolygonHandle *pL;
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) )
  if ( pL->p==p ) return pL;
 return null;
}

mPolygon *PolygonList::pointInPolysTest( int viewing, float xt, float yt ) { 
 PolygonHandle *pL;
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) )
  if ( pL->p && pL->p->inPoly(viewing,xt,yt) ) return pL->p; 
 return null;
}

mPolygon *PolygonList::vertexInList(Vertex *v) {
 PolygonHandle *pL;
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) )
  if ( pL->p && pL->p->vertexInPoly(v) ) return pL->p; 
 return null; 
}

// The averaged center of N points over P polygons
Vertex *PolygonList::barycenter() {
 int rolling_total=0; 
 PolygonHandle *pL;
 Vertex *r=new Vertex;  // result
 Vertex *v;
 for ( pL=(PolygonHandle *)first; pL; pL=(PolygonHandle *) (pL->next) )
  if ( pL->p ) 
   for ( v=(Vertex *) (pL->p->points.first); v; v=(Vertex *) (v->next) ) {
    rolling_total++;
    r->x+= v->x;
    r->y+= v->y;
    r->z+= v->z;
   }
 r->x/=rolling_total;  r->y/=rolling_total;  r->z/=rolling_total;
 return r;
}

// Find the "least" point on a certain view plane
Vertex *PolygonList::leastPoint() {
 PolygonHandle *p=(PolygonHandle *) first;
 Vertex *r=new Vertex; // result;
 Vertex *v;
 if ( p && p->p && p->p->points.first ) { 
  v=(Vertex *) (p->p->points.first); 
  r->x=v->x; 
  r->y=v->y; 
  r->z=v->z; 
 }
 for ( p=(PolygonHandle *)first; p; p=(PolygonHandle *) (p->next) ) //if ( polygons->p && polygons->(p->points.first) )
  for ( v=(Vertex *) (p->p->points.first); v; v=(Vertex *) (v->next) ) {
   if ( v->x < r->x ) r->x=v->x; 
   if ( v->y < r->y ) r->y=v->y;
   if ( v->z < r->z ) r->z=v->z;
  }
 return r;
}

void PolygonList::rotateXY( float angleDeg ) {
 Vertex *center=barycenter();
 Vertex *v;
 PolygonHandle *polygons;
 for ( polygons=(PolygonHandle *)first; polygons; polygons=(PolygonHandle *) (polygons->next) )
  if ( polygons->p ) 
   for ( v=(Vertex *) (polygons->p->points.first); v; v=(Vertex *) (v->next) )
    v->rotateXY( center, angleDeg );
 delete center;
}

void PolygonList::rotateYZ( float angleDeg ) {
 Vertex *center=barycenter();
 PolygonHandle *polygons;
 Vertex *v;
 center->x = center->z;
 for ( polygons=(PolygonHandle *)first; polygons; polygons=(PolygonHandle *) (polygons->next) )
  if ( polygons->p )
   for ( v=(Vertex *) (polygons->p->points.first); v; v=(Vertex *) (v->next) ) {
    Vertex *w=v->Copy();
    w->x=v->z;
    w->y=v->y;
    w->rotateXY( center, angleDeg );
    v->z=w->x;
    v->y=w->y;
    delete w;
   }
 delete center;
}
