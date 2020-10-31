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

struct VBO_VT {
 GLfloat pos[3];      
 GLfloat texCoord[2]; 
 VBO_VT() {
  pos[0]=pos[1]=pos[2]=0.0f;
  texCoord[0]=texCoord[1]=0.0f;
 }
};

class VBOStaticVT : public ModernVBO<VBO_VT> {
public:
 VBOStaticVT() : ModernVBO() {}
 virtual void fromModel( ImmediateModel *m );
 virtual void fromModel( ImmediateModel *m, int group );
 virtual void fromPLY( PLY *p );
 virtual void fromPLY( PLY *p, bool create );
 void BindToStreams() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(VBO_VT), BUFFER_OFFSET(offsetof(VBO_VT,pos)));
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, sizeof(VBO_VT), BUFFER_OFFSET(offsetof(VBO_VT,texCoord)));
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
 }
 void Set( VBO_VT *target, Vertex *source ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->texCoord[0]=(GLfloat) source->q;
  target->texCoord[1]=(GLfloat) source->r;
 }
 void Get( VBO_VT *target, Vertex *source ) {
  source->x=(GLfloat) target->pos[0];
  source->y=(GLfloat) target->pos[1];
  source->z=(GLfloat) target->pos[2];
  source->q=(GLfloat) target->texCoord[0];
  source->r=(GLfloat) target->texCoord[1];
 }
};