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

// The VBO_VNTTa is used for implementing parallax mapping specifically,
// and a new variant should be created in the situation where it needs
// a different BindToStreams() configuration.
// Incoming tangent data should use the CalcTangent function of the Vertex class
// Assumptions: Color map is tex0, Normal map is tex1

struct VBO_VNTTan {
 GLfloat pos[3];
 GLfloat normal[3];
 GLfloat texCoord[2];
 GLfloat tangent[4];
};

class VBOStaticVNTTan : public ModernVBO<VBO_VNTTan> {
public:
 virtual void fromModel( ImmediateModel *m );
 virtual void fromModel( ImmediateModel *m, int group );
 virtual void fromPLY( PLY *p );
 virtual void fromPLY( PLY *p, bool create );
 virtual void BindToStreams() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(VBO_VNTTan), BUFFER_OFFSET(offsetof(VBO_VNTTan,pos)));
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, sizeof(VBO_VNTTan), BUFFER_OFFSET(offsetof(VBO_VNTTan,normal)));
  glClientActiveTexture(GL_TEXTURE0); // Tex-coords sent to GL_TEXTURE0, default texture (color map)
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(VBO_VNTTan), BUFFER_OFFSET(offsetof(VBO_VNTTan,texCoord)));
  glClientActiveTexture(GL_TEXTURE1); // Tangents implemented for parallax as GL_TEXTURE1 (normal map)
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(4, GL_FLOAT, sizeof(VBO_VNTTan), BUFFER_OFFSET(offsetof(VBO_VNTTan,tangent)));
 }
 Zp<Vertex> normal;
 Zp<Vertex> tangent;
 void Precompute( Vertex *a, Vertex *b, Vertex *c ) {
  normal=c->TriNormal(a,b,c);
  tangent = c->calcTangentVector(a,b,c,normal);
 }
 void Set( VBO_VNTTan *target, Vertex *source ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  if ( source->normal ) {
   target->normal[0]=(GLfloat) source->normal->x;
   target->normal[1]=(GLfloat) source->normal->y;
   target->normal[2]=(GLfloat) source->normal->z;
  } else {
   target->normal[0]=(GLfloat) normal->x;
   target->normal[1]=(GLfloat) normal->y;
   target->normal[2]=(GLfloat) normal->z;
  }
  if ( source->tangent ) {
   target->tangent[0]=source->tangent->x;
   target->tangent[1]=source->tangent->y;
   target->tangent[2]=source->tangent->z;
   target->tangent[3]=source->tangent->w;
  } else {
   target->tangent[0]=tangent->x;
   target->tangent[1]=tangent->y;
   target->tangent[2]=tangent->z;
   target->tangent[3]=tangent->w;
  }
  target->texCoord[0]=(GLfloat) source->r;
  target->texCoord[1]=(GLfloat) source->q;
 }
 void Get( VBO_VNTTan *target, Vertex *source ) {
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
  source->r=(GLfloat) target->texCoord[0];
  source->q=(GLfloat) target->texCoord[1];
 }
 void Postcompute() {
  delete normal;
  delete tangent;
 }
};
