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
/* In collaboration with: */
/************************************************************** Author: H. Elwood Gilliland III
 *  _____  _                 _____       _                    * (c) 2014 PieceMaker Technologies
 * |  _  ||_| ___  ___  ___ |     | ___ | |_  ___  ___        *----------------------------------
 * |   __|| || -_||  _|| -_|| | | || .'|| '_|| -_||  _|       * 3D Systems STL format handler
 * |__|__ |_||___||___||___||_|_|_||__,||_,_||____|_|         * 
 * |_   _| ___  ___ | |_  ___  ___ | | ___  ___ |_| ___  ___  * 
 *   | |  | -_||  _||   ||   || . || || . || . || || -_||_ -| * 
 *   |_|  |___||___||_|_||_|_||___||_||___||_  ||_||___||___| * 
 *                                         |___|              *
 **************************************************************/

#include "ZeroTypes.h"
#include "BinaryFile.h"
#include "Vertexd.h"
#include "LookAtPerspective.h"
#include "Cartesian.h"
#include "Model.h"
#include "VBO.h"

class VBONormalsPolysGroup;

extern const char STLHeader[80];

class STLFacet : public ListItem {
public:
 Vertexd trinormal;
 Vertexd a,b,c;
 STLFacet() : ListItem() {}
 STLFacet( STLFacet *in ) : ListItem() {
  a.Set(&in->a);
  b.Set(&in->b);
  c.Set(&in->c);
  trinormal.Set(&in->trinormal);
 }
 void Least( Vertexd *out ) {
  out->x=a.x; if ( b.x < out->x ) out->x=b.x; if ( c.x < out->x ) out->x=c.x;
  out->y=a.y; if ( b.y < out->y ) out->y=b.y; if ( c.y < out->y ) out->y=c.y;
  out->z=a.z; if ( b.z < out->z ) out->z=b.z; if ( c.z < out->z ) out->z=c.z;
 }
 void Greatest( Vertexd *out ) {
  out->x=a.x; if ( b.x > out->x ) out->x=b.x; if ( c.x > out->x ) out->x=c.x;
  out->y=a.y; if ( b.y > out->y ) out->y=b.y; if ( c.y > out->y ) out->y=c.y;
  out->z=a.z; if ( b.z > out->z ) out->z=b.z; if ( c.z > out->z ) out->z=c.z;
 }
};

