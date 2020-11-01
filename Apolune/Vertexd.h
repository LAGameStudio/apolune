/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once
#include "Strings.h"
#include "BinaryFile.h"

struct Coord2d { double x,y; Coord2d() { x=0.0; y=0.0; } };

/*
 * Vertex Double Precision Minimalist Classes
 */
class Vertex;
class Vertexd {
public:
 double x,y,z;
 Vertexd() { x=y=z=0.0; }
 Vertexd( Vertexd *d ) { x=d->x; y=d->y; z=d->z; }
 Vertexd( double X, double Y ) { x=X; y=Y; z=0.0; }
 Vertexd( double X, double Y, double Z ) { x=X; y=Y; z=Z; }
 void BinaryRead( BinaryFile *open ) {
  open->read(&x);
  open->read(&y);
  open->read(&z);
 }
 void BinaryWrite( BinaryFile *open ) {
  open->write(&x);
  open->write(&y);
  open->write(&z);
 }
 void Setf( float X, float Y, float Z ) { x=(double) X; y=(double) Y; z=(double) Z; }
 void Set( double X, double Y, double Z ) { x=X; y=Y; z=Z; }
 void Set( double X, double Y ) { x=X; y=Y; /*z=0.0;*/ }
 void Set( Vertexd d ) {
  x=d.x;
  y=d.y;
  z=d.z;
 }
 void Set( Vertexd *d ) {
  x=d->x;
  y=d->y;
  z=d->z;
 }
 void Set( Vertex *d );
 void Duplicate( Vertexd *target ) {
  target->x=x;
  target->y=y;
  target->z=z;
 }
 void SetAdd( Vertexd *a, Vertexd *b ) {
  x=a->x+b->x;
  y=a->y+b->y;
  z=a->z+b->z;
 }
 void SetSubtract( Vertexd *a, Vertexd *b ) {
  x=a->x-b->x;
  y=a->y-b->y;
  z=a->z-b->z;
 }
 void SetMultiply( Vertexd *a, Vertexd *b ) {
  x=a->x*b->x;
  y=a->y*b->y;
  z=a->z*b->z;
 }
 void SetDivide( Vertexd *a, Vertexd *b ) {
  x=a->x/b->x;
  y=a->y/b->y;
  z=a->z/b->z;
 }
 void Negate() { x=-x; y=-y; z=-z; }
 void None() { x=y=z=0.0; }
 void Zero() { None(); }
 void Add( double factor )      { x+=factor; y+=factor; z+=factor; }
 void Add( double x, double y ) { this->x+=x; this->y+=y; }
 void Add( double x, double y, double z ) { this->x+=x; this->y+=y; this->z+=z; }
 void Add( Vertexd *d )         { x+= d->x;  y+= d->y;  z+= d->z;  }
 void AddHalf( double factor )      { x+=factor/2.0; y+=factor/2.0; z+=factor/2.0; }
 void AddHalf( double x, double y ) { this->x+=x/2.0; this->y+=y/2.0; }
 void AddHalf( double x, double y, double z ) { this->x+=x/2.0; this->y+=y/2.0; this->z+=z/2.0; }
 void AddHalf( Vertexd *d )         { x+= d->x/2.0;  y+= d->y/2.0;  z+= d->z/2.0;  }
 void Add( Vertex *v );
 void Subtract( double factor ) { x-=factor; y-=factor; z-=factor; }
 void Subtract( Vertexd *d )    { x-=d->x;   y-=d->y;   z-=d->z;   }
 void Multiply( double X, double Y, double Z ) { x*=X; y*=Y; z*=Z; }
 void Multiply( double factor ) { x*=factor; y*=factor; z*=factor; }
 void Multiply( Vertexd *d )    { x*=d->x;   y*=d->y;   z*=d->z;   }
 void Divide( double factor )   { x/=factor; y/=factor; z/=factor; }
 void Divide( Vertexd *d )      { x/=d->x;   y/=d->y;   z/=d->z;   }
 bool Equals( Vertexd *d )      { return (x==d->x && y==d->y && z==d->z); }
 double Magnitude() {
  if ( x==0.0 && y==0.0 && z==0.0 ) return 0.0;
  return sqrt(x*x + y*y + z*z);
 }
 void Normalize() {
  double m=Magnitude();
  if (m != 0.0 && m != 1.0) Divide(m);
 }
 void Normalize( double magnitude ) {
  if (magnitude != 0.0 && magnitude != 1.0) Divide(magnitude);
 }
 void Limit(double max) {
  double m=Magnitude();
  if ( m > max ) {
   Normalize(m);
   Multiply(max);
  }
 }
 double Heading2D() {
  double a = atan2(-y, x);
  return -1.0*a;
 }
 void Rotate(double rads) {
  double old = x;
  // Might need to check for rounding errors?
  x = x*cos(rads) - y*sin(rads);
  y = old*sin(rads) + y*cos(rads);
 }
 bool OnScreen();
 bool OnScreen( double w, double h );
 SKEYSTART
  SKEY("x", {SPARAM(x,double,atof)} )
  SKEY("y", {SPARAM(y,double,atof)} )
  SKEY("z", {SPARAM(z,double,atof)} )
 SKEYEND("Vertexd");
 string toString() {
  char buf[128];
  FORMAT(buf,128,"x %1.5f y %1.5f z %1.5f", (float) x, (float) y, (float) z);
  return string(buf);
 }
 string toStringJustified() {
  char buf[128];
  FORMAT(buf,128,"x %-4.3f y %-4.3f z %-4.3f", (float) x, (float) y, (float) z);
  return string(buf);
 }
 bool LessThan( Vertexd *v ) {
  return ( x<v->x && y<v->y && z<v->z );
 }
 bool GreaterThan( Vertexd *v ) {
  return ( x>v->x && y>v->y && z>v->z );
 }
 double distance( Vertexd *f ) { return ddistance( f->x, f->y, f->z, x,y,z ); }
 double distance2d( Vertexd *f ) { return ddistance( f->x, f->y, x,y ); }
 // Given a triangle, calculate the normal. (Windedness matters here.)
 void TriNormal( Vertexd *a, Vertexd *b, Vertexd *c ) {
  Vertexd U; U.Set(b); U.Subtract(a);
  Vertexd V; V.Set(c); V.Subtract(a);
  x=(U.y * V.z) - (U.z * V.y);
  y=(U.z * V.x) - (U.x * V.z);
  z=(U.x * V.y) - (U.y * V.x);
  Normalize();
 }

