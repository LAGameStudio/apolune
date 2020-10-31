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
 *********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                    *
 *********************************************************************************************/
 /* Six functions written by Jerome Jacovella-St. Louis (c) 2009 and perhaps earlier.  I didn't have to
    put this notice here, but out of courtesy I did. */

#pragma once

#include <iostream>
#include <string>
using namespace std;

#include <Windows.h>
#include <stdio.h>
#include <math.h>

#include "GLWindow.h"
#include "GLImage.h"

#include "GLSetup.h"
#include <glm/glm.hpp>

#include "macros.h"
#include "Strings.h"
#include "moremath.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "Matrix16.h"
#include "Matrix.h" // Generic Matrix Class (Black-and-White author)
#include "Crayon.h"
#include "Numbers.h"
#include "Vertexd.h"
#include "BinaryFile.h"
#include "Art.h"
#include "UglyFont.h"

class Circles;
class TweenHandles;

enum AxisTypes { axis_x=0, axis_y=1, axis_z=2, axis_w=3 };
enum PlaneTypes { plane_xy=0, plane_yz=1, plane_xz=2 }; 

enum VertexSortMethods { 
 VertexSortYAscending=0,
 VertexSortZAscending=1,
 VertexSortXAscending=2,
 VertexSortClockwise=3
};

struct Coord2i {
 int x,y;
 Coord2i() { Set(0,0); }
 Coord2i( int x, int y ) { Set(x,y); }
 void Set( int x, int y ) { this->x=x; this->y=y; }
};

struct Dim2i {
 int w,h;
 Dim2i() { Set(0,0); }
 Dim2i( int w, int h ) { Set(w,h); }
 void Set( int w, int h ) { this->w=w; this->h=h; }
};

struct Dim2d {
 double w,h;
 Dim2d() { Set(0,0); }
 Dim2d( double w, double h ) { Set(w,h); }
 void Set( double w, double h ) { this->w=w; this->h=h; }
};

struct Coord2ui {
 unsigned int x,y;
 Coord2ui() { Set(0,0); }
 Coord2ui( unsigned int x, unsigned int y ) { Set(x,y); }
 void Set( unsigned int x, unsigned int y ) { this->x=x; this->y=y; }
};

struct Coord3ui {
 unsigned int w,h,d;
 Coord3ui() { w=h=d=0; }
 Coord3ui( unsigned int x, unsigned int y, unsigned int z ) { Set(x,y,z); }
 void Set( unsigned int x, unsigned int y, unsigned int z ) { w=x; h=y; d=z; }
};

struct Coord2f {
 float x,y;
 Coord2f( float a, float b ) { Set(a,b); }
 Coord2f() { Set(0.0f,0.0f); }
 void Set( float a, float b ) { x=a; y=b; }
};

struct AABBf {
 GLfloat ax,ay,bx,by, az,bz;
 AABBf() { ax=ay=bx=by=0.0f; az=bz=0.0f; }
 bool Within( float tx, float ty ) { return WITHIN(tx,ty,ax,ay,bx,by); }
};

struct Plane3d {
 double a,b,c;
};

class Vertices;
class Vector;
class Vertexd;

// Used for binary files, related to VBOs sometimes
enum VertexFormat {
 VF_V=0,
 VF_VC=1,
 VF_VT2=2,
 VF_VT2C=3,
 VF_VNT2=4,
 VF_VNT3=5,
 VF_VNT4=6,
 VF_VN=7,
 VF_VNC=8,
 VF_VNT2C=9,
 VF_VNT3C=10,
 VF_VNT4C=11,
 VF_VNT2TaC=12,
 VF_VNT3TaC=13,
 VF_VNT4TaC=14,
 VF_VNT2Ta=15,
 VF_Verbose=16 // Saves everything possible
};

enum VertexFlags {
 vertex_edge=Zuint_0,
 vertex_contour_end=Zuint_1,
 vertex_contour_interior=Zuint_2,
};

class Vertex : public ListItem {
public:
 Zstring hc, vc;  // *vc is the hash code of the target in a vector
 GLfloat x,y,z,w;
 GLfloat A,B,C;  // used for triangulation
 Zuint c;
 GLfloat rgba[4];  // color
 Zp<void> data;
// GLfloat *u,*v;
 // Texcoords
 GLfloat r,q,s,t;
 Zdisposable<Vertex> normal;
 Zdisposable<Vertex> vector;
 Zdisposable<Vertex> tangent;
 Zdisposable<Vertex> bitangent;
 Integers indices;
 Zfloat R,a,la;
 Zuint flags;   // Could be VertexFlags, could be used for other things.

 Vertex() { Constructor(); }
 Vertex(float xx, float yy );
 Vertex(float xx, float yy, float zz);
 Vertex(float xx, float yy, float zz, float ww);
 Vertex(double xx, double yy );
 Vertex(double xx, double yy, double zz);
 Vertex(double xx, double yy, double zz, double ww);
 Vertex(int xx, int yy );
 Vertex(int xx, int yy, int zz );
 Vertex(int xx, int yy, int zz, int ww);
 Vertex( Vertex *v ) {
  Constructor();
  v->Duplicate(this);
 }
 Vertex( Vertexd *v );

 void Constructor() {
  x=y=z=w=A=B=C=0.0f;
  rgba[0]=rgba[1]=rgba[2]=rgba[3]=1.0f;
  r=s=t=0.0f;
  q=1.0f;
 }

 void Destructor() {
  DeleteAssociatedData();
 }

 virtual ~Vertex(void) { Destructor(); }

 void TexCoord( int x, int y       ) { TexCoord((float)x,(float)y); }
 void TexCoord( double x, double y ) { TexCoord((float)x,(float)y); }
 void TexCoord( float x, float y ) {
  this->r=x;
  this->q=y;
 }
 void TexCoord( int x, int y, int z, int w ) { TexCoord((float)x,(float)y); }
 void TexCoord( double x, double y, double z, double w ) { TexCoord((float)x,(float)y); }
 void TexCoord( float x, float y, float z, float w ) {
  this->r=x;
  this->q=y;
  this->s=z;
  this->t=w;
 }

// This is not recommended as it seems to consume a large amount of memory
 void fromSimpleString( const char *s ) {
  Zstring split(s);
  const char *werd=split.Next();
  x=(GLfloat) atof(werd);
  werd=split.Next();
  y=(GLfloat) atof(werd);
  werd=split.Next();
  z=(GLfloat) atof(werd);
 }

 SKEYSTART
  SKEY("hc", { SPARAMS(hc); });
  SKEY("vc", { SPARAMS(vc); });
  SKEY("x",  { SPARAM(x,GLfloat,atof); });
  SKEY("y",  { SPARAM(y,GLfloat,atof); });
  SKEY("z",  { SPARAM(z,GLfloat,atof); });
  SKEY("w",  { SPARAM(w,GLfloat,atof); });
  SKEY("a",  { SPARAM(A,GLfloat,atof); });
  SKEY("b",  { SPARAM(B,GLfloat,atof); });
  SKEY("c",  { SPARAM(C,GLfloat,atof); });
  SKEY("ci", { SPARAM(c,unsigned int,atoi); });
  SKEY("rgba",    {
   SKEYPAIR;
   string param;
   const char *q=key.c_str();
   q=string_argument(q,&param); rgba[0]=(GLfloat) atof(param.c_str());
   q=string_argument(q,&param); rgba[1]=(GLfloat) atof(param.c_str());
   q=string_argument(q,&param); rgba[2]=(GLfloat) atof(param.c_str());
   q=string_argument(q,&param); rgba[3]=(GLfloat) atof(param.c_str());
  });
  // ignore void *data
  SKEY("r",       { SPARAM(r,GLfloat,atof); });
  SKEY("q",       { SPARAM(q,GLfloat,atof); });
  SKEY("s",       { SPARAM(s,GLfloat,atof); });
  SKEY("t",       { SPARAM(t,GLfloat,atof); });
  SKEY("normal",    { if ( !normal ) normal.Instantiate();       SPARAMZd(Vertex,normal); });
  SKEY("vector",    { if ( !vector ) vector.Instantiate();       SPARAMZd(Vertex,vector); });
  SKEY("tangent",   { if ( !tangent ) tangent.Instantiate();     SPARAMZd(Vertex,tangent); });
  SKEY("bitangent", { if ( !bitangent ) bitangent.Instantiate(); SPARAMZd(Vertex,tangent); });
  SKEY("indices", { SPARAMType(indices); });
  SKEY("capr",    { SPARAM(R,GLfloat,atof); });
  SKEY("angle",   { SPARAM(a,GLfloat,atof); });
  SKEY("la",      { SPARAM(la,GLfloat,atof); });
  SKEY("flags",   { SPARAM(flags,int,atoi); });
 SKEYEND("Vertex");
 SOUTSTART
  if ( x != 0.0f ) SOUTV("x",float,x,"%1.9f");
  if ( y != 0.0f ) SOUTV("y",float,y,"%1.9f");
  if ( z != 0.0f ) SOUTV("z",float,z,"%1.9f");
  if ( w != 0.0f ) SOUTV("w",float,w,"%1.9f");
  if ( A != 0.0f ) SOUTV("a",float,A,"%1.9f");
  if ( B != 0.0f ) SOUTV("b",float,B,"%1.9f");
  if ( C != 0.0f ) SOUTV("c",float,C,"%1.9f");
  if ( c != 0    ) SOUTV("ci",int,c,"%d");
  if ( rgba[0] != 1.0f || rgba[1] != 1.0f || rgba[2] != 1.0f || rgba[3] != 1.0f ) {
   char buf[128];
   FORMAT(buf,128,"rgba { %1.9f %1.9f %1.9f %1.9f }",
    (float) rgba[0], (float) rgba[1], (float) rgba[2], (float) rgba[3]);
   SOUTbuf;
  }
  if ( r != 0.0f ) SOUTV("r",float,r,"%1.9f");
  if ( q != 1.0f ) SOUTV("q",float,q,"%1.9f");
  if ( s != 0.0f ) SOUTV("s",float,s,"%1.9f");
  if ( t != 0.0f ) SOUTV("t",float,t,"%1.9f");
  if ( normal    ) SOUT("normal",normal);
  if ( vector    ) SOUT("vector",vector);
  if ( tangent   ) SOUT("tangent",tangent);
  if ( indices.count > 0 ) SOUTT("indices",indices);
  if ( R != 0.0f )  SOUTV("capr",float,R,"%1.9f");
  if ( a != 0.0f )  SOUTV("angle",float,a,"%1.9f");
  if ( la != 0.0f ) SOUTV("la",float,la,"%1.9f");
  if ( flags != 0 ) SOUTV("flags",int,flags,"%d");
 SOUTEND;

