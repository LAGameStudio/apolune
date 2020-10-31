#include "VBO_VNT.h"

#include "PLY.h"
#include "Model.h"

void VBOStaticVNT::fromModel( ImmediateModel *m ) { EACH(m->polygons.first,mPolygon,p) PushTriangle(p); }
void VBOStaticVNT::fromModel( ImmediateModel *m, int group ) { EACH(m->polygons.first,mPolygon,p) if ( p->group == group ) PushTriangle(p); } 
void VBOStaticVNT::fromPLY( PLY *p ) { fromModel(&p->model); Create(); }
void VBOStaticVNT::fromPLY( PLY *p, bool create ) { fromModel(&p->model); if ( create ) Create(); }
void VBOStaticVNT::FromOBJ( ImmediateModel *m ) {
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
void VBOStaticVNT::DebugRender() {
	glBegin(GL_TRIANGLES);
 for ( unsigned int i=0; i<indices.size(); i++ ) {
  VBO_VNT *v=&data[indices[i]];
  glm::vec3 p(v->pos[0],v->pos[1],v->pos[2]);
  glColor3f(v->texCoord[0],v->texCoord[1],0.0f);
		glVertex3fv(&p.x);
  glNormal3f(v->normal[0],v->normal[1],v->normal[2]);
 }
 glEnd();
}

#include "GLMCamera.h"

void VBOStaticVNT::DebugRender( glm::vec3 lightPos, GLMCamera *camera ) {
  camera->Upload();  
  Coordinator();
  if ( input.KeyDown(DX_LSHIFT) ) {
 		// normals
 		glColor3f(0,0,1);
 		glBegin(GL_LINES);
 		for (unsigned int i=0; i<indices.size(); i++){
    VBO_VNT *v=&data[indices[i]];
    glm::vec3 p(v->pos[0],v->pos[1],v->pos[2]);
 			glVertex3fv(&p.x);
 			glm::vec3 o = glm::vec3(v->normal[0],v->normal[1],v->normal[2]);
    o=glm::normalize(o);
 			p+=o*0.1f;
 			glVertex3fv(&p.x);
 		}
 		glEnd();
  }
		// light pos
		glColor3f(1,1,1);
		glBegin(GL_LINES);
			glVertex3fv(&lightPos.x);
			lightPos+=glm::vec3(1,0,0)*0.1f;
			glVertex3fv(&lightPos.x);
			lightPos-=glm::vec3(1,0,0)*0.1f;
			glVertex3fv(&lightPos.x);
			lightPos+=glm::vec3(0,1,0)*0.1f;
			glVertex3fv(&lightPos.x);
		glEnd();
 }

