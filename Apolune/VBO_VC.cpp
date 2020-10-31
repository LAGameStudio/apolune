#include "VBO_VC.h"

#include "PLY.h"
#include "Model.h"

void VBOStaticVC::fromModel( ImmediateModel *m ) { EACH(m->polygons.first,mPolygon,p) PushTriangle(p); }
void VBOStaticVC::fromModel( ImmediateModel *m, int group ) { EACH(m->polygons.first,mPolygon,p) if ( p->group == group ) PushTriangle(p); } 
void VBOStaticVC::toModel( ImmediateModel *m ) {
 int end=data.size();
 for ( int i=0; i<end; i+=3 ) {
  mPolygon *p=new mPolygon;
  Vertex *a=new Vertex;
  Vertex *b=new Vertex;
  Vertex *c=new Vertex;
  Get(&data[i],  a);
  Get(&data[i+1],b);
  Get(&data[i+2],c);
  p->addVertex(a);
  p->addVertex(b);
  p->addVertex(c);
  m->addPolygon(p);
 }
}
void VBOStaticVC::toModel( ImmediateModel *m, int group ) {
 int end=data.size();
 for ( int i=0; i<end; i+=3 ) {
  mPolygon *p=new mPolygon;
  Vertex *a=new Vertex;
  Vertex *b=new Vertex;
  Vertex *c=new Vertex;
  Get(&data[i],  a);
  Get(&data[i+1],b);
  Get(&data[i+2],c);
  p->addVertex(a);
  p->addVertex(b);
  p->addVertex(c);
  p->group=group;
  m->addPolygon(p);
 }
} 
void VBOStaticVC::fromPLY( PLY *p ) { fromModel(&p->model); Create(); }
void VBOStaticVC::fromPLY( PLY *p, bool create ) { fromModel(&p->model); if ( create ) Create(); }
