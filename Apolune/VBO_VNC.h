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

struct VBO_VNC	{
 GLfloat pos[3];
 GLfloat normal[3];
 GLfloat color[4];
 VBO_VNC() {
  pos[0]=pos[1]=pos[2]=0.0f;
  normal[0]=normal[1]=normal[2]=0.0f;
  color[0]=color[1]=color[2]=color[3]=0.0f;
 }
};

class VBOStaticVNC : public ModernVBO<VBO_VNC> {
public:
 GLuint *aposition,*acolor,*anormal;
 void Constructor() {
  aposition=acolor=anormal=null;
 }
 virtual void fromModel( ImmediateModel *m );
 virtual void FromOBJ( ImmediateModel *m );
 virtual void fromModel( ImmediateModel *m, int group );
 virtual void fromPLY( PLY *p );
 virtual void fromPLY( PLY *p, bool create );
 void BindToStreams() {
  glEnableClientState(GL_VERTEX_ARRAY);  glVertexPointer(3, GL_FLOAT, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNC,pos)));
  glEnableClientState(GL_NORMAL_ARRAY);  glNormalPointer(GL_FLOAT, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNC,normal)));
  glEnableClientState(GL_COLOR_ARRAY);   glColorPointer(4, GL_FLOAT, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNC,color)));
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
 }
 void BindToStreams2() {
  glEnableVertexAttribArray(0);	glVertexAttribPointer(	*aposition,	3,	GL_FLOAT, GL_FALSE,	(GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNC,pos))	);
 	glEnableVertexAttribArray(1); glVertexAttribPointer( *anormal, 3, GL_FLOAT, GL_FALSE, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNC,normal)) );
 	glEnableVertexAttribArray(2);	glVertexAttribPointer( *acolor, 4, GL_FLOAT, GL_FALSE, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNC,color)) );
 }
 void UnbindFromStreams2() {
 	glDisableVertexAttribArray(0);
 	glDisableVertexAttribArray(1);
 	glDisableVertexAttribArray(2);
 }
 Zdisposable<Vertex> normal;
 void Precompute( Vertex *a, Vertex *b, Vertex *c ) {
  normal.Instantiate();
  if ( a->normal ) normal->Set(a->normal);
  else normal.Recycle(c->TriNormal(a,b,c));
 }
 void Precompute( Vertexd *a, Vertexd *b, Vertexd *c ) {
  normal.Instantiate();
  normal->TriNormal(a,b,c);
 }
 void Set( VBO_VNC *target, Vertex *source ) {
  target->pos[0]  =source->x;
  target->pos[1]  =source->y;
  target->pos[2]  =source->z;
  target->normal[0]=this->normal->x;
  target->normal[1]=this->normal->y;
  target->normal[2]=this->normal->z;
  target->color[0]=source->rgba[0];
  target->color[1]=source->rgba[1];
  target->color[2]=source->rgba[2];
  target->color[3]=source->rgba[3];
 }
 void Set( VBO_VNC *target, Vertex *source, Vertex *normal ) {
  target->pos[0]  =source->x;
  target->pos[1]  =source->y;
  target->pos[2]  =source->z;
  target->normal[0]=normal->x;
  target->normal[1]=normal->y;
  target->normal[2]=normal->z;
  target->color[0]=source->rgba[0];
  target->color[1]=source->rgba[1];
  target->color[2]=source->rgba[2];
  target->color[3]=source->rgba[3];
 }
 void Set( VBO_VNC *target, Vertexd *source, Vertexd *normal, Crayon rgba ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->color[0]=(GLfloat) rgba.rf;
  target->color[1]=(GLfloat) rgba.gf;
  target->color[2]=(GLfloat) rgba.bf;
  target->color[3]=(GLfloat) rgba.af;
  target->normal[0]=(GLfloat) normal->x;
  target->normal[1]=(GLfloat) normal->y;
  target->normal[2]=(GLfloat) normal->z;
 }
 void Get( VBO_VNC *target, Vertex *source ) {
  source->x      =target->pos[0];
  source->y      =target->pos[1];
  source->z      =target->pos[2];
  source->rgba[0]=target->color[0];
  source->rgba[1]=target->color[1];
  source->rgba[2]=target->color[2];
  source->rgba[3]=target->color[3];
  source->normal.Instantiate();
  source->normal->Set(
   (float) target->normal[0],
   (float) target->normal[1],
   (float) target->normal[2]
  );
 }
 void Get( VBO_VNC *target, Vertexd *source, Vertexd *normal, Crayon *out ) {
  source->x      =(double) target->pos[0];
  source->y      =(double) target->pos[1];
  source->z      =(double) target->pos[2];
  out->Float(
   (GLfloat) target->color[0],
   (GLfloat) target->color[1],
   (GLfloat) target->color[2],
   (GLfloat) target->color[3]
  );
  normal->x=(double) target->normal[0];
  normal->y=(double) target->normal[1];
  normal->z=(double) target->normal[2];
 }
 void Postcompute( Vertex *a, Vertex *b, Vertex *c ) {
  normal.Deinstantiate();
 }
 void Postcompute( Vertexd *a, Vertexd *b, Vertexd *c ) {
  normal.Deinstantiate();
 }
 void DebugRender();
 //void DebugRender( glm::vec3 lightPos, GLMCamera *camera );
};