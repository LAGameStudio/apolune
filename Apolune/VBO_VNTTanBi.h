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
#pragma warning (disable:4267) // conversion warning

struct VBO_VNTTanBi {
 GLfloat pos[3];
 GLfloat normal[3];
 GLfloat texCoord[2];
 GLfloat tangent[3]; // 3 or 4?
 GLfloat bitangent[3]; // 3 or 4?
};

class VBOStaticVNTTanBi : public ModernVBO<VBO_VNTTanBi> {
public:
 GLuint *position,*texcoord,*normal,*tangent,*bitangent;
 void Constructor() {
  position=texcoord=normal=tangent=bitangent=null;
 }
 virtual void fromModel( ImmediateModel *m );
// virtual void fromModel( ImmediateModel *m, int group );
 virtual void fromPLY( PLY *p );
 virtual void fromPLY( PLY *p, bool create );
 virtual void fromVBO( VBO16 *v );
 void BindToStreams();
 void UnbindFromStreams();
 void BindToStreams2();
 void UnbindFromStreams2();
 void BindToStreams3();
 void UnbindFromStreams3();
 void Precompute() {
  // Occurs in fromModel() or must be done manually
 }
 void Precompute( Vertex *a, Vertex *b, Vertex *c ) {
  // Doesn't use this (yet).
 }
 void Set( VBO_VNTTanBi *target, Vertex *source ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->texCoord[0]=(GLfloat) source->q;
  target->texCoord[1]=(GLfloat) source->r;
  target->tangent[0]=(GLfloat) source->tangent->x;
  target->tangent[1]=(GLfloat) source->tangent->y;
  target->tangent[2]=(GLfloat) source->tangent->z;
//  target->tangent[3]=(GLfloat) source->tangent->w; // ? is this a bug...
  target->bitangent[0]=(GLfloat) source->bitangent->x;
  target->bitangent[1]=(GLfloat) source->bitangent->y;
  target->bitangent[2]=(GLfloat) source->bitangent->z;
//  target->bitangent[3]=(GLfloat) source->bitangent->z; // ...or to save attributes?
  target->normal[0]=(GLfloat) source->normal->x;
  target->normal[1]=(GLfloat) source->normal->y;
  target->normal[2]=(GLfloat) source->normal->z;
 }
 void Get( VBO_VNTTanBi *target, Vertex *source ) {
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
   target->tangent[2]
  ));
  source->bitangent.Recycle(new Vertex(
   target->bitangent[0],
   target->bitangent[1],
   target->bitangent[2]
  ));
  source->r=(GLfloat) target->texCoord[0];
  source->q=(GLfloat) target->texCoord[1];
 }
 void DebugRender( glm::vec3 lightPos, GLMCamera *camera );
 void DebugRender();
};