class STLFacets : public LinkedList {
public:
 Vertexd extents;
 CLEARLISTRESET(STLFacet);
 void MoveBy( double x, double y, double z ) {
  FOREACH(STLFacet,f) {
   f->a.Add(x,y,z);
   f->b.Add(x,y,z);
   f->c.Add(x,y,z);
  }
 }
 void Duplicate( STLFacets *in ) {
  EACH(in->first,STLFacet,f) Append(new STLFacet(f));
 }
 void Add( BinaryFile *file ) {
  Vertexd v;
  float fromfile=0.0f;
  unsigned int i=(unsigned int) count;
  STLFacet *f=new STLFacet;
  file->read(&fromfile); v.x=fromfile;
  file->read(&fromfile); v.y=fromfile;
  file->read(&fromfile); v.z=fromfile;
  f->trinormal.Set(&v);
  file->read(&fromfile); v.x=fromfile;
  file->read(&fromfile); v.y=fromfile;
  file->read(&fromfile); v.z=fromfile;
  f->a.Set(&v);
  file->read(&fromfile); v.x=fromfile;
  file->read(&fromfile); v.y=fromfile;
  file->read(&fromfile); v.z=fromfile;
  f->b.Set(&v);
  file->read(&fromfile); v.x=fromfile;
  file->read(&fromfile); v.y=fromfile;
  file->read(&fromfile); v.z=fromfile;
  f->c.Set(&v);
  char empty='\0';
  file->read(&empty);
  file->read(&empty);
  Append(f);
 }
 void Add( Vertexd *a, Vertexd *b, Vertexd *c ) {
  STLFacet *f=new STLFacet;
  f->a.Set(a);
  f->b.Set(b);
  f->c.Set(c);
  f->trinormal.TriNormal(a,b,c);
  Append(f);
 }
 void Add( Vertexd *a, Vertexd *b, Vertexd *c, Vertexd *n ) {
  STLFacet *f=new STLFacet;
  f->a.Set(a);
  f->b.Set(b);
  f->c.Set(c);
  f->trinormal.TriNormal(a,b,c);
  Append(f);
 }
 void Add( Vertex *a, Vertex *b, Vertex *c, Vertex *n ) {
  STLFacet *f=new STLFacet;
  f->a.Set(a);
  f->b.Set(b);
  f->c.Set(c);
  f->trinormal.TriNormal(&f->a,&f->b,&f->c);
  Append(f);
 }
 void PushTriangle( Vertex *A, Vertex *B, Vertex *C, Vertex *N ) {
  STLFacet *f=new STLFacet;
  f->a.Set(A);
  f->b.Set(B);
  f->c.Set(C);
  f->trinormal.Set(N);
  Append(f);
 }
 void PushTri( VBOStaticVCLines *normals, VBOStaticVC *polys, Vertexd *a, Vertexd *b, Vertexd *c ) {
  Crayon color; color.ColorInterp( crayons.Pick(skyBlue), crayons.Pick(cyan), (float) abs( b->z/extents.z ) );
  mPolygon tri;
  Vertex normalVector;
  Vertex *A,*B,*C;
  color.ColorInterp( crayons.Pick(skyBlue), crayons.Pick(cyan), (float) abs( b->z/extents.z ) );
  B=tri.points.Add(b->x,b->y,b->z); /*B->Color(0.0,1.0,1.0,1.0);*/ B->ColorAbs(color.rf,color.gf,color.bf,1.0); B->q=1.0; B->r=0.0; //B->Normal();// B->normal->Invert();
  color.ColorInterp( crayons.Pick(macNcheese), crayons.Pick(cyan),  (float) abs( c->z/extents.z ) );
  C=tri.points.Add(c->x,c->y,c->z); /*C->Color(1.0,0.0,1.0,1.0);*/ C->ColorAbs(color.rf,color.gf,color.bf,1.0); C->q=1.0; C->r=0.0; //C->Normal();// C->normal->Invert();
  color.ColorInterp( crayons.Pick(magenta), crayons.Pick(cyan),  (float) abs( a->z/extents.z ) );
  A=tri.points.Add(a->x,a->y,a->z); /*A->Color(1.0,0.0,0.0,1.0);*/ A->ColorAbs(color.rf,color.gf,color.bf,1.0); A->q=0.0; A->r=0.0; //A->Normal();// A->normal->Invert();
  tri.Barycenter(); tri.TriNormal(); tri.normal->normalize(); /*tri.normal->Negate();*/ tri.bary->Color(1.0,0.0,0.0,1.0); normals->PushVertex(tri.bary);
  normalVector.Set(tri.normal.pointer); normalVector.Add(tri.bary); normalVector.Color(0.0,1.0,0.0,1.0); normals->PushVertex(&normalVector);
//  vbo.PushTriangle(&tri);
  polys->PushTriangle(&tri);
 }
 void PushTri( VBONormalsPolysGroup *vbos, Vertexd *a, Vertexd *b, Vertexd *c );
 void PushTri( VBONormalsPolysGroup *vbos, Vertexd *a, Vertexd *b, Vertexd *c, Crayon tint );
 
