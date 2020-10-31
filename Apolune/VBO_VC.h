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

struct VBO_VC	{
 GLfloat pos[3];
 GLfloat color[4];
 VBO_VC() {
  pos[0]=pos[1]=pos[2]=0.0f;
  color[0]=color[1]=color[2]=color[3]=0.0f;
 }
};

class VBOStaticVC : public ModernVBO<VBO_VC> {
public:
 VBOStaticVC() : ModernVBO() {
  draw_method=GL_TRIANGLES;
 }
 virtual void fromModel( ImmediateModel *m );
 virtual void fromModel( ImmediateModel *m, int group );
 virtual void toModel( ImmediateModel *m );
 virtual void toModel( ImmediateModel *m, int group );
 virtual void fromPLY( PLY *p );
 virtual void fromPLY( PLY *p, bool create ); 
 void PushVertexColor( Vertexd *a, Crayon color ) {
  size_t end=data.size();
  data.resize(end+1);
  indices.resize(end+1);
  Set(&data[end],a,color);
  indices[end]=(preferred_type)end;
  count++;
 }
 void PushVertexColor( Vertexd *a, float r, float g, float b, float af ) {
  static Crayon color(r,g,b,af);
  size_t end = data.size();
  data.resize(end+1);
  indices.resize(end+1);
  Set(&data[end],a,color);
  indices[end]=(preferred_type)end;
  count++;
 }
 void BindToStreams() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(VBO_VC), BUFFER_OFFSET(offsetof(VBO_VC,pos)) );
  glEnableClientState(GL_COLOR_ARRAY);
  glColorPointer(4, GL_FLOAT, sizeof(VBO_VC), BUFFER_OFFSET(offsetof(VBO_VC,color)) );
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
 }
 void Set( VBO_VC *target, Vertex *source ) {
  target->pos[0]  =(GLfloat) source->x;
  target->pos[1]  =(GLfloat) source->y;
  target->pos[2]  =(GLfloat) source->z;
  target->color[0]=(GLfloat) source->rgba[0];
  target->color[1]=(GLfloat) source->rgba[1];
  target->color[2]=(GLfloat) source->rgba[2];
  target->color[3]=(GLfloat) source->rgba[3];
 }
 void Set( VBO_VC *target, Vertexd *source ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->color[0]=target->color[1]=target->color[2]=target->color[3]=1.0f;
 }
 void Set( VBO_VC *target, Vertexd *source, Crayon rgba ) {
  target->pos[0]=(GLfloat) source->x;
  target->pos[1]=(GLfloat) source->y;
  target->pos[2]=(GLfloat) source->z;
  target->color[0]=rgba.rf;
  target->color[1]=rgba.gf;
  target->color[2]=rgba.bf;
  target->color[3]=rgba.af;
 }
 void Get( VBO_VC *target, Vertex *source ) {
  source->x      =(GLfloat) target->pos[0];
  source->y      =(GLfloat) target->pos[1];
  source->z      =(GLfloat) target->pos[2];
  source->rgba[0]=(GLfloat) target->color[0];
  source->rgba[1]=(GLfloat) target->color[1];
  source->rgba[2]=(GLfloat) target->color[2];
  source->rgba[3]=(GLfloat) target->color[3];
 }
 void Get( VBO_VC *target, Vertexd *source ) {
  source->x      =(GLfloat) target->pos[0];
  source->y      =(GLfloat) target->pos[1];
  source->z      =(GLfloat) target->pos[2];
 }
 void Get( VBO_VC *target, Vertexd *source, Crayon *color_out ) {
  source->x      =(GLfloat) target->pos[0];
  source->y      =(GLfloat) target->pos[1];
  source->z      =(GLfloat) target->pos[2];
  color_out->Float(
   (GLfloat) target->color[0],
   (GLfloat) target->color[1],
   (GLfloat) target->color[2],
   (GLfloat) target->color[3]
  );
 }
};
