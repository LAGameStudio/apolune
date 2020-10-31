#pragma once
#include "ListItem.h"
#include "Trigger.h"
#include "Cartesian.h"
#include "Vertexd.h"
#include "Polygon.h"

/*
 * Used to overcome limitations of OpenGL with respect to drawing trapezoids in 2D.
 */

ONE(IsotrapGrid,{})
 Line AB,DC,AD,BC; // External lines
 Lines H,V;        // Grid lines horizontal, vertical
 Zint precision;
 Zbool flipY,flipX;  // Invert the texture coordinates by Y or X
 Quads quads;
 Polygons triangles;
 IsotrapGrid( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, int precision=8 ) : ListItem() {
  this->precision=precision;
  Set(x1,y1,x2,y2,x3,y3,x4,y4);
 }
 void Set(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, int precision=8 ) {
  this->precision=precision;
  AB.Set(x1,y1,x2,y2); // Top line (left to right)
  DC.Set(x4,y4,x3,y3); // Bottom line (left to right)
  AD.Set(x1,y1,x4,y4); // Left side (top to bottom)
  BC.Set(x2,y2,x3,y3); // Right side (top to bottom )
  CalculateGrid();
 }
 void CalculateGrid() {
  if ( precision < 2 ) precision=2;
  // Horizontals
  H.Clear();
  H.Append(new Line(&AD));
  for ( int k=1; k<precision; k++ ) {
   Vertexd a,b;
   double time=iratiod(k,precision+1);
   AD.Point(time,&a);
   BC.Point(time,&b);
   H.Append(new Line(a.x,a.y,b.x,b.y));
  }
  H.Append(new Line(&BC));
  // Verticals
  V.Clear();
  V.Append(new Line(&AB));
  for ( int k=1; k<precision; k++ ) {
   Vertexd a,b;
   double time=iratiod(k,precision+1);
   BC.Point(time,&a);
   AB.Point(time,&b);
   V.Append(new Line(a.x,a.y,b.x,b.y));
  }
  V.Append(new Line(&DC));
  // Quads
  quads.Clear();
  for ( int j=0; j<V.count-1; j++ ) {
   double jt1=(double)iratiod(j,V.count);
   double jt2=(double)iratiod(j+1,V.count);
   Line *Va=(Line *) V.Element(j);
   Line *Vb=(Line *) V.Element(j+1);
   for ( int i=0; i<H.count-1; i++ ) {
    Quad *q=new Quad;
    double it1=(double)iratiod(i,V.count);
    double it2=(double)iratiod(i+1,V.count);
    Line *Ha=(Line *) V.Element(j);
    Line *Hb=(Line *) V.Element(j+1);
    Vertexd vx,vy;
    Vertexd A,B,C,D;
    Ha->Point(it1,&vx); Va->Point(jt1,&vy); A.Set(vx.x,vy.y);
    Ha->Point(it2,&vx); Vb->Point(jt1,&vy); B.Set(vx.x,vy.y);
    Ha->Point(it2,&vx); Va->Point(jt2,&vy); C.Set(vx.x,vy.y);
    Ha->Point(it1,&vx); Va->Point(jt2,&vy); D.Set(vx.x,vy.y);
    q->TL.x=A.x; //  Grid Lines Monograph:
    q->TL.y=A.y; //        Va    Vb
    q->TR.x=B.x; //        |     |
    q->TR.y=B.y; //  Ha ---A-----B---
    q->BR.x=C.x; //        |     |
    q->BR.y=C.y; //        |     |
    q->BL.x=D.x; //  Hb ---D-----C---
    q->BL.y=D.y; //        |     |
    // Apply texture coordinates
    q->tcTL.x=it1; q->tcTL.y=jt2;
    q->tcTR.x=it2; q->tcTR.y=jt2;
    q->tcBR.x=it2; q->tcBR.y=jt1;
    q->tcBL.x=it1; q->tcBL.y=jt1;
    quads.Append(q);
   }
  }
  // Triangles
  triangles.Clear();
  quads.triangles(&triangles);
 }
MANY(IsotrapGrid,IsotrapGridHandle,IsotrapGridHandles,"IsotrapGrid",IsotrapGrids,{})
DONE(IsotrapGrid);

extern IsotrapGrid isoGrid;