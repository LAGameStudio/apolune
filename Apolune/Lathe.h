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

#include "GLWindow.h"
#include "Vertex.h"
#include "Curve2d.h"
#include "Model.h"
#include "Shape.h"
#include "UglyFont.h"
#include "STLFile.h"
#include "Numbers.h"
#include "VBOGroup.h"

/*
 * How to connect individual guides that don't have matching endpoints.
 */
enum LatheLayerConnection {
 lathe_inscribed=0, // Inscribe a disc parallel to base connecting A and B guides
 lathe_midpoint=1,  // Choose the midpoint (average) of both juxtaposed ends A and B
 lathe_greater=2,   // Choose the greater width A or B
 lathe_lesser=3,    // Choose the lesser width A or B
 lathe_previous=4,  // Match the 'previous' guide A
 lathe_next=5       // Match the 'next' guide B
};

/*
 * A single guide which is either a line or a curve.
 */
class LatheGuide : public ListItem {
public:
 LatheLayerConnection connection;
 Zbool curved;
 Vertexd a,b,c,d;
 Zdouble low,high,thickness; // Height extents
 LatheGuide() : ListItem() {
  d.Set(0.0,1.0,0.0);
 }
 Vertices *Points( int segments ) {
  if ( segments <= 1 ) {
   Vertices * points=new Vertices;
   points->Add(a.x,a.y,a.z);
   points->Add(b.x,b.y,b.z);
   return points;
  }
  if ( curved ) { // Curved
   curve2d.SetLOD(segments);
   return curve2d.Bezier( a.x, 0.0, b.x, b.x, c.x, c.y, d.x, 1.0 );
  } else { // Linear
   Vertices *points=new Vertices;
   points->Add(a.x,0.0,a.z);
   unsigned int i;
   for ( i=1; i< (unsigned int) (segments-1); i++ ) {
    points->LinePoint(a.x,0.0,a.z,b.x,1.0,b.z,iratiod(i,segments));
   }
   points->Add(b.x,1.0,b.z);
   return points;
  }
 }
 void Draw( double x, double y, double w, double h ) {
  if ( curved ) {
   Zdis<Vertices> curve;
   curve.Recycle(curve2d.Bezier( a.x, 0.0, b.x, b.x, c.x, c.y, d.x, 1.0 ));
   EACH(curve->first,Vertex,p) if ( p->next ) {
    Vertex *n=(Vertex *) (p->next);
    DrawLine2Color(crayons.Pick(teal), crayons.Pick(lavender),
     x+w*(double)p->x, y+h*(double)p->y, x+w*(double)n->x, y+h*(double)n->y );
   }
  } else {
   DrawLine2Color(crayons.Pick(teal), crayons.Pick(puce), x+w*a.x,y+h*a.y, x+w*b.x, y+h*b.y );
  }
 }
};

/*
 * A set of guides stacked in order that make up one slab or layer description.
 */
class LatheGuides : public LinkedList {
public:
 Zint segments;
 LatheGuides() : LinkedList() {
  segments=10;
 }
 // Add a curve
 void Add( Vertexd *start, Vertexd *startAnglePoint, Vertexd *endAnglePoint, Vertexd *end ) {
  LatheGuide *g=(new LatheGuide);
  Append(g);
  g->a.Set(start);
  g->b.Set(startAnglePoint);
  g->c.Set(endAnglePoint);
  g->d.Set(end);
  g->curved=true;
 }
 // Add a line
 void Add( Vertexd *start, Vertexd *end ) {
  LatheGuide *g=new LatheGuide;
  Append(g);
  g->a.Set(start);
  g->b.Set(end);
  g->curved=false;
 }
 void Points( Vertices *out, Vertexd *extents ) {
  double total=(double) count;
  double delta=extents->y/total;
  unsigned int i=0;
  FOREACH(LatheGuide,g) {
   Vertices *result=g->Points((int)segments);
   result->Addition(0.0,delta*(double)i,0.0);
   out->AppendAndDispose(result);
   i++;
  }
 }
 CLEARLISTRESET(LatheGuide);
};