 void BinaryWrite( BinaryFile *open ) {
  open->scrawl(hc.value);
  open->scrawl(vc.value);
  open->scrawl(x);
  open->scrawl(y);
  open->scrawl(z);
  open->scrawl(q);
  open->scrawl(r);
  open->scrawl(s);
  open->scrawl(t);
  open->scrawl(rgba[0]);
  open->scrawl(rgba[1]);
  open->scrawl(rgba[2]);
  open->scrawl(rgba[3]);
 }

 void BinaryRead( BinaryFile *open ) {
  float k=0.0f;
  open->read(&hc.value);
  open->read(&vc.value);
  open->Fread(1,&k,sizeof(float)); x=(GLfloat) k;
  open->Fread(1,&k,sizeof(float)); y=(GLfloat) k;
  open->Fread(1,&k,sizeof(float)); z=(GLfloat) k;
  open->Fread(1,&k,sizeof(float)); q=(GLfloat) k;
  open->Fread(1,&k,sizeof(float)); r=(GLfloat) k;
  open->Fread(1,&k,sizeof(float)); s=(GLfloat) k;
  open->Fread(1,&k,sizeof(float)); t=(GLfloat) k;
  open->Fread(1,&k,sizeof(float)); rgba[0]=(GLfloat) k;
  open->Fread(1,&k,sizeof(float)); rgba[1]=(GLfloat) k;
  open->Fread(1,&k,sizeof(float)); rgba[2]=(GLfloat) k;
  open->Fread(1,&k,sizeof(float)); rgba[3]=(GLfloat) k;
 }

 void Read_V( BinaryFile *open ) {
  float k;
  open->read(&k); x=(GLfloat) k;
  open->read(&k); y=(GLfloat) k;
  open->read(&k); z=(GLfloat) k;
 }
 void Write_V( BinaryFile *open ) {
  float k;
  k=x; open->write(&k);
  k=y; open->write(&k);
  k=z; open->write(&k);
 }
 void Read_VC( BinaryFile *open ) {
  float k;
  open->read(&k); x=(GLfloat) k;
  open->read(&k); y=(GLfloat) k;
  open->read(&k); z=(GLfloat) k;
  open->read(&k); rgba[0]=(GLfloat) k;
  open->read(&k); rgba[1]=(GLfloat) k;
  open->read(&k); rgba[2]=(GLfloat) k;
  open->read(&k); rgba[3]=(GLfloat) k;
 }
 void Write_VC( BinaryFile *open ) {
  float k;
  k=x; open->write(&k);
  k=y; open->write(&k);
  k=z; open->write(&k);
  k=rgba[0]; open->write(&k);
  k=rgba[1]; open->write(&k);
  k=rgba[2]; open->write(&k);
  k=rgba[3]; open->write(&k);
 }
 void Read_VT2( BinaryFile *open ) {
  float k;
  open->read(&k); x=(GLfloat) k;
  open->read(&k); y=(GLfloat) k;
  open->read(&k); z=(GLfloat) k;
  open->read(&k); q=(GLfloat) k;
  open->read(&k); r=(GLfloat) k;
 }
 void Write_VT2( BinaryFile *open ) {
  float k;
  k=x; open->write(&k);
  k=y; open->write(&k);
  k=z; open->write(&k);
  k=q; open->write(&k);
  k=r; open->write(&k);
 }
 void Read_VT2C( BinaryFile *open ) {
  float k;
  open->read(&k); x=(GLfloat) k;
  open->read(&k); y=(GLfloat) k;
  open->read(&k); z=(GLfloat) k;
  open->read(&k); q=(GLfloat) k;
  open->read(&k); r=(GLfloat) k;
  open->read(&k); rgba[0]=(GLfloat) k;
  open->read(&k); rgba[1]=(GLfloat) k;
  open->read(&k); rgba[2]=(GLfloat) k;
  open->read(&k); rgba[3]=(GLfloat) k;
 }
 void Write_VT2C( BinaryFile *open ) {
  float k;
  k=x; open->write(&k);
  k=y; open->write(&k);
  k=z; open->write(&k);
  k=q; open->write(&k);
  k=r; open->write(&k);
  open->read(&k); rgba[0]=(GLfloat) k;
  open->read(&k); rgba[1]=(GLfloat) k;
  open->read(&k); rgba[2]=(GLfloat) k;
  open->read(&k); rgba[3]=(GLfloat) k;
 }
 void Read_VNT2( BinaryFile *open ) {
 }
 void Write_VNT2( BinaryFile *open ) {
 }
 void Read_VNT3( BinaryFile *open ) {
 }
 void Write_VNT3( BinaryFile *open ) {
 }
 void Read_VNT4( BinaryFile *open ) {
 }
 void Write_VNT4( BinaryFile *open ) {
 }
 void Read_VN( BinaryFile *open ) {
 }
 void Write_VN( BinaryFile *open ) {
 }
 void Read_VNC( BinaryFile *open ) {
  float k;
  open->read(&k); x=(GLfloat) k;
  open->read(&k); y=(GLfloat) k;
  open->read(&k); z=(GLfloat) k;
  open->read(&k); q=(GLfloat) k;
  open->read(&k); r=(GLfloat) k;
  if ( !normal ) normal=this->Normal();
  open->read(&k); normal->x=(GLfloat) k;
  open->read(&k); normal->y=(GLfloat) k;
  open->read(&k); normal->z=(GLfloat) k;
  open->read(&k); rgba[0]=(GLfloat) k;
  open->read(&k); rgba[1]=(GLfloat) k;
  open->read(&k); rgba[2]=(GLfloat) k;
  open->read(&k); rgba[3]=(GLfloat) k;
 }
 void Write_VNC( BinaryFile *open ) {
  float k;
  k=x; open->write(&k);
  k=y; open->write(&k);
  k=z; open->write(&k);
  if ( !normal ) normal=this->Normal();
  k=normal->x; open->write(&k);
  k=normal->y; open->write(&k);
  k=normal->z; open->write(&k);
  k=rgba[0]; open->write(&k);
  k=rgba[1]; open->write(&k);
  k=rgba[2]; open->write(&k);
  k=rgba[3]; open->write(&k);
 }
 void Read_VNT2C( BinaryFile *open ) {
 }
 void Write_VNT2C( BinaryFile *open ) {
 }
 void Read_VNT3C( BinaryFile *open ) {
 }
 void Write_VNT3C( BinaryFile *open ) {
 }
 void Read_VNT4C( BinaryFile *open ) {
 }
 void Write_VNT4C( BinaryFile *open ) {
 }
 void Read_VNT2TaC( BinaryFile *open ) {
 }
 void Write_VNT2TaC( BinaryFile *open ) {
 }
 void Read_VNT3TaC( BinaryFile *open ) {
 }
 void Write_VNT3TaC( BinaryFile *open ) {
 }
 void Read_VNT4TaC( BinaryFile *open ) {
 }
 void Write_VNT4TaC( BinaryFile *open ) {
 }
 void Read_VNT2Ta( BinaryFile *open ) {
 }
 void Write_VNT2Ta( BinaryFile *open ) {
 }
 void Read_Verbose( BinaryFile *open ) {
 }
 void Write_Verbose( BinaryFile *open ) {
 }

 virtual void CopyAssociatedData() {}
 Vertex *Copy();

 virtual void DeleteAssociatedData() {}

