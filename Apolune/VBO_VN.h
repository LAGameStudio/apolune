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
#include "ModernVBO.h"

struct VBO_VN	{
 GLfloat pos[3];
 GLfloat normal[3];
 VBO_VN() {
  pos[0]=pos[1]=pos[2]=0.0f;
  normal[0]=normal[1]=normal[2]=0.0f;
 }
};

class VBOStaticVN : public ModernVBO<VBO_VN> {
public:
 GLuint *position,*anormal;
 void Constructor() {
  position=anormal=null;
 }
 virtual void fromModel( ImmediateModel *m );
 virtual void FromOBJ( ImmediateModel *m );
 virtual void fromModel( ImmediateModel *m, int group );
 virtual void fromPLY( PLY *p );
 virtual void fromPLY( PLY *p, bool create );
 void BindToStreams() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(VBO_VN), BUFFER_OFFSET(0));
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, sizeof(VBO_VN), BUFFER_OFFSET(12));
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
 }
 void BindToStreams2() {
  glEnableVertexAttribArray(0);	glVertexAttribPointer(	*position,	3,	GL_FLOAT, GL_FALSE,	sizeof(VBO_VN), BUFFER_OFFSET(offsetof(VBO_VN,pos))	);
 	glEnableVertexAttribArray(1); glVertexAttribPointer( *anormal, 3, GL_FLOAT, GL_FALSE, sizeof(VBO_VN), BUFFER_OFFSET(offsetof(VBO_VN,normal)) );
 }
 void UnbindFromStreams2() {
 	glDisableVertexAttribArray(0);
 	glDisableVertexAttribArray(1);
 	glDisableVertexAttribArray(2);
 }
 Zdisposable<Vertex> normal;
 void Precompute( Vertex *a, Vertex *b, Vertex *c ) {
  if ( a->normal ) {
   normal.Instantiate();
   normal->Set(a->normal);
  } else normal.Recycle(c->TriNormal(a,b,c));
 }
 void Set( VBO_VN *target, Vertexd *source, Vertexd *normal ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->normal[0]=(GLfloat) normal->x;
  target->normal[1]=(GLfloat) normal->y;
  target->normal[2]=(GLfloat) normal->z;
 }
 void Set( VBO_VN *target, Vertex *source, Vertex *normal ) {
  target->pos[0]  =(GLfloat) source->x;
  target->pos[1]  =(GLfloat) source->y;
  target->pos[2]  =(GLfloat) source->z;
  target->normal[0]=(GLfloat) normal->x;
  target->normal[1]=(GLfloat) normal->y;
  target->normal[2]=(GLfloat) normal->z;
 }
 void Get( VBO_VN *target, Vertex *source ) {
  (GLfloat) source->x=target->pos[0];
  (GLfloat) source->y=target->pos[1];
  (GLfloat) source->z=target->pos[2];
  source->normal.Recycle(new Vertex(
   target->normal[0],
   target->normal[1],
   target->normal[2]
  ));
 }
 void Get( VBO_VN *target, Vertexd *source, Vertexd *normal ) {
  source->x=(GLfloat) target->pos[0];
  source->y=(GLfloat) target->pos[1];
  source->z=(GLfloat) target->pos[2];
  normal->x=(double) target->normal[0];
  normal->y=(double) target->normal[1];
  normal->z=(double) target->normal[2];
 }
 void DebugRender();
// void DebugRender( glm::vec3 lightPos, GLMCamera *camera );
};