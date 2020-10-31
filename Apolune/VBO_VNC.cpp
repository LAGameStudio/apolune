#include "VBO_VNC.h"

#include "PLY.h"
#include "Model.h"

void VBOStaticVNC::fromModel( ImmediateModel *m ) { EACH(m->polygons.first,mPolygon,p) PushTriangle(p); }
void VBOStaticVNC::fromModel( ImmediateModel *m, int group ) { EACH(m->polygons.first,mPolygon,p) if ( p->group == group ) PushTriangle(p); } 
void VBOStaticVNC::fromPLY( PLY *p ) { fromModel(&p->model); Create(); }
void VBOStaticVNC::fromPLY( PLY *p, bool create ) { fromModel(&p->model); if ( create ) Create(); }
void VBOStaticVNC::FromOBJ( ImmediateModel *m ) {
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
void VBOStaticVNC::DebugRender() {
	glBegin(GL_TRIANGLES);
 for ( unsigned int i=0; i<indices.size(); i++ ) {
  VBO_VNC *v=&data[indices[i]];
  glm::vec3 p(v->pos[0],v->pos[1],v->pos[2]);
  glColor4f(v->color[0],v->color[1],v->color[2],v->color[3]);
		glVertex3fv(&p.x);
  glNormal3f(v->normal[0],v->normal[1],v->normal[2]);
 }
 glEnd();
}