 void Duplicate( Vertex *dest );
// Vertex *VertexNormal( PolygonList *polygons ) {} // finds the average of all related polygon normals
 Vertex *Normal();
 Vertex *Tangent( Vertex *a, Vertex *b );
 Vertex *BiTangent( Vertex *a, Vertex *b );
 void TangentBasis( Vertex *a, Vertex *b );
 Vertex *Normal( Vertex *a, Vertex *b, Vertex *c );
 Vertex *TriNormal( Vertex *a, Vertex *b, Vertex *c );
 void TriNormal( Vertexd *a, Vertexd *b, Vertexd *c );
 void SetTriNormal( Vertex *a, Vertex *b, Vertex *c );
 Vertex *calcTangentVector(Vertex *v1, Vertex *v2, Vertex *v3, Vertex *normal);
 Vertex *calcTangentVector(Vertexd *v1, Vertexd *v2, Vertexd *v3, Vertexd *normal);
 void MoveBy(float byx,float byy,float byz);
 void MoveBy(float byx,float byy,float byz,float byw);
 bool Equals( Vertex *c ) {
  return ( c->x==x && c->y==y && c->z==z );
 }
 Vertex *Midpoint( Vertex *b ) {
  Vertex *store=new Vertex;
  store->x= (x+b->x)/2.0f;
  store->y= (y+b->y)/2.0f;
  store->z= (z+b->z)/2.0f;
  return store;
 }
 void Midpoint( Vertex *b, Vertex *store ) {
  store->x= (x+b->x)/2.0f;
  store->y= (y+b->y)/2.0f;
  store->z= (z+b->z)/2.0f;
 }
 void Add(Vertex *v) { x+=v->x; y+=v->y; z+=v->z; }
 void Add(Vertex *vector1, Vertex *vector2);
 void Subtract(Vertex *v) { x-=v->x; y-=v->y; z-=v->z; }
 void Subtract(Vertex *vector1, Vertex *vector2);
 void Scale(Vertex *vector1, float s);
 void MultMatrix(Vertex *source, Matrix16 *matrix);
 void DivideMatrix(Vertex *source, Matrix16 *matrix);
 float length() { return (float)sqrt(x * x + y * y + z * z); }
 float lengthApprox() {
  float ix = abs(x), iy = abs(y), iz = abs(z);
  float tmp;
  if(ix < iy) { tmp = ix; ix = iy; iy = tmp; }
  if(ix < iz) { tmp = ix; ix = iz; iz = tmp; }
  if(iz > iy) { iz = iy; }
  return ix + (iz/2);
 }  
 Vertex CrossProduct(Vertex *V1, Vertex *V2) {
  Vertex r;
  r.x = V1->y * V2->z - V1->z * V2->y;
  r.y = V1->z * V2->x - V1->x * V2->z;
  r.z = V1->x * V2->y - V1->y * V2->x;
  return r;
 }
 void StoreCrossProduct(Vertex *V1, Vertex *V2) {
  x = V1->y * V2->z - V1->z * V2->y;
  y = V1->z * V2->x - V1->x * V2->z;
  z = V1->x * V2->y - V1->y * V2->x;
 }
 inline float DotProduct (Vertex *V2) {
  return x * V2->x + y * V2->y + z * V2->z;	
 }
 inline float Magnitude (Vertex *V2);
 Vertex *Normalize(Vertex *V2);
 void normalize();
 Vertex *rotateXY(Vertex *basePoint, float rotationAngle);
 Vertex *rotateZY(Vertex *basePoint, float rotationAngle);
 Vertex *rotateXZ(Vertex *basePoint, float rotationAngle);
 Vertex *rotateXY(Vertex *basePoint, double rotationRadians);
 Vertex *rotateZY(Vertex *basePoint, double rotationRadians);
 Vertex *rotateXZ(Vertex *basePoint, double rotationRadians);
 void setPosition(Vertex *b);
 void getRandomVector( void ) {
  // Pick a random Z between -1.0f and 1.0f.
  z = (float) (random(2.0)-1.0);// getRandomMinMax( -1.0f, 1.0f );
  
  // Get radius of this circle
  float radius = (float)sqrt(1-z*z);
  
  // Pick a random point on a circle.
  float t = (float) (random(TWO_PI)-PI); //getRandomMinMax( -PI, PI );

  // Compute matching X and Y for our Z.
  x = (float)cosf(t) * radius;
  y = (float)sinf(t) * radius;
 }
 void Invert() {
  x=-x;
  y=-y;
  z=-z;
 }
 void multiplyf(float f) {
  x*=f;
  y*=f;
  z*=f;
 }
 void Negate() {
  x=-x;
  y=-y;
  z=-z;
 }
 void Flip() {
  x=-x;
  y=-y;
  z=-z;
 }

	bool isZero()	{	return x == 0.0f && y == 0.0f && z == 0.0f;	}

 void Divide(Vertex *divisor) {
  if ( divisor->x == 0 ) x=0.0f; else x/=divisor->x;
  if ( divisor->y == 0 ) y=0.0f; else y/=divisor->y;
  if ( divisor->z == 0 ) z=0.0f; else z/=divisor->z;
 }

 void Divide(Vertexd *divisor);
 void Divide( float X, float Y, float Z );
 void Divide( float d );
 void Divide( double d );

 void None() { x=0.0f; y=0.0f; z=0.0f; }
 void Set( float X, float Y, float Z ) {x=X;y=Y;z=Z;}
 void Set( float X, float Y, float Z, float W ) {x=X;y=Y;z=Z;w=W;}
 void Set( double X, double Y, double Z ) {x=(float)X;y=(float)Y;z=(float)Z;}
 void Set( int X, int Y ) {x=(float)X; y=(float)Y; }
 void Set( int X, int Y, int Z ) {x=(float)X; y=(float)Y;z=(float)Z;}
 void Set( float X, float Y ) {x=X;y=Y;}
 void Set( double X, double Y ) {x=(float)X;y=(float)Y;}
 void Set( Vertex *p );
 void Set( const Vertex& p );
	void setX( float X )	{x=X;}
	void setY( float Y )	{y=Y;}
	void setZ( float Z ) {z=Z;}

 void Color( double rr, double gg, double bb, double aa ) {
  Color( (float) rr, (float) gg, (float) bb, (float) aa );
 }

 void ColorAbs( double rr, double gg, double bb, double aa ) {
  Color( (float) abs(rr), (float) abs(gg), (float) abs(bb), (float) aa );
 }

 void Color( float rr, float gg, float bb, float aa ) {
  rgba[0]=rr;
  rgba[1]=gg;
  rgba[2]=bb;
  rgba[3]=aa;
 }

 void Color( Crayon c ) {
  rgba[0]=c.rf;
  rgba[1]=c.gf;
  rgba[2]=c.bf;
  rgba[3]=c.af;
 }
 
 void ColorMultiply( Crayon c ) {
  rgba[0]*=c.rf;
  rgba[1]*=c.gf;
  rgba[2]*=c.bf;
  rgba[3]*=c.af;
 }

 void Alpha( float a ) {
  rgba[3]=a;
 }

	void add( Vertex *p )      { x+=p->x; y+=p->y; z+=p->z; }
	void subtract( Vertex *p )	{ x-=p->x; y-=p->y; z-=p->z; }
	  
	void Add( float a, float b, float c )   		 { x+=a; y+=b; z+=c; } 
	void Subtract( float a, float b, float c ) { x-=a; y-=b; z-=c; } 

	void Add( double a, double b, double c )   		 { x+=(float)a; y+=(float)b; z+=(float)c; } 
	void Subtract( double a, double b, double c ) { x-=(float)a; y-=(float)b; z-=(float)c; } 

	Vertex *multiplyBy( float f )			     { x*=f; y*=f; z*=f; return this; }

 void Scale( double x, double y, double z ) { Scale((float)x,(float)y,(float)z); }
 void Scale( float x, float y, float z ) {
  this->x*=x;
  this->y*=y;
  this->z*=z;
 }

 // Rotates about the origin, openGL-style (right handed, Z Y X order)
 void Rotate( double x, double y, double z ) { Rotate((float)x,(float)y,(float)z); }
 void Rotate( float ax, float ay, float az ) {
  static Vertex temp;
  ax=deg2rad(ax);
  ay=deg2rad(ay);
  az=deg2rad(az);     
  // Rotate by az around Z axis
  temp.Set(
   x*cos(az) - y*sin(az),
   x*sin(az) + y*cos(az),
   z
  );
  x=temp.x;
  y=temp.y;
  z=temp.z;
  // Rotate by ay around Y axis
  temp.Set(
   z*sin(ay) + x*cos(ay),
   y,
   z*cos(ay) - x*sin(ay)
  );
  x=temp.x;
  y=temp.y;
  z=temp.z;
  // Rotate by ax around X axis
  temp.Set(
   x,
   y*cos(ax) - z*sin(ax),
   y*sin(ax) + z*cos(ax)
  );
  x=temp.x;
  y=temp.y;
  z=temp.z;
 }
	
	float distanceTo( Vertex *p )     		 { return (float)sqrt( distanceSquaredTo( p ) ); }

 float distanceSquaredTo( Vertex *p ) {
  float dx = (float) (x-p->x);
  float dy = (float) (y-p->y);
  float dz = (float) (z-p->z);
  return (float) (dx*dx + dy*dy + dz*dz);
 }
	
	float DistanceTo( float x, float y, float z )	{
		float dx = this->x - x;
		float dy = this->y - y;
		float dz = this->z - z;
		return (float) sqrt( dx*dx + dy*dy + dz*dz );
	}
	
	float dot( Vertex p )     	{ return x*p.x + y*p.y + z*p.z; }
	float lengthSquared()						{ return x*x + y*y + z*z; }
	  
	void clear()             		{ x = 0.0f; y = 0.0f; z = 0.0f; }

	//String toString()              				{ return new String( "(" + x + ", " + y + ", " + z + ")" ); }

 // We consider p as the B vector and this as the A vector
	Vertex *cross( Vertex *p ) {
	 Vertex *a=new Vertex;
  a->x=this->y*p->z - this->z*p->y;
	 a->y=this->z*p->x - this->x*p->z;
	 a->z=this->x*p->y - this->y*p->x;
  return a;
	}

// z' = z*cos q - x*sin q
// x' = z*sin q + x*cos q
// y' = y
 void rotateAround( Vertex *point, Euler *by ) {
  static Vertex origin;
  this->Subtract(this,point);
  this->rotateXY(&origin,rad2deg((float) by->roll));
  this->rotateZY(&origin,rad2deg((float) by->pitch));
  this->Add(this,point);
 }

 void RotateCentroid( float x, float y, float z, Vertex *centroid ) {
  Euler by((double)deg2rad(x),(double)deg2rad(y),(double)deg2rad(z));
  this->rotateXY(centroid,z);
  this->rotateZY(centroid,y);
  this->rotateXZ(centroid,x);
 }

 Vertex fastNormalize();

 void RenderImmediate() {
  glNormal3f(normal->x,normal->y,normal->z);
  glTexCoord2f(r,q);
  glColor4fv(rgba);
  glVertex3f(x,y,z);
 }