/*
 * One slab which can be extrapolated as a single piece of a larger lathed piece.
 */
class LatheLayer : public ListItem {
public:
 LatheGuides guides;
 Vertices points;
 Zdouble low,high,thickness; // Height extents
 LatheLayer() : ListItem() {
  low=0.0;
  high=1.0;
  thickness=1.0;
 }
 void Extrapolate( Vertexd *extents ) {
  points.Clear();
  guides.Points(&points,extents);
 }
 string toString() { return string(""); }
 void fromString( const char *in ) {}
 void Load( const char *filename ) { fromString(file_as_string(filename).c_str()); }
 void Save( const char *filename ) { string_as_file(toString().c_str(),filename);  }
 void Draw( double x, double y, double w, double h, bool info ) {
  double delta=1.0/(double) guides.count * h;
  double h1=0.0;
  EACH(guides.first,LatheGuide,g) {
   g->Draw(x,y+(h-h1),w,-delta);
   h1+=delta;
  }
  if( info )
  Text(
   FORMAT("L=%f H=%f T=%f", (double)low, (double)high, (double)thickness ).c_str(),
   x+w, y, 6, 8, false
  ); 
 }
 void Add( Vertexd *start, Vertexd *startAngle, Vertexd *endAngle, Vertexd *end ) {
  guides.Add(start,startAngle,endAngle,end);
 }
 void Add( Vertexd *start, Vertexd *end ) { guides.Add(start,end); }
};

class LatheLayers : public LinkedList {
public:
 Vertexd layerExtents;
 LatheLayer *Add() { LatheLayer *L=new LatheLayer; Append(L); return L; }
 void Profile( Vertices *out, Vertexd *extents ) {
  layerExtents.Set( extents->x, extents->y, extents->z / (double) count );
  unsigned int i=0;
  FOREACH(LatheLayer,L) {
   Vertices points;
   L->Extrapolate(&layerExtents);
   points.Concat(&L->points);
   points.Multiply(layerExtents.x,layerExtents.x,L->thickness);
   points.Add(layerExtents.x,layerExtents.y,L->low);
   out->Concat(&points);
   i++;
  }
 }
 void Draw( double x, double y, double w, double h ) {
  double delta=1.0/(double) count * h;
  double h1=0.0;
  unsigned int i=0;
  FOREACH(LatheLayer,L) { // Draw from bottom up
   L->Draw(x,y+(h-h1),w,-delta,true);
   h1+=delta;
   i++;
  }
 }
 void Apply( Doubles *values ) {
  Double *d=(Double *) values->first;
  LatheLayer *L=(LatheLayer *) first;
  double running=0.0;
  while ( d && L ) {
   L->low=running;
   L->high=d->d;
   L->thickness=d->d-running;
   running+=d->d;
   d=(Double *) d->next;
   L=(LatheLayer *) L->next;
  }
 }
 CLEARLISTRESET(LatheLayer);
};

/*
 * A stack of layers which describe a volume.
 */
class LatheStack {
public:
 LatheLayers layers;
 void Add( LatheLayer *L ) { layers.Append(L); }
 void Profile( Vertices *out, Vertexd *extents ) {
  layers.Profile(out,extents);
 }
 void Load( const char *filename ) {
 }
 void Save( const char *filename ) {
 }
 void Draw( double x, double y, double w, double h ) {
  layers.Draw(x,y,w,h);
 }
};

/*
 * A volume that perturbs a cylindrical expression.
 */
