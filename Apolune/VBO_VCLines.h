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
#include "VBO_VC.h"

class VBOStaticVCLines : public VBOStaticVC {
public:
 VBOStaticVCLines() : VBOStaticVC() {
  draw_method=GL_LINES;
 }
 // Generate a grid: XY plane, no offset
 void Grid( double w, double h, unsigned int divisionsx, unsigned int divisionsy ) {
  Vertex a,b;
  if ( divisionsx < 1 ) divisionsx=1;
  if ( divisionsy < 1 ) divisionsy=1;
  double dx=w/(double)divisionsx;
  double dy=h/(double)divisionsy;
  b.Set(w,0.0,0.0);  PushLine(&a,&b);
  b.Set(0.0,h,0.0);  PushLine(&a,&b);
  b.Set(w,h,0.0);
  a.Set(0.0,h,0.0);  PushLine(&a,&b);
  a.Set(w,0.0,0.0);  PushLine(&a,&b);
  unsigned int i;
  for ( i=1; i<divisionsx; i++ ) {
   double x=dx*(double)i;
   a.Set(x,0.0);
   b.Set(x,h);
   PushLine(&a,&b);
  }
  for ( i=1; i<divisionsy; i++ ) {
   double y=dy*(double)i;
   a.Set(0.0,dy);
   b.Set(w,dy);
   PushLine(&a,&b);
  }
 }
 // Generate a grid: XY plane, with offset
 void Grid( double ox, double oy, double w, double h, unsigned int divisionsx, unsigned int divisionsy ) {
  Vertex a,b;
  if ( divisionsx < 1 ) divisionsx=1;
  if ( divisionsy < 1 ) divisionsy=1;
  double sx=ox;
  double sy=oy;
  double ex=ox+w;
  double ey=oy+h;
  double dx=w/(double)divisionsx;
  double dy=h/(double)divisionsy;
  b.Set(ex,sy,0.0);  PushLine(&a,&b);
  b.Set(sx,ey,0.0);  PushLine(&a,&b);
  b.Set(ex,ey,0.0);
  a.Set(sx,ey,0.0);  PushLine(&a,&b);
  a.Set(ex,sy,0.0);  PushLine(&a,&b);
  unsigned int i;
  for ( i=1; i<divisionsx; i++ ) {
   double x=sx+dx*(double)i;
   a.Set(x,sy);
   b.Set(x,ey);
   PushLine(&a,&b);
  }
  for ( i=1; i<divisionsy; i++ ) {
   double y=sy+dy*(double)i;
   a.Set(sx,y);
   b.Set(ex,y);
   PushLine(&a,&b);
  }
 }
 void fromLinesModel( ImmediateModel *m );

 void toLinesVBO( ImmediateModel *m, Crayon outer, Crayon inner );

 void Lines( mPolygon *lines ) {
  this->draw_method=lines->draw_method;
  EACH(lines->points.first,Vertex,v) if ( v->next ) {
   PushLine(v,(Vertex*)v->next);
  }
 }
 void PushLine( Vertex *a, Vertex *b ) {
  PushVertex(a);
  PushVertex(b);
 }
 void Polygon( mPolygon *lineloop ) {
  this->draw_method=lineloop->draw_method;
  EACH(lineloop->points.first,Vertex,v) {
   PushLine(v,v->next?(Vertex*)v->next:(Vertex*)lineloop->points.first);
  }
 }
};