	operator const float *() const {	return &x;	}
	float &operator [] (const int index);
	void operator += (const Vertex &v);
	void operator -= (const Vertex &v);
	void operator *= (const float scalar);
	void operator /= (const float dividend);
 void operator = (const Vertex &b ) {
  hc = b.hc;
  vc = b.vc;
  x=b.x;  y=b.y;  z=b.z;
  A=b.A;  B=b.B;  C=b.C;
  c=b.c;
  rgba[0] = b.rgba[0]; 
  rgba[1] = b.rgba[1]; 
  rgba[2] = b.rgba[2]; 
  rgba[3] = b.rgba[3]; 
  q=b.q;
  r=b.r;
  s=b.s;
  t=b.t;
  a=b.a;
  la=b.la;
//  data=null;
// dest->u=u;  dest->v=v;
// if ( normal ) dest->normal = normal->Copy();
// if ( vector ) dest->vector = vector->Copy();
  R=b.R;
  flags=b.flags;
 }// Duplicate, not duplicating normals or barycenters

};

bool operator == (const Vertex &v0, const Vertex &v1);
bool operator != (const Vertex &v0, const Vertex &v1);

Vertex operator + (const Vertex &u, const Vertex &v);        // Add to v
Vertex operator - (const Vertex &u, const Vertex &v);        // Subtract u from v
Vertex operator - (const Vertex &v);                         // Invert v
float  operator * (const Vertex &u, const Vertex &v);        // Fast Distance
Vertex operator * (const float scalar, const Vertex &v);     // Multiply by scalar
Vertex operator * (const Vertex &v, const float scalar);     // Multiply by scalar
Vertex operator / (const Vertex &v, const float dividend);   // Divide by scalar
Vertex operator % (const Vertex &a, const Vertex &b );       // Cross product

class Vector : public ListItem {
public:
 Vertices *points; 
 float x, y, z;
 // constructors
 Vector() : ListItem() { Set(0, 0, 0); }
 Vector(float _x, float _y, float _z) : ListItem() { Set(_x, _y, _z); }
 Vector(Vector &v) : ListItem() { Set(v.x, v.y, v.z); }

 Vertex *midpoint( ) {
  Vertex *m=new Vertex;
  // ...
  return m;
 }
 
 void Versor2d() {
  x/=sqrt(pow(x,2)+pow(y,2));
  y/=sqrt(pow(x,2)+pow(y,2));
 }

 inline float DotProduct (Vertex *V1, Vertex *V2)		// Calculate The Angle Between The 2 Vectors
 {
	 return V1->x * V2->x + V1->y * V2->y + V1->z * V2->z;		
 }

 inline float Magnitude (Vertex *V1, Vertex *V2)				// Calculate The Length Of The Vector
 {
	 return sqrtf (V1->x * V2->x + V1->y * V2->y + V1->z * V2->z);	
 }

 // Assumes the origin
 Vertex *Normalize (Vertex *V1, Vertex *V2)		// Creates A Vector With A Unit Length Of 1
 {
 	float M = Magnitude (V1, V2);	// Calculate The Length Of The Vector 
  Vertex *v=new Vertex;
 	if (M != 0.0f)			// Make Sure We Don't Divide By 0 
 	{
   v->x = V1->x / M;		// Normalize The 3 Components 
	  v->y = V1->y / M;
	  v->z = V1->z / M;
	 } else { v->x=V1->x; v->y=V1->y; v->z=V1->z; }
  return v;
 }

 // operators
 Vector operator +(Vector &v);	// return VectorAdd(this, v)
 void operator +=(Vector &v);	// this = VectorAdd(this, v)
 
 Vector operator -(Vector &v);	// return VectorSub(this, v)
 void operator -=(Vector &v);	// this = VectorSub(this, v)
 
 Vector operator *(Matrix &m);	// return MatrixMulVector(this, m)
 Vector operator *(Vector &v);	// return VectorMul(this, v)
 Vector operator *(float f);		// return VectorScale(this, f)
 void operator *=(Matrix &m);	// this = MatrixMulVector(this, m)
 void operator *=(Vector &v);	// this = VectorMul(this, v)
 void operator *=(float f);		// this = VectorScale(this, f)
 
 Vector operator ^(Vector &v);	// return VectorCross(this, v)
 void operator ^=(Vector &v);	// this = VectorCross(this, v)
 
 Vector operator !();			// return VectorNegate(this)
 
 // functions
 void	Set(float _x, float _y, float _z);
 float	Dot(Vector &v);
 float	SelfDot();
 Vector	Normalize();
 void	SelfNormalize();
 float	Length();
 float	Distance(Vector &v);
 Vector	Lerp(Vector &v, float f);
 void	SelfLerp(Vector &v, float f);
 
 float	*GetPtr() { return (float*)this; };
 
};

class Vectors : public LinkedList {
public:
 Vector *Add() {

 }
 CLEARLISTRESET(Vector);
 ~Vectors() { Clear(); }
};

enum Windings {
 clockwise=0,
 counterclockwise=1
};

class VertexHandle : public Handle<Vertex> {
public:
 VertexHandle( Vertex * v ) : Handle() { p=v; }
};
class VertexHandles : public LinkedList {
public:
 CLEARLISTRESET(VertexHandle);
 void Push( Vertex *v ) {
  Append(new VertexHandle(v));
 }
 void Pop( Vertex *v ) {
  EACHN(first,VertexHandle,vh,{
   if ( vh->p == v ) {
    Remove(vh);
    delete vh;
   }
  });
 }
 bool Contains( Vertex *v ) {
  FOREACH(VertexHandle,vh) if ( vh->p == v ) return true;
  return false;
 }
 bool Contains( float x, float y, float z ) {
  FOREACH(VertexHandle,vh) if ( vh->p->x == x && vh->p->y == y && vh->p->z == z ) return true;
  return false;
 }
 void PopNear( float boxRange, Vertex *v ) {
  EACHN(first,VertexHandle,vh,{
   if ( (vh->p->x-boxRange < v->x) && (vh->p->x+boxRange > v->x)
     && (vh->p->y-boxRange < v->y) && (vh->p->y+boxRange > v->y)
     && (vh->p->z-boxRange < v->z) && (vh->p->z+boxRange > v->z) ) {
    Remove(vh);
    delete vh;
   }
  });
 }
 void Combine( VertexHandles *input ) {
  EACHN(input->first,VertexHandle,vh,{
   input->Remove(vh);
   Append(vh);
  });
 }
 void Combine( VertexHandles *input, bool Delete ) {
  EACHN(input->first,VertexHandle,vh,{
   input->Remove(vh);
   Append(vh);
  });
  if ( Delete ) delete input;
 }
};

enum CurveRelativePositionTest {
 is_curve_error=-1,
 is_above_curve=0,
 is_below_curve=1,
 is_on_curve=3
};

class Vertices : public LinkedList {
public:
 VertexSortMethods sortBy;
 Vertices() : LinkedList() { sortBy=VertexSortYAscending; }
 Vertices( Vertices *in ) : LinkedList() {
  Add(in);
  sortBy=VertexSortYAscending;
 }
 Vertices( Vertices *in, Vertexd *translation ) : LinkedList() {
  sortBy=VertexSortYAscending;
  EACH(in->first,Vertex,v) {
   Append(
    new Vertex(
     (float)((double)v->x+translation->x),
     (float)((double)v->y+translation->y),
     (float)((double)v->z+translation->z)
    )
   );
  }
 }
 void Rotate( double x, double y, double z ) {
  FOREACH(Vertex,v) v->Rotate((float)x,(float)y,(float)z);
 }
 void Rotate( float x, float y, float z ) {
  FOREACH(Vertex,v) v->Rotate(x,y,z);
 }
 SORTING(Vertex,Sort,{},{
  switch ( sortBy ) {
   case VertexSortXAscending: if ( A->x<B->x ) result=-1; break;
   case VertexSortYAscending: if ( A->y<B->y ) result=-1; break;
   case VertexSortZAscending: if ( A->z<B->z ) result=-1; break;
   case VertexSortClockwise:  if ( A->a<B->a ) result=-1; break;
  }
 },{})
 void MoveBy( float dx, float dy, float dz ) {
  FOREACH(Vertex,v) v->Add(dx,dy,dz);
 }
 void Divide( Vertex * divisor ) {
  FOREACH(Vertex,v) v->Divide(divisor);
 }
 void Color( Crayon c ) {
  FOREACH(Vertex,v) v->Color(c);
 }
 void ColorMultiply( Crayon c ) {
  FOREACH(Vertex,v) v->ColorMultiply(c);
 }
 bool within(double x,double y);
 // Queries a position returning the two vertices of the line segment list (lineloop) and the closest point on the line
 bool PointLineQuery( double x, double y, double z, Vertex **outA, Vertex **outB, Vertex *out, double *outDist );
 // Treats the vertices as a segment series and returns the Vertex on the line based on the time element.
 void GetPoint( Vertex * out, double time );
 void GetPoint( Vertex * out, double time, TweenHandles *tween );
 bool isWoundClockwise() {
 	// Calculating the polygon area. A > 0 means polygon counter clockwise oriented
 	int area = 0;
  FOREACH(Vertex,v) if ( v->next ) {
   Vertex *w=(Vertex *) (v->next);
   area += (int)(v->x * w->y - w->x * v->y);
  }
  // Connect first and last
  {
   Vertex *v=(Vertex *) (last);
   Vertex *w=(Vertex *) (first);
   area += (int)(v->x * w->y - w->x * v->y);
  }
 	area /= 2;
 	return !(area > 0);
 }
 void CopyAppend( Vertices *in ) {
  EACH(in->first,Vertex,v) Append(v->Copy());
 }
 void CopyAppendIfUnique( Vertices *in ) {
  EACH(in->first,Vertex,v) if ( !this->Contains(v) ) Append(v->Copy());
 }
 void Add( Vertices *in ) {
  EACH(in->first,Vertex,v) {
   Append(
    new Vertex(
     (float)((double)v->x),
     (float)((double)v->y),
     (float)((double)v->z)
    )
   );
  }
 }
 void Add( Vertices *in, double scale ) {
  EACH(in->first,Vertex,v) {
   Append(
    new Vertex(
     (float)((double)v->x*scale),
     (float)((double)v->y*scale),
     (float)((double)v->z*scale)
    )
   );
  }
 }
 bool inList( Vertex *p ) { FOREACH(Vertex,v) if ( p==v ) return true; return false; }
 bool Contains( Vertex *in ) {
  FOREACH(Vertex,v) {
   if ( in->Equals(v) ) return true;
  }
  return false;
 }
 bool AddIfUnique( Vertex *in ) {
  FOREACH(Vertex,v) {
   if ( in->Equals(v) ) return false;
  }
  Append(in);
  return true;
 }
 void Addition( float x, float y, float z ) {
  FOREACH(Vertex,v) v->Add(x,y,z);
 }
 void Addition( double x, double y, double z ) {
  FOREACH(Vertex,v) v->Add(x,y,z);
 }
 void Translate( double x, double y, double z ) {
  FOREACH(Vertex,v) v->Add(x,y,z);
 }
 void Multiply( float f ) {
  FOREACH(Vertex,v) v->multiplyf(f);
 }
 void Multiply( float a, float b, float c ) {
  FOREACH(Vertex,v) {
   v->x*=a;
   v->y*=b;
   v->z*=c;
  }
 }
 void Multiply( double a, double b, double c ) {
  FOREACH(Vertex,v) {
   v->x*=(float)a;
   v->y*=(float)b;
   v->z*=(float)c;
  }
 }
 void ReadAppend( BinaryFile *file ) {
  Vertex *v=new Vertex;
  v->BinaryRead(file);
  Append(v);
 }
 
