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

struct VBO_VNT	{
 GLfloat pos[3];
 GLfloat normal[3];
 GLfloat texCoord[2];
 VBO_VNT() {
  pos[0]=pos[1]=pos[2]=0.0f;
  normal[0]=normal[1]=normal[2]=0.0f;
  texCoord[0]=texCoord[1]=0.0f;
 }
};

class VBOStaticVNT : public ModernVBO<VBO_VNT> {
public:
 GLuint *position,*texcoord,*anormal;
 void Constructor() {
  position=texcoord=anormal=null;
 }
 virtual void fromModel( ImmediateModel *m );
 virtual void FromOBJ( ImmediateModel *m );
 virtual void fromModel( ImmediateModel *m, int group );
 virtual void fromPLY( PLY *p );
 virtual void fromPLY( PLY *p, bool create );
 void BindToStreams() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNT,pos)));
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNT,normal)));
  glClientActiveTexture(GL_TEXTURE0); // Tex-coords sent to GL_TEXTURE0, default texture (color map)
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNT,texCoord)));
  glClientActiveTexture(GL_TEXTURE1); // Tangents implemented for parallax as GL_TEXTURE1 (normal map)
  glDisableClientState(GL_COLOR_ARRAY);
 }
 void BindToStreams2() {
  glEnableVertexAttribArray(0);	glVertexAttribPointer(	*position,	3,	GL_FLOAT, GL_FALSE,	sizeof(VBO_VNT), BUFFER_OFFSET(0)	);
 	glEnableVertexAttribArray(1); glVertexAttribPointer( *anormal, 3, GL_FLOAT, GL_FALSE, sizeof(VBO_VNT), BUFFER_OFFSET(4*3) );
 	glEnableVertexAttribArray(2);	glVertexAttribPointer( *texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(VBO_VNT), BUFFER_OFFSET(4*3+4*3) );
 }
 void UnbindFromStreams2() {
 	glDisableVertexAttribArray(0);
 	glDisableVertexAttribArray(1);
 	glDisableVertexAttribArray(2);
 }
 Zdisposable<Vertex> normal;
 void Precompute( Vertex *a, Vertex *b, Vertex *c ) {
  if ( a->normal ) normal.Recycle(new Vertex(a->normal));
  else normal=c->TriNormal(a,b,c);
 }
 void Precompute( Vertexd *a, Vertexd *b, Vertexd *c ) {
  normal.Recycle();
  normal->TriNormal(a,b,c);
 }
 void Set( VBO_VNT *target, Vertexd *source, Vertexd *tc ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->normal[0]=(GLfloat) this->normal->x;
  target->normal[1]=(GLfloat) this->normal->y;
  target->normal[2]=(GLfloat) this->normal->z;
  target->texCoord[0]=(GLfloat) tc->x;
  target->texCoord[1]=(GLfloat) tc->y;
 }
 void Set( VBO_VNT *target, Vertexd *source, Vertexd *normal, Vertexd *tc ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->normal[0]=(GLfloat) normal->x;
  target->normal[1]=(GLfloat) normal->y;
  target->normal[2]=(GLfloat) normal->z;
  target->texCoord[0]=(GLfloat) tc->x;
  target->texCoord[1]=(GLfloat) tc->y;
 }
 void Set( VBO_VNT *target, Vertex *source, Vertex *normal ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->normal[0]=normal->x;
  target->normal[1]=normal->y;
  target->normal[2]=normal->z;
  target->texCoord[0]=source->q;
  target->texCoord[1]=source->r;
 }
 void Get( VBO_VNT *target, Vertex *source ) {
  (GLfloat) source->x=target->pos[0];
  (GLfloat) source->y=target->pos[1];
  (GLfloat) source->z=target->pos[2];
  source->normal.Recycle(new Vertex(
   target->normal[0],
   target->normal[1],
   target->normal[2]
  ));
  (GLfloat) source->q=target->texCoord[0];
  (GLfloat) source->r=target->texCoord[1];
 }
 void Postcompute( Vertex *a, Vertex *b, Vertex *c ) {
  if ( a->normal ) normal=null;
  else normal.Deinstantiate();
 }
 void Postcompute( Vertexd *a, Vertexd *b, Vertexd *c ) {
  normal.Deinstantiate();
 }
 void DebugRender();
 void DebugRender( glm::vec3 lightPos, GLMCamera *camera );
};
