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

#include "Indexed.h"
#include "VBO.h"
#include "STLFile.h"

class VBOGroup : public ListItem {
public:
 VBOGroup() : ListItem() {}
};

class VBOGroupedNormalsPolys : public VBOGroup {
public:
 Zint remaining;
 VBOStaticVCLines normals;
 VBOStaticVC polys;
 VBOGroupedNormalsPolys() : VBOGroup() { remaining=20000; }
 void Create() {
  normals.Create();
  polys.Create();
 }
 void CreateClear() {
  normals.Create();
  normals.ClearCPU();
  polys.Create();
  polys.ClearCPU();
 }
 void toModel( ImmediateModel *m ) {
  polys.toModel(m);
 }
 void toModel( ImmediateModel *m, bool norms ) {
  normals.toModel(m);
 }
  // Clockwise a,b,c of the triangle
 void PushTri(
   Vertex *a, Crayon *ca,
   Vertex *b, Crayon *cb,
   Vertex *c, Crayon *cc ) {
  mPolygon tri;
  Vertex normalVector;
  Vertex *A,*B,*C;
  B=tri.points.Add(b->x,b->y,b->z); B->Color(*ca); B->q=1.0; B->r=0.0;
  C=tri.points.Add(c->x,c->y,c->z); C->Color(*cb); C->q=1.0; C->r=0.0;
  A=tri.points.Add(a->x,a->y,a->z); A->Color(*ca);
  tri.Barycenter(); tri.TriNormal(); tri.normal->normalize(); tri.bary->Color(1.0,0.0,0.0,1.0); normals.PushVertex(tri.bary);
  normalVector.Set(tri.normal.pointer); normalVector.Add(tri.bary); normalVector.Color(0.0,1.0,0.0,1.0); normals.PushVertex(&normalVector);
//  vbo.PushTriangle(&tri);
  polys.PushTriangle(&tri);
  remaining-=3*12;
 }
 void PushTri(
   Vertex *a, Crayon *ca,
   Vertex *b, Crayon *cb,
   Vertex *c, Crayon *cc,
   STLFile *stl ) {
  mPolygon tri;
  Vertex normalVector;
  Vertex *A,*B,*C;
  B=tri.points.Add(b->x,b->y,b->z); B->Color(*ca); B->q=1.0; B->r=0.0;
  C=tri.points.Add(c->x,c->y,c->z); C->Color(*cb); C->q=1.0; C->r=0.0;
  A=tri.points.Add(a->x,a->y,a->z); A->Color(*ca);
  tri.Barycenter(); tri.TriNormal(); tri.normal->normalize(); tri.bary->Color(1.0,0.0,0.0,1.0); normals.PushVertex(tri.bary);
  normalVector.Set(tri.normal.pointer); normalVector.Add(tri.bary); normalVector.Color(0.0,1.0,0.0,1.0); normals.PushVertex(&normalVector);
//  vbo.PushTriangle(&tri);
  polys.PushTriangle(&tri);
  stl->facets.Add(A,C,B,&normalVector);
  remaining-=3*12;
 }
 void PushTri(
   Vertexd *a, Crayon *ca,
   Vertexd *b, Crayon *cb,
   Vertexd *c, Crayon *cc,
   STLFile *stl ) {
  mPolygon tri;
  Vertex normalVector;
  Vertex *A,*B,*C;
  B=tri.points.Add(b->x,b->y,b->z); B->Color(*ca); B->q=1.0; B->r=0.0;
  C=tri.points.Add(c->x,c->y,c->z); C->Color(*cb); C->q=1.0; C->r=0.0;
  A=tri.points.Add(a->x,a->y,a->z); A->Color(*ca);
  tri.Barycenter(); tri.TriNormal(); tri.normal->normalize(); tri.bary->Color(1.0,0.0,0.0,1.0); normals.PushVertex(tri.bary);
  normalVector.Set(tri.normal.pointer); normalVector.Add(tri.bary); normalVector.Color(0.0,1.0,0.0,1.0); normals.PushVertex(&normalVector);
//  vbo.PushTriangle(&tri);
  polys.PushTriangle(&tri);
  stl->facets.Add(A,C,B,&normalVector);
  remaining-=3*12;
 }
 void PushTri( mPolygon *tri ) {
  if ( tri->points.count < 3 ) return;
  Vertex normalVector;
  tri->Barycenter();
  tri->TriNormal();
  tri->normal->normalize();
  tri->bary->Color(1.0,0.0,0.0,1.0);
  normals.PushVertex(tri->bary);
  normalVector.Set(tri->normal.pointer);
  normalVector.Add(tri->bary);
  normalVector.Color(0.0,1.0,0.0,1.0);
  normals.PushVertex(&normalVector);
  polys.PushTriangle(tri);
  remaining-=3*12;
 }
  // Clockwise a,b,c of the triangle
 void PushTri(
   Vertexd *a, Crayon *ca,
   Vertexd *b, Crayon *cb,
   Vertexd *c, Crayon *cc ) {
  mPolygon tri;
  Vertex normalVector;
  Vertex *A,*B,*C;
  B=tri.points.Add(b->x,b->y,b->z); B->Color(*cb); B->q=1.0; B->r=0.0;
  C=tri.points.Add(c->x,c->y,c->z); C->Color(*cc); C->q=1.0; C->r=0.0;
  A=tri.points.Add(a->x,a->y,a->z); A->Color(*ca); 
  tri.Barycenter(); tri.TriNormal(); tri.normal->normalize(); tri.bary->Color(1.0,0.0,0.0,1.0); normals.PushVertex(tri.bary);
  normalVector.Set(tri.normal.pointer); normalVector.Add(tri.bary); normalVector.Color(0.0,1.0,0.0,1.0); normals.PushVertex(&normalVector);
//  vbo.PushTriangle(&tri);
  polys.PushTriangle(&tri);
  remaining-=3*12;
 }
 // Clockwise a,b,c,d of the quad
 void PushQuad(
   Vertexd *a, Crayon *ca,
   Vertexd *b, Crayon *cb,
   Vertexd *c, Crayon *cc,
   Vertexd *d, Crayon *cd,
   STLFile *stl ) {
  mPolygon tri;
  Vertex normalVector;
  Vertex *A,*B,*C;
  B=tri.points.Add(b->x,b->y,b->z); B->Color(*cb); B->q=1.0; B->r=0.0;
  C=tri.points.Add(c->x,c->y,c->z); C->Color(*cc); C->q=1.0; C->r=0.0;
  A=tri.points.Add(a->x,a->y,a->z); A->Color(*ca);
  tri.Barycenter(); tri.TriNormal(); tri.normal->normalize(); tri.bary->Color(1.0,0.0,0.0,1.0); normals.PushVertex(tri.bary);
  normalVector.Set(tri.normal.pointer); normalVector.Add(tri.bary); normalVector.Color(0.0,1.0,0.0,1.0); normals.PushVertex(&normalVector);
//  vbo.PushTriangle(&tri);
  polys.PushTriangle(&tri);
  stl->facets.Add(A,C,B,&normalVector);
  tri.points.Clear();
  B=tri.points.Add(c->x,c->y,c->z); B->Color(*cc); B->q=1.0; B->r=0.0;
  C=tri.points.Add(d->x,d->y,d->z); C->Color(*cd); C->q=1.0; C->r=0.0;
  A=tri.points.Add(a->x,a->y,a->z); A->Color(*ca);
  tri.Barycenter(); tri.TriNormal(); tri.normal->normalize(); tri.bary->Color(1.0,0.0,0.0,1.0); normals.PushVertex(tri.bary);
  normalVector.Set(tri.normal.pointer); normalVector.Add(tri.bary); normalVector.Color(0.0,1.0,0.0,1.0); normals.PushVertex(&normalVector);
//  vbo.PushTriangle(&tri);
  polys.PushTriangle(&tri);
  stl->facets.Add(A,C,B,&normalVector);
  remaining-=4*12;
 }
 void PushQuad(
   Vertexd *a, Crayon *ca,
   Vertexd *b, Crayon *cb,
   Vertexd *c, Crayon *cc,
   Vertexd *d, Crayon *cd ) {
  mPolygon tri;
  Vertex normalVector;
  Vertex *A,*B,*C;
  B=tri.points.Add(b->x,b->y,b->z); B->Color(*cb); B->q=1.0; B->r=0.0;
  C=tri.points.Add(c->x,c->y,c->z); C->Color(*cc); C->q=1.0; C->r=0.0;
  A=tri.points.Add(a->x,a->y,a->z); A->Color(*ca);
  tri.Barycenter(); tri.TriNormal(); tri.normal->normalize(); tri.bary->Color(1.0,0.0,0.0,1.0); normals.PushVertex(tri.bary);
  normalVector.Set(tri.normal.pointer); normalVector.Add(tri.bary); normalVector.Color(0.0,1.0,0.0,1.0); normals.PushVertex(&normalVector);
//  vbo.PushTriangle(&tri);
  polys.PushTriangle(&tri);
  tri.points.Clear();
  B=tri.points.Add(c->x,c->y,c->z); B->Color(*cc); B->q=1.0; B->r=0.0;
  C=tri.points.Add(d->x,d->y,d->z); C->Color(*cd); C->q=1.0; C->r=0.0;
  A=tri.points.Add(a->x,a->y,a->z); A->Color(*ca);
  tri.Barycenter(); tri.TriNormal(); tri.normal->normalize(); tri.bary->Color(1.0,0.0,0.0,1.0); normals.PushVertex(tri.bary);
  normalVector.Set(tri.normal.pointer); normalVector.Add(tri.bary); normalVector.Color(0.0,1.0,0.0,1.0); normals.PushVertex(&normalVector);
//  vbo.PushTriangle(&tri);
  polys.PushTriangle(&tri);
  remaining-=4*12;
 }
 void Render() {
  normals.Render();
  polys.Render();
 }
 void RenderPolys() {
  polys.Render();
 }
 void RenderNormals() {
  normals.Render();
 }
 void Clear() {
  normals.Clear();
  polys.Clear();
  remaining=20000;
 }
 bool Available() {
  return ( remaining > 5*12 );
 }
};