class Lathe : public ListItem {
public:
 LatheStack stack;
 Vertexd extents;
 Vertices profile;
// ImmediateModel model;
 VBOGroupedNormalsPolys vbos;
 STLFile stl;
 Zbool clockwise;
 // Clockwise a,b,c of the triangle
 void PushTri( Vertexd *a, Vertexd *b, Vertexd *c ) {
  mPolygon tri;
  Vertex normalVector;
  Crayon A,B,C;
  B.Double(b->x/extents.x/2.0,b->y/extents.y/2.0,b->z/extents.z/2.0,1.0);
  C.Double(c->x/extents.x/2.0,c->y/extents.y/2.0,c->z/extents.z/2.0,1.0);
  A.Double(a->x/extents.x/2.0,a->y/extents.y/2.0,a->z/extents.z/2.0,1.0);
  if ( !this->clockwise ) vbos.PushTri( a,&A,b,&B,c,&C,&stl );
  else vbos.PushTri( c,&C,b,&B,a,&A,&stl );
 }
 // Clockwise a,b,c,d of the quad
 void PushQuad( Vertexd *a, Vertexd *b, Vertexd *c, Vertexd *d ) {
  mPolygon tri;
  Crayon A,B,C,D;
  A.Double(a->x/extents.x/2.0,a->y/extents.y/2.0,a->z/extents.z/2.0,1.0);
  B.Double(b->x/extents.x/2.0,b->y/extents.y/2.0,b->z/extents.z/2.0,1.0); 
  C.Double(c->x/extents.x/2.0,c->y/extents.y/2.0,c->z/extents.z/2.0,1.0); 
  D.Double(d->x/extents.x/2.0,d->y/extents.y/2.0,d->z/extents.z/2.0,1.0);
  if ( this->clockwise ) vbos.PushQuad( a,&A,b,&B,c,&C,d,&D,&stl );
  else vbos.PushQuad( d,&D,c,&C,b,&B,a,&A,&stl );
 }
 // Append a circle as a tube
 void PushCirclesAsTube( double h1, double h2, Circle *lo, Circle *hi, int precision ) {
  Vertexd a,b,c,d;
  double delta=1.0/(double)precision;
  /* Do the first segment */
  lo->Point(0.0,&a);   a.Set(a.x,a.y,h1);
  hi->Point(0.0,&b);   b.Set(b.x,b.y,h2);
  hi->Point(delta,&c); c.Set(c.x,c.y,h2);
  lo->Point(delta,&d); d.Set(d.x,d.y,h1);
  PushQuad( &a, &b, &c, &d );
  /* Do the rest except last */
  double oneminusdelta=1.0-delta;
  double time=delta;
  for ( ; time<oneminusdelta; time+=delta ) {
   lo->Point(time,&a);       a.Set(a.x,a.y,h1);
   hi->Point(time,&b);       b.Set(b.x,b.y,h2);
   hi->Point(time+delta,&c); c.Set(c.x,c.y,h2);
   lo->Point(time+delta,&d); d.Set(d.x,d.y,h1);
   PushQuad( &a, &b, &c, &d );
  }
  /* Do the last */
  lo->Point(oneminusdelta,&a); a.Set(a.x,a.y,h1);
  hi->Point(oneminusdelta,&b); b.Set(b.x,b.y,h2);
  hi->Point(1.0,&c);           c.Set(c.x,c.y,h2);
  lo->Point(1.0,&d);           d.Set(d.x,d.y,h1);
  PushQuad( &a, &b, &c, &d );
 }
 // Append a circle as a hollow tube
 void PushCirclesAsHollowTube( double h1, double h2, Circle *lo, Circle *hi, int precision, double innerPercent ) {
  Circle loi,hii;
  loi.Set(lo->R*innerPercent);
  hii.Set(lo->R*innerPercent);
  Vertexd a,b,c,d;
  double delta=1.0/(double)precision;
  /* Do the first segment */
  lo->Point(0.0,&a);   a.Set(a.x,a.y,h1);
  hi->Point(0.0,&b);   b.Set(b.x,b.y,h2);
  hi->Point(delta,&c); c.Set(c.x,c.y,h2);
  lo->Point(delta,&d); d.Set(d.x,d.y,h1);
  PushQuad( &a, &b, &c, &d );
  loi.Point(0.0,&d);   a.Set(a.x,a.y,h1);
  hii.Point(0.0,&c);   b.Set(b.x,b.y,h2);
  hii.Point(delta,&b); c.Set(c.x,c.y,h2);
  loi.Point(delta,&a); d.Set(d.x,d.y,h1);
  PushQuad( &a, &b, &c, &d );
  /* Do the rest except last */
  double oneminusdelta=1.0-delta;
  double time=delta;
  for ( ; time<oneminusdelta; time+=delta ) {
   lo->Point(time,&a);       a.Set(a.x,a.y,h1);
   hi->Point(time,&b);       b.Set(b.x,b.y,h2);
   hi->Point(time+delta,&c); c.Set(c.x,c.y,h2);
   lo->Point(time+delta,&d); d.Set(d.x,d.y,h1);
   PushQuad( &a, &b, &c, &d );
   loi.Point(time,&d);       a.Set(a.x,a.y,h1);
   hii.Point(time,&c);       b.Set(b.x,b.y,h2);
   hii.Point(time+delta,&b); c.Set(c.x,c.y,h2);
   loi.Point(time+delta,&a); d.Set(d.x,d.y,h1);
   PushQuad( &a, &b, &c, &d );
  }
  /* Do the last */
  lo->Point(oneminusdelta,&a); a.Set(a.x,a.y,h1);
  hi->Point(oneminusdelta,&b); b.Set(b.x,b.y,h2);
  hi->Point(1.0,&c);           c.Set(c.x,c.y,h2);
  lo->Point(1.0,&d);           d.Set(d.x,d.y,h1);
  PushQuad( &a, &b, &c, &d );
  loi.Point(oneminusdelta,&d); a.Set(a.x,a.y,h1);
  hii.Point(oneminusdelta,&c); b.Set(b.x,b.y,h2);
  hii.Point(1.0,&b);           c.Set(c.x,c.y,h2);
  loi.Point(1.0,&a);           d.Set(d.x,d.y,h1);
  if ( this->clockwise ) PushQuad( &a, &b, &c, &d ); else PushQuad( &d, &c, &b, &a );
 }
 // Append a triangle disc, clockwise flips normals
 void PushCircleAsDisc( double h, Circle *lo, int precision, bool clockwise ) {
  Vertexd center,a,b;
  double delta=1.0/(double)precision;
  double oneminusdelta=1.0-delta;
  /* Do the first segment */
  lo->Point(0.0,&a);    a.Set(a.x,h,a.y);
  lo->Point(delta,&b);  b.Set(b.x,h,b.y);
  if ( clockwise ) PushTri( &a, &b, &center ); else PushTri( &center, &b, &a );
  /* Do the rest except last */
  double time=delta;
  for ( ; time<oneminusdelta; time+=delta ) {
   lo->Point(time,&a);       a.Set(a.x,h,a.y);
   lo->Point(time+delta,&b); b.Set(b.x,h,b.y);
   PushTri( &a, &b, &center );
  }
  /* Do the last */
  lo->Point(oneminusdelta,&a); a.Set(a.x,h,a.y);
  lo->Point(1.0,&b);           b.Set(b.x,h,b.y);
  PushTri( &a, &b, &center );
 }
 // Append an "aerobie" shaped circle, clockwise flips normals
 void PushCircleAsInscribedDisc( double h, Circle *lo, int precision, double innerPercent, bool clockwise ) {
  Circle hi;
  hi.Set(lo->R*innerPercent);
  Vertexd a,b,c,d;
  double delta=1.0/(double)precision;
  double oneminusdelta=1.0-delta;
  /* Do the first segment */
  lo->Point(0.0,&a);   a.Set(a.x,a.y,h);
  hi.Point(0.0,&b);    b.Set(b.x,b.y,h);
  hi.Point(delta,&c);  c.Set(c.x,c.y,h);
  lo->Point(delta,&d); d.Set(d.x,d.y,h);
  PushQuad( &a, &b, &c, &d );
  /* Do the rest except last */
  double time=delta;
  for ( ; time<oneminusdelta; time+=delta ) {
   lo->Point(time,&a);        a.Set(a.x,a.y,h);
   hi.Point(time,&b);         b.Set(b.x,b.y,h);
   hi.Point(time+delta,&c);   c.Set(c.x,c.y,h);
   lo->Point(time+delta,&d);  d.Set(d.x,d.y,h);
   PushQuad( &a, &b, &c, &d );
  }
  /* Do the last */
  lo->Point(oneminusdelta,&a);  a.Set(a.x,a.y,h);
  hi.Point(oneminusdelta,&b);   b.Set(b.x,b.y,h);
  hi.Point(1.0,&c);             c.Set(c.x,c.y,h);
  lo->Point(1.0,&d);            d.Set(d.x,d.y,h);
  PushQuad( &a, &b, &c, &d );
 }
  // Populate model, vboNormals, debugVbo based on content of guides
 void Turn( Vertices *in, Vertexd *extents, int rotationSegments, bool hollow, double thickness ) {
  stl.facets.Clear();
  vbos.Clear();
  if ( rotationSegments < 3 ) rotationSegments=3;
  this->extents.Set(extents);
  profile.Clear();
  profile.Concat(in); // Snip the points off the incoming list
  if ( profile.count < 3 ) {
   OUTPUT("Lathe.Turn(): Not enough data (less than 3 profile points), aborting.\n");
   in->Concat(&profile); // Return the points to the original list
   return;
  }
  Vertexd a,b,c,d;
  Circles circlesZY;
  profile.toCirclesX(&circlesZY,(float)this->extents.x);
  double delta=this->extents.z/(double)circlesZY.count;
  double h1=0.0;
  double h2=delta;
  if ( hollow && thickness > 0 ) {
   EACH(circlesZY.first,Circle,c) if ( c->next ) {
    Circle *hi=(Circle *) c->next;
    PushCirclesAsHollowTube(h1,h2,c,hi,rotationSegments,1.0-thickness);
    h1+=delta;
    h2+=delta;
   }
//   Circle insideBottom;
//   insideBottom.Set( ((Circle *) (circlesZY.first->next))->R*(1.0-thickness) );
//   PushCircleAsDisc( delta, &insideBottom, rotationSegments, false );
  } else {
   EACH(circlesZY.first,Circle,c) if ( c->next ) {
    Circle *hi=(Circle *) c->next;
    PushCirclesAsTube(h1,h2,c,hi,rotationSegments);
    h1+=delta;
    h2+=delta;
   }
  }
  vbos.CreateClear();
  in->Concat(&profile); // Return the points to the original list
 }

