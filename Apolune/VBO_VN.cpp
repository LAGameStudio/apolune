#include "VBO_VN.h"

#include "PLY.h"
#include "Model.h"

void VBOStaticVN::fromModel( ImmediateModel *m ) { EACH(m->polygons.first,mPolygon,p) PushTriangle(p); }
void VBOStaticVN::fromModel( ImmediateModel *m, int group ) { EACH(m->polygons.first,mPolygon,p) if ( p->group == group ) PushTriangle(p); } 
void VBOStaticVN::fromPLY( PLY *p ) { fromModel(&p->model); Create(); }
void VBOStaticVN::fromPLY( PLY *p, bool create ) { fromModel(&p->model); if ( create ) Create(); }
void VBOStaticVN::FromOBJ( ImmediateModel *m ) {
  if ( m->polygons.count > 1 ) {
   EACH(m->polygons.first,mPolygon,p) PushTriangle(p);
  } else {
   mPolygon *p=(mPolygon *) m->polygons.first;
   Vertex *v=(Vertex *) p->points.first;
   int i=0;
   Vertex *a=null;
   Vertex *b=null;
   EACH(p->points.first,Vertex,v) {
    i++;
    if ( i % 3 == 1 ) a=v;
    else if ( i % 3 == 2 ) b=v;
    else PushTriangle( a, b, v );
   }
  }
 }
void VBOStaticVN::DebugRender() {
	glBegin(GL_TRIANGLES);
 for ( unsigned int i=0; i<indices.size(); i++ ) {
  VBO_VN *v=&data[indices[i]];
  glm::vec3 p(v->pos[0],v->pos[1],v->pos[2]);
		glVertex3fv(&p.x);
  glNormal3fv(v->normal);//[0],v->normal[1],v->normal[2]);
 }
 glEnd();
}