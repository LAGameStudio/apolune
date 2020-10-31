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

struct VBO_VNTC	{
 GLfloat pos[3];
 GLfloat normal[3];
 GLfloat texCoord[2];
 GLfloat color[4];
};

class VBOStaticVNTC : public ModernVBO<VBO_VNTC> {
public:
 virtual void fromModel( ImmediateModel *m );
 virtual void fromModel( ImmediateModel *m, int group );
 virtual void fromPLY( PLY *p );
 virtual void fromPLY( PLY *p, bool create );
 void BindToStreams() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNTC,pos)));
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNTC,normal)));
  glClientActiveTexture(GL_TEXTURE0); // Tex-coords sent to GL_TEXTURE0, default texture (color map)
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNTC,texCoord)));
  glClientActiveTexture(GL_TEXTURE1); // Tangents implemented for parallax as GL_TEXTURE1 (normal map)
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(4, GL_FLOAT, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNTC,color)));
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
 void Set( VBO_VNTC *target, Vertex *source ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  if ( source->normal ) {
   target->normal[0]=(GLfloat) source->normal->x;
   target->normal[1]=(GLfloat) source->normal->y;
   target->normal[2]=(GLfloat) source->normal->z;
  } else {
   target->normal[0]=(GLfloat) this->normal->x;
   target->normal[1]=(GLfloat) this->normal->y;
   target->normal[2]=(GLfloat) this->normal->z;
  }
  target->texCoord[0]=(GLfloat) source->q;
  target->texCoord[1]=(GLfloat) source->r;
  target->color[0]=(GLfloat) source->rgba[0];
  target->color[1]=(GLfloat) source->rgba[1];
  target->color[2]=(GLfloat) source->rgba[2];
  target->color[3]=(GLfloat) source->rgba[3];
 }
 void Set( VBO_VNTC *target, Vertexd *source, Vertexd *normal, Vertexd *tc, Crayon rgba ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->normal[0]=(GLfloat) normal->x;
  target->normal[1]=(GLfloat) normal->y;
  target->normal[2]=(GLfloat) normal->z;
  target->texCoord[0]=(GLfloat) tc->x;
  target->texCoord[1]=(GLfloat) tc->y;
  target->color[0]=(GLfloat) rgba.rf;
  target->color[1]=(GLfloat) rgba.gf;
  target->color[2]=(GLfloat) rgba.bf;
  target->color[3]=(GLfloat) rgba.af;
 }
 void Get( VBO_VNTC *target, Vertex *source ) {
  source->x=target->pos[0];
  source->y=target->pos[1];
  source->z=target->pos[2];
  source->normal.Recycle(new Vertex(
   target->normal[0],
   target->normal[1],   
   target->normal[2]
  ));
  source->q=target->texCoord[0];
  source->r=target->texCoord[1];
  source->rgba[0]=target->color[0];
  source->rgba[1]=target->color[1];
  source->rgba[2]=target->color[2];
  source->rgba[3]=target->color[3];
 }
 void Get( VBO_VNTC *target, Vertexd *source, Vertexd *normal, Vertexd *tc, Crayon *out ) {
  source->x=(double) target->pos[0];
  source->y=(double) target->pos[1];
  source->z=(double) target->pos[2];
  normal->x=(double) target->normal[0];
  normal->y=(double) target->normal[1];
  normal->z=(double) target->normal[2];
  tc->x=(double) target->texCoord[0];
  tc->y=(double) target->texCoord[1];
  out->Float(
   (float) target->color[0],
   (float) target->color[1],
   (float) target->color[2],
   (float) target->color[3]
  );
 }
 void Postcompute( Vertex *a, Vertex *b, Vertex *c ) {
  if ( a->normal ) normal=null;
  else normal.Deinstantiate();
 }
 void Postcompute( Vertexd *a, Vertexd *b, Vertexd *c ) {
  normal.Deinstantiate();
 }
};