 void operator() ( float x, float y, float z ) { Setf(x,y,z); }
 void operator() ( double x, double y, double z ) { Set(x,y,z); }
};

class Vertices;
class Polygons;
ONE(Quad,{})
 Vertexd TL, TR, BR, BL;
 Vertexd tcTL, tcTR, tcBR, tcBL;
 Quad( Quad *Q ) : ListItem() { Set(Q); DefaultTexCoords(); }
 void DefaultTexCoords() {
  tcTL.x=0.0; tcTL.y=1.0;
  tcTR.x=1.0; tcTR.y=1.0;
  tcBR.x=1.0; tcBR.y=0.0;
  tcBL.x=1.0; tcBL.y=0.0;
 }
 void Set(Quad *q) {
  TL.Set(&q->TL);
  TR.Set(&q->TR);
  BR.Set(&q->BR);
  BL.Set(&q->BL);
  tcTL.Set(&q->tcTL);
  tcTR.Set(&q->tcTR);
  tcBR.Set(&q->tcBR);
  tcBL.Set(&q->tcBL);
 }
 void vertices( Vertices *out );
 void triangles( Polygons *out );
 KEYWORDS({
  KEYWORD("TL",{TL.fromString(w);})
  KEYWORD("TR",{TR.fromString(w);})
  KEYWORD("BR",{BR.fromString(w);})
  KEYWORD("BL",{BL.fromString(w);})
 })
 string toString() {
  return FORMAT("TL {%s} TR {%s} BR {%s} BL {%s}",
   TL.toString().c_str(),
   TR.toString().c_str(),
   BR.toString().c_str(),
   BL.toString().c_str()
  );
 }
 void Lerp( Quad *b, double time, Quad *out );
 void Render();
 void RenderYInvert();
MANY(Quad,QuadHandle,QuadHandles,"Quad",Quads,{})
 void triangles( Polygons *out ) {
  FOREACH(Quad,q) q->triangles(out);
 }
DONE(Quad);