 // Populate model, vboNormals, debugVbo based on content of guides
 void Turn( Vertexd *extents, int rotationSegments, bool hollow, double thickness ) {
  stl.facets.Clear();
  vbos.Clear();
  if ( rotationSegments < 3 ) rotationSegments=3;
  this->extents.Set(extents);
  stack.Profile(&profile,&this->extents);
  if ( profile.count < 3 ) {
   OUTPUT("Lathe.Turn(): Not enough data (less than 3 profile points), aborting.\n");
   return;
  }
  Vertexd a,b,c,d;
  Circles circlesZY;
  profile.toCirclesX(&circlesZY,(float)this->extents.x);
  double delta=this->extents.z/(double)circlesZY.count;
  double h1=0.0;
  double h2=delta;
  if ( hollow && thickness > 0 ) {
   EACH(circlesZY.first,Circle,c) if ( c->next ) {
    Circle *hi=(Circle *) c->next;
    PushCirclesAsHollowTube(h1,h2,c,hi,rotationSegments,1.0-thickness);
    h1+=delta;
    h2+=delta;
   }
//   Circle insideBottom;
//   insideBottom.Set( ((Circle *) (circlesZY.first->next))->R*(1.0-thickness) );
//   PushCircleAsDisc( delta, &insideBottom, rotationSegments, false );
  } else {
   EACH(circlesZY.first,Circle,c) if ( c->next ) {
    Circle *hi=(Circle *) c->next;
    PushCirclesAsTube(h1,h2,c,hi,rotationSegments);
    h1+=delta;
    h2+=delta;
   }
  }
  vbos.CreateClear();
 }
 void Render() {
//  unsigned int p,q;
//  normalmappingShader.vbo=vbo;//&vboQuadVNTTanBi;//&vbo;
//  normalmappingShader.UpdateUniforms();
//  normalmappingShader.Disable();
 }
 void RenderWireframe() {
  vbos.Render();
 }
 void Apply( Doubles *d ) {
  stack.layers.Apply(d);
 }
};