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

#include "SGjs.h"
#include "Model.h"
#include "STLFile.h"

class jsCSGModel : public ListItem {
public:
 Zstring name;
 csgjs_model model;
 jsCSGModel() : ListItem() {
 }
 jsCSGModel( ImmediateModel *in ) : ListItem() {
  fromModel(in);
 }
 jsCSGModel( STLFile *in ) : ListItem() {
  fromSTL(in);
 }
 jsCSGModel( const char *n, ImmediateModel *in ) : ListItem() {
  name=n;
  fromModel(in);
 }
 jsCSGModel( const char *n, STLFile *in ) : ListItem() {
  name=n;
  fromSTL(in);
 }
 void fromModel( ImmediateModel *in ) {
  int j=0,i=0;
  EACH(in->polygons.first,mPolygon,p) {
   EACH(p->points.first,Vertex,v) {
    csgjs_vertex ver;
    ver.pos.x=v->x;
    ver.pos.y=v->y;
    ver.pos.z=v->z;
    ver.normal.x=p->normal->x;
    ver.normal.y=p->normal->y;
    ver.normal.z=p->normal->z;
    ver.uv.x=v->r;
    ver.uv.y=v->s;
    ver.uv.z=v->t;
    model.vertices.push_back(ver);
   }
   model.indices.push_back(i);
   model.indices.push_back(i+1);
   model.indices.push_back(i+2);
   i+=3;
   j+=1;
  }
 }
 void fromSTL( STLFile *in ) {
  int i=0;
  EACH(in->facets.first,STLFacet,p) {
   csgjs_vertex verA;
   verA.pos.x=(float)p->a.x;
   verA.pos.y=(float)p->a.y;
   verA.pos.z=(float)p->a.z;
   verA.normal.x=(float)p->trinormal.x;
   verA.normal.y=(float)p->trinormal.y;
   verA.normal.z=(float)p->trinormal.z;
   model.vertices.push_back(verA);
   model.indices.push_back(i); i++;

   csgjs_vertex verB;
   verB.pos.x=(float)p->a.x;
   verB.pos.y=(float)p->a.y;
   verB.pos.z=(float)p->a.z;
   verB.normal.x=(float)p->trinormal.x;
   verB.normal.y=(float)p->trinormal.y;
   verB.normal.z=(float)p->trinormal.z;
   model.vertices.push_back(verB);
   model.indices.push_back(i); i++;

   csgjs_vertex verC;
   verC.pos.x=(float)p->a.x;
   verC.pos.y=(float)p->a.y;
   verC.pos.z=(float)p->a.z;
   verC.normal.x=(float)p->trinormal.x;
   verC.normal.y=(float)p->trinormal.y;
   verC.normal.z=(float)p->trinormal.z;
   model.vertices.push_back(verC);
   model.indices.push_back(i); i++;
  }
 }
 void toModel( ImmediateModel *out ) {
  for ( unsigned int i=0; i<model.indices.size(); i+=3 ) {
   int a=model.indices[i];
   int b=model.indices[i+1];
   int c=model.indices[i+2];
   mPolygon *P=new mPolygon;
   Vertex *A=new Vertex( model.vertices[a].pos.x, model.vertices[a].pos.y, model.vertices[a].pos.z );
   A->r=model.vertices[a].uv.x;
   A->s=model.vertices[a].uv.y;
   A->t=model.vertices[a].uv.y;
   Vertex *B=new Vertex( model.vertices[b].pos.x, model.vertices[b].pos.y, model.vertices[b].pos.z );
   B->r=model.vertices[b].uv.x;
   B->s=model.vertices[b].uv.y;
   B->t=model.vertices[b].uv.y;
   Vertex *C=new Vertex( model.vertices[c].pos.x, model.vertices[c].pos.y, model.vertices[c].pos.z );
   C->r=model.vertices[c].uv.x;
   C->s=model.vertices[c].uv.y;
   C->t=model.vertices[c].uv.y;
   P->addVertex(A);
   P->addVertex(B);
   P->addVertex(C);
   P->normal.Recycle(
    new Vertex( model.vertices[c].normal.x, model.vertices[c].normal.y, model.vertices[c].normal.z )
   );
   out->polygons.Append(P);
  }
 }
 void toSTL( STLFile *out ) {
  for ( unsigned int i=0; i<model.indices.size(); i+=3 ) {
   int a=model.indices[i];
   int b=model.indices[i+1];
   int c=model.indices[i+2];
   STLFacet *P=new STLFacet;
   P->a.Set( model.vertices[a].pos.x, model.vertices[a].pos.y, model.vertices[a].pos.z );
   P->b.Set( model.vertices[b].pos.x, model.vertices[b].pos.y, model.vertices[b].pos.z );
   P->c.Set( model.vertices[c].pos.x, model.vertices[c].pos.y, model.vertices[c].pos.z );
   P->trinormal.Set(
    model.vertices[c].normal.x, model.vertices[c].normal.y, model.vertices[c].normal.z
   );
   out->facets.Append(P);
  }
 }
};

class jsCSGModels : public LinkedList {
public:
 jsCSGModel *Add( const char *name, ImmediateModel *in ) {
  jsCSGModel *m=new jsCSGModel(name,in);
  Append(m);
  return m;
 }
 jsCSGModel *Add( const char *name, STLFile *in ) {
  jsCSGModel *m=new jsCSGModel(name,in);
  Append(m);
  return m;
 }
 jsCSGModel *byName( const char *match ) {
  FOREACH(jsCSGModel,m) if ( m->name == match ) return m;
  return null;
 }
 CLEARLISTRESET(jsCSGModel);
};

class jsCSG {
public:
 jsCSGModels models;
 string Union( const char *a, const char *b ) {
  jsCSGModel *A=models.byName(a);
  jsCSGModel *B=models.byName(b);
  if ( !A || !B ) return string("");
  jsCSGModel *R=new jsCSGModel;
  R->name=FORMAT("(%s+%s)",a,b);
  OUTPUT("jsCSG calculating: %s\n",R->name.c_str());
  R->model = csgjs_union( A->model, B->model );
  models.Append(R);
  return R->name.value;
 }
 string Difference( const char *a, const char *b ) {
  jsCSGModel *A=models.byName(a);
  jsCSGModel *B=models.byName(b);
  if ( !A || !B ) return string("");
  jsCSGModel *R=new jsCSGModel;
  R->name=FORMAT("(%s-%s)",a,b);
  OUTPUT("jsCSG calculating: %s\n",R->name.c_str());
  R->model = csgjs_difference( A->model, B->model );
  models.Append(R);
  return R->name.value;
 }
 string Intersection( const char *a, const char *b ) {
  jsCSGModel *A=models.byName(a);
  jsCSGModel *B=models.byName(b);
  if ( !A || !B ) return string("");
  jsCSGModel *R=new jsCSGModel;
  R->name=FORMAT("(%s^%s)",a,b);
  OUTPUT("jsCSG calculating: %s\n",R->name.c_str());
  R->model = csgjs_intersection( A->model, B->model );
  models.Append(R);
  return R->name.value;
 }
};

extern jsCSG jscsg;