 Vertexd least,greatest,centroid;
 void CalculateExtents() {
  Vertexd v;
  if ( count == 0 ) return;
  centroid.Set(0,0,0);
  least.Set( &((STLFacet *) first)->a );
  greatest.Set( &((STLFacet *) first)->a );
  FOREACH(STLFacet,facet) {
   centroid.x+=v.x;
   centroid.y+=v.y;
   centroid.z+=v.z;
   facet->Least(&v);
   if ( v.x < least.x ) least.x=v.x;
   if ( v.y < least.y ) least.y=v.y;
   if ( v.z < least.z ) least.z=v.z;
   facet->Greatest(&v);
   if ( v.x > greatest.x ) greatest.x=v.x;
   if ( v.y > greatest.y ) greatest.y=v.y;
   if ( v.z > greatest.z ) greatest.z=v.z;
  }
  centroid.Divide((double)count);
  extents.Set( abs(greatest.x-least.x), abs(greatest.y-least.y), abs(greatest.z-least.z) );
 }
 void toVBOGroup( VBONormalsPolysGroup *vbos, bool reverseWinding=false );
 void toVBOGroup( VBONormalsPolysGroup *vbos, bool reverseWinding, Crayon tint );
 void toVBO( VBOStaticVCLines *normals, VBOStaticVC *polys ) {
  OUTPUT("STLFacets: Calculating extents.\n");
  CalculateExtents();
  normals->Clear();
  polys->Clear();
  OUTPUT("STLFacets: Preparing VBOs...");
  FOREACH(STLFacet,facet) {
   PushTri( normals, polys, &(facet->a), &(facet->b), &(facet->c) );
  }
  normals->Create();
  polys->Create();
  OUTPUT("ready.\n");
 }
 string asString() {
  string out=string("solid model\n");
  FOREACH(STLFacet,facet) {
   out+=FORMAT("facet normal %f %f %f\n\touter loop\n\t\tvertex %f %f %f\n\t\tvertex %f %f %f\n\t\tvertex %f %f %f\n\tendloop\nendfacet\n",
    facet->trinormal.x,facet->trinormal.y,facet->trinormal.z,
    facet->a.x,facet->a.y,facet->a.z,
    facet->b.x,facet->b.y,facet->b.z,
    facet->c.x,facet->c.y,facet->c.z
   );
  }
  return out;
 }
 bool write( BinaryFile *out ) {
  bool error=false;
  FOREACH(STLFacet,facet) {
   out->scrawl((float)(facet->trinormal.x));
   out->scrawl((float)(facet->trinormal.y));
   out->scrawl((float)(facet->trinormal.z));
   out->scrawl((float)(facet->a.x));
   out->scrawl((float)(facet->a.y));
   out->scrawl((float)(facet->a.z));
   out->scrawl((float)(facet->b.x));
   out->scrawl((float)(facet->b.y));
   out->scrawl((float)(facet->b.z));
   out->scrawl((float)(facet->c.x));
   out->scrawl((float)(facet->c.y));
   out->scrawl((float)(facet->c.z));
   out->scrawl((char)0);
   out->scrawl((char)0);
  }
  OUTPUT("STLFile.write() wrote %d facets to '%s'\n", (int)count, out->filename.c_str() );
  return error;
 }
 // Unlike in an STL file (use write for that), BinaryWrite writes the facets in a compact format with a length
 // Use BinaryWrite to write a binary-safe (no checksum) block of triangles with normals only, no padding
 void BinaryWrite( BinaryFile *file ) {
  bool error=false;
  file->scrawl((int)count.value);
  FOREACH(STLFacet,facet) {
   file->scrawl((facet->trinormal.x));
   file->scrawl((facet->trinormal.y));
   file->scrawl((facet->trinormal.z));
   file->scrawl((facet->a.x));
   file->scrawl((facet->a.y));
   file->scrawl((facet->a.z));
   file->scrawl((facet->b.x));
   file->scrawl((facet->b.y));
   file->scrawl((facet->b.z));
   file->scrawl((facet->c.x));
   file->scrawl((facet->c.y));
   file->scrawl((facet->c.z));
  }
  OUTPUT("STLFile.BinaryWrite(`%s`) wrote %d facets\n", file->filename.c_str(), (int)count );  
 }
 void BinaryRead( BinaryFile *file ) {
  int length=0;
  file->read(&length);
  while ( length > 0 ) {
   STLFacet *facet=new STLFacet;
   file->read(&(facet->trinormal.x));
   file->read(&(facet->trinormal.y));
   file->read(&(facet->trinormal.z));
   file->read(&(facet->a.x));
   file->read(&(facet->a.y));
   file->read(&(facet->a.z));
   file->read(&(facet->b.x));
   file->read(&(facet->b.y));
   file->read(&(facet->b.z));
   file->read(&(facet->c.x));
   file->read(&(facet->c.y));
   file->read(&(facet->c.z));
   Append(facet);
   length--;
  }
  OUTPUT("STLFile.BinaryRead(`%s`) read %d facets\n", file->filename.c_str(), (int)count );  
 }
 void toModel( ImmediateModel *model, bool toUnitCube=false ) {
  if ( toUnitCube ) {
   CalculateExtents();
   Vertex box((float)extents.x,(float)extents.y,(float)extents.z);
   FOREACH(STLFacet,f) {
    mPolygon *tri=new mPolygon();
    Vertex *v;
    v=tri->addVertex(&f->a); v->Divide(&box);
    v=tri->addVertex(&f->b); v->Divide(&box);
    v=tri->addVertex(&f->c); v->Divide(&box);
    tri->normal=new Vertex((float)(f->trinormal.x),(float)(f->trinormal.y),(float)(f->trinormal.z));
    model->addPolygon(tri);
   }
  } else FOREACH(STLFacet,f) {
   mPolygon *tri=new mPolygon();
   tri->addVertex(&f->a);
   tri->addVertex(&f->b);
   tri->addVertex(&f->c);
   tri->normal=new Vertex((float)f->trinormal.x,(float)f->trinormal.y,(float)f->trinormal.z);
   model->addPolygon(tri);
  }
  model->BoxBounds();
  model->ColorMapVertices();
 }
 void toModelTM( ImmediateModel *model, bool toUnitCube=false ) {
  if ( toUnitCube ) {
   CalculateExtents();
   Vertex box((float)extents.x,(float)extents.y,(float)extents.z);
   FOREACH(STLFacet,f) {
    mPolygon *tri=new mPolygon();
    Vertex *v;
    v=tri->addVertex(&f->a); v->Divide(&box);
    v=tri->addVertex(&f->b); v->Divide(&box);
    v=tri->addVertex(&f->c); v->Divide(&box);
    tri->normal=new Vertex((float)(f->trinormal.x),(float)(f->trinormal.y),(float)(f->trinormal.z));
    model->addPolygon(tri);
   }
  } else FOREACH(STLFacet,f) {
   mPolygon *tri=new mPolygon();
   tri->addVertex(&f->a);
   tri->addVertex(&f->b);
   tri->addVertex(&f->c);
   tri->normal=new Vertex((float)f->trinormal.x,(float)f->trinormal.y,(float)f->trinormal.z);
   model->addPolygon(tri);
  }
  model->BoxBounds();
  model->ColorTransitiveMapVertices();
 }
 void toModel( ImmediateModel *model, Crayon vertexColor, bool toUnitCube=false ) {
  if ( toUnitCube ) {
   CalculateExtents();
   Vertex box((float)extents.x,(float)extents.y,(float)extents.z);
   FOREACH(STLFacet,f) {
    mPolygon *tri=new mPolygon();
    Vertex *v;
    v=tri->addVertex(&f->a); v->Divide(&box); v->Color(vertexColor);
    v=tri->addVertex(&f->b); v->Divide(&box); v->Color(vertexColor);
    v=tri->addVertex(&f->c); v->Divide(&box); v->Color(vertexColor);
    tri->normal=new Vertex((float)(f->trinormal.x),(float)(f->trinormal.y),(float)(f->trinormal.z));
    model->addPolygon(tri);
   }
  } else FOREACH(STLFacet,f) {
   mPolygon *tri=new mPolygon();
   tri->addVertex(&f->a,vertexColor);
   tri->addVertex(&f->b,vertexColor);
   tri->addVertex(&f->c,vertexColor);
   tri->normal=new Vertex((float)f->trinormal.x,(float)f->trinormal.y,(float)f->trinormal.z);
   model->addPolygon(tri);
  }
  model->BoxBounds();
  model->ColorMapVertices();
 }
 void toVBOStaticVNTC( VBOStaticVNTC *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  vbo->Clear();
  if ( toUnitCube ) {
   if ( extents.x == 0 || extents.y == 0 || extents.z == 0 ) this->CalculateExtents();
   Vertexd A,B,C;
   FOREACH(STLFacet,f) {
    A.Set(&f->a); A.Divide(extents.x);
    B.Set(&f->b); B.Divide(extents.y);
    C.Set(&f->c); C.Divide(extents.z);
    vbo->PushTriangle(&A,&A,&B,&B,&C,&C,&f->trinormal); // Map vertex to texture coordinates
   }
  } else {   
   Vertexd A,B,C;
   FOREACH(STLFacet,f) {
    A.Set(&f->a); A.Divide(extents.x);
    B.Set(&f->b); B.Divide(extents.y);
    C.Set(&f->c); C.Divide(extents.z);
    vbo->PushTriangle(&f->a,&A,&f->b,&B,&f->c,&C,&f->trinormal);  
   }
  }
  vbo->Create();
  if ( clearcpu ) vbo->ClearCPU();
 }
 void toVBOStaticVNT( VBOStaticVNT *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  vbo->Clear();
  if ( toUnitCube ) {
   if ( extents.x == 0 || extents.y == 0 || extents.z == 0 ) this->CalculateExtents();
   Vertexd A,B,C;
   FOREACH(STLFacet,f) {
    A.Set(&f->a); A.Divide(extents.x);
    B.Set(&f->b); B.Divide(extents.y);
    C.Set(&f->c); C.Divide(extents.z);
    vbo->PushTriangle(&A,&A,&B,&B,&C,&C,&f->trinormal);  
   }
  } else {
   Vertexd A,B,C;
   FOREACH(STLFacet,f) {
    A.Set(&f->a); A.Divide(extents.x);
    B.Set(&f->b); B.Divide(extents.y);
    C.Set(&f->c); C.Divide(extents.z);
    vbo->PushTriangle(&f->a,&A,&f->b,&B,&f->c,&C,&f->trinormal);  
   }
  }
  vbo->Create();
  if ( clearcpu ) vbo->ClearCPU();
 }
 void toVBOStaticVT( VBOStaticVT *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  vbo->Clear();
  if ( toUnitCube ) {
   if ( extents.x == 0 || extents.y == 0 || extents.z == 0 ) this->CalculateExtents();
   Vertexd A,B,C;
   FOREACH(STLFacet,f) {
    A.Set(&f->a); A.Divide(extents.x);
    B.Set(&f->b); B.Divide(extents.y);
    C.Set(&f->c); C.Divide(extents.z);
    vbo->PushTriangle(&A,&A,&B,&B,&C,&C,&f->trinormal);  
   }
  } else {
   Vertexd A,B,C;
   FOREACH(STLFacet,f) {
    A.Set(&f->a); A.Divide(extents.x);
    B.Set(&f->b); B.Divide(extents.y);
    C.Set(&f->c); C.Divide(extents.z);
    vbo->PushTriangle(&f->a,&A,&f->b,&B,&f->c,&C,&f->trinormal);  
   }
  }
  vbo->Create();
  if ( clearcpu ) vbo->ClearCPU();
 }
 void toVBOStaticVNC( VBOStaticVNC *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  vbo->Clear();
  Crayon gray(0.7f,0.7f,0.7f,1.0f);
  if ( toUnitCube ) {
  if ( extents.x == 0 || extents.y == 0 || extents.z == 0 ) this->CalculateExtents();
   Vertexd A,B,C;
   FOREACH(STLFacet,f) {
    Vertex N(f->trinormal.x,f->trinormal.y,f->trinormal.z);
    A.Set(&f->a); A.Divide(extents.x);
    B.Set(&f->b); B.Divide(extents.y);
    C.Set(&f->c); C.Divide(extents.z);
    vbo->PushTriangle(&A,gray,&B,gray,&C,gray,&f->trinormal);
   }
  } else {
   FOREACH(STLFacet,f) {
    vbo->PushTriangle(&f->a,gray,&f->b,gray,&f->c,gray,&f->trinormal);
   }
  }
  vbo->Create();
  if ( clearcpu ) vbo->ClearCPU();
 }
 void toVBOStaticVN( VBOStaticVN *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  vbo->Clear();
  if ( toUnitCube ) {
   if ( extents.x == 0 || extents.y == 0 || extents.z == 0 ) this->CalculateExtents();
   Vertexd A,B,C;
   FOREACH(STLFacet,f) {
    A.Set(&f->a); A.Divide(extents.x);
    B.Set(&f->b); B.Divide(extents.y);
    C.Set(&f->c); C.Divide(extents.z);
    vbo->PushTriangle(&A,&B,&C,&f->trinormal);  
   }
  } else {
   FOREACH(STLFacet,f) vbo->PushTriangle(&f->a,&f->b,&f->c,&f->trinormal);  
  }
  vbo->Create();
  if ( clearcpu ) vbo->ClearCPU();
 }
 void toVBOStaticV( VBOStaticV *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  vbo->Clear();
  if ( toUnitCube ) {
   if ( extents.x == 0 || extents.y == 0 || extents.z == 0 ) this->CalculateExtents();
   Vertexd A,B,C;
   FOREACH(STLFacet,f) {
    A.Set(&f->a); A.Divide(extents.x);
    B.Set(&f->b); B.Divide(extents.y);
    C.Set(&f->c); C.Divide(extents.z);
    vbo->PushTriangle(&A,&B,&C);  
   }
  } else {
   FOREACH(STLFacet,f) vbo->PushTriangle(&f->a,&f->b,&f->c);  
  }
  vbo->Create();
  if ( clearcpu ) vbo->ClearCPU();
 }
 void toVBOStaticVC( VBOStaticVC *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  vbo->Clear();
  if ( toUnitCube ) {
   if ( extents.x == 0 || extents.y == 0 || extents.z == 0 ) this->CalculateExtents();
   Vertexd A,B,C;
   FOREACH(STLFacet,f) {
    A.Set(&f->a); A.Divide(extents.x);
    B.Set(&f->b); B.Divide(extents.y);
    C.Set(&f->c); C.Divide(extents.z);
    vbo->PushTriangle(&A,&B,&C);  
   }
  } else {
   FOREACH(STLFacet,f) vbo->PushTriangle(&f->a,&f->b,&f->c);
  }
  vbo->Create();
  if ( clearcpu ) vbo->ClearCPU();
 }
 void toVBOStaticVCLines( VBOStaticVCLines *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  vbo->Clear();
  if ( toUnitCube ) {
  } else {
   Vertex A,B,C;
   FOREACH(STLFacet,f) {
    A.Set(&f->a); A.Color(0.7f,0.5f,0.7f,1.0f);
    B.Set(&f->b); B.Color(0.35f,0.35f,0.35f,1.0f);
    C.Set(&f->c); C.Color(0.125f,0.125f,0.125f,1.0f);
    vbo->PushLine(&A,&B);
    vbo->PushLine(&B,&C);
    vbo->PushLine(&C,&A);
   }
  }
  vbo->Create();
  if ( clearcpu ) vbo->ClearCPU();
 }
};

class STLFile : public ListItem {
public:
 Zstring filename;
 Zbool ascii,error;
 STLFacets facets;
 Zpointer<VBOStaticVNT> vbo;
 Zpointer<VBOStaticVN> vbovn;
 Zpointer<VBOStaticVNC> vbovnc;
 Zpointer<VBONormalsPolysGroup> vboGroup;
 STLFile() : ListItem() {}
 STLFile( STLFile *in ) { Copy(in); }
 STLFile( const char *newFilename, STLFile *in ) { Copy(newFilename,in); }
 void Copy( STLFile *in ) {
  ascii=in->ascii;
  error=in->error;
  facets.Duplicate(&in->facets);
 }
 void Copy( const char *newFilename, STLFile *in ) {
  Copy(in);
  filename=newFilename;
 }
 STLFile( ImmediateModel *in ) : ListItem() { fromModel(in); }
 STLFile( const char *filename ) : ListItem() { this->filename=filename; }
 STLFile( const char *filename, ImmediateModel *out ) : ListItem() { this->filename=filename; Decode(out); }
 void Write( const char *filename ) {
  this->filename=filename;
  Encode();
 }
 void SaveAs( const char *s ) {
  Zstring was=filename;
  this->filename=s;
  Encode();
  this->filename=was;
 }
 void SaveAscii( const char *s ) {
  Zstring was=filename;
  bool was_ascii=ascii;
  this->filename=s;
  ascii=true;
  Encode();
  ascii=was_ascii;
  this->filename=was;
 }
 void Encode() {
  OUTPUT("Encoding STL\n");
  if ( ascii ) {
   string out=facets.asString();
   string_as_file(out.c_str(),this->filename.c_str());
   OUTPUT("STLFile.Encode() wrote %d bytes (%d facets) to '%s'\n", out.length(), (int) this->facets.count, this->filename.c_str() );
  } else {
   error=true;
   BinaryFile *file=new BinaryFile(filename.c_str(),true);
   if ( !file ) {
    OUTPUT("STLFile.Encode() could not create the file handler.\n");
    return;
   }
   error=false;
   for ( int i=0; i<80; i++ ) file->scrawl('\0');
   //file->write(80,STLHeader);
#ifndef STEAMTYPES_H
   file->scrawl((uint32) (int) facets.count);
#else
   file->scrawl((unsigned int) facets.count);
#endif
   error=facets.write(file);
   delete file;
  }
 }
 void _Decode() {
  OUTPUT("STLFile Loading '%s' (%s format)\n", filename.c_str(), ascii?"ascii":"binary");
  if ( ascii ) {
   Zpointer<STLFacet> facet;
   Zstring file;
   file.Load(filename.c_str());
   Zint vertnumber;
   char *word;
   word=file.next();
   while ( *word != '\0' ) {
    if ( !str_cmp(word,"facet") ) {
     facets.Append(facet=new STLFacet);
     vertnumber=0;
    } else if ( !str_cmp(word,"normal") && facet ) {
     word=file.next();
     facet->trinormal.x=atof(word);
     word=file.next();
     facet->trinormal.y=atof(word);
     word=file.next();
     facet->trinormal.z=atof(word);
    } else if ( !str_cmp(word,"vertex") && facet ) {
     switch ( (int) vertnumber ) {
     case 0:
       word=file.next();       facet->a.x=atof(word);
       word=file.next();       facet->a.y=atof(word);
       word=file.next();       facet->a.z=atof(word);
       vertnumber+=1;
      break;
     case 1:
       word=file.next();       facet->b.x=atof(word);
       word=file.next();       facet->b.y=atof(word);
       word=file.next();       facet->b.z=atof(word);
       vertnumber+=1;
      break;
     case 2:
       word=file.next();       facet->c.x=atof(word);
       word=file.next();       facet->c.y=atof(word);
       word=file.next();       facet->c.z=atof(word);
       vertnumber+=1;
      break;
     default: OUTPUT("STLFile.Decode(): Non-triangle detected!\n"); break;
     }
    } else if ( !str_cmp(word,"outer") ) {}
    else if ( !str_cmp(word,"endloop") ) {}
    else if ( !str_cmp(word,"endfacet") ) { facet.pointer=null; }
    else if ( !str_cmp(word,"endsolid") ) {}
    word=file.next();
   }
  } else {
   BinaryFile *file=new BinaryFile(this->filename.c_str(),false);
   char header[80];
   file->Fread(80,&header,sizeof(unsigned char));
   unsigned int len;
   file->Fread(1,&len,sizeof(unsigned int));
   unsigned int i;
   for ( i=0; i<len; i++ ) {
    facets.Add(file);
   }
   delete file;
  }
  OUTPUT("Facets loaded: %d\n", (int)facets.count);
 }
 void Load( const char *filename ) {
  if ( !file_exists(filename) ) {
   OUTPUT("STLFile:Load(`%s`) file was not found\n", filename );
   return;
  }
  if ( filesize(filename) == 0 ) {
   OUTPUT("STLFile:Load(`%s`) file was empty\n", filename );
   return;
  }
  string loaded=file_as_string(filename,10);
  if ( !str_prefix("model",loaded.c_str())
    || !str_prefix("solid",loaded.c_str()) ) {
   Ascii(filename);
  } else Binary(filename);
 }
 void Ascii( const char *filename ) {
  ascii=true;
  Decode(filename);
 }
 void Binary( const char *filename ) {
  ascii=false;
  Decode(filename);
 }
 void Decode( const char *filename ) {
  this->filename=filename;
  if ( !file_exists(this->filename.c_str()) ) return;
  _Decode();
 }
 void Decode( ImmediateModel *out ) {
  if ( !file_exists(this->filename.c_str()) ) return;
  _Decode();
 }
 void Decode( const char *filename, ImmediateModel *out ) {
  this->filename=filename;
  Decode(out);
 }
 void toVBO( VBOStaticVCLines *normals, VBOStaticVC *polys ) {
  facets.toVBO( normals, polys );
 }
 void toVBOGroup( VBONormalsPolysGroup *vbos, bool reverseWinding=false ) {
  facets.toVBOGroup(vbos,reverseWinding);
 }
 void toVBOGroup( VBONormalsPolysGroup *vbos, bool reverseWinding, Crayon tint ) {
  facets.toVBOGroup(vbos,reverseWinding,tint);
 }
 void toModel( ImmediateModel *model, bool toUnitCube=true ) {
  facets.toModel(model,toUnitCube);
 }
 void toVBOStaticVNTC( VBOStaticVNTC *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  facets.toVBOStaticVNTC(vbo,clearcpu,toUnitCube);
 }
 void toVBOStaticVNT( VBOStaticVNT *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  facets.toVBOStaticVNT(vbo,clearcpu,toUnitCube);
 }
 void toVBOStaticVT( VBOStaticVT *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  facets.toVBOStaticVT(vbo,clearcpu,toUnitCube);
 }
 void toVBOStaticVNC( VBOStaticVNC *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  facets.toVBOStaticVNC(vbo,clearcpu,toUnitCube);
 }
 void toVBOStaticVN( VBOStaticVN *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  facets.toVBOStaticVN(vbo,clearcpu,toUnitCube);
 }
 void toVBOStaticVC( VBOStaticVC *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  facets.toVBOStaticVC(vbo,clearcpu,toUnitCube);
 }
 void toVBOStaticV( VBOStaticV *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  facets.toVBOStaticV(vbo,clearcpu,toUnitCube);
 }
 void toVBOStaticVCLines( VBOStaticVCLines *vbo, bool clearcpu=true, bool toUnitCube=true ) {
  facets.toVBOStaticVCLines(vbo,clearcpu,toUnitCube);
 }
 void VBOGroupRender( LookAtPerspective *lap, Cartesiand *position, Cartesiand *rotation, double x, double y, double w, double h );
 void fromModel( ImmediateModel *m ) {
  EACH(m->polygons.first,mPolygon,p) {
   if ( p->points.count != 3 ) {
    OUTPUT("STLFile:fromModel Error, mPolygon with %d points was not a triangle, skipping.\n",
     (int)p->points.count
    );
   }
   Vertex *a=(Vertex *) p->points.first;
   Vertex *b=(Vertex *) p->points.first->next;
   Vertex *c=(Vertex *) p->points.first->next->next;
   Vertex *n=null;
   if ( p->normal ) n=p->normal;
   else {
    p->TriNormal();
    n=p->normal;
   }
   facets.Add(a,b,c,n);
  }
 }
};

class STLFiles : public LinkedList {
public:
 void Load( const char *list_of_filenames ) {
  Zstring files(list_of_filenames);
  const char *p=files.next();
  while ( *p != '\0' ) {
   if ( file_exists(p) ) {
    Append(new STLFile(p));
   }
   p=files.next();
  }
 }
 void Load() { FOREACH(STLFile,s) s->Load(s->filename.c_str()); }
 void Encode() { FOREACH(STLFile,s) s->Encode(); }
 STLFile *Copy( STLFile *in ) {
  STLFile *n=new STLFile(in);
  Append(n);
  return n;
 }
 STLFile *Add( ImmediateModel *in ) {
  STLFile *n=new STLFile(in);
  Append(n);
  return n;
 }
 void SaveAsPrefix( const char *path, const char *prefix ) {
  string filepath=string(path)+string(prefix);
  string_as_file(
   FORMAT("%d",(int)count.value).c_str(),
   FORMAT("%s-total.txt",filepath.c_str()).c_str()
  );
  int i=0;
  FOREACH(STLFile,s) {
   s->Write((filepath+FORMAT("-%d.stl",(int)i)).c_str());
   i++;
  }
  OUTPUT("STLFiles:SaveAsPrefix(`%s`,`%s`) exported %d STL files\n",path,prefix,(int)count.value);
 }
 void SaveAsPrefix( const char *path, const char *prefix, bool combineUnions ) {
  if ( !first ) {
   OUTPUT("STLFiles:SaveAsPrefix(`%s`,`%s`) empty\n",path,prefix);
  }
  string filepath=string(path)+string(prefix);
  STLFile *one=(STLFile *) first;
  one->ascii=true;
  one->Write( (filepath+string("-Base.stl")).c_str() );
  if ( first->next ) {
   STLFile temp;
   temp.ascii=true;
   FOREACH(STLFile,s) if ( s == first ) continue; else {
	  temp.facets.Duplicate(&s->facets);
   }
   temp.Write((filepath+string("-Unions.stl")).c_str());
  }
  OUTPUT("STLFiles:SaveAsPrefix(`%s`,`%s`) exported base/union\n",path,prefix);
  Zstring data=(!first->next ? "1" : "2" );
  data+='\n';
  FOREACH(STLFile,f) {
   data+=f->facets.count.toString().c_str();
   data+=' ';
   data+=FORMAT("%d",(int)filesize(
    f == one ? (filepath+string("-Unions.stl")).c_str()
    : (filepath+string("-Base.stl")).c_str()
   ));
   data+='\n';
  }
  string_as_file(
   data,
   FORMAT("%s-total.txt",filepath.c_str()).c_str()
  );
 }
 CLEARLISTRESET(STLFile);
};