 void DebugDraw2DInvertY( float ox, float oy, float sx, float sy ) {
  Zuint i;
  FOREACH(Vertex,v) if ( v->next ) {
   Vertex *w=(Vertex *) v->next;
   DrawLine2Color(crayons.Pick(red255),crayons.Pick(blue255),v->x*sx+ox,-v->y*sy+oy,w->x*sx+ox,-w->y*sy+oy);
   crayons.Pick(green255).gl();
   Text(i.toString(),(double)(v->x*sx+ox-8),(double)(-v->y*sy+oy),7,10,false);
   i++;
  } else {
   Vertex *w=(Vertex *) first;
   Text(i.toString(),(double)v->x*sx+ox-8,(double)-v->y*sy+oy,7,10,false);
   DrawLine2Color(crayons.Pick(red255),crayons.Pick(blue255),v->x*sx+ox,-v->y*sy+oy,w->x*sx+ox,-w->y*sy+oy);
   i++;
  }
 }

 void DebugDraw2D( float ox, float oy, float sx, float sy ) {
  Zuint i;
  FOREACH(Vertex,v) if ( v->next ) {
   Vertex *w=(Vertex *) v->next;
   DrawLine2Color(crayons.Pick(red255),crayons.Pick(blue255),v->x*sx+ox,v->y*sy+oy,w->x*sx+ox,w->y*sy+oy);
   crayons.Pick(green255).gl();
   Text(i.toString(),(double)v->x*sx+ox-8,(double)v->y*sy+oy,7,10,false);
   i++;
  } else {
   Vertex *w=(Vertex *) first;
   Text(i.toString(),(double)v->x*sx+ox-8,(double)-v->y*sy+oy,7,10,false);
   DrawLine2Color(crayons.Pick(red255),crayons.Pick(blue255),v->x*sx+ox,v->y*sy+oy,w->x*sx+ox,w->y*sy+oy);
   i++;
  }
 }

 void DrawInvertY( Crayon color, float ox, float oy, float sx, float sy, bool drawBaseline=false ) {
  Zuint i;
  FOREACH(Vertex,v) if ( v->next ) {
   Vertex *w=(Vertex *) v->next;
   DrawLine(color,v->x*sx+ox,-v->y*sy+oy,w->x*sx+ox,-w->y*sy+oy);
   i++;
  } else if ( drawBaseline ) {
   Vertex *w=(Vertex *) first;
   DrawLine(color,v->x*sx+ox,-v->y*sy+oy,w->x*sx+ox,-w->y*sy+oy);
   i++;
  }
 }

 void Draw( Crayon color, float ox, float oy, float sx, float sy, bool drawBaseline=false ) {
  Zuint i;
  FOREACH(Vertex,v) if ( v->next ) {
   Vertex *w=(Vertex *) v->next;
   DrawLine(color,v->x*sx+ox,v->y*sy+oy,w->x*sx+ox,w->y*sy+oy);
   i++;
  } else if ( drawBaseline ) {
   Vertex *w=(Vertex *) first;
   DrawLine(color,v->x*sx+ox,v->y*sy+oy,w->x*sx+ox,w->y*sy+oy);
   i++;
  }
 }

 void DebugDraw( float offsetx, float offsety, float scalex, float scaley ) {
  Zstring s=string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");
  glPush();
  glMatrixMode(GL_MODELVIEW);
  glTranslatef( offsetx, offsety, 0.0f );
  unsigned int i=0;
  FOREACH(Vertex,v) if ( v->next ) {
   Vertex *n=(Vertex *) (v->next);
   glPushMatrix();
   glTranslatef( v->x*scalex,v->y*scaley,v->z );
   glColor3d(1.0,1.0,1.0);
   Text(FORMAT("%c%d",s(i),i).c_str(),0,0,5,6,true,0.0);
   glPopMatrix();
   glBegin(GL_LINES);
   glColor3d(1.0,0.0,1.0);
   glVertex3f(v->x*scalex,v->y*scaley,v->z);
   glColor3d(0.0,1.0,0.0);
   glVertex3f(n->x*scalex,n->y*scaley,n->z);
   glEnd();
   i++;
  } else {
   glPushMatrix();
   glTranslatef( v->x*scalex,v->y*scaley,v->z );
   glColor3d(1.0,1.0,1.0);
   Text(FORMAT("%c%d",s(i),i).c_str(),0,0,5,6,true,0.0);
   glPopMatrix();
  }
  glPop();
 }

 void DebugDraw( float offsetx, float offsety, float offsetz, float scalex, float scaley, float scalez ) {
  Zstring s=string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");
  glPush();
  glMatrixMode(GL_MODELVIEW);
  glTranslatef( offsetx, offsety, offsetz );
  unsigned int i=0;
  FOREACH(Vertex,v) if ( v->next ) {
   Vertex *n=(Vertex *) (v->next);
   glPushMatrix();
   glTranslatef( v->x*scalex,v->y*scaley,v->z*scalez );
   glColor3d(1.0,1.0,1.0);
   Text(FORMAT("%c%d",s(i),i).c_str(),0,0,5,6,true,0.0);
   glPopMatrix();
   glBegin(GL_LINES);
   glColor3d(1.0,0.0,1.0);
   glVertex3f(v->x*scalex,v->y*scaley,v->z*scalez);
   glColor3d(0.0,1.0,0.0);
   glVertex3f(n->x*scalex,n->y*scaley,n->z*scalez);
   glEnd();
   i++;
  } else {
   glPushMatrix();
   glTranslatef( v->x*scalex,v->y*scaley,v->z*scalez );
   glColor3d(1.0,1.0,1.0);
   Text(FORMAT("%c%d",s(i),i).c_str(),0,0,5,6,true,0.0);
   glPopMatrix();
  }
  glPop();
 }

 bool isPolygon() {
  return (count >= 3);
 }
 // Assumption: all points are coplanar already; ignores Vertex Z
 // 0 = inconclusive, 1 = convex, 2 = concave
 int isConvex() {
  if ( !isPolygon() ) {
   OUTPUT("Vertices::isConvex() didn't have enough points to make a determination.\n");
   return 0;
  }
  int flag=0;
  double z=0.0;
  for (int i=0; i<count;i++) {
   Vertex *v=(Vertex *) Element( (i) );
   Vertex *j=(Vertex *) Element( (i + 1) % count );
   Vertex *k=(Vertex *) Element( (i + 2) % count );
   z  = (j->x - v->x) * (k->y - j->y);
   z -= (j->y - v->y) * (k->x - j->x);
   if (z < 0)        flag |= 1;
   else if (z > 0)   flag |= 2;
   if (flag == 3) return 2;
  }
  if (flag != 0) return 1;
  else return 0;
 }

