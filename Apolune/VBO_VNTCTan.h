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

struct VBO_VNTCTan {
 GLfloat pos[3];
 GLfloat normal[3];
 GLfloat texCoord[2];
 GLfloat color[4];
 GLfloat tangent[4]; // can this be a 3?  what should 4th (w) be?
};

class VBOStaticVNTCTan : public ModernVBO<VBO_VNTCTan> {
public:
 virtual void fromModel( ImmediateModel *m );
 virtual void fromModel( ImmediateModel *m, int group );
 virtual void fromPLY( PLY *p );
 virtual void fromPLY( PLY *p, bool create );
 void BindToStreams() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNTCTan,pos)));
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNTCTan,normal)));
  glClientActiveTexture(GL_TEXTURE0); // Tex-coords sent to GL_TEXTURE0, default texture (color map)
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNTCTan,texCoord)));
  glClientActiveTexture(GL_TEXTURE1); // Tangents implemented for parallax as GL_TEXTURE1 (normal map)
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(4, GL_FLOAT, (GLsizei) stride, BUFFER_OFFSET(offsetof(VBO_VNTCTan,color)));
  // Tangents not yet  implemented!
 }
 Zdisposable<Vertex> normal,tangent;
 void Precompute( Vertex *a, Vertex *b, Vertex *c ) {
  if ( a->normal ) normal.Recycle(new Vertex(a->normal));
  else normal.Recycle(c->TriNormal(a,b,c));
  tangent.Recycle(c->calcTangentVector(a,b,c,normal));
 }
 void Precompute( Vertexd *a, Vertexd *b, Vertexd *c ) {
  normal.Recycle();
  normal->TriNormal(a,b,c);
  Vertexd n(normal->x,normal->y,normal->z);
  tangent.Recycle(normal->calcTangentVector(a,b,c,&n));
 }
 void Set( VBO_VNTCTan *target, Vertex *source ) {
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
  target->tangent[0]=tangent->x;
  target->tangent[1]=tangent->y;
  target->tangent[2]=tangent->z;
  target->tangent[3]=tangent->w;
  target->texCoord[0]=(GLfloat) source->q;
  target->texCoord[1]=(GLfloat) source->r;
  target->color[0]=(GLfloat) source->rgba[0];
  target->color[1]=(GLfloat) source->rgba[1];
  target->color[2]=(GLfloat) source->rgba[2];
  target->color[3]=(GLfloat) source->rgba[3];
 }
 void Set( VBO_VNTCTan *target, Vertexd *source ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->normal[0]=(GLfloat) this->normal->x;
  target->normal[1]=(GLfloat) this->normal->y;
  target->normal[2]=(GLfloat) this->normal->z;
  target->tangent[0]=tangent->x;
  target->tangent[1]=tangent->y;
  target->tangent[2]=tangent->z;
  target->tangent[3]=tangent->w;
  target->texCoord[0]=(GLfloat) source->x;
  target->texCoord[1]=(GLfloat) source->y;
  target->color[0]=1.0;
  target->color[1]=1.0;
  target->color[2]=1.0;
  target->color[3]=1.0;
 }
 void Get( VBO_VNTCTan *target, Vertex *source ) {
  source->x=(GLfloat) target->pos[0];
  source->y=(GLfloat) target->pos[1];
  source->z=(GLfloat) target->pos[2];
  source->normal.Recycle(new Vertex(
   target->normal[0],
   target->normal[1],
   target->normal[2]
  ));
  source->tangent.Recycle(new Vertex(
   target->tangent[0],
   target->tangent[1],
   target->tangent[2],
   target->tangent[3]
  ));
  source->q=(GLfloat) target->texCoord[0];
  source->r=(GLfloat) target->texCoord[1];
  source->rgba[0]=(GLfloat) target->color[0];
  source->rgba[1]=(GLfloat) target->color[1];
  source->rgba[2]=(GLfloat) target->color[2];
  source->rgba[3]=(GLfloat) target->color[3];
 }
 void Get( VBO_VNTCTan *target, Vertexd *source, Vertexd *normal, Vertexd *tc, Vertex *tangent, Crayon *out ) {
  source->x=(GLfloat) target->pos[0];
  source->y=(GLfloat) target->pos[1];
  source->z=(GLfloat) target->pos[2];
  normal->x=target->normal[0];
  normal->y=target->normal[1];
  normal->z=target->normal[2];
  tangent->x=target->tangent[0];
  tangent->y=target->tangent[1];
  tangent->z=target->tangent[2];
  tangent->w=target->tangent[3];
  tc->x=(GLfloat) target->texCoord[0];
  tc->y=(GLfloat) target->texCoord[1];
  out->Float(
   (GLfloat) target->color[0],
   (GLfloat) target->color[1],
   (GLfloat) target->color[2],
   (GLfloat) target->color[3]
  );
 }
 void Postcompute() {
 }
};