class VBONormalsPolysGroup : public LinkedList {
public:
 CLEARLISTRESET(VBOGroupedNormalsPolys);
 void Create() { FOREACH(VBOGroupedNormalsPolys,p) p->Create(); }
 void CreateClear() { FOREACH(VBOGroupedNormalsPolys,p) p->CreateClear(); }
 void Render() { FOREACH(VBOGroupedNormalsPolys,p) p->Render(); }
 void RenderPolys() { FOREACH(VBOGroupedNormalsPolys,p) p->RenderPolys(); }
 void RenderNormals() { FOREACH(VBOGroupedNormalsPolys,p) p->RenderNormals(); }
 void PushTri( mPolygon *tri ) {
  if ( last ) {
   VBOGroupedNormalsPolys *candidate=((VBOGroupedNormalsPolys *)last);
   if ( candidate->Available() ) candidate->PushTri(tri);
   else {
    VBOGroupedNormalsPolys *candidate=new VBOGroupedNormalsPolys;
    Append(candidate);
    candidate->PushTri(tri);
   }
  } else {
   VBOGroupedNormalsPolys *candidate=new VBOGroupedNormalsPolys;
   Append(candidate);
   candidate->PushTri(tri);
  }
 }
 void fromTriModel( ImmediateModel *m ) {
  Clear();
  EACH(m->polygons.first,mPolygon,p) PushTri(p);
  CreateClear();
 }
 void fromTriModel( ImmediateModel *m, Crayon c ) {
  Clear();
  EACH(m->polygons.first,mPolygon,p) PushTri(
   (Vertex*)p->points.first,&c,
   (Vertex*)p->points.first->next,&c,
   (Vertex*)p->points.first->next->next,&c
   );
  CreateClear();
 }
 void fromTriModel( ImmediateModel *m, Crayon j, Crayon k ) {
  Clear();
  m->BoxBounds();
  EACH(m->polygons.first,mPolygon,p) {
   Crayon a,b,c;
   Vertex *A=(Vertex*)p->points.first;
   Vertex *B=(Vertex*)p->points.first->next;
   Vertex *C=(Vertex*)p->points.first->next->next;
   a.Lerp( ( (A->z-m->leasts.z)/m->boxMagnitude.z ), j,k); a.Alpha(1.0);
   b.Lerp( ( (B->z-m->leasts.z)/m->boxMagnitude.z ), j,k); b.Alpha(1.0);
   c.Lerp( ( (C->z-m->leasts.z)/m->boxMagnitude.z ), j,k); c.Alpha(1.0);
   PushTri( A,&a, B,&b, C,&c );
  }
  CreateClear();
 }
 void fromTriModel( ImmediateModel *m, float mx, float my, float mz ) {
  Clear();
  mPolygon t;
  EACH(m->polygons.first,mPolygon,p) {
   t.points.CopyAppend(&p->points);
   t.points.MoveBy(mx,my,mz);
   PushTri(&t);
   t.points.Clear();
  }
  CreateClear();
 }
 void fromTriModelUnitCubeZeroed( ImmediateModel *m, float mx, float my, float mz ) {
  Clear();
  mPolygon t;
  EACH(m->polygons.first,mPolygon,p) {
   t.points.CopyAppend(&p->points);
   t.points.MoveBy(-m->leasts.x,-m->leasts.y,-m->leasts.z);
   t.points.Divide(&m->boxMagnitude);
   PushTri(&t);
   t.points.Clear();
  }
  CreateClear();
 }
 void fromTriModelUnitCubeZeroed( ImmediateModel *m, float mx, float my, float mz, bool keep ) {
  Clear();
  mPolygon t;
  EACH(m->polygons.first,mPolygon,p) {
   t.points.CopyAppend(&p->points);
   t.points.MoveBy(-m->leasts.x,-m->leasts.y,-m->leasts.z);
   t.points.Divide(&m->boxMagnitude);
   t.points.MoveBy(mx,my,mz);
   PushTri(&t);
   t.points.Clear();
  }
  Create();
 }
 void fromTriModel( ImmediateModel *m, Crayon c, float mx, float my, float mz ) {
  Clear();
  mPolygon t;
  EACH(m->polygons.first,mPolygon,p) {
   t.points.CopyAppend(&p->points);
   t.points.MoveBy(mx,my,mz);
   PushTri(
    (Vertex*)t.points.first,&c,
    (Vertex*)t.points.first->next,&c,
    (Vertex*)t.points.first->next->next,&c
   );
   t.points.Clear();
  }
  CreateClear();
 }
 void PushTri(
   Vertex *a, Crayon *ca,
   Vertex *b, Crayon *cb,
   Vertex *c, Crayon *cc ) {
  if ( last ) {
   VBOGroupedNormalsPolys *candidate=((VBOGroupedNormalsPolys *)last);
   if ( candidate->Available() ) candidate->PushTri(a,ca,b,cb,c,cc);
   else {
    VBOGroupedNormalsPolys *candidate=new VBOGroupedNormalsPolys;
    Append(candidate);
    candidate->PushTri(a,ca,b,cb,c,cc);
   }
  } else {
   VBOGroupedNormalsPolys *candidate=new VBOGroupedNormalsPolys;
   Append(candidate);
   candidate->PushTri(a,ca,b,cb,c,cc);
  }
 }
 void PushTri(
   Vertexd *a, Crayon *ca,
   Vertexd *b, Crayon *cb,
   Vertexd *c, Crayon *cc ) {
  if ( last ) {
   VBOGroupedNormalsPolys *candidate=((VBOGroupedNormalsPolys *)last);
   if ( candidate->Available() ) candidate->PushTri(a,ca,b,cb,c,cc);
   else {
    VBOGroupedNormalsPolys *candidate=new VBOGroupedNormalsPolys;
    Append(candidate);
    candidate->PushTri(a,ca,b,cb,c,cc);
   }
  } else {
   VBOGroupedNormalsPolys *candidate=new VBOGroupedNormalsPolys;
   Append(candidate);
   candidate->PushTri(a,ca,b,cb,c,cc);
  }
 }
 void PushTri(
   Vertexd *a, Crayon *ca,
   Vertexd *b, Crayon *cb,
   Vertexd *c, Crayon *cc,
   STLFile *stl ) {
  if ( last ) {
   VBOGroupedNormalsPolys *candidate=((VBOGroupedNormalsPolys *)last);
   if ( candidate->Available() ) candidate->PushTri(a,ca,b,cb,c,cc,stl);
   else {
    VBOGroupedNormalsPolys *candidate=new VBOGroupedNormalsPolys;
    Append(candidate);
    candidate->PushTri(a,ca,b,cb,c,cc,stl);
   }
  } else {
   VBOGroupedNormalsPolys *candidate=new VBOGroupedNormalsPolys;
   Append(candidate);
   candidate->PushTri(a,ca,b,cb,c,cc,stl);
  }
 }
 void PushQuad(
   Vertexd *a, Crayon *ca,
   Vertexd *b, Crayon *cb,
   Vertexd *c, Crayon *cc,
   Vertexd *d, Crayon *cd,
   STLFile *stl ) {
  if ( last ) {
   VBOGroupedNormalsPolys *candidate=((VBOGroupedNormalsPolys *)last);
   if ( candidate->Available() ) candidate->PushQuad(a,ca,b,cb,c,cc,d,cd,stl);
   else {
    VBOGroupedNormalsPolys *candidate=new VBOGroupedNormalsPolys;
    Append(candidate);
    candidate->PushQuad(a,ca,b,cb,c,cc,d,cd,stl);
   }
  } else {
   VBOGroupedNormalsPolys *candidate=new VBOGroupedNormalsPolys;
   Append(candidate);
   candidate->PushQuad(a,ca,b,cb,c,cc,d,cd,stl);
  }
 }
 void PushQuad(
   Vertexd *a, Crayon *ca,
   Vertexd *b, Crayon *cb,
   Vertexd *c, Crayon *cc,
   Vertexd *d, Crayon *cd ) {
  if ( last ) {
   VBOGroupedNormalsPolys *candidate=((VBOGroupedNormalsPolys *)last);
   if ( candidate->Available() ) candidate->PushQuad(a,ca,b,cb,c,cc,d,cd);
   else {
    VBOGroupedNormalsPolys *candidate=new VBOGroupedNormalsPolys;
    Append(candidate);
    candidate->PushQuad(a,ca,b,cb,c,cc,d,cd);
   }
  } else {
   VBOGroupedNormalsPolys *candidate=new VBOGroupedNormalsPolys;
   Append(candidate);
   candidate->PushQuad(a,ca,b,cb,c,cc,d,cd);
  }
 }
 void toModel( ImmediateModel *out ) {
  FOREACH(VBOGroupedNormalsPolys,p) {
   p->toModel(out);
  }
 }
 void toModel( ImmediateModel *out, bool norms ) {
  FOREACH(VBOGroupedNormalsPolys,p) {
   p->toModel(out,true);
  }
 }
};

class VBONormalsPolysGroups : public LinkedList {
public:
 void Render() {
  FOREACH(VBONormalsPolysGroup,g) g->Render();
 }
 void RenderPolys() {
  FOREACH(VBONormalsPolysGroup,g) g->RenderPolys();
 }
 void RenderNormals() {
  FOREACH(VBONormalsPolysGroup,g) g->RenderNormals();
 }
 void Add( STLFile *s ) {
  VBONormalsPolysGroup *g=new VBONormalsPolysGroup;
  s->toVBOGroup(g);
  Append(g);
 }
 void FromSTLs( STLFiles *in ) {
  EACH(in->first,STLFile,s) Add(s);
 }
 CLEARLISTRESET(VBONormalsPolysGroup);
};

HANDLES(VBONormalsPolysGroup,VBONormalsPolysGroupHandle,VBONormalsPolysGroupHandles,"VBONormalsPolysGroup");