 /*
  * Sorts vertices on the 2d XY-plane in counterclockwise order.
  * Reverse resulting list for clockwise.  (cx,cy) is the centroid,
  * the same point one would use to create GL_TRI_FAN from a GL_POLYGON.
  */
 void Wind2d( float cx, float cy ) {
  Cartesian calculator;
  FOREACH(Vertex,v) {
   calculator.fx=cx;
   calculator.fy=cy;
   calculator.fw=v->x;
   calculator.fh=v->y;
   v->la=calculator.LineAnglef();
  }
  Sort();
 }
 /* Assuming points are coplanar, sort vertices on the common plane
  * Reverse resulting list for clockwise.  Provide the centroid
  * and common plane.
  * This is used to convert PLY quads into triangles, by "RebuildAsTriangleMesh"
  */
// void Wind3d( Vertex *centroid, ) {
// }
 Vertex *Add( float x, float y ) {
  Vertex *v=new Vertex;
  v->x=x;
  v->y=y;
  Append(v);
  return v;
 }
 Vertex *addIfUnique( float x, float y ) {
  bool found=false;
  FOREACH(Vertex,v) if ( v->x==x && v->y==y ) found=true;
  if ( !found ) return Add(x,y);
  return null;
 }
 Vertex *Add( double x, double y, double z, bool edge ) {
  return Add( (float) x, (float) y, (float) z, edge );
 }
 Vertex *Add( double x, double y, double z ) {
  return Add( (float) x, (float) y, (float) z );
 }
 Vertex *Add( float x, float y, float z, bool edge ) {
  Vertex *v=new Vertex;
  v->x=x;
  v->y=y;
  v->z=z;
  v->flags.Toggle(vertex_edge,edge);
  Append(v);  
  return v;
 }
 Vertex *Add( float x, float y, float z ) {
  Vertex *v=new Vertex;
  v->x=x;
  v->y=y;
  v->z=z;
  Append(v);  
  return v;
 }
 string toString() {
  string r=string("");
  EACH(first,Vertex,v) r+=string(" {")+v->toString()+string("},\n");
  return r;
 }
 void fromString( const char *s ) {
  const char *p=s;
  string buf;
  while ( *p != '\0' ) {
   p=string_argument(p,&buf);
   if ( buf.length() > 0 ) {
    Vertex *v= new Vertex;
    v->fromString(buf.c_str());
    Append(v);
   }
  }
 }
 void centroid( Vertex *store ) {
  FOREACH(Vertex,v) {
   store->x+=v->x;
   store->y+=v->y;
   store->z+=v->z;
  }
  store->x/=(float)count;
  store->y/=(float)count;
  store->z/=(float)count;
 }
 Vertex *centroid() {
  Vertex *c=new Vertex;
  FOREACH(Vertex,v) {
   c->x+=v->x;
   c->y+=v->y;
   c->z+=v->z;
  }
  c->x/=(float)count;
  c->y/=(float)count;
  c->z/=(float)count;
  return c;
 }
 Vertex *closestToZero() {
  Vertex *candidate=null;
  FOREACH(Vertex,v) {
   if ( !candidate ) candidate=v;
   else {
    if ( abs(v->x) < abs(candidate->x)
      && abs(v->y) < abs(candidate->y)
      && abs(v->z) < abs(candidate->z) )
     candidate=v;
   }
  }
  return candidate;
 }
 Vertex *closest( Vertex *t ) {
  Vertex *candidate=null;
  float d=10000.0f;
  FOREACH(Vertex,v) {
   if ( t->Equals(v) || t==v ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,v->z,t->x,t->y,t->z);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }
 Vertex *closest( Vertex *t, float dfar ) {
  Vertex *candidate=null;
  float d=dfar;
  FOREACH(Vertex,v) {
   if ( t->Equals(v) || t==v ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,v->z,t->x,t->y,t->z);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }
 Vertex *closest2d( Vertex *t ) {
  Vertex *candidate=null;
  float d=10000.0f;
  FOREACH(Vertex,v) {
   if ( t->Equals(v) || t==v ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,t->x,t->y);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }
 Vertex *closest2d( Vertex *t, float dfar ) {
  Vertex *candidate=null;
  float d=dfar;
  FOREACH(Vertex,v) {
   if ( t->Equals(v) || t==v ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,t->x,t->y);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }
 Vertex *greatestX() {
  Vertex *candidate=null;
  FOREACH(Vertex,v) if ( !candidate ) candidate=v; else if ( v->x > candidate->x ) candidate=v;
  return candidate;
 }
 Vertex *greatestY() {
  Vertex *candidate=null;
  FOREACH(Vertex,v) if ( !candidate ) candidate=v; else if ( v->y > candidate->y ) candidate=v;
  return candidate;
 }
 Vertex *greatestZ() {
  Vertex *candidate=null;
  FOREACH(Vertex,v) if ( !candidate ) candidate=v; else if ( v->z > candidate->z ) candidate=v;
  return candidate;
 }
 Vertex *leastX() {
  Vertex *candidate=null;
  FOREACH(Vertex,v) if ( !candidate ) candidate=v; else if ( v->x < candidate->x ) candidate=v;
  return candidate;
 }
 Vertex *leastY() {
  Vertex *candidate=null;
  FOREACH(Vertex,v) if ( !candidate ) candidate=v; else if ( v->y < candidate->y ) candidate=v;
  return candidate;
 }
 Vertex *leastZ() {
  Vertex *candidate=null;
  FOREACH(Vertex,v) if ( !candidate ) candidate=v; else if ( v->z < candidate->z ) candidate=v;
  return candidate;
 }
 VertexHandles *BelowX( float x ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( v->x < x ) selection->Push(v);
  return selection;
 }
 VertexHandles *AboveX( float x ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( v->x > x ) selection->Push(v);
  return selection;
 }
 VertexHandles *BelowY( float y ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( v->y < y ) selection->Push(v);
  return selection;
 }
 VertexHandles *AboveY( float y ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( v->y > y ) selection->Push(v);
  return selection;
 }
 VertexHandles *BelowZ( float z ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( v->z < z ) selection->Push(v);
  return selection;
 }
 VertexHandles *AboveZ( float z ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( v->z > z ) selection->Push(v);
  return selection;
 }
 VertexHandles *SameOrBelowX( float x ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( v->x <= x ) selection->Push(v);
  return selection;
 }
 VertexHandles *SameOrAboveX( float x ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( v->x >= x ) selection->Push(v);
  return selection;
 }
 VertexHandles *SameOrBelowY( float y ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( v->y <= y ) selection->Push(v);
  return selection;
 }
 VertexHandles *SameOrAboveY( float y ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( v->y >= y ) selection->Push(v);
  return selection;
 }
 VertexHandles *SameOrBelowZ( float z ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( v->z <= z ) selection->Push(v);
  return selection;
 }
 VertexHandles *SameOrAboveZ( float z ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( v->z >= z ) selection->Push(v);
  return selection;
 } 
 VertexHandles *Near( float xNear, float yNear, float zNear, Vertex *p ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v)
   if ( (p->x-xNear < v->x && p->x+xNear > v->x)
     && (p->y-yNear < v->y && p->y+yNear > v->y)
     && (p->z-zNear < v->z && p->z+zNear > v->z) ) selection->Push(v);
  return selection;
 }
 VertexHandles *Near( float dist, Vertex *p ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v)
   if ( fdistance(p->x,p->y,p->z,v->x,v->y,v->z) < dist ) selection->Push(v);
  return selection;
 }
 Vertex *closestToZero( VertexHandles *excluding ) {
  Vertex *candidate=null;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( !candidate ) candidate=v;
   else {
    if ( abs(v->x) < abs(candidate->x)
      && abs(v->y) < abs(candidate->y)
      && abs(v->z) < abs(candidate->z) )
     candidate=v;
   }
  }
  return candidate;
 }
 Vertex *closest( Vertex *t, VertexHandles *excluding ) {
  Vertex *candidate=null;
  float d=10000.0f;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( t->Equals(v) || t==v ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,v->z,t->x,t->y,t->z);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }
 Vertex *closest( Vertex *t, float dfar, VertexHandles *excluding ) {
  Vertex *candidate=null;
  float d=dfar;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( t->Equals(v) || t==v ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,v->z,t->x,t->y,t->z);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }
 Vertex *closest( float x, float y, float z, float dfar, VertexHandles *excluding ) {
  Vertex t(x,y,z);
  Vertex *candidate=null;
  float d=dfar;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( t.Equals(v) ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,v->z,t.x,t.y,t.z);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }
 Vertex *closest( float x, float y, float z, float dfar ) {
  Vertex t(x,y,z);
  Vertex *candidate=null;
  float d=dfar;
  FOREACH(Vertex,v) {
   if ( t.Equals(v) ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,v->z,t.x,t.y,t.z);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }
 // First find largest distance and use that to determine which is closest to the point from the list.
 Vertex *closest( float x, float y, float z ) {
  if ( count == 0 ) return null;
  if ( count == 1 ) return (Vertex *) first;
  Vertex t(x,y,z);
  Vertex *candidate=null;
  float d=0.0f;
  FOREACH(Vertex,v) {
   float dist=fdistance(x,y,z,v->x,v->y,v->z);
   if ( dist > d ) d=dist;
  }
  d+=1.0f;
  FOREACH(Vertex,v) {
   if ( t.Equals(v) ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,v->z,t.x,t.y,t.z);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }
 // First find largest distance and use that to determine which is closest to the point from the list.
 Vertex *closest( float x, float y, float z, float *distOut ) {
  if ( count == 0 ) return null;
  if ( count == 1 ) return (Vertex *) first;
  Vertex t(x,y,z);
  Vertex *candidate=null;
  float d=0.0f;
  FOREACH(Vertex,v) {
   float dist=fdistance(x,y,z,v->x,v->y,v->z);
   if ( dist > d ) d=dist;
  }
  d+=1.0f;
  FOREACH(Vertex,v) {
   if ( t.Equals(v) ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,v->z,t.x,t.y,t.z);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  *distOut=d;
  return candidate;
 }
 // First find largest distance and use that to determine which is closest to the point from the list.
 Vertex *closestX( float x, float *distOut ) {
  if ( count == 0 ) return null;
  if ( count == 1 ) return (Vertex *) first;
  Vertex *candidate=null;
  float d=0.0f;
  FOREACH(Vertex,v) {
   float dist=fdistance(0,x,0,0,v->x,0);
   if ( dist > d ) d=dist;
  }
  d+=1.0f;
  FOREACH(Vertex,v) {
   if ( x == v->x ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(0,v->x,0,0,x,0);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  *distOut=d;
  return candidate;
 }
 // First find largest distance and use that to determine which is closest to the point from the list.
 Vertex *closestY( float y, float *distOut ) {
  if ( count == 0 ) return null;
  if ( count == 1 ) return (Vertex *) first;
  Vertex *candidate=null;
  float d=0.0f;
  FOREACH(Vertex,v) {
   float dist=fdistance(0,y,0,0,v->y,0);
   if ( dist > d ) d=dist;
  }
  d+=1.0f;
  FOREACH(Vertex,v) {
   if ( y == v->y ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(0,v->y,0,0,y,0);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  *distOut=d;
  return candidate;
 }
 // First find largest distance and use that to determine which is closest to the point from the list.
 Vertex *closestZ( float z, float *distOut ) {
  if ( count == 0 ) return null;
  if ( count == 1 ) return (Vertex *) first;
  Vertex *candidate=null;
  float d=0.0f;
  FOREACH(Vertex,v) {
   float dist=fdistance(0,z,0,0,v->z,0);
   if ( dist > d ) d=dist;
  }
  d+=1.0f;
  FOREACH(Vertex,v) {
   if ( z == v->z ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(0,v->z,0,0,z,0);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  *distOut=d;
  return candidate;
 }
 Vertex *closest2d( Vertex *t, VertexHandles *excluding ) {
  Vertex *candidate=null;
  float d=10000.0f;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( t->Equals(v) || t==v ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,t->x,t->y);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }
 Vertex *closest2d( Vertex *t, float dfar, VertexHandles *excluding ) {
  Vertex *candidate=null;
  float d=dfar;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( t->Equals(v) || t==v ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,t->x,t->y);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }
 Vertex *greatestX( VertexHandles *excluding ) {
  Vertex *candidate=null;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( !candidate ) candidate=v; else if ( v->x > candidate->x ) candidate=v;
  }
  return candidate;
 }
 Vertex *greatestY( VertexHandles *excluding ) {
  Vertex *candidate=null;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( !candidate ) candidate=v; else if ( v->y > candidate->y ) candidate=v;
  }
  return candidate;
 }
 Vertex *greatestZ( VertexHandles *excluding ) {
  Vertex *candidate=null;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( !candidate ) candidate=v; else if ( v->z > candidate->z ) candidate=v;
  }
  return candidate;
 }
 Vertex *leastX( VertexHandles *excluding ) {
  Vertex *candidate=null;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if (!candidate ) candidate=v;
   else if ( v->x < candidate->x ) candidate=v;
  }
  return candidate;
 }
 Vertex *leastY( VertexHandles *excluding ) {
  Vertex *candidate=null;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( !candidate ) candidate=v;
   else if ( v->y < candidate->y ) candidate=v;
  }
  return candidate;
 }
 Vertex *leastZ( VertexHandles *excluding ) {
  Vertex *candidate=null;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( !candidate ) candidate=v;
   else if ( v->z < candidate->z ) candidate=v;
  }
  return candidate;
 }
 VertexHandles *BelowX( float x, VertexHandles *excluding ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( v->x < x ) selection->Push(v);
  }
  return selection;
 }
 VertexHandles *AboveX( float x, VertexHandles *excluding ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( v->x > x ) selection->Push(v);
  }
  return selection;
 }
 VertexHandles *BelowY( float y, VertexHandles *excluding ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( v->y < y ) selection->Push(v);
  }
  return selection;
 }
 VertexHandles *AboveY( float y, VertexHandles *excluding ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( v->y > y ) selection->Push(v);
  }
  return selection;
 }
 VertexHandles *BelowZ( float z, VertexHandles *excluding ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( v->z < z ) selection->Push(v);
  }
  return selection;
 }
 VertexHandles *AboveZ( float z, VertexHandles *excluding ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( v->z > z ) selection->Push(v);
  }
  return selection;
 }
 VertexHandles *SameOrBelowX( float x, VertexHandles *excluding ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( v->x <= x ) selection->Push(v);
  }
  return selection;
 }
 VertexHandles *SameOrAboveX( float x, VertexHandles *excluding ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( v->x >= x ) selection->Push(v);
  }
  return selection;
 }
 VertexHandles *SameOrBelowY( float y, VertexHandles *excluding ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( v->y <= y ) selection->Push(v);
  }
  return selection;
 }
 VertexHandles *SameOrAboveY( float y, VertexHandles *excluding ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( v->y >= y ) selection->Push(v);
  }
  return selection;
 }
 VertexHandles *SameOrBelowZ( float z, VertexHandles *excluding ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( v->z <= z ) selection->Push(v);
  }
  return selection;
 }
 VertexHandles *SameOrAboveZ( float z, VertexHandles *excluding ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v) if ( !excluding->Contains(v) ) {
   if ( v->z >= z ) selection->Push(v);
  }
  return selection;
 } 
 VertexHandles *Near( float xNear, float yNear, float zNear, Vertex *p, VertexHandles *excluding ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v)
   if ( !excluding->Contains(v)
     && (p->x-xNear < v->x && p->x+xNear > v->x)
     && (p->y-yNear < v->y && p->y+yNear > v->y)
     && (p->z-zNear < v->z && p->z+zNear > v->z) ) selection->Push(v);
  return selection;
 }
 VertexHandles *Near( float dist, Vertex *p, VertexHandles *excluding ) {
  VertexHandles *selection=new VertexHandles;
  FOREACH(Vertex,v)
   if ( !excluding->Contains(v)
     && fdistance(p->x,p->y,p->z,v->x,v->y,v->z) < dist ) selection->Push(v);
  return selection;
 }
 Vertex *findByColor( float *rgba_in ) {
  FOREACH(Vertex,v)
   if ( v->rgba[0] == rgba_in[0]
     && v->rgba[1] == rgba_in[1]
     && v->rgba[2] == rgba_in[2]
     && v->rgba[3] == rgba_in[3] ) return v;
  return null;
 }
 Vertex *findByColor( float *rgba_in, float nearness ) {
  FOREACH(Vertex,v)
   if ( NEARBY(v->rgba[0],rgba_in[0],nearness)
     && NEARBY(v->rgba[1],rgba_in[1],nearness)
     && NEARBY(v->rgba[2],rgba_in[2],nearness)
     && NEARBY(v->rgba[3],rgba_in[3],nearness) ) return v;
  return null;
 }
 Cartesiand bounds;
 void Bounds() {
  FOREACH(Vertex,v) {
   if ( bounds.x > v->x ) bounds.x=v->x;
   if ( bounds.y > v->y ) bounds.y=v->y;
   if ( bounds.z > v->z ) bounds.z=v->z;
   if ( bounds.x2 < v->x ) bounds.x2=v->x;
   if ( bounds.y2 < v->y ) bounds.y2=v->y;
   if ( bounds.z2 < v->z ) bounds.z2=v->z;
  }
  bounds.Volume();
 }
 bool AxisAlignedBoundingBoxXY( AABBf *out ) {
  if ( first ) {
   out->ax = ((Vertex *)first)->x;
   out->ay = ((Vertex *)first)->y;
   out->bx = ((Vertex *)first)->x;
   out->by = ((Vertex *)first)->y;
   out->az = ((Vertex *)first)->z;
   out->bz = ((Vertex *)first)->z;
  } else return false; // this was an empty list describing a polygon
  FOREACH(Vertex,v) {
   if ( v->x < out->ax ) out->ax=v->x;
   if ( v->y < out->ay ) out->ay=v->y;
   if ( v->x > out->bx ) out->bx=v->x;
   if ( v->y > out->by ) out->by=v->y;
   if ( v->z < out->az ) out->az=v->z;
   if ( v->z > out->bz ) out->bz=v->z;
  }
  return true;
 }
 // This test only can be used if the Vertices contain a curve generated with Curve2d,
 // and the curve must consist of points with integer values (stored as float);
 // this is used to detect pixel-level precision only, and does not operate in 1.0 space,
 // so imagine sparse intervals under the curve.  (Does not use integral calculus)
 // Incoming X must be equal to at least one X in the vertex list
 CurveRelativePositionTest AboveOrBelowTheCurve( int x, int y, Vertex **out ) {
  FOREACH(Vertex,v) {
   int Vx;
   Vx=(int) v->x;
   if ( x == Vx ) {
    int Vy=(int) v->y;
    *out=v;
    if ( Vy == y ) return is_on_curve;
    if ( Vy < y ) return is_below_curve;
    return is_above_curve;
   }
  }
  OUTPUT("Vertices::AboveOrBelowTheCurve(%d,%d) found to be beyond bounds of the curve.\n", x, y );
  return is_curve_error;
 }
 void Debug( double x, double y ) {
  int i=0;
  FOREACH(Vertex,v) {
   Vertex *w=null;
   string name=FORMAT(buf,123,"%d",i);
   DrawNamedPoint(crayons.Pick(gold),name.c_str(),x+v->x,y+v->y);
   if ( v->next ) {
    w=(Vertex *)v->next;
   } else {
    w=(Vertex *)first;
   }
   DrawLine2Color(crayons.Pick(cyan),crayons.Pick(magenta),x+v->x,y+v->y,x+w->x,y+w->y);
   i++;
  }
 }
 void CalcTangentBasis() {
  if ( count == 3 ) {
   Vertex *a=(Vertex *) first;
   Vertex *b=(Vertex *) a->next;
   Vertex *c=(Vertex *) b->next;
   a->TangentBasis( b, c );
   b->TangentBasis( a, c );
   c->TangentBasis( a, b );
  } else if ( count % 3 == 0 && count >= 3 ) {
   int i=0;
   EACH(first,Vertex,a) {
    if ( i%3 == 0 ) {
     Vertex *b=(Vertex *) a->next;
     Vertex *c=(Vertex *) b->next;
     a->TangentBasis( b, c );
     b->TangentBasis( a, c );
     c->TangentBasis( a, b );
    }
    i++;
   }
  } else OUTPUT("Vertices::CalcTangentBasis() could not calculate because no method has been implemented for this criteria.\n");
 }
 bool minimum( Vertex *out ) {
  if ( !first ) return false;
  out->x=((Vertex *)first)->x;
  out->y=((Vertex *)first)->y;
  out->z=((Vertex *)first)->z;
  EACH(first->next,Vertex,a) {
   if ( a->x < out->x ) out->x=a->x;
   if ( a->y < out->y ) out->y=a->y;
   if ( a->z < out->z ) out->z=a->z;
  }
  return true;
 }
 bool maximum( Vertex *out ) {
  if ( !first ) return false;
  out->x=((Vertex *)first)->x;
  out->y=((Vertex *)first)->y;
  out->z=((Vertex *)first)->z;
  EACH(first->next,Vertex,a) {
   if ( a->x > out->x ) out->x=a->x;
   if ( a->y > out->y ) out->y=a->y;
   if ( a->z > out->z ) out->z=a->z;
  }
  return true;
 }
 void LinePoint( float ax, float ay, float az, float bx, float by, float bz, float lerp ) {
  Add( ax+lerp*(bx-ax), ay+lerp*(by-ay), az+lerp*(bz-az) );
 }
 void LinePoint( double ax, double ay, double az, double bx, double by, double bz, double lerp ) {
  Add( ax+lerp*(bx-ax), ay+lerp*(by-ay), az+lerp*(bz-az) );
 }
 void toCirclesX( Circles *out, float unitRadius );
 void toCirclesY( Circles *out, float unitRadius );
 void toCirclesZ( Circles *out, float unitRadius );
 CLEARLISTRESET(Vertex);
 
 void BinaryWrite( BinaryFile *file ) {
  file->scrawl(count.value);
  FOREACH(Vertex,v) v->BinaryWrite(file);
 }
 void BinaryRead( BinaryFile *file ) {
  int total=0;
  file->Fread(1,&total,sizeof(int));
  while ( total > 0 ) {
   Vertex *v=new Vertex;
   v->BinaryRead(file);
   Append(v);
   total--;
  }
 }
};

class VertexList : public LinkedList {
public:
 Vertex *barycenter(void);
 Vertex *leastPoint(void);
 void move( float x, float y, float z );
 void rotateXY( float angleDeg );
 void rotateZY( float angleDeg );
 void Push( Vertex *v );
 VertexHandle *find( Vertex *v );
 void Postpend( VertexList appendix );
 void deleteVertex( Vertex *p );
 bool inList( Vertex *v );
 void addIfUnique( Vertex *v );
 void toggleVertex( Vertex *v );
// void freeListandVertices() { VERTEX_LIST_ITEM vL; for ( vL:this ) delete vL.v; Free(); }
 VertexList *copy(void);
 VertexList *Duplicate(void);
 void setEqual( VertexList *b );

 inline void ScaleX( float factor ) { VertexHandle *vL; for ( vL=(VertexHandle *) first; vL; vL=(VertexHandle *) (vL->next) ) vL->p->x*=factor; }
 inline void ScaleY( float factor ) { VertexHandle *vL; for ( vL=(VertexHandle *) first; vL; vL=(VertexHandle *) (vL->next) ) vL->p->y*=factor; }
 inline void ScaleZ( float factor ) { VertexHandle *vL; for ( vL=(VertexHandle *) first; vL; vL=(VertexHandle *) (vL->next) ) vL->p->z*=factor; }

 inline void UnscaleX( float factor ) { VertexHandle *vL; for ( vL=(VertexHandle *) first; vL; vL=(VertexHandle *) (vL->next) ) vL->p->x/=factor; }
 inline void UnscaleY( float factor ) { VertexHandle *vL; for ( vL=(VertexHandle *) first; vL; vL=(VertexHandle *) (vL->next) ) vL->p->y/=factor; }
 inline void UnscaleZ( float factor ) { VertexHandle *vL; for ( vL=(VertexHandle *) first; vL; vL=(VertexHandle *) (vL->next) ) vL->p->z/=factor; }

 inline void OfsXYZ( float x, float y, float z ) { VertexHandle *vL; for ( vL=(VertexHandle *) first; vL; vL=(VertexHandle *) (vL->next) ) { vL->p->x+=x; vL->p->y+=y; vL->p->z+=z; } }
 inline void OfsX( float factor ) { VertexHandle *vL; for ( vL=(VertexHandle *) first; vL; vL=(VertexHandle *) (vL->next) ) vL->p->x+=factor; }
 inline void OfsY( float factor ) { VertexHandle *vL; for ( vL=(VertexHandle *) first; vL; vL=(VertexHandle *) (vL->next) ) vL->p->y+=factor; }
 inline void OfsZ( float factor ) { VertexHandle *vL; for ( vL=(VertexHandle *) first; vL; vL=(VertexHandle *) (vL->next) ) vL->p->z+=factor; }
 
 void centroid( Vertex *store ) {
  FOREACH(Vertex,v) {
   store->x+=v->x;
   store->y+=v->y;
   store->z+=v->z;
  }
  store->x/=(float)count;
  store->y/=(float)count;
  store->z/=(float)count;
 }
 Vertex *centroid() {
  Vertex *c=new Vertex;
  FOREACH(Vertex,v) {
   c->x+=v->x;
   c->y+=v->y;
   c->z+=v->z;
  }
  c->x/=(float)count;
  c->y/=(float)count;
  c->z/=(float)count;
  return c;
 }
 Vertex *closestToZero() {
  Vertex *candidate=null;
  FOREACH(Vertex,v) {
   if ( !candidate ) candidate=v;
   else {
    if ( abs(v->x) < abs(candidate->x)
      && abs(v->y) < abs(candidate->y)
      && abs(v->z) < abs(candidate->z) )
     candidate=v;
   }
  }
  return candidate;
 }
 Vertex *closest( Vertex *t ) {
  Vertex *candidate=null;
  float d=10000.0f;
  FOREACH(Vertex,v) {
   if ( t->Equals(v) || t==v ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,v->z,t->x,t->y,t->z);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }
 Vertex *closest( Vertex *t, float dfar ) {
  Vertex *candidate=null;
  float d=dfar;
  FOREACH(Vertex,v) {
   if ( t->Equals(v) || t==v ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,v->z,t->x,t->y,t->z);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }
 Vertex *closest2d( Vertex *t ) {
  Vertex *candidate=null;
  float d=10000.0f;
  FOREACH(Vertex,v) {
   if ( t->Equals(v) || t==v ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,t->x,t->y);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }
 Vertex *closest2d( Vertex *t, float dfar ) {
  Vertex *candidate=null;
  float d=dfar;
  FOREACH(Vertex,v) {
   if ( t->Equals(v) || t==v ) continue;
   if ( !candidate ) candidate=v;
   else {
    float dist=fdistance(v->x,v->y,t->x,t->y);
    if ( dist < d ) {
     d=dist;
     candidate=v;
    }
   }
  }
  return candidate;
 }

 CLEARLISTRESET(VertexHandle);
 ~VertexList(void) {
  Clear();
 }

};


/*
static inline bool pointinbox(VERTEX v, VERTEX bo, VERTEX br)
{
    return v.x <= bo.x+br.x &&
           v.x >= bo.x-br.x &&
           v.y <= bo.y+br.y &&
           v.y >= bo.y-br.y &&
           v.z <= bo.z+br.z &&
           v.z >= bo.z-br.z;
}

bool pointincube(VERTEX o, VERTEX r, VERTEX v)
{
    if(!pointinbox(v, o, r)) return false;
    loopi(p.size) if(p.p[i].dist(v)>1e-3f) return false;
    return true;
}
*/

extern Vertex origin;

class Rectd {
public:
 double x,y,z,x2,y2,z2,w,h,d;
 Rectd() { x=y=z=x2=y2=z2=w=h=d=0.0; }
 Rectd( double X, double Y, double Z ) {
  x=X;
  y=Y;
  z=Z;
  x2=y2=z2=w=h=d=0.0;
 }
 Rectd( double X, double Y, double X2, double Y2 ) {
  x=X < X2 ? X : X2;
  y=Y < Y2 ? Y : Y2;
  x2=X2 > X ? X2 : X;
  y2=Y2 > Y ? Y2 : Y;
  w= (x>=0) ? (x2-x) : (x<0 && x2>0) ? (x2+abs(x)) : (x<x2 ? abs(x)-abs(x2) : abs(x2)-abs(x));
  h= (y>=0) ? (y2-y) : (y<0 && y2>0) ? (y2+abs(y)) : (y<y2 ? abs(y)-abs(y2) : abs(y2)-abs(y));
  z=z2=d=0.0;
 }
 void Set( double X, double Y, double Z, double W, double H, double D ) {
  x=X;
  y=Y;
  z=Z;
  w=W;
  h=H;
  d=D;
  x2=X+w;
  y2=Y+h;
  z2=Z+d;
 }
 void Set( double X, double Y, double Z ) {
  x=X;
  y=Y;
  z=Z;
  x2=X+w;
  y2=Y+h;
  z2=Z+d;
 }
 void Set( double X, double Y ) {
  x=X;
  y=Y;
  z=0.0;
  x2=X+w;
  y2=Y+h;
  z2=0.0;
 }
 void None() {
  x=y=z=0.0;
 }
 void Multiply( double factor ) {
  x*=factor;
  y*=factor;
  z*=factor;
 }
};

HANDLED(Vertices,VerticesHandle,VerticesHandles,VerticesHandlesHandle,VerticesHandlesHandles);

// A list of lists of vertices
class VerticesList : public LinkedList {
public:
 CLEARLISTRESET(Vertices);
};

extern Vertex